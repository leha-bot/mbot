// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#include "utils/thread_balancer.hpp"

thread_balancer::thread_balancer(unsigned threads_count)
    : balancer_logger("Thread Balancer", log_manager::get_global_manager()) {
    balancer_logger.log(logger::debug, "Creating " + std::to_string(threads_count) + " thread(s).");
    while (threads_count --> 0) {
        threads.emplace_back(thread_balancer::thread_entry, std::ref(*this));
    }
}

thread_balancer::~thread_balancer() {
    stop = true;
    for (std::thread& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void thread_balancer::schedule(std::function<void()> func) {
    std::lock_guard<std::mutex> lock(queue_access_lock);
    queue.push(func);
}

void thread_balancer::thread_entry(thread_balancer& controller) {
    while (!controller.stop) {
        controller.queue_access_lock.lock();
        if (!controller.queue.empty()) {
            std::function<void()> func = controller.queue.front();
            controller.queue.pop();
            controller.queue_access_lock.unlock();

            if (!func) {
                controller.balancer_logger.log(logger::warning, "Null functor, skipping...");
                continue;
            }

            try {
                func();
            } catch (std::exception& excp) {
                controller.balancer_logger.log(logger::error, "Uncaught exception during job execution. what(): "
                                               + std::string(excp.what()));
            } catch (...) {
                controller.balancer_logger.log(logger::error, "Uncaught exception during job execution.");
            }
        } else {
            controller.queue_access_lock.unlock();
        }
    }
}
