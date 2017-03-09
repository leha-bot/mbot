// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#ifndef MBOT_LOG_MANAGER_HPP
#define MBOT_LOG_MANAGER_HPP

#include <fstream>
#include <string>
#include <unordered_map>
#include <boost/filesystem.hpp>
#include "../../api/logger_interface.hpp"

class log_manager {
public:
    class message_type_hasher {
    public:
        std::size_t operator()(const logger_interface::message_type& v) const {
            return static_cast<std::size_t>(v);
        }
    };

    static const std::unordered_map<logger_interface::message_type,
                                    std::string, message_type_hasher> type_strings;

    log_manager(const boost::filesystem::path& logs_directory);

    log_manager(log_manager&&) = default;
    log_manager(const log_manager&) = default;

    static std::shared_ptr<log_manager> get_global_manager();

    void log(logger_interface::message_type msg_type,
             const std::string& component_name, const std::string& message) noexcept;
private:
    void renew_log();
    static std::string format_message(logger_interface::message_type msg_type,
                                      const std::string& component_name, const std::string& message);

    std::ofstream log_file;
    boost::filesystem::path directory;

    static std::shared_ptr<log_manager> global_manager;
};

#endif // MBOT_LOG_MANAGER_HPP
