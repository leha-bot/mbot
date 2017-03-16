// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#ifndef MBOT_LOGGER_HPP
#define MBOT_LOGGER_HPP

#include <memory>
#include "../../api/logger_interface.hpp"
#include "log_manager.hpp"

class logger : public logger_interface {
public:
    logger(const std::string component_name, std::shared_ptr<log_manager> manager);

    void log(message_type type, const std::string& message) const override;

private:
    std::string component_name;
    std::shared_ptr<log_manager> manager;
};

#endif //MBOT_LOGGER_HPP
