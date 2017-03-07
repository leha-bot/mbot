// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#ifndef MBOT_EXCEPTION_HPP_HPP
#define MBOT_EXCEPTION_HPP_HPP

#include <stdexcept>
#include <string>

/// Creates trivial exception type 'name' which inherits 'base'.
#define DECLARE_EXCEPTION(name, base) \
    class name : public base { \
    public: \
        name(const std::string& what) : base(what) {} \
        name(const char* what) : base(what) {} \
    }

// Some common errors.
DECLARE_EXCEPTION(invalid_argument, std::logic_error);
DECLARE_EXCEPTION(io_failure,       std::runtime_error);

// All modules should throw exceptions, which is only inherit this class.
DECLARE_EXCEPTION(module_exception, std::runtime_error);

#endif // MBOT_EXCEPTION_HPP_HPP
