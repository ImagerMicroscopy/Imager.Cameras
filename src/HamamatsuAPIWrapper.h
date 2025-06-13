#ifndef HAMAMATSUAPIWRAPPER_H
#define HAMAMATSUAPIWRAPPER_H

#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <map>
#include <stdexcept> // For std::runtime_error

#include "Hamamatsu/dcamapi4.h"

// Platform-specific includes and definitions
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

// Macro to define function pointer types
#define DEFINE_FUNC_PTR(name, return_type, ...) \
    using name##_func = return_type(DCAMAPI *)(__VA_ARGS__);

// Define function pointer types for each function in the library
DEFINE_FUNC_PTR(dcamapi_init, DCAMERR, DCAMAPI_INIT*);
DEFINE_FUNC_PTR(dcamapi_uninit, DCAMERR, void);
DEFINE_FUNC_PTR(dcamdev_open, DCAMERR, DCAMDEV_OPEN*);
DEFINE_FUNC_PTR(dcamdev_close, DCAMERR, HDCAM);
DEFINE_FUNC_PTR(dcamdev_showpanel, DCAMERR, HDCAM, int32);
DEFINE_FUNC_PTR(dcamdev_getcapability, DCAMERR, HDCAM, DCAMDEV_CAPABILITY*);
DEFINE_FUNC_PTR(dcamdev_getstring, DCAMERR, HDCAM, DCAMDEV_STRING*);
DEFINE_FUNC_PTR(dcamdev_setdata, DCAMERR, HDCAM, DCAMDATA_HDR*);
DEFINE_FUNC_PTR(dcamdev_getdata, DCAMERR, HDCAM, DCAMDATA_HDR*);

DEFINE_FUNC_PTR(dcambuf_alloc, DCAMERR, HDCAM, int32);
DEFINE_FUNC_PTR(dcambuf_attach, DCAMERR, HDCAM, const DCAMBUF_ATTACH*);
DEFINE_FUNC_PTR(dcambuf_release, DCAMERR, HDCAM, int32);
DEFINE_FUNC_PTR(dcambuf_lockframe, DCAMERR, HDCAM, DCAMBUF_FRAME*);
DEFINE_FUNC_PTR(dcambuf_copyframe, DCAMERR, HDCAM, DCAMBUF_FRAME*);
DEFINE_FUNC_PTR(dcambuf_copymetadata, DCAMERR, HDCAM, DCAM_METADATAHDR*);

DEFINE_FUNC_PTR(dcamcap_start, DCAMERR, HDCAM, int32);
DEFINE_FUNC_PTR(dcamcap_stop, DCAMERR, HDCAM);
DEFINE_FUNC_PTR(dcamcap_status, DCAMERR, HDCAM, int32*);
DEFINE_FUNC_PTR(dcamcap_transferinfo, DCAMERR, HDCAM, DCAMCAP_TRANSFERINFO*);
DEFINE_FUNC_PTR(dcamcap_firetrigger, DCAMERR, HDCAM, int32);
DEFINE_FUNC_PTR(dcamcap_record, DCAMERR, HDCAM, HDCAMREC);

DEFINE_FUNC_PTR(dcamwait_open, DCAMERR, DCAMWAIT_OPEN*);
DEFINE_FUNC_PTR(dcamwait_close, DCAMERR, HDCAMWAIT);
DEFINE_FUNC_PTR(dcamwait_start, DCAMERR, HDCAMWAIT, DCAMWAIT_START*);
DEFINE_FUNC_PTR(dcamwait_abort, DCAMERR, HDCAMWAIT);

DEFINE_FUNC_PTR(dcamprop_getattr, DCAMERR, HDCAM, DCAMPROP_ATTR*);
DEFINE_FUNC_PTR(dcamprop_getvalue, DCAMERR, HDCAM, int32, double*);
DEFINE_FUNC_PTR(dcamprop_setvalue, DCAMERR, HDCAM, int32, double);
DEFINE_FUNC_PTR(dcamprop_setgetvalue, DCAMERR, HDCAM, int32, double*, int32);
DEFINE_FUNC_PTR(dcamprop_queryvalue, DCAMERR, HDCAM, int32, double*, int32);
DEFINE_FUNC_PTR(dcamprop_getnextid, DCAMERR, HDCAM, int32*, int32);
DEFINE_FUNC_PTR(dcamprop_getname, DCAMERR, HDCAM, int32, char*, int32);
DEFINE_FUNC_PTR(dcamprop_getvaluetext, DCAMERR, HDCAM, DCAMPROP_VALUETEXT*);

