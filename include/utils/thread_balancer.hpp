// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#ifndef MBOT_THREAD_BALANCER_HPP
#define MBOT_THREAD_BALANCER_HPP

#include <functional>
#include <queue>
#include <mutex>
#include <thread>
#include "logger.hpp"

class thread_balancer {
public:
    thread_balancer(unsigned threads_count);
    ~thread_balancer();

    void schedule(std::function<void()> func);
private:
    static void thread_entry(thread_balancer& controller);

    logger balancer_logger;

    bool stop = false;
    std::vector<std::thread> threads;
    std::queue<std::function<void()> > queue;
    std::mutex queue_access_lock;
};

#endif //MBOT_THREAD_BALANCER_HPP
