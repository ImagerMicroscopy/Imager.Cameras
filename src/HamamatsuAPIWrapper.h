#ifndef HAMAMATSUAPIWRAPPER_H
#define HAMAMATSUAPIWRAPPER_H

#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <any>

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
    using name##_func = return_type(__stdcall*)(__VA_ARGS__);

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

// Define function pointer types for dcambuf functions
DEFINE_FUNC_PTR(dcambuf_alloc, DCAMERR, HDCAM, int32);
DEFINE_FUNC_PTR(dcambuf_attach, DCAMERR, HDCAM, const DCAMBUF_ATTACH*);
DEFINE_FUNC_PTR(dcambuf_release, DCAMERR, HDCAM, int32);
DEFINE_FUNC_PTR(dcambuf_lockframe, DCAMERR, HDCAM, DCAMBUF_FRAME*);
DEFINE_FUNC_PTR(dcambuf_copyframe, DCAMERR, HDCAM, DCAMBUF_FRAME*);
DEFINE_FUNC_PTR(dcambuf_copymetadata, DCAMERR, HDCAM, DCAM_METADATAHDR*);

// Define function pointer types for dcamcap functions
DEFINE_FUNC_PTR(dcamcap_start, DCAMERR, HDCAM, int32);
DEFINE_FUNC_PTR(dcamcap_stop, DCAMERR, HDCAM);
DEFINE_FUNC_PTR(dcamcap_status, DCAMERR, HDCAM, int32*);
DEFINE_FUNC_PTR(dcamcap_transferinfo, DCAMERR, HDCAM, DCAMCAP_TRANSFERINFO*);
DEFINE_FUNC_PTR(dcamcap_firetrigger, DCAMERR, HDCAM, int32);
DEFINE_FUNC_PTR(dcamcap_record, DCAMERR, HDCAM, HDCAMREC);

// Define function pointer types for dcamwait functions
DEFINE_FUNC_PTR(dcamwait_open, DCAMERR, DCAMWAIT_OPEN*);
DEFINE_FUNC_PTR(dcamwait_close, DCAMERR, HDCAMWAIT);
DEFINE_FUNC_PTR(dcamwait_start, DCAMERR, HDCAMWAIT, DCAMWAIT_START*);
DEFINE_FUNC_PTR(dcamwait_abort, DCAMERR, HDCAMWAIT);