class HamamatsuAPIWrapper {
public:
    explicit HamamatsuAPIWrapper(const std::string& libraryPath) : _allFunctionsLoaded(true) {
        // Load the library
        _dllHandle = LOAD_LIBRARY(libraryPath.c_str());
        if (!_dllHandle) {
            _allFunctionsLoaded = false;
            // the Hamamatsu API is not installed
        }

        // Define a map of function names to their raw pointer locations
        // This avoids std::any and simplifies the map
        std::map<std::string, void**> functionPointersToLoad = {
            {"dcamapi_init", (void**)&_dcamapi_init},
            {"dcamapi_uninit", (void**)&_dcamapi_uninit},
            {"dcamdev_open", (void**)&_dcamdev_open},
            {"dcamdev_close", (void**)&_dcamdev_close},
            {"dcamdev_showpanel", (void**)&_dcamdev_showpanel},
            {"dcamdev_getcapability", (void**)&_dcamdev_getcapability},
            {"dcamdev_getstring", (void**)&_dcamdev_getstring},
            {"dcamdev_setdata", (void**)&_dcamdev_setdata},
            {"dcamdev_getdata", (void**)&_dcamdev_getdata},
            {"dcambuf_alloc", (void**)&_dcambuf_alloc},
            {"dcambuf_attach", (void**)&_dcambuf_attach},
            {"dcambuf_release", (void**)&_dcambuf_release},
            {"dcambuf_lockframe", (void**)&_dcambuf_lockframe},
            {"dcambuf_copyframe", (void**)&_dcambuf_copyframe},
            {"dcambuf_copymetadata", (void**)&_dcambuf_copymetadata},
            {"dcamcap_start", (void**)&_dcamcap_start},
            {"dcamcap_stop", (void**)&_dcamcap_stop},
            {"dcamcap_status", (void**)&_dcamcap_status},
            {"dcamcap_transferinfo", (void**)&_dcamcap_transferinfo},
            {"dcamcap_firetrigger", (void**)&_dcamcap_firetrigger},
            {"dcamcap_record", (void**)&_dcamcap_record},
            {"dcamwait_open", (void**)&_dcamwait_open},
            {"dcamwait_close", (void**)&_dcamwait_close},
            {"dcamwait_start", (void**)&_dcamwait_start},
            {"dcamwait_abort", (void**)&_dcamwait_abort},
            {"dcamprop_getattr", (void**)&_dcamprop_getattr},
            {"dcamprop_getvalue", (void**)&_dcamprop_getvalue},
            {"dcamprop_setvalue", (void**)&_dcamprop_setvalue},
            {"dcamprop_setgetvalue", (void**)&_dcamprop_setgetvalue},
            {"dcamprop_queryvalue", (void**)&_dcamprop_queryvalue},
            {"dcamprop_getnextid", (void**)&_dcamprop_getnextid},
            {"dcamprop_getname", (void**)&_dcamprop_getname},
            {"dcamprop_getvaluetext", (void**)&_dcamprop_getvaluetext},
        };

        // Load each function
        for (auto const& [funcName, funcPtrAddress] : functionPointersToLoad) {
            void* loadedFuncPtr = GET_PROC_ADDRESS(_dllHandle, funcName.c_str());
            if (!loadedFuncPtr) {
                _allFunctionsLoaded = false;
                continue;
            }
            *funcPtrAddress = loadedFuncPtr;
        }
    }

    // Delete the copy constructor to prevent copying
    HamamatsuAPIWrapper(const HamamatsuAPIWrapper&) = delete;
    // Optionally, delete the copy assignment operator as well
    HamamatsuAPIWrapper& operator=(const HamamatsuAPIWrapper&) = delete;

