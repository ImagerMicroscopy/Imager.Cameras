#ifndef ANDORSDK3APIWRAPPER_H
#define ANDORSDK3APIWRAPPER_H

#include <string>
#include <stdexcept>

// Platform-specific includes and macros
#ifdef _WIN32
#include <windows.h>
#define LIB_HANDLE HMODULE
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

// Include the header files
#include "Andor/atcore.h"
#include "Andor/atutility.h"

class AndorSDK3APIWrapper {
public:
    // Function pointers from atutility.dll
    decltype(&::AT_ConvertBuffer) AT_ConvertBuffer = nullptr;
    decltype(&::AT_InitialiseUtilityLibrary) AT_InitialiseUtilityLibrary = nullptr;
    decltype(&::AT_FinaliseUtilityLibrary) AT_FinaliseUtilityLibrary = nullptr;

    // Function pointers from atcore.dll
    decltype(&::AT_InitialiseLibrary) AT_InitialiseLibrary = nullptr;
    decltype(&::AT_FinaliseLibrary) AT_FinaliseLibrary = nullptr;
    decltype(&::AT_GetInt) AT_GetInt = nullptr;
    decltype(&::AT_Open) AT_Open = nullptr;
    decltype(&::AT_Close) AT_Close = nullptr;
    decltype(&::AT_GetString) AT_GetString = nullptr;
    decltype(&::AT_SetEnumeratedString) AT_SetEnumeratedString = nullptr;
    decltype(&::AT_GetEnumIndex) AT_GetEnumIndex = nullptr;
    decltype(&::AT_GetEnumCount) AT_GetEnumCount = nullptr;
    decltype(&::AT_GetEnumStringByIndex) AT_GetEnumStringByIndex = nullptr;
    decltype(&::AT_SetFloat) AT_SetFloat = nullptr;
    decltype(&::AT_GetFloat) AT_GetFloat = nullptr;
    decltype(&::AT_GetFloatMin) AT_GetFloatMin = nullptr;
    decltype(&::AT_GetFloatMax) AT_GetFloatMax = nullptr;
    decltype(&::AT_SetInt) AT_SetInt = nullptr;
    decltype(&::AT_GetIntMin) AT_GetIntMin = nullptr;
    decltype(&::AT_GetIntMax) AT_GetIntMax = nullptr;
    decltype(&::AT_GetBool) AT_GetBool = nullptr;
    decltype(&::AT_SetBool) AT_SetBool = nullptr;
    decltype(&::AT_Command) AT_Command = nullptr;
    decltype(&::AT_WaitBuffer) AT_WaitBuffer = nullptr;
    decltype(&::AT_QueueBuffer) AT_QueueBuffer = nullptr;
    decltype(&::AT_Flush) AT_Flush = nullptr;

