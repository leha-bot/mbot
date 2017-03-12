// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#include "core/bot.hpp"

bot::bot(const std::string& token)
    : _bot(token)
    , core_logger("Core", log_manager::get_global_manager())
    , balancer(std::thread::hardware_concurrency() - 1) {
    core_logger.log(logger::info, "Loading modules....");
    mod_manager.load_modules("./modules");
    for (const auto& module : mod_manager.get_all_modules()) {
        core_logger.log(logger::info, "Initializing modules...");
        module.second->on_init(std::unique_ptr<infastructure_facade>(new infastructure_facade));
    }
    setup_callbacks();
}

void bot::run() {
    core_logger.log(logger::info, "Initialization finished.");
    TgBot::TgLongPoll longpool(_bot);
    while (true) {
        core_logger.log(logger::debug, "Fetching updates...");
        longpool.start();
    }
}

void bot::setup_callbacks() {
    core_logger.log(logger::debug, "Loaded " + std::to_string(mod_manager.get_all_modules().size()) + " modules.");
    module_interface::command_table table;
    for (const auto& module : mod_manager.get_all_modules()) {
        auto module_table = module.second->get_command_table();
        table.reserve(module_table.size());
        for (const auto& entry : module_table) {
            table.emplace(entry.first, entry.second);
        }

        module_interface* interface = module.second.get();
        _bot.getEvents().onNonCommandMessage([this,interface](TgBot::Message::Ptr msg) {
            balancer.schedule([this,interface,msg]() {
                try {
                    interface->on_non_command_message(msg);
                } catch (std::exception& excp) {
                    core_logger.log(logger::error, "Error in module " + interface->get_name() + ": " + excp.what());
                } catch (...) {
                    core_logger.log(logger::error, "Unknown error in module " + interface->get_name() + ".");
                }
            });
        });
        _bot.getEvents().onAnyMessage([this,interface](TgBot::Message::Ptr msg) {
            balancer.schedule([this,interface,msg]() {
                try {
                    interface->on_any_message(msg);
                } catch (std::exception& excp) {
                    core_logger.log(logger::error, "Error in module " + interface->get_name() + ": " + excp.what());
                } catch (...) {
                    core_logger.log(logger::error, "Unknown error in module " + interface->get_name() + ".");
                }
            });
        });
    }
    cmd_manager.register_callbacks(_bot, balancer, table);
}