    ~HamamatsuAPIWrapper() {
        if (_dllHandle) {
            FREE_LIBRARY(_dllHandle);
            _dllHandle = nullptr; // Clear handle after freeing
        }
    }

    // Accessor function for _allFunctionsLoaded
    bool areAllFunctionsLoaded() const {
        return _allFunctionsLoaded;
    }

    // Wrapper methods for each function
    DCAMERR dcamapi_init(DCAMAPI_INIT* param) {
        if (!_dcamapi_init) {
            throw std::logic_error("Function dcamapi_init is not loaded.");
        }
        return _dcamapi_init(param);
    }

    DCAMERR dcamapi_uninit() {
        if (!_dcamapi_uninit) {
            throw std::logic_error("Function dcamapi_uninit is not loaded.");
        }
        return _dcamapi_uninit();
    }

    DCAMERR dcamdev_open(DCAMDEV_OPEN* param) {
        if (!_dcamdev_open) {
            throw std::logic_error("Function dcamdev_open is not loaded.");
        }
        return _dcamdev_open(param);
    }

    DCAMERR dcamdev_close(HDCAM h) {
        if (!_dcamdev_close) {
            throw std::logic_error("Function dcamdev_close is not loaded.");
        }
        return _dcamdev_close(h);
    }

    DCAMERR dcamdev_showpanel(HDCAM h, int32 iKind) {
        if (!_dcamdev_showpanel) {
            throw std::logic_error("Function dcamdev_showpanel is not loaded.");
        }
        return _dcamdev_showpanel(h, iKind);
    }

    DCAMERR dcamdev_getcapability(HDCAM h, DCAMDEV_CAPABILITY* param) {
        if (!_dcamdev_getcapability) {
            throw std::logic_error("Function dcamdev_getcapability is not loaded.");
        }
        return _dcamdev_getcapability(h, param);
    }

    DCAMERR dcamdev_getstring(HDCAM h, DCAMDEV_STRING* param) {
        if (!_dcamdev_getstring) {
            throw std::logic_error("Function dcamdev_getstring is not loaded.");
        }
        return _dcamdev_getstring(h, param);
    }

    DCAMERR dcamdev_setdata(HDCAM h, DCAMDATA_HDR* param) {
        if (!_dcamdev_setdata) {
            throw std::logic_error("Function dcamdev_setdata is not loaded.");
        }
        return _dcamdev_setdata(h, param);
    }

    DCAMERR dcamdev_getdata(HDCAM h, DCAMDATA_HDR* param) {
        if (!_dcamdev_getdata) {
            throw std::logic_error("Function dcamdev_getdata is not loaded.");
        }
        return _dcamdev_getdata(h, param);
    }

    DCAMERR dcambuf_alloc(HDCAM h, int32 framecount) {
        if (!_dcambuf_alloc) {
            throw std::logic_error("Function dcambuf_alloc is not loaded.");
        }
        return _dcambuf_alloc(h, framecount);
    }

    DCAMERR dcambuf_attach(HDCAM h, const DCAMBUF_ATTACH* param) {
        if (!_dcambuf_attach) {
            throw std::logic_error("Function dcambuf_attach is not loaded.");
        }
        return _dcambuf_attach(h, param);
    }

    DCAMERR dcambuf_release(HDCAM h, int32 iKind) {
        if (!_dcambuf_release) {
            throw std::logic_error("Function dcambuf_release is not loaded.");
        }
        return _dcambuf_release(h, iKind);
    }

    DCAMERR dcambuf_lockframe(HDCAM h, DCAMBUF_FRAME* pFrame) {
        if (!_dcambuf_lockframe) {
            throw std::logic_error("Function dcambuf_lockframe is not loaded.");
        }
        return _dcambuf_lockframe(h, pFrame);
    }

    DCAMERR dcambuf_copyframe(HDCAM h, DCAMBUF_FRAME* pFrame) {
        if (!_dcambuf_copyframe) {
            throw std::logic_error("Function dcambuf_copyframe is not loaded.");
        }
        return _dcambuf_copyframe(h, pFrame);
    }