// Define function pointer types for dcamprop functions
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
    bool allFunctionsLoaded;

    HamamatsuAPIWrapper(const std::string& libraryPath) : allFunctionsLoaded(true) {
        // Load the library
        handle = LOAD_LIBRARY(libraryPath.c_str());
        if (!handle) {
            allFunctionsLoaded = false;
            return;
        }

        // Define a map of function names to their pointer types
        functionMap = {
            {"dcamapi_init", std::make_pair(std::any(dcamapi_init_func()), (void**)&dcamapi_init)},
            {"dcamapi_uninit", std::make_pair(std::any(dcamapi_uninit_func()), (void**)&dcamapi_uninit)},
            {"dcamdev_open", std::make_pair(std::any(dcamdev_open_func()), (void**)&dcamdev_open)},
            {"dcamdev_close", std::make_pair(std::any(dcamdev_close_func()), (void**)&dcamdev_close)},
            {"dcamdev_showpanel", std::make_pair(std::any(dcamdev_showpanel_func()), (void**)&dcamdev_showpanel)},
            {"dcamdev_getcapability", std::make_pair(std::any(dcamdev_getcapability_func()), (void**)&dcamdev_getcapability)},
            {"dcamdev_getstring", std::make_pair(std::any(dcamdev_getstring_func()), (void**)&dcamdev_getstring)},
            {"dcamdev_setdata", std::make_pair(std::any(dcamdev_setdata_func()), (void**)&dcamdev_setdata)},
            {"dcamdev_getdata", std::make_pair(std::any(dcamdev_getdata_func()), (void**)&dcamdev_getdata)},
            {"dcambuf_alloc", std::make_pair(std::any(dcambuf_alloc_func()), (void**)&dcambuf_alloc)},
            {"dcambuf_attach", std::make_pair(std::any(dcambuf_attach_func()), (void**)&dcambuf_attach)},
            {"dcambuf_release", std::make_pair(std::any(dcambuf_release_func()), (void**)&dcambuf_release)},
            {"dcambuf_lockframe", std::make_pair(std::any(dcambuf_lockframe_func()), (void**)&dcambuf_lockframe)},
            {"dcambuf_copyframe", std::make_pair(std::any(dcambuf_copyframe_func()), (void**)&dcambuf_copyframe)},
            {"dcambuf_copymetadata", std::make_pair(std::any(dcambuf_copymetadata_func()), (void**)&dcambuf_copymetadata)},
            {"dcamcap_start", std::make_pair(std::any(dcamcap_start_func()), (void**)&dcamcap_start)},
            {"dcamcap_stop", std::make_pair(std::any(dcamcap_stop_func()), (void**)&dcamcap_stop)},
            {"dcamcap_status", std::make_pair(std::any(dcamcap_status_func()), (void**)&dcamcap_status)},
            {"dcamcap_transferinfo", std::make_pair(std::any(dcamcap_transferinfo_func()), (void**)&dcamcap_transferinfo)},
            {"dcamcap_firetrigger", std::make_pair(std::any(dcamcap_firetrigger_func()), (void**)&dcamcap_firetrigger)},
            {"dcamcap_record", std::make_pair(std::any(dcamcap_record_func()), (void**)&dcamcap_record)},
            {"dcamwait_open", std::make_pair(std::any(dcamwait_open_func()), (void**)&dcamwait_open)},
            {"dcamwait_close", std::make_pair(std::any(dcamwait_close_func()), (void**)&dcamwait_close)},
            {"dcamwait_start", std::make_pair(std::any(dcamwait_start_func()), (void**)&dcamwait_start)},
            {"dcamwait_abort", std::make_pair(std::any(dcamwait_abort_func()), (void**)&dcamwait_abort)},
            {"dcamprop_getattr", std::make_pair(std::any(dcamprop_getattr_func()), (void**)&dcamprop_getattr)},
            {"dcamprop_getvalue", std::make_pair(std::any(dcamprop_getvalue_func()), (void**)&dcamprop_getvalue)},
            {"dcamprop_setvalue", std::make_pair(std::any(dcamprop_setvalue_func()), (void**)&dcamprop_setvalue)},
            {"dcamprop_setgetvalue", std::make_pair(std::any(dcamprop_setgetvalue_func()), (void**)&dcamprop_setgetvalue)},
            {"dcamprop_queryvalue", std::make_pair(std::any(dcamprop_queryvalue_func()), (void**)&dcamprop_queryvalue)},
            {"dcamprop_getnextid", std::make_pair(std::any(dcamprop_getnextid_func()), (void**)&dcamprop_getnextid)},
            {"dcamprop_getname", std::make_pair(std::any(dcamprop_getname_func()), (void**)&dcamprop_getname)},
            {"dcamprop_getvaluetext", std::make_pair(std::any(dcamprop_getvaluetext_func()), (void**)&dcamprop_getvaluetext)},
        };

        // Load each function
        for (auto& func : functionMap) {
            void* funcPtr = GET_PROC_ADDRESS(handle, func.first.c_str());
            if (!funcPtr) {
                allFunctionsLoaded = false;
                continue;
            }
            *func.second.second = reinterpret_cast<LIB_HANDLE>(funcPtr);
        }
    }

    ~HamamatsuAPIWrapper() {
        if (handle) {
            FREE_LIBRARY(handle);
        }
    }

    // Wrapper methods for each function
    DCAMERR dcamapi_init(DCAMAPI_INIT* param) {
        return dcamapi_init(param);
    }

    DCAMERR dcamapi_uninit() {
        return dcamapi_uninit();
    }

    DCAMERR dcamdev_open(DCAMDEV_OPEN* param) {
        return dcamdev_open(param);
    }

    DCAMERR dcamdev_close(HDCAM h) {
        return dcamdev_close(h);
    }

    DCAMERR dcamdev_showpanel(HDCAM h, int32 iKind) {
        return dcamdev_showpanel(h, iKind);
    }

    DCAMERR dcamdev_getcapability(HDCAM h, DCAMDEV_CAPABILITY* param) {
        return dcamdev_getcapability(h, param);
    }

    DCAMERR dcamdev_getstring(HDCAM h, DCAMDEV_STRING* param) {
        return dcamdev_getstring(h, param);
    }

    DCAMERR dcamdev_setdata(HDCAM h, DCAMDATA_HDR* param) {
        return dcamdev_setdata(h, param);
    }

    DCAMERR dcamdev_getdata(HDCAM h, DCAMDATA_HDR* param) {
        return dcamdev_getdata(h, param);
    }

    DCAMERR dcambuf_alloc(HDCAM h, int32 framecount) {
        return dcambuf_alloc(h, framecount);
    }

    DCAMERR dcambuf_attach(HDCAM h, const DCAMBUF_ATTACH* param) {
        return dcambuf_attach(h, param);
    }

    DCAMERR dcambuf_release(HDCAM h, int32 iKind) {
        return dcambuf_release(h, iKind);
    }

    DCAMERR dcambuf_lockframe(HDCAM h, DCAMBUF_FRAME* pFrame) {
        return dcambuf_lockframe(h, pFrame);
    }

    DCAMERR dcambuf_copyframe(HDCAM h, DCAMBUF_FRAME* pFrame) {
        return dcambuf_copyframe(h, pFrame);
    }

    DCAMERR dcambuf_copymetadata(HDCAM h, DCAM_METADATAHDR* hdr) {
        return dcambuf_copymetadata(h, hdr);
    }

    DCAMERR dcamcap_start(HDCAM h, int32 mode) {
        return dcamcap_start(h, mode);
    }

    DCAMERR dcamcap_stop(HDCAM h) {
        return dcamcap_stop(h);
    }

    DCAMERR dcamcap_status(HDCAM h, int32* pStatus) {
        return dcamcap_status(h, pStatus);
    }

    DCAMERR dcamcap_transferinfo(HDCAM h, DCAMCAP_TRANSFERINFO* param) {
        return dcamcap_transferinfo(h, param);
    }

    DCAMERR dcamcap_firetrigger(HDCAM h, int32 iKind) {
        return dcamcap_firetrigger(h, iKind);
    }

    DCAMERR dcamcap_record(HDCAM h, HDCAMREC hrec) {
        return dcamcap_record(h, hrec);
    }

    DCAMERR dcamwait_open(DCAMWAIT_OPEN* param) {
        return dcamwait_open(param);
    }

    DCAMERR dcamwait_close(HDCAMWAIT hWait) {
        return dcamwait_close(hWait);
    }

    DCAMERR dcamwait_start(HDCAMWAIT hWait, DCAMWAIT_START* param) {
        return dcamwait_start(hWait, param);
    }

    DCAMERR dcamwait_abort(HDCAMWAIT hWait) {
        return dcamwait_abort(hWait);
    }

    DCAMERR dcamprop_getattr(HDCAM h, DCAMPROP_ATTR* param) {
        return dcamprop_getattr(h, param);
    }

    DCAMERR dcamprop_getvalue(HDCAM h, int32 iProp, double* pValue) {
        return dcamprop_getvalue(h, iProp, pValue);
    }

    DCAMERR dcamprop_setvalue(HDCAM h, int32 iProp, double fValue) {
        return dcamprop_setvalue(h, iProp, fValue);
    }

    DCAMERR dcamprop_setgetvalue(HDCAM h, int32 iProp, double* pValue, int32 option) {
        return dcamprop_setgetvalue(h, iProp, pValue, option);
    }

    DCAMERR dcamprop_queryvalue(HDCAM h, int32 iProp, double* pValue, int32 option) {
        return dcamprop_queryvalue(h, iProp, pValue, option);
    }

    DCAMERR dcamprop_getnextid(HDCAM h, int32* pProp, int32 option) {
        return dcamprop_getnextid(h, pProp, option);
    }

    DCAMERR dcamprop_getname(HDCAM h, int32 iProp, char* text, int32 textbytes) {
        return dcamprop_getname(h, iProp, text, textbytes);
    }

    DCAMERR dcamprop_getvaluetext(HDCAM h, DCAMPROP_VALUETEXT* param) {
        return dcamprop_getvaluetext(h, param);
    }

