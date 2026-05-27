#ifndef HAMAMATSUAPIWRAPPER_H
#define HAMAMATSUAPIWRAPPER_H

#include <string>
#include <stdexcept>

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

class HamamatsuAPIWrapper {
public:
    decltype(&::dcamapi_init) dcamapi_init = nullptr;
    decltype(&::dcamapi_uninit) dcamapi_uninit = nullptr;
    decltype(&::dcamdev_open) dcamdev_open = nullptr;
    decltype(&::dcamdev_close) dcamdev_close = nullptr;
    decltype(&::dcamdev_showpanel) dcamdev_showpanel = nullptr;
    decltype(&::dcamdev_getcapability) dcamdev_getcapability = nullptr;
    decltype(&::dcamdev_getstring) dcamdev_getstring = nullptr;
    decltype(&::dcamdev_setdata) dcamdev_setdata = nullptr;
    decltype(&::dcamdev_getdata) dcamdev_getdata = nullptr;
    decltype(&::dcambuf_alloc) dcambuf_alloc = nullptr;
    decltype(&::dcambuf_attach) dcambuf_attach = nullptr;
    decltype(&::dcambuf_release) dcambuf_release = nullptr;
    decltype(&::dcambuf_lockframe) dcambuf_lockframe = nullptr;
    decltype(&::dcambuf_copyframe) dcambuf_copyframe = nullptr;
    decltype(&::dcambuf_copymetadata) dcambuf_copymetadata = nullptr;
    decltype(&::dcamcap_start) dcamcap_start = nullptr;
    decltype(&::dcamcap_stop) dcamcap_stop = nullptr;
    decltype(&::dcamcap_status) dcamcap_status = nullptr;
    decltype(&::dcamcap_transferinfo) dcamcap_transferinfo = nullptr;
    decltype(&::dcamcap_firetrigger) dcamcap_firetrigger = nullptr;
    decltype(&::dcamcap_record) dcamcap_record = nullptr;
    decltype(&::dcamwait_open) dcamwait_open = nullptr;
    decltype(&::dcamwait_close) dcamwait_close = nullptr;
    decltype(&::dcamwait_start) dcamwait_start = nullptr;
    decltype(&::dcamwait_abort) dcamwait_abort = nullptr;
    decltype(&::dcamprop_getattr) dcamprop_getattr = nullptr;
    decltype(&::dcamprop_getvalue) dcamprop_getvalue = nullptr;
    decltype(&::dcamprop_setvalue) dcamprop_setvalue = nullptr;
    decltype(&::dcamprop_setgetvalue) dcamprop_setgetvalue = nullptr;
    decltype(&::dcamprop_queryvalue) dcamprop_queryvalue = nullptr;
    decltype(&::dcamprop_getnextid) dcamprop_getnextid = nullptr;
    decltype(&::dcamprop_getname) dcamprop_getname = nullptr;
    decltype(&::dcamprop_getvaluetext) dcamprop_getvaluetext = nullptr;