    DCAMERR dcambuf_copymetadata(HDCAM h, DCAM_METADATAHDR* hdr) {
        if (!_dcambuf_copymetadata) {
            throw std::logic_error("Function dcambuf_copymetadata is not loaded.");
        }
        return _dcambuf_copymetadata(h, hdr);
    }

    DCAMERR dcamcap_start(HDCAM h, int32 mode) {
        if (!_dcamcap_start) {
            throw std::logic_error("Function dcamcap_start is not loaded.");
        }
        return _dcamcap_start(h, mode);
    }

    DCAMERR dcamcap_stop(HDCAM h) {
        if (!_dcamcap_stop) {
            throw std::logic_error("Function dcamcap_stop is not loaded.");
        }
        return _dcamcap_stop(h);
    }

    DCAMERR dcamcap_status(HDCAM h, int32* pStatus) {
        if (!_dcamcap_status) {
            throw std::logic_error("Function dcamcap_status is not loaded.");
        }
        return _dcamcap_status(h, pStatus);
    }

    DCAMERR dcamcap_transferinfo(HDCAM h, DCAMCAP_TRANSFERINFO* param) {
        if (!_dcamcap_transferinfo) {
            throw std::logic_error("Function dcamcap_transferinfo is not loaded.");
        }
        return _dcamcap_transferinfo(h, param);
    }

    DCAMERR dcamcap_firetrigger(HDCAM h, int32 iKind) {
        if (!_dcamcap_firetrigger) {
            throw std::logic_error("Function dcamcap_firetrigger is not loaded.");
        }
        return _dcamcap_firetrigger(h, iKind);
    }

    DCAMERR dcamcap_record(HDCAM h, HDCAMREC hrec) {
        if (!_dcamcap_record) {
            throw std::logic_error("Function dcamcap_record is not loaded.");
        }
        return _dcamcap_record(h, hrec);
    }

    DCAMERR dcamwait_open(DCAMWAIT_OPEN* param) {
        if (!_dcamwait_open) {
            throw std::logic_error("Function dcamwait_open is not loaded.");
        }
        return _dcamwait_open(param);
    }

    DCAMERR dcamwait_close(HDCAMWAIT hWait) {
        if (!_dcamwait_close) {
            throw std::logic_error("Function dcamwait_close is not loaded.");
        }
        return _dcamwait_close(hWait);
    }

    DCAMERR dcamwait_start(HDCAMWAIT hWait, DCAMWAIT_START* param) {
        if (!_dcamwait_start) {
            throw std::logic_error("Function dcamwait_start is not loaded.");
        }
        return _dcamwait_start(hWait, param);
    }

    DCAMERR dcamwait_abort(HDCAMWAIT hWait) {
        if (!_dcamwait_abort) {
            throw std::logic_error("Function dcamwait_abort is not loaded.");
        }
        return _dcamwait_abort(hWait);
    }

    DCAMERR dcamprop_getattr(HDCAM h, DCAMPROP_ATTR* param) {
        if (!_dcamprop_getattr) {
            throw std::logic_error("Function dcamprop_getattr is not loaded.");
        }
        return _dcamprop_getattr(h, param);
    }

    DCAMERR dcamprop_getvalue(HDCAM h, int32 iProp, double* pValue) {
        if (!_dcamprop_getvalue) {
            throw std::logic_error("Function dcamprop_getvalue is not loaded.");
        }
        return _dcamprop_getvalue(h, iProp, pValue);
    }

    DCAMERR dcamprop_setvalue(HDCAM h, int32 iProp, double fValue) {
        if (!_dcamprop_setvalue) {
            throw std::logic_error("Function dcamprop_setvalue is not loaded.");
        }
        return _dcamprop_setvalue(h, iProp, fValue);
    }

    DCAMERR dcamprop_setgetvalue(HDCAM h, int32 iProp, double* pValue, int32 option) {
        if (!_dcamprop_setgetvalue) {
            throw std::logic_error("Function dcamprop_setgetvalue is not loaded.");
        }
        return _dcamprop_setgetvalue(h, iProp, pValue, option);
    }

