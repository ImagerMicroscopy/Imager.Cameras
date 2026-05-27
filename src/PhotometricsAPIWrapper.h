#ifndef PHOTOMETRICSAPIWRAPPER_H
#define PHOTOMETRICSAPIWRAPPER_H

#include <string>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#define LIB_HANDLE HINSTANCE
#define LOAD_LIBRARY(path) LoadLibraryA(path)
#define GET_PROC_ADDRESS GetProcAddress
#define FREE_LIBRARY FreeLibrary
#else
#include <dlfcn.h>
#define LIB_HANDLE void*
#define LOAD_LIBRARY(path) dlopen(path, RTLD_LAZY)
#define GET_PROC_ADDRESS dlsym
#define FREE_LIBRARY dlclose
#endif

#include "PVCAM/pvcam.h"

class PhotometricsAPIWrapper {
public:
    decltype(&::pl_pvcam_init) pl_pvcam_init = nullptr;
    decltype(&::pl_pvcam_uninit) pl_pvcam_uninit = nullptr;
    decltype(&::pl_cam_get_total) pl_cam_get_total = nullptr;
    decltype(&::pl_cam_get_name) pl_cam_get_name = nullptr;
    decltype(&::pl_cam_open) pl_cam_open = nullptr;
    decltype(&::pl_cam_close) pl_cam_close = nullptr;
    decltype(&::pl_get_param) pl_get_param = nullptr;
    decltype(&::pl_set_param) pl_set_param = nullptr;
    decltype(&::pl_get_enum_param) pl_get_enum_param = nullptr;
    decltype(&::pl_enum_str_length) pl_enum_str_length = nullptr;
    decltype(&::pl_exp_setup_cont) pl_exp_setup_cont = nullptr;
    decltype(&::pl_exp_start_cont) pl_exp_start_cont = nullptr;
    decltype(&::pl_exp_check_cont_status) pl_exp_check_cont_status = nullptr;
    decltype(&::pl_exp_stop_cont) pl_exp_stop_cont = nullptr;
    decltype(&::pl_exp_get_oldest_frame) pl_exp_get_oldest_frame = nullptr;
    decltype(&::pl_exp_unlock_oldest_frame) pl_exp_unlock_oldest_frame = nullptr;
    decltype(&::pl_error_code) pl_error_code = nullptr;
    decltype(&::pl_error_message) pl_error_message = nullptr;
    decltype(&::pl_cam_register_callback_ex3) pl_cam_register_callback_ex3 = nullptr;
    decltype(&::pl_cam_deregister_callback) pl_cam_deregister_callback = nullptr;

