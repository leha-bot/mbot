// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#include "core/command_manager.hpp"
#include "string_utils.hpp"

void command_manager::register_callbacks(TgBot::Bot& bot,
                                         thread_balancer& balancer,
                                         const module_interface::command_table& table) {
    cmd_logger.log(logger::info, "Registering callbacks for " + std::to_string(table.size()) + " command(s)...");
    for (const auto& info : table) {
        cmd_logger.log(logger::debug, "Registered command /" + info.first +
                       " by module " + info.second.listener->get_name());
        bot.getEvents().onCommand(info.first, [this,&balancer,info](TgBot::Message::Ptr msg) {
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
        this->table.insert(info);
    }
    bot.getEvents().onCommand("help", [&bot,this](TgBot::Message::Ptr ptr) {
        std::string module;
        std::ostringstream answer;

        {
            std::vector<std::u16string> splitten;
            utils::split(utils::utf8_to_utf16(ptr->text), u' ', std::back_inserter(splitten));
            module = utils::utf16_to_utf8(utils::join(splitten.begin() + 1, splitten.end(), u" "));
        }

        if (module.empty()) {
            answer << "_Listing all commands._\n\n";
            for (const auto& command : this->table) {
                answer << get_command_description(command.first) << '\n';
            }
        } else {
            answer << "_Listing commands from " << module << " module._\n\n";
            for (const auto& command : this->table) {
                bool at_least_one_command_found = false;
                if (command.second.listener->get_name() == module) {
                    at_least_one_command_found = true;
                    answer << get_command_description(command.first) << '\n';
                }
                if (!at_least_one_command_found) {
                    bot.getApi().sendMessage(ptr->chat->id, "Unknown module.", false, ptr->messageId);
                    return;
                }
            }
        }
        bot.getApi().sendMessage(ptr->chat->id, answer.str(), false, ptr->messageId, TgBot::GenericReply::Ptr(),
                                 "Markdown", true);
    });
}

std::string command_manager::get_command_description(const std::string& command) const {
    std::string result;
    auto it = this->table.find(command);
    if (it == this->table.end()) {
        return "Unknown command.";
    }
    return "/" + (it->second.synopsis.empty() ? it->first : it->second.synopsis) + " - " +
                 (it->second.description.empty() ? "*No description.*" : it->second.description);
}
