// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#include "utils/logger.hpp"

logger::logger(const std::string component_name, std::shared_ptr<log_manager> manager)
    : component_name(component_name)
    , manager(manager) {
}

void logger::log(logger_interface::message_type type, const std::string& message) const {
    manager->log(type, this->component_name, message);
}
