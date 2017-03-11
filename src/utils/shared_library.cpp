#include "utils/shared_library.hpp"

#include <cassert>
#include <dlfcn.h>

namespace _impl {
    shared_library::native_handle load_library(const std::string& path) {
        dlerror(); // Clear error state.
        char *error = nullptr;
        void *handle = dlopen(path.data(), RTLD_NOW | RTLD_GLOBAL);
        if ((error = dlerror()) != nullptr) {
            throw dll_exception(error);
        }
        return handle;
    }

    void unload_library(shared_library::native_handle handle) {
        assert(handle);
        if (dlclose(handle)) {
            // TODO: Log.
        }
    }

    void* resolve_symbol(shared_library::native_handle handle, const std::string& symbol_name) {
        assert(handle);
        dlerror();
        char *error = nullptr;
        void *symbol = dlsym(handle, symbol_name.data());
        if ((error = dlerror()) != nullptr) {
            throw dll_exception(error);
        }
        return symbol;
    }
}

shared_library::shared_library(const std::string& path) : handle(_impl::load_library(path)) {
}

shared_library::shared_library(shared_library&& other) : handle(other.handle) {
    other.detached = true;
}

shared_library::~shared_library() {
    if (!detached) {
        _impl::unload_library(this->handle);
    }
}

void* shared_library::resolve_symbol(const std::string& symbol) const {
    return _impl::resolve_symbol(this->handle, symbol);
}

void shared_library::detach() {
    this->detached  = true;
}

bool shared_library::is_detached() const {
    return this->detached;
}

shared_library::native_handle& shared_library::get_native_handle() {
    return this->handle;
}