    PhotometricsAPIWrapper(const std::string& libPath) {
        _hLib = LOAD_LIBRARY(libPath.c_str());
        if (!_hLib) {
            // we don't have the library, so the runtime probably isn't installed.
            _allFunctionsLoaded = false;
            return;
        }
        pl_pvcam_init = reinterpret_cast<decltype(pl_pvcam_init)>(GET_PROC_ADDRESS(_hLib, "pl_pvcam_init"));
        if (!pl_pvcam_init) throw std::runtime_error("Failed to load function: pl_pvcam_init");
        pl_pvcam_uninit = reinterpret_cast<decltype(pl_pvcam_uninit)>(GET_PROC_ADDRESS(_hLib, "pl_pvcam_uninit"));
        if (!pl_pvcam_uninit) throw std::runtime_error("Failed to load function: pl_pvcam_uninit");
        pl_cam_get_total = reinterpret_cast<decltype(pl_cam_get_total)>(GET_PROC_ADDRESS(_hLib, "pl_cam_get_total"));
        if (!pl_cam_get_total) throw std::runtime_error("Failed to load function: pl_cam_get_total");
        pl_cam_get_name = reinterpret_cast<decltype(pl_cam_get_name)>(GET_PROC_ADDRESS(_hLib, "pl_cam_get_name"));
        if (!pl_cam_get_name) throw std::runtime_error("Failed to load function: pl_cam_get_name");
        pl_cam_open = reinterpret_cast<decltype(pl_cam_open)>(GET_PROC_ADDRESS(_hLib, "pl_cam_open"));
        if (!pl_cam_open) throw std::runtime_error("Failed to load function: pl_cam_open");
        pl_cam_close = reinterpret_cast<decltype(pl_cam_close)>(GET_PROC_ADDRESS(_hLib, "pl_cam_close"));
        if (!pl_cam_close) throw std::runtime_error("Failed to load function: pl_cam_close");
        pl_get_param = reinterpret_cast<decltype(pl_get_param)>(GET_PROC_ADDRESS(_hLib, "pl_get_param"));
        if (!pl_get_param) throw std::runtime_error("Failed to load function: pl_get_param");
        pl_set_param = reinterpret_cast<decltype(pl_set_param)>(GET_PROC_ADDRESS(_hLib, "pl_set_param"));
        if (!pl_set_param) throw std::runtime_error("Failed to load function: pl_set_param");
        pl_get_enum_param = reinterpret_cast<decltype(pl_get_enum_param)>(GET_PROC_ADDRESS(_hLib, "pl_get_enum_param"));
        if (!pl_get_enum_param) throw std::runtime_error("Failed to load function: pl_get_enum_param");
        pl_enum_str_length = reinterpret_cast<decltype(pl_enum_str_length)>(GET_PROC_ADDRESS(_hLib, "pl_enum_str_length"));
        if (!pl_enum_str_length) throw std::runtime_error("Failed to load function: pl_enum_str_length");
        pl_exp_setup_cont = reinterpret_cast<decltype(pl_exp_setup_cont)>(GET_PROC_ADDRESS(_hLib, "pl_exp_setup_cont"));
        if (!pl_exp_setup_cont) throw std::runtime_error("Failed to load function: pl_exp_setup_cont");
        pl_exp_start_cont = reinterpret_cast<decltype(pl_exp_start_cont)>(GET_PROC_ADDRESS(_hLib, "pl_exp_start_cont"));
        if (!pl_exp_start_cont) throw std::runtime_error("Failed to load function: pl_exp_start_cont");
        pl_exp_check_cont_status = reinterpret_cast<decltype(pl_exp_check_cont_status)>(GET_PROC_ADDRESS(_hLib, "pl_exp_check_cont_status"));
        if (!pl_exp_check_cont_status) throw std::runtime_error("Failed to load function: pl_exp_check_cont_status");
        pl_exp_stop_cont = reinterpret_cast<decltype(pl_exp_stop_cont)>(GET_PROC_ADDRESS(_hLib, "pl_exp_stop_cont"));
        if (!pl_exp_stop_cont) throw std::runtime_error("Failed to load function: pl_exp_stop_cont");
        pl_exp_get_oldest_frame = reinterpret_cast<decltype(pl_exp_get_oldest_frame)>(GET_PROC_ADDRESS(_hLib, "pl_exp_get_oldest_frame"));
        if (!pl_exp_get_oldest_frame) throw std::runtime_error("Failed to load function: pl_exp_get_oldest_frame");
        pl_exp_unlock_oldest_frame = reinterpret_cast<decltype(pl_exp_unlock_oldest_frame)>(GET_PROC_ADDRESS(_hLib, "pl_exp_unlock_oldest_frame"));
        if (!pl_exp_unlock_oldest_frame) throw std::runtime_error("Failed to load function: pl_exp_unlock_oldest_frame");
        pl_error_code = reinterpret_cast<decltype(pl_error_code)>(GET_PROC_ADDRESS(_hLib, "pl_error_code"));
        if (!pl_error_code) throw std::runtime_error("Failed to load function: pl_error_code");
        pl_error_message = reinterpret_cast<decltype(pl_error_message)>(GET_PROC_ADDRESS(_hLib, "pl_error_message"));
        if (!pl_error_message) throw std::runtime_error("Failed to load function: pl_error_message");
        pl_cam_register_callback_ex3 = reinterpret_cast<decltype(pl_cam_register_callback_ex3)>(GET_PROC_ADDRESS(_hLib, "pl_cam_register_callback_ex3"));
        if (!pl_cam_register_callback_ex3) throw std::runtime_error("Failed to load function: pl_cam_register_callback_ex3");
        pl_cam_deregister_callback = reinterpret_cast<decltype(pl_cam_deregister_callback)>(GET_PROC_ADDRESS(_hLib, "pl_cam_deregister_callback"));
        if (!pl_cam_deregister_callback) throw std::runtime_error("Failed to load function: pl_cam_deregister_callback");

        _allFunctionsLoaded = true;
    }

    ~PhotometricsAPIWrapper() {
        if (_hLib) {
            FREE_LIBRARY(_hLib);
        }
    }

    PhotometricsAPIWrapper(const PhotometricsAPIWrapper&) = delete;
    PhotometricsAPIWrapper& operator=(const PhotometricsAPIWrapper&) = delete;

    bool areAllFunctionsLoaded() const { return _allFunctionsLoaded; }

private:
    LIB_HANDLE _hLib = nullptr;
    bool _allFunctionsLoaded = false;
};

inline PhotometricsAPIWrapper GetPhotometricsAPIWrapper() {
    return PhotometricsAPIWrapper("PVCAM64.dll");
}

#endif
