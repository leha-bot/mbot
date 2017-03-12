// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#ifndef MBOT_COMMAND_MANAGER_HPP
#define MBOT_COMMAND_MANAGER_HPP

#include <utils/thread_balancer.hpp>
#include "../../api/module_interface.hpp"

class command_manager {
public:
    /// Adds callbacks for each command in table.
    /// In addition it adds callbacks for some commands like /help /about /start
    void register_callbacks(TgBot::Bot& bot,
                            thread_balancer& balancer,
                            const module_interface::command_table& table);
private:
    std::string get_command_description(const std::string& command) const;

    module_interface::command_table table;

    logger cmd_logger = logger("Command Manager", log_manager::get_global_manager());
};

#endif //MBOT_COMMAND_MANAGER_HPP
