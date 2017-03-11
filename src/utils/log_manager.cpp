// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#include "utils/log_manager.hpp"
#include <boost/date_time.hpp>
#include <mutex>

std::shared_ptr<log_manager> log_manager::global_manager;

const std::unordered_map<logger_interface::message_type,
                         std::string, log_manager::message_type_hasher> log_manager::type_strings {
    { logger_interface::message_type::fatal,   "FATAL"   },
    { logger_interface::message_type::warning, "Warning" },
    { logger_interface::message_type::error,   "Error"   },
    { logger_interface::message_type::debug,   "Debug"   },
    { logger_interface::message_type::info,    "Info"    },
};

log_manager::log_manager(const boost::filesystem::path& logs_directory) : directory(logs_directory) {
}

void log_manager::log(logger_interface::message_type msg_type,
                      const std::string& component_name,
                      const std::string& message) noexcept {
    std::string formatted_msg = format_message(msg_type, component_name, message);
    std::cerr << formatted_msg;
    this->log_file << formatted_msg;
}

void log_manager::renew_log() {
    std::ostringstream ss;

    using namespace boost::posix_time;
    using namespace boost::gregorian;

    static time_facet* facet = new time_facet;
    facet->format("%m%d%y");

    ptime now  = microsec_clock::universal_time();
    ss.imbue(std::locale(std::locale::classic(), facet));
    ss << "mbot_" << now << ".log";

    this->log_file = std::ofstream(boost::filesystem::canonical(directory).string() + "/" + ss.str());
}

std::string log_manager::format_message(logger_interface::message_type msg_type,
                                        const std::string& component_name,
                                        const std::string& message) {
    std::ostringstream ss;

    std::time_t time = std::time(nullptr);
    std::tm tm = *std::localtime(&time);

    ss << '[' << std::put_time(&tm, "%T") << "] [" << component_name << '/' << type_strings.at(msg_type) << "] " << message << '\n';
    return ss.str();
}

std::shared_ptr<log_manager> log_manager::get_global_manager() {
    static std::mutex spawn_lock;
    std::lock_guard<std::mutex> lock(spawn_lock);
    if (!global_manager) {
        global_manager.reset(new log_manager("./logs"));
    }
    return global_manager;
}