    AndorSDK3APIWrapper() {
        // Load the DLLs
        _hAtCoreDll = LOAD_LIBRARY("atcore.dll");
        if (!_hAtCoreDll) {
            // we don't have the library, so the runtime probably isn't installed.
            _allFunctionsLoaded = false;
            return;
        }
        _hAtUtilityDll = LOAD_LIBRARY("atutility.dll");
        if (!_hAtUtilityDll) {
            throw std::runtime_error("Failed to load library: atutility.dll");
        }

        // Load functions from atcore.dll
        AT_InitialiseLibrary = reinterpret_cast<decltype(AT_InitialiseLibrary)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_InitialiseLibrary"));
        if (!AT_InitialiseLibrary) throw std::runtime_error("Failed to load function: AT_InitialiseLibrary");
        AT_FinaliseLibrary = reinterpret_cast<decltype(AT_FinaliseLibrary)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_FinaliseLibrary"));
        if (!AT_FinaliseLibrary) throw std::runtime_error("Failed to load function: AT_FinaliseLibrary");
        AT_GetInt = reinterpret_cast<decltype(AT_GetInt)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetInt"));
        if (!AT_GetInt) throw std::runtime_error("Failed to load function: AT_GetInt");
        AT_Open = reinterpret_cast<decltype(AT_Open)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_Open"));
        if (!AT_Open) throw std::runtime_error("Failed to load function: AT_Open");
        AT_Close = reinterpret_cast<decltype(AT_Close)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_Close"));
        if (!AT_Close) throw std::runtime_error("Failed to load function: AT_Close");
        AT_GetString = reinterpret_cast<decltype(AT_GetString)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetString"));
        if (!AT_GetString) throw std::runtime_error("Failed to load function: AT_GetString");
        AT_SetEnumeratedString = reinterpret_cast<decltype(AT_SetEnumeratedString)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_SetEnumeratedString"));
        if (!AT_SetEnumeratedString) throw std::runtime_error("Failed to load function: AT_SetEnumeratedString");
        AT_GetEnumIndex = reinterpret_cast<decltype(AT_GetEnumIndex)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetEnumIndex"));
        if (!AT_GetEnumIndex) throw std::runtime_error("Failed to load function: AT_GetEnumIndex");
        AT_GetEnumCount = reinterpret_cast<decltype(AT_GetEnumCount)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetEnumCount"));
        if (!AT_GetEnumCount) throw std::runtime_error("Failed to load function: AT_GetEnumCount");
        AT_GetEnumStringByIndex = reinterpret_cast<decltype(AT_GetEnumStringByIndex)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetEnumStringByIndex"));
        if (!AT_GetEnumStringByIndex) throw std::runtime_error("Failed to load function: AT_GetEnumStringByIndex");
        AT_SetFloat = reinterpret_cast<decltype(AT_SetFloat)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_SetFloat"));
        if (!AT_SetFloat) throw std::runtime_error("Failed to load function: AT_SetFloat");
        AT_GetFloat = reinterpret_cast<decltype(AT_GetFloat)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetFloat"));
        if (!AT_GetFloat) throw std::runtime_error("Failed to load function: AT_GetFloat");
        AT_GetFloatMin = reinterpret_cast<decltype(AT_GetFloatMin)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetFloatMin"));
        if (!AT_GetFloatMin) throw std::runtime_error("Failed to load function: AT_GetFloatMin");
        AT_GetFloatMax = reinterpret_cast<decltype(AT_GetFloatMax)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetFloatMax"));
        if (!AT_GetFloatMax) throw std::runtime_error("Failed to load function: AT_GetFloatMax");
        AT_SetInt = reinterpret_cast<decltype(AT_SetInt)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_SetInt"));
        if (!AT_SetInt) throw std::runtime_error("Failed to load function: AT_SetInt");
        AT_GetIntMin = reinterpret_cast<decltype(AT_GetIntMin)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetIntMin"));
        if (!AT_GetIntMin) throw std::runtime_error("Failed to load function: AT_GetIntMin");
        AT_GetIntMax = reinterpret_cast<decltype(AT_GetIntMax)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetIntMax"));
        if (!AT_GetIntMax) throw std::runtime_error("Failed to load function: AT_GetIntMax");
        AT_GetBool = reinterpret_cast<decltype(AT_GetBool)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetBool"));
        if (!AT_GetBool) throw std::runtime_error("Failed to load function: AT_GetBool");
        AT_SetBool = reinterpret_cast<decltype(AT_SetBool)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_SetBool"));
        if (!AT_SetBool) throw std::runtime_error("Failed to load function: AT_SetBool");
        AT_Command = reinterpret_cast<decltype(AT_Command)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_Command"));
        if (!AT_Command) throw std::runtime_error("Failed to load function: AT_Command");
        AT_WaitBuffer = reinterpret_cast<decltype(AT_WaitBuffer)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_WaitBuffer"));
        if (!AT_WaitBuffer) throw std::runtime_error("Failed to load function: AT_WaitBuffer");
        AT_QueueBuffer = reinterpret_cast<decltype(AT_QueueBuffer)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_QueueBuffer"));
        if (!AT_QueueBuffer) throw std::runtime_error("Failed to load function: AT_QueueBuffer");
        AT_Flush = reinterpret_cast<decltype(AT_Flush)>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_Flush"));
        if (!AT_Flush) throw std::runtime_error("Failed to load function: AT_Flush");

        // Load functions from atutility.dll
        AT_ConvertBuffer = reinterpret_cast<decltype(AT_ConvertBuffer)>(GET_PROC_ADDRESS(_hAtUtilityDll, "AT_ConvertBuffer"));
        if (!AT_ConvertBuffer) throw std::runtime_error("Failed to load function: AT_ConvertBuffer");
        AT_InitialiseUtilityLibrary = reinterpret_cast<decltype(AT_InitialiseUtilityLibrary)>(GET_PROC_ADDRESS(_hAtUtilityDll, "AT_InitialiseUtilityLibrary"));
        if (!AT_InitialiseUtilityLibrary) throw std::runtime_error("Failed to load function: AT_InitialiseUtilityLibrary");
        AT_FinaliseUtilityLibrary = reinterpret_cast<decltype(AT_FinaliseUtilityLibrary)>(GET_PROC_ADDRESS(_hAtUtilityDll, "AT_FinaliseUtilityLibrary"));
        if (!AT_FinaliseUtilityLibrary) throw std::runtime_error("Failed to load function: AT_FinaliseUtilityLibrary");

        _allFunctionsLoaded = true;
    }

    ~AndorSDK3APIWrapper() {
        if (_hAtCoreDll) {
            FREE_LIBRARY(_hAtCoreDll);
        }
        if (_hAtUtilityDll) {
            FREE_LIBRARY(_hAtUtilityDll);
        }
    }

    AndorSDK3APIWrapper(const AndorSDK3APIWrapper&) = delete;
    AndorSDK3APIWrapper& operator=(const AndorSDK3APIWrapper&) = delete;

    bool areAllFunctionsLoaded() const { return _allFunctionsLoaded; }

private:
    LIB_HANDLE _hAtCoreDll = nullptr;
    LIB_HANDLE _hAtUtilityDll = nullptr;
    bool _allFunctionsLoaded = false;
};

inline AndorSDK3APIWrapper GetAndorSDK3APIWrapper() {
    return AndorSDK3APIWrapper();
}

#endif
