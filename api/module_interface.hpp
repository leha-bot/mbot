// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#ifndef MBOT_MODULE_INTERFACE_HPP
#define MBOT_MODULE_INTERFACE_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <tgbot/tgbot.h>
#include "infastructure_facade.hpp"

class module_interface {
    using namespace TgBot;
public:
    struct command_info {
        std::string synopsis;
        std::string description;

        bool admin_only = false;
        bool creator_only = false;
    };
    using command_table = std::unordered_map<std::string, command_info>;

    virtual std::string   get_nane() = 0;
    virtual command_table get_command_table() { return command_table(); }

    virtual void on_any_message(Message::Ptr message) {}
    virtual void on_command(const std::string& command, const std::string parameters, Message::Ptr message) {}
    virtual void on_non_command_message(Message::Ptr message) {}

    virtual void on_init(std::unique_ptr<infastructure_facade> facade) {}
    virtual void on_enabled() {}
    virtual void on_disabled() {}
};

#endif //MBOT_MODULE_INTERFACE_HPP
