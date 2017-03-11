// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#ifndef MBOT_COMMAND_MANAGER_HPP
#define MBOT_COMMAND_MANAGER_HPP

#include <utils/thread_balancer.hpp>
#include "../../api/module_interface.hpp"

class command_manager {
public:
    void register_callbacks(TgBot::EventBroadcaster& broadcaster,
                            thread_balancer& balancer,
                            const module_interface::command_table& table);
private:
    std::unordered_map<std::string, std::string> description_strings;
    std::unordered_map<std::string, std::string> synopsis_strings;

    logger cmd_logger = logger("Command Manager", log_manager::get_global_manager());
};

#endif //MBOT_COMMAND_MANAGER_HPP
