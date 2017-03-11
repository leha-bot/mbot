// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#include "core/command_manager.hpp"

void command_manager::register_callbacks(TgBot::EventBroadcaster& broadcaster,
                                         thread_balancer& balancer,
                                         const module_interface::command_table& table) {
    cmd_logger.log(logger::info, "Registering callbacks for " + std::to_string(table.size()) + " command(s)...");
    for (const auto& info : table) {
        broadcaster.onCommand(info.first, [this,&balancer,info](TgBot::Message::Ptr msg) {
            cmd_logger.log(logger::debug, "Command /" + info.first + " from " + msg->from->username + " in " +
                msg->chat->username);
            balancer.schedule([this,info,msg]() {
                try {
                    info.second.listener->on_command(info.first, msg);
                } catch (std::exception& excp) {
                    cmd_logger.log(logger::error, "Error in module " + info.first + ": " + excp.what());
                } catch (...) {
                    cmd_logger.log(logger::error, "Unknown error in module " + info.first + ".");
                }
            });
        });
        description_strings.insert({ info.first, info.second.description });
        synopsis_strings.insert({ info.first, info.second.synopsis });
    }
}