private:
    LIB_HANDLE handle;
    std::map<std::string, std::pair<std::any, void**>> functionMap;

    // Function pointers
    dcamapi_init_func dcamapi_init;
    dcamapi_uninit_func dcamapi_uninit;
    dcamdev_open_func dcamdev_open;
    dcamdev_close_func dcamdev_close;
    dcamdev_showpanel_func dcamdev_showpanel;
    dcamdev_getcapability_func dcamdev_getcapability;
    dcamdev_getstring_func dcamdev_getstring;
    dcamdev_setdata_func dcamdev_setdata;
    dcamdev_getdata_func dcamdev_getdata;
    dcambuf_alloc_func dcambuf_alloc;
    dcambuf_attach_func dcambuf_attach;
    dcambuf_release_func dcambuf_release;
    dcambuf_lockframe_func dcambuf_lockframe;
    dcambuf_copyframe_func dcambuf_copyframe;
    dcambuf_copymetadata_func dcambuf_copymetadata;
    dcamcap_start_func dcamcap_start;
    dcamcap_stop_func dcamcap_stop;
    dcamcap_status_func dcamcap_status;
    dcamcap_transferinfo_func dcamcap_transferinfo;
    dcamcap_firetrigger_func dcamcap_firetrigger;
    dcamcap_record_func dcamcap_record;
    dcamwait_open_func dcamwait_open;
    dcamwait_close_func dcamwait_close;
    dcamwait_start_func dcamwait_start;
    dcamwait_abort_func dcamwait_abort;
    dcamprop_getattr_func dcamprop_getattr;
    dcamprop_getvalue_func dcamprop_getvalue;
    dcamprop_setvalue_func dcamprop_setvalue;
    dcamprop_setgetvalue_func dcamprop_setgetvalue;
    dcamprop_queryvalue_func dcamprop_queryvalue;
    dcamprop_getnextid_func dcamprop_getnextid;
    dcamprop_getname_func dcamprop_getname;
    dcamprop_getvaluetext_func dcamprop_getvaluetext;
};

HamamatsuAPIWrapper GetHamamatsuAPIWrapper();

#endif
