// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#ifndef MBOT_BOT_HPP
#define MBOT_BOT_HPP

#include <unordered_map>
#include "core/module_manager.hpp"
#include "utils/log_manager.hpp"
#include "../../api/module_interface.hpp"
#include "command_manager.hpp"

class bot {
public:
    bot(const std::string& token);

    void run();
private:
    void setup_callbacks();

    logger core_logger;

    TgBot::Bot _bot;
    module_manager<module_interface> mod_manager;
    command_manager cmd_manager;

    thread_balancer balancer;
};

#endif //MBOT_BOT_HPP