    explicit HamamatsuAPIWrapper(const std::string& libraryPath) {
        _dllHandle = LOAD_LIBRARY(libraryPath.c_str());
        if (!_dllHandle) {
            // we don't have the library, so the runtime probably isn't installed.
            _allFunctionsLoaded = false;
            return;
        }
        dcamapi_init = reinterpret_cast<decltype(dcamapi_init)>(GET_PROC_ADDRESS(_dllHandle, "dcamapi_init"));
        if (!dcamapi_init) throw std::runtime_error("Failed to load function: dcamapi_init");
        dcamapi_uninit = reinterpret_cast<decltype(dcamapi_uninit)>(GET_PROC_ADDRESS(_dllHandle, "dcamapi_uninit"));
        if (!dcamapi_uninit) throw std::runtime_error("Failed to load function: dcamapi_uninit");
        dcamdev_open = reinterpret_cast<decltype(dcamdev_open)>(GET_PROC_ADDRESS(_dllHandle, "dcamdev_open"));
        if (!dcamdev_open) throw std::runtime_error("Failed to load function: dcamdev_open");
        dcamdev_close = reinterpret_cast<decltype(dcamdev_close)>(GET_PROC_ADDRESS(_dllHandle, "dcamdev_close"));
        if (!dcamdev_close) throw std::runtime_error("Failed to load function: dcamdev_close");
        dcamdev_showpanel = reinterpret_cast<decltype(dcamdev_showpanel)>(GET_PROC_ADDRESS(_dllHandle, "dcamdev_showpanel"));
        if (!dcamdev_showpanel) throw std::runtime_error("Failed to load function: dcamdev_showpanel");
        dcamdev_getcapability = reinterpret_cast<decltype(dcamdev_getcapability)>(GET_PROC_ADDRESS(_dllHandle, "dcamdev_getcapability"));
        if (!dcamdev_getcapability) throw std::runtime_error("Failed to load function: dcamdev_getcapability");
        dcamdev_getstring = reinterpret_cast<decltype(dcamdev_getstring)>(GET_PROC_ADDRESS(_dllHandle, "dcamdev_getstring"));
        if (!dcamdev_getstring) throw std::runtime_error("Failed to load function: dcamdev_getstring");
        dcamdev_setdata = reinterpret_cast<decltype(dcamdev_setdata)>(GET_PROC_ADDRESS(_dllHandle, "dcamdev_setdata"));
        if (!dcamdev_setdata) throw std::runtime_error("Failed to load function: dcamdev_setdata");
        dcamdev_getdata = reinterpret_cast<decltype(dcamdev_getdata)>(GET_PROC_ADDRESS(_dllHandle, "dcamdev_getdata"));
        if (!dcamdev_getdata) throw std::runtime_error("Failed to load function: dcamdev_getdata");
        dcambuf_alloc = reinterpret_cast<decltype(dcambuf_alloc)>(GET_PROC_ADDRESS(_dllHandle, "dcambuf_alloc"));
        if (!dcambuf_alloc) throw std::runtime_error("Failed to load function: dcambuf_alloc");
        dcambuf_attach = reinterpret_cast<decltype(dcambuf_attach)>(GET_PROC_ADDRESS(_dllHandle, "dcambuf_attach"));
        if (!dcambuf_attach) throw std::runtime_error("Failed to load function: dcambuf_attach");
        dcambuf_release = reinterpret_cast<decltype(dcambuf_release)>(GET_PROC_ADDRESS(_dllHandle, "dcambuf_release"));
        if (!dcambuf_release) throw std::runtime_error("Failed to load function: dcambuf_release");
        dcambuf_lockframe = reinterpret_cast<decltype(dcambuf_lockframe)>(GET_PROC_ADDRESS(_dllHandle, "dcambuf_lockframe"));
        if (!dcambuf_lockframe) throw std::runtime_error("Failed to load function: dcambuf_lockframe");
        dcambuf_copyframe = reinterpret_cast<decltype(dcambuf_copyframe)>(GET_PROC_ADDRESS(_dllHandle, "dcambuf_copyframe"));
        if (!dcambuf_copyframe) throw std::runtime_error("Failed to load function: dcambuf_copyframe");
        dcambuf_copymetadata = reinterpret_cast<decltype(dcambuf_copymetadata)>(GET_PROC_ADDRESS(_dllHandle, "dcambuf_copymetadata"));
        if (!dcambuf_copymetadata) throw std::runtime_error("Failed to load function: dcambuf_copymetadata");
        dcamcap_start = reinterpret_cast<decltype(dcamcap_start)>(GET_PROC_ADDRESS(_dllHandle, "dcamcap_start"));
        if (!dcamcap_start) throw std::runtime_error("Failed to load function: dcamcap_start");
        dcamcap_stop = reinterpret_cast<decltype(dcamcap_stop)>(GET_PROC_ADDRESS(_dllHandle, "dcamcap_stop"));
        if (!dcamcap_stop) throw std::runtime_error("Failed to load function: dcamcap_stop");
        dcamcap_status = reinterpret_cast<decltype(dcamcap_status)>(GET_PROC_ADDRESS(_dllHandle, "dcamcap_status"));
        if (!dcamcap_status) throw std::runtime_error("Failed to load function: dcamcap_status");
        dcamcap_transferinfo = reinterpret_cast<decltype(dcamcap_transferinfo)>(GET_PROC_ADDRESS(_dllHandle, "dcamcap_transferinfo"));
        if (!dcamcap_transferinfo) throw std::runtime_error("Failed to load function: dcamcap_transferinfo");
        dcamcap_firetrigger = reinterpret_cast<decltype(dcamcap_firetrigger)>(GET_PROC_ADDRESS(_dllHandle, "dcamcap_firetrigger"));
        if (!dcamcap_firetrigger) throw std::runtime_error("Failed to load function: dcamcap_firetrigger");
        dcamcap_record = reinterpret_cast<decltype(dcamcap_record)>(GET_PROC_ADDRESS(_dllHandle, "dcamcap_record"));
        if (!dcamcap_record) throw std::runtime_error("Failed to load function: dcamcap_record");
        dcamwait_open = reinterpret_cast<decltype(dcamwait_open)>(GET_PROC_ADDRESS(_dllHandle, "dcamwait_open"));
        if (!dcamwait_open) throw std::runtime_error("Failed to load function: dcamwait_open");
        dcamwait_close = reinterpret_cast<decltype(dcamwait_close)>(GET_PROC_ADDRESS(_dllHandle, "dcamwait_close"));
        if (!dcamwait_close) throw std::runtime_error("Failed to load function: dcamwait_close");
        dcamwait_start = reinterpret_cast<decltype(dcamwait_start)>(GET_PROC_ADDRESS(_dllHandle, "dcamwait_start"));
        if (!dcamwait_start) throw std::runtime_error("Failed to load function: dcamwait_start");
        dcamwait_abort = reinterpret_cast<decltype(dcamwait_abort)>(GET_PROC_ADDRESS(_dllHandle, "dcamwait_abort"));
        if (!dcamwait_abort) throw std::runtime_error("Failed to load function: dcamwait_abort");
        dcamprop_getattr = reinterpret_cast<decltype(dcamprop_getattr)>(GET_PROC_ADDRESS(_dllHandle, "dcamprop_getattr"));
        if (!dcamprop_getattr) throw std::runtime_error("Failed to load function: dcamprop_getattr");
        dcamprop_getvalue = reinterpret_cast<decltype(dcamprop_getvalue)>(GET_PROC_ADDRESS(_dllHandle, "dcamprop_getvalue"));
        if (!dcamprop_getvalue) throw std::runtime_error("Failed to load function: dcamprop_getvalue");
        dcamprop_setvalue = reinterpret_cast<decltype(dcamprop_setvalue)>(GET_PROC_ADDRESS(_dllHandle, "dcamprop_setvalue"));
        if (!dcamprop_setvalue) throw std::runtime_error("Failed to load function: dcamprop_setvalue");
        dcamprop_setgetvalue = reinterpret_cast<decltype(dcamprop_setgetvalue)>(GET_PROC_ADDRESS(_dllHandle, "dcamprop_setgetvalue"));
        if (!dcamprop_setgetvalue) throw std::runtime_error("Failed to load function: dcamprop_setgetvalue");
        dcamprop_queryvalue = reinterpret_cast<decltype(dcamprop_queryvalue)>(GET_PROC_ADDRESS(_dllHandle, "dcamprop_queryvalue"));
        if (!dcamprop_queryvalue) throw std::runtime_error("Failed to load function: dcamprop_queryvalue");
        dcamprop_getnextid = reinterpret_cast<decltype(dcamprop_getnextid)>(GET_PROC_ADDRESS(_dllHandle, "dcamprop_getnextid"));
        if (!dcamprop_getnextid) throw std::runtime_error("Failed to load function: dcamprop_getnextid");
        dcamprop_getname = reinterpret_cast<decltype(dcamprop_getname)>(GET_PROC_ADDRESS(_dllHandle, "dcamprop_getname"));
        if (!dcamprop_getname) throw std::runtime_error("Failed to load function: dcamprop_getname");
        dcamprop_getvaluetext = reinterpret_cast<decltype(dcamprop_getvaluetext)>(GET_PROC_ADDRESS(_dllHandle, "dcamprop_getvaluetext"));
        if (!dcamprop_getvaluetext) throw std::runtime_error("Failed to load function: dcamprop_getvaluetext");

        _allFunctionsLoaded = true;
    }

    ~HamamatsuAPIWrapper() {
        if (_dllHandle) {
            FREE_LIBRARY(_dllHandle);
        }
    }

    HamamatsuAPIWrapper(const HamamatsuAPIWrapper&) = delete;
    HamamatsuAPIWrapper& operator=(const HamamatsuAPIWrapper&) = delete;

    bool areAllFunctionsLoaded() const { return _allFunctionsLoaded; }

private:
    LIB_HANDLE _dllHandle = nullptr;
    bool _allFunctionsLoaded = false;
};

inline HamamatsuAPIWrapper GetHamamatsuAPIWrapper() {
    return HamamatsuAPIWrapper("dcamapi.dll");
}

#endif
