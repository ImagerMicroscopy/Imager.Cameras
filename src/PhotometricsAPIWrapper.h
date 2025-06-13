#ifndef PHOTOMETRICSAPIWRAPPER_H
#define PHOTOMETRICSAPIWRAPPER_H

#include <string>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include "PVCAM/pvcam.h"

class PhotometricsAPIWrapper {
public:
    PhotometricsAPIWrapper(const std::string& libPath) : _isLoaded(false) {
#ifdef _WIN32
        _hLib = LoadLibrary(libPath.c_str());
#else
        _hLib = dlopen(libPath.c_str(), RTLD_LAZY);
#endif
        if (!_hLib) {
            return;
        }

        // Load each function
        _isLoaded = loadFunction(_pl_cam_open, "pl_cam_open") &&
                   loadFunction(_pl_cam_close, "pl_cam_close") &&
                   loadFunction(_pl_get_param, "pl_get_param") &&
                   loadFunction(_pl_set_param, "pl_set_param") &&
                   loadFunction(_pl_get_enum_param, "pl_get_enum_param") &&
                   loadFunction(_pl_enum_str_length, "pl_enum_str_length") &&
                   loadFunction(_pl_exp_setup_cont, "pl_exp_setup_cont") &&
                   loadFunction(_pl_exp_start_cont, "pl_exp_start_cont") &&
                   loadFunction(_pl_exp_check_cont_status, "pl_exp_check_cont_status") &&
                   loadFunction(_pl_exp_stop_cont, "pl_exp_stop_cont") &&
                   loadFunction(_pl_exp_get_oldest_frame, "pl_exp_get_oldest_frame") &&
                   loadFunction(_pl_exp_unlock_oldest_frame, "pl_exp_unlock_oldest_frame") &&
                   loadFunction(_pl_error_code, "pl_error_code") &&
                   loadFunction(_pl_error_message, "pl_error_message") &&
                   loadFunction(_pl_cam_register_callback_ex3, "pl_cam_register_callback_ex3") &&
                   loadFunction(_pl_cam_deregister_callback, "pl_cam_deregister_callback");
    }

    ~PhotometricsAPIWrapper() {
        if (_hLib) {
#ifdef _WIN32
            FreeLibrary((HMODULE)_hLib);
#else
            dlclose(_hLib);
#endif
        }
    }

    PhotometricsAPIWrapper(const PhotometricsAPIWrapper&) = delete;
    PhotometricsAPIWrapper& operator=(const PhotometricsAPIWrapper&) = delete;

    bool areAllFunctionsLoaded() const {
        return _isLoaded;
    }

    // Wrapper functions
    rs_bool pl_cam_open(char* camera_name, int16* hcam, int16 o_mode) {
        if (!_pl_cam_open) throw std::logic_error("Function pl_cam_open not loaded.");
        return _pl_cam_open(camera_name, hcam, o_mode);
    }

    rs_bool pl_cam_close(int16 hcam) {
        if (!_pl_cam_close) throw std::logic_error("Function pl_cam_close not loaded.");
        return _pl_cam_close(hcam);
    }

    rs_bool pl_get_param(int16 hcam, uns32 param_id, int16 param_attribute, void* param_value) {
        if (!_pl_get_param) throw std::logic_error("Function pl_get_param not loaded.");
        return _pl_get_param(hcam, param_id, param_attribute, param_value);
    }

    rs_bool pl_set_param(int16 hcam, uns32 param_id, void* param_value) {
        if (!_pl_set_param) throw std::logic_error("Function pl_set_param not loaded.");
        return _pl_set_param(hcam, param_id, param_value);
    }

    rs_bool pl_get_enum_param(int16 hcam, uns32 param_id, uns32 index, int32* value, char* desc, uns32 length) {
        if (!_pl_get_enum_param) throw std::logic_error("Function pl_get_enum_param not loaded.");
        return _pl_get_enum_param(hcam, param_id, index, value, desc, length);
    }

    rs_bool pl_enum_str_length(int16 hcam, uns32 param_id, uns32 index, uns32* length) {
        if (!_pl_enum_str_length) throw std::logic_error("Function pl_enum_str_length not loaded.");
        return _pl_enum_str_length(hcam, param_id, index, length);
    }

    rs_bool pl_exp_setup_cont(int16 hcam, uns16 rgn_total, const rgn_type* rgn_array, int16 exp_mode, uns32 exposure_time, uns32* exp_bytes, int16 buffer_mode) {
        if (!_pl_exp_setup_cont) throw std::logic_error("Function pl_exp_setup_cont not loaded.");
        return _pl_exp_setup_cont(hcam, rgn_total, rgn_array, exp_mode, exposure_time, exp_bytes, buffer_mode);
    }

    rs_bool pl_exp_start_cont(int16 hcam, void* pixel_stream, uns32 size) {
        if (!_pl_exp_start_cont) throw std::logic_error("Function pl_exp_start_cont not loaded.");
        return _pl_exp_start_cont(hcam, pixel_stream, size);
    }

    rs_bool pl_exp_check_cont_status(int16 hcam, int16* status, uns32* bytes_arrived, uns32* buffer_cnt) {
        if (!_pl_exp_check_cont_status) throw std::logic_error("Function pl_exp_check_cont_status not loaded.");
        return _pl_exp_check_cont_status(hcam, status, bytes_arrived, buffer_cnt);
    }

