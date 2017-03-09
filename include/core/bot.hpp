// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#ifndef MBOT_BOT_HPP
#define MBOT_BOT_HPP

#include <unordered_map>
#include "core/module_manager.hpp"
#include "utils/log_manager.hpp"
#include "../../api/module_interface.hpp"

class bot {
public:
    bot(const std::string& token);

    void run();
private:
    void setup_callbacks();

    logger core_logger;

    TgBot::Bot bot;
    module_manager<module_interface> manager;
    std::shared_ptr<log_manager> log_mgr;
    std::unordered_map<std::string, module_interface*> command_bindings;
};

#endif //MBOT_BOT_HPP
