// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#ifndef MBOT_MODULE_LOADER_HPP
#define MBOT_MODULE_LOADER_HPP

#include <unordered_map>
#include <string>
#include <boost/filesystem.hpp>
#include "../utils/shared_library.hpp"
#include "../utils/logger.hpp"

/// Loads, holds ownership and unloads modules.
/// \tparam ModuleInterface - module interface, should define method 'std::string get_name() const'
/// DLL with module should export function 'ModuleInterface* init_module(void)' with C linkage.
template<typename ModuleInterface>
class module_manager {
public:
    using module_init_function = ModuleInterface* (*)();

    module_manager() : manager_logger("Module Manager", log_manager::get_global_manager()) {}
    module_manager(const module_manager&) = delete;
    module_manager(module_manager&&) = default;

    /// Tries to load all modules from specified directory.
    /// Skips module if it cannot be loaded for some reason.
    /// \param path - path to directory with modules.
    void load_modules(const boost::filesystem::path& path) {
        for (auto& entry : boost::filesystem::directory_iterator(path)) {
            if (entry.path().extension() != ".so") {
                continue;
            }
            try {
                manager_logger.log(logger::info, "Loading " + entry.path().filename().string() + "...");
                load_module(entry);
            } catch (std::exception& excp) {
                manager_logger.log(logger::error, "Failed to load " +
                    entry.path().filename().string() + ": " + excp.what());
            } catch (...) {
                manager_logger.log(logger::error, "Failed to load " +
                    entry.path().filename().string() + ": Unkonwn error.");
            }
        }
    }

    /// Loads module from file.
    /// \throws dll_exception and invalid_argument.
    void load_module(const boost::filesystem::path& path) {
        if (!boost::filesystem::exists(path)) {
            throw invalid_argument("No such file: " + path.string());
        }
        if (boost::filesystem::is_directory(path)) {
            throw invalid_argument(path.string() + " is directory.");
        }

        shared_library dll(boost::filesystem::canonical(path).string());

        module_init_function init = reinterpret_cast<module_init_function>(dll.resolve_symbol("init_module"));
        ModuleInterface* module = init();

        dlls.emplace(module->get_name(), std::move(dll));
        modules.emplace(module->get_name(), std::unique_ptr<ModuleInterface>(module));
    }

    void unload_module(const std::string& module_name) {
        auto it = modules.find(module_name);
        if (it == modules.end()) {
            throw invalid_argument("Module " + module_name + " not loaded.");
        }

        modules.erase(it);
        dlls.erase(dlls.find(module_name));
    }

    /// \returns refernce to loaded module interface, object is guaranteed to be valid
    /// until appropriate unload_module call or module_manager destruction.
    ModuleInterface& get_module(const std::string& module_name) const {
        auto it = modules.find(module_name);
        if (it == modules.end()) {
            throw invalid_argument("Module " + module_name + " not loaded.");
        }
        return **it;
    }

    const std::unordered_map<std::string, std::unique_ptr<ModuleInterface> >& get_all_modules() const {
        return modules;
    }
private:
    logger manager_logger;

    std::unordered_map<std::string, shared_library> dlls;
    std::unordered_map<std::string, std::unique_ptr<ModuleInterface> > modules;
};

#endif //MBOT_MODULE_LOADER_HPP
