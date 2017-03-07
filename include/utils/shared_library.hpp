#ifndef MBOT_SHARED_LIBRARY_HPP
#define MBOT_SHARED_LIBRARY_HPP

#include "exception.hpp"

DECLARE_EXCEPTION(dll_exception, std::runtime_error);

///
/// \brief Encapsulates native C shared library functions.
///
class shared_library {
public:
    using native_handle = /* implementation-defined */ void*;

    /// Tries to load shared library, \throws dll_exception if fails.
    /// \param path - absolute or relative to LD_LIBRARY_PATH.
    shared_library(const std::string& path);

    /// Unloads shared library if handle is not detached.
    ~shared_library();

    shared_library(const shared_library&) = delete;
    shared_library(shared_library&&) = default;

    ///
    /// \brief Searches for \arg symbol in DLL.
    /// \param symbol - symbol name string.
    /// \return Address of symbol loaded into memory (can be casted to real symbol type).
    /// \throws dll_exception if symbol wasn't loaded.
    /// \note Thread safety is not guaranteed.
    ///
    void* resolve_symbol(const std::string& symbol) const;

    ///
    /// \brief After detach is called, dll class will release handle ownership.
    /// DLL will not be unloaded after destruction of dll instance.
    ///
    void detach();
    bool is_detached() const;

    native_handle& get_native_handle();

private:
    bool detached = false;
    native_handle handle;
};

#endif // MBOT_SHARED_LIBRARY_HPP