    rs_bool pl_exp_stop_cont(int16 hcam, int16 cam_state) {
        if (!_pl_exp_stop_cont) throw std::logic_error("Function pl_exp_stop_cont not loaded.");
        return _pl_exp_stop_cont(hcam, cam_state);
    }

    rs_bool pl_exp_get_oldest_frame(int16 hcam, void** frame) {
        if (!_pl_exp_get_oldest_frame) throw std::logic_error("Function pl_exp_get_oldest_frame not loaded.");
        return _pl_exp_get_oldest_frame(hcam, frame);
    }

    rs_bool pl_exp_unlock_oldest_frame(int16 hcam) {
        if (!_pl_exp_unlock_oldest_frame) throw std::logic_error("Function pl_exp_unlock_oldest_frame not loaded.");
        return _pl_exp_unlock_oldest_frame(hcam);
    }

    int16 pl_error_code() {
        if (!_pl_error_code) throw std::logic_error("Function pl_error_code not loaded.");
        return _pl_error_code();
    }

    rs_bool pl_error_message(int16 err_code, char* msg) {
        if (!_pl_error_message) throw std::logic_error("Function pl_error_message not loaded.");
        return _pl_error_message(err_code, msg);
    }

    rs_bool pl_cam_register_callback_ex3(int16 hcam, int32 callback_event, void* callback, void* context) {
        if (!_pl_cam_register_callback_ex3) throw std::logic_error("Function pl_cam_register_callback_ex3 not loaded.");
        return _pl_cam_register_callback_ex3(hcam, callback_event, callback, context);
    }

    rs_bool pl_cam_deregister_callback(int16 hcam, int32 callback_event) {
        if (!_pl_cam_deregister_callback) throw std::logic_error("Function pl_cam_deregister_callback not loaded.");
        return _pl_cam_deregister_callback(hcam, callback_event);
    }

private:
    void* _hLib;
    bool _isLoaded;

    // Define function pointer types
    typedef rs_bool (*pl_cam_open_t)(char*, int16*, int16);
    typedef rs_bool (*pl_cam_close_t)(int16);
    typedef rs_bool (*pl_get_param_t)(int16, uns32, int16, void*);
    typedef rs_bool (*pl_set_param_t)(int16, uns32, void*);
    typedef rs_bool (*pl_get_enum_param_t)(int16, uns32, uns32, int32*, char*, uns32);
    typedef rs_bool (*pl_enum_str_length_t)(int16, uns32, uns32, uns32*);
    typedef rs_bool (*pl_exp_setup_cont_t)(int16, uns16, const rgn_type*, int16, uns32, uns32*, int16);
    typedef rs_bool (*pl_exp_start_cont_t)(int16, void*, uns32);
    typedef rs_bool (*pl_exp_check_cont_status_t)(int16, int16*, uns32*, uns32*);
    typedef rs_bool (*pl_exp_stop_cont_t)(int16, int16);
    typedef rs_bool (*pl_exp_get_oldest_frame_t)(int16, void**);
    typedef rs_bool (*pl_exp_unlock_oldest_frame_t)(int16);
    typedef int16 (*pl_error_code_t)();
    typedef rs_bool (*pl_error_message_t)(int16, char*);
    typedef rs_bool (*pl_cam_register_callback_ex3_t)(int16, int32, void*, void*);
    typedef rs_bool (*pl_cam_deregister_callback_t)(int16, int32);

    // Function pointers as member variables
    pl_cam_open_t _pl_cam_open;
    pl_cam_close_t _pl_cam_close;
    pl_get_param_t _pl_get_param;
    pl_set_param_t _pl_set_param;
    pl_get_enum_param_t _pl_get_enum_param;
    pl_enum_str_length_t _pl_enum_str_length;
    pl_exp_setup_cont_t _pl_exp_setup_cont;
    pl_exp_start_cont_t _pl_exp_start_cont;
    pl_exp_check_cont_status_t _pl_exp_check_cont_status;
    pl_exp_stop_cont_t _pl_exp_stop_cont;
    pl_exp_get_oldest_frame_t _pl_exp_get_oldest_frame;
    pl_exp_unlock_oldest_frame_t _pl_exp_unlock_oldest_frame;
    pl_error_code_t _pl_error_code;
    pl_error_message_t _pl_error_message;
    pl_cam_register_callback_ex3_t _pl_cam_register_callback_ex3;
    pl_cam_deregister_callback_t _pl_cam_deregister_callback;

    template <typename T>
    bool loadFunction(T& funcPtr, const std::string& funcName) {
#ifdef _WIN32
        FARPROC procAddress = GetProcAddress((HMODULE)_hLib, funcName.c_str());
#else
        void* procAddress = dlsym(_hLib, funcName.c_str());
#endif
        if (!procAddress) {
            return false;
        }
        funcPtr = reinterpret_cast<T>(procAddress);
        return true;
    }
};

inline PhotometricsAPIWrapper GetPhotometricsAPIWrapper() {
    return PhotometricsAPIWrapper("PVCAM64.dll");
}

#endif