    DCAMERR dcamprop_queryvalue(HDCAM h, int32 iProp, double* pValue, int32 option) {
        if (!_dcamprop_queryvalue) {
            throw std::logic_error("Function dcamprop_queryvalue is not loaded.");
        }
        return _dcamprop_queryvalue(h, iProp, pValue, option);
    }

    DCAMERR dcamprop_getnextid(HDCAM h, int32* pProp, int32 option) {
        if (!_dcamprop_getnextid) {
            throw std::logic_error("Function dcamprop_getnextid is not loaded.");
        }
        return _dcamprop_getnextid(h, pProp, option);
    }

    DCAMERR dcamprop_getname(HDCAM h, int32 iProp, char* text, int32 textbytes) {
        if (!_dcamprop_getname) {
            throw std::logic_error("Function dcamprop_getname is not loaded.");
        }
        return _dcamprop_getname(h, iProp, text, textbytes);
    }

    DCAMERR dcamprop_getvaluetext(HDCAM h, DCAMPROP_VALUETEXT* param) {
        if (!_dcamprop_getvaluetext) {
            throw std::logic_error("Function dcamprop_getvaluetext is not loaded.");
        }
        return _dcamprop_getvaluetext(h, param);
    }

private:
    LIB_HANDLE _dllHandle = nullptr; // Renamed from 'handle'
    bool _allFunctionsLoaded; // Made private and renamed

    // Function pointers with leading underscore
    dcamapi_init_func _dcamapi_init = nullptr;
    dcamapi_uninit_func _dcamapi_uninit = nullptr;
    dcamdev_open_func _dcamdev_open = nullptr;
    dcamdev_close_func _dcamdev_close = nullptr;
    dcamdev_showpanel_func _dcamdev_showpanel = nullptr;
    dcamdev_getcapability_func _dcamdev_getcapability = nullptr;
    dcamdev_getstring_func _dcamdev_getstring = nullptr;
    dcamdev_setdata_func _dcamdev_setdata = nullptr;
    dcamdev_getdata_func _dcamdev_getdata = nullptr;
    dcambuf_alloc_func _dcambuf_alloc = nullptr;
    dcambuf_attach_func _dcambuf_attach = nullptr;
    dcambuf_release_func _dcambuf_release = nullptr;
    dcambuf_lockframe_func _dcambuf_lockframe = nullptr;
    dcambuf_copyframe_func _dcambuf_copyframe = nullptr;
    dcambuf_copymetadata_func _dcambuf_copymetadata = nullptr;
    dcamcap_start_func _dcamcap_start = nullptr;
    dcamcap_stop_func _dcamcap_stop = nullptr;
    dcamcap_status_func _dcamcap_status = nullptr;
    dcamcap_transferinfo_func _dcamcap_transferinfo = nullptr;
    dcamcap_firetrigger_func _dcamcap_firetrigger = nullptr;
    dcamcap_record_func _dcamcap_record = nullptr;
    dcamwait_open_func _dcamwait_open = nullptr;
    dcamwait_close_func _dcamwait_close = nullptr;
    dcamwait_start_func _dcamwait_start = nullptr;
    dcamwait_abort_func _dcamwait_abort = nullptr;
    dcamprop_getattr_func _dcamprop_getattr = nullptr;
    dcamprop_getvalue_func _dcamprop_getvalue = nullptr;
    dcamprop_setvalue_func _dcamprop_setvalue = nullptr;
    dcamprop_setgetvalue_func _dcamprop_setgetvalue = nullptr;
    dcamprop_queryvalue_func _dcamprop_queryvalue = nullptr;
    dcamprop_getnextid_func _dcamprop_getnextid = nullptr;
    dcamprop_getname_func _dcamprop_getname = nullptr;
    dcamprop_getvaluetext_func _dcamprop_getvaluetext = nullptr;
};

inline HamamatsuAPIWrapper GetHamamatsuAPIWrapper() {
    return HamamatsuAPIWrapper("dcamapi.dll");
}

#endif
