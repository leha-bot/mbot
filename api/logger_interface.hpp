// Copyright ©  2017 Maks Mazurov
// Licensed under AGPLv3 as published by Free Software Foundation.

#ifndef MBOT_LOGGERINTERFACE_HPP
#define MBOT_LOGGERINTERFACE_HPP

#include <string>

///
/// \brief Defines interface to access internal log infastructure from modules.
/// Passed to modules through infastructure_facade.
///
class logger_interface {
public:
    enum message_type {
        /// Extra information about current process for debugging purposes.
        debug,
        /// Information about current process (such as progress reports).
        info,
        /// Something unexcepted/unusual happened.
        warning,
        /// Non-critical (recoverable) error.
        error,
        /// Critical error. Can't continue.
        fatal
    };

    virtual void log(message_type type, const std::string& message) const = 0;
};

#endif //MBOT_LOGGERINTERFACE_HPP
