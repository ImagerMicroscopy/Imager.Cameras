#ifndef ANDORSDK3APIWRAPPER_H
#define ANDORSDK3APIWRAPPER_H

#include <string>
#include <stdexcept>
#include <memory>

// Platform-specific includes and macros
#ifdef _WIN32
#include <windows.h>
#define DLL_HANDLE HMODULE
#define LOAD_LIBRARY(path) LoadLibraryA(path)
#define GET_PROC_ADDRESS(handle, name) GetProcAddress(handle, name)
#define FREE_LIBRARY(handle) FreeLibrary(handle)
#else
#include <dlfcn.h>
#define DLL_HANDLE void*
#define LOAD_LIBRARY(path) dlopen(path, RTLD_LAZY)
#define GET_PROC_ADDRESS(handle, name) dlsym(handle, name)
#define FREE_LIBRARY(handle) dlclose(handle)
#endif

// Include the header files
#include "Andor/atcore.h"
#include "Andor/atutility.h"

class AndorSDK3APIWrapper {
public:
    // Define function pointer types for the functions in the DLLs
    typedef int (*AT_ConvertBufferFunc)(AT_U8*, AT_U8*, AT_64, AT_64, AT_64, const AT_WC*, const AT_WC*);
    typedef int (*AT_InitialiseUtilityLibraryFunc)();
    typedef int (*AT_FinaliseUtilityLibraryFunc)();
    typedef int (*AT_InitialiseLibraryFunc)();
    typedef int (*AT_FinaliseLibraryFunc)();
    typedef int (*AT_GetIntFunc)(AT_H, const AT_WC*, AT_64*);
    typedef int (*AT_OpenFunc)(int, AT_H*);
    typedef int (*AT_CloseFunc)(AT_H);
    typedef int (*AT_GetStringFunc)(AT_H, const AT_WC*, AT_WC*, int);
    typedef int (*AT_SetEnumeratedStringFunc)(AT_H, const AT_WC*, const AT_WC*);
    typedef int (*AT_GetEnumIndexFunc)(AT_H, const AT_WC*, int*);
    typedef int (*AT_GetEnumCountFunc)(AT_H, const AT_WC*, int*);
    typedef int (*AT_GetEnumStringByIndexFunc)(AT_H, const AT_WC*, int, AT_WC*, int);
    typedef int (*AT_SetFloatFunc)(AT_H, const AT_WC*, double);
    typedef int (*AT_GetFloatFunc)(AT_H, const AT_WC*, double*);
    typedef int (*AT_GetFloatMinFunc)(AT_H, const AT_WC*, double*);
    typedef int (*AT_GetFloatMaxFunc)(AT_H, const AT_WC*, double*);
    typedef int (*AT_SetIntFunc)(AT_H, const AT_WC*, AT_64);
    typedef int (*AT_GetIntMinFunc)(AT_H, const AT_WC*, AT_64*);
    typedef int (*AT_GetIntMaxFunc)(AT_H, const AT_WC*, AT_64*);
    typedef int (*AT_GetBoolFunc)(AT_H, const AT_WC*, AT_BOOL*);
    typedef int (*AT_SetBoolFunc)(AT_H, const AT_WC*, AT_BOOL);
    typedef int (*AT_CommandFunc)(AT_H, const AT_WC*);
    typedef int (*AT_WaitBufferFunc)(AT_H, AT_U8**, int*, unsigned int);
    typedef int (*AT_QueueBufferFunc)(AT_H, AT_U8*, int);
    typedef int (*AT_FlushFunc)(AT_H);

    AndorSDK3Wrapper() : _allFunctionsLoaded(true) {
        // Load the DLLs
        _hAtCoreDll = LOAD_LIBRARY("atcore.dll");
        _hAtUtilityDll = LOAD_LIBRARY("atutility.dll");

        // Load functions from atcore.dll
        loadAtCoreFunctions();

        // Load functions from atutility.dll
        loadAtUtilityFunctions();
    }

    ~AndorSDK3Wrapper() {
        if (_hAtCoreDll) {
            FREE_LIBRARY(_hAtCoreDll);
        }
        if (_hAtUtilityDll) {
            FREE_LIBRARY(_hAtUtilityDll);
        }
    }

    bool areAllFunctionsLoaded() const {
        return _allFunctionsLoaded;
    }

    // Wrapper functions
    int AT_ConvertBuffer(AT_U8* inputBuffer, AT_U8* outputBuffer, AT_64 width, AT_64 height, AT_64 stride, const AT_WC* inputPixelEncoding, const AT_WC* outputPixelEncoding) {
        if (!_AT_ConvertBuffer) {
            throw std::logic_error("Function AT_ConvertBuffer not loaded");
        }
        return _AT_ConvertBuffer(inputBuffer, outputBuffer, width, height, stride, inputPixelEncoding, outputPixelEncoding);
    }

    int AT_InitialiseUtilityLibrary() {
        if (!_AT_InitialiseUtilityLibrary) {
            throw std::logic_error("Function AT_InitialiseUtilityLibrary not loaded");
        }
        return _AT_InitialiseUtilityLibrary();
    }

    int AT_FinaliseUtilityLibrary() {
        if (!_AT_FinaliseUtilityLibrary) {
            throw std::logic_error("Function AT_FinaliseUtilityLibrary not loaded");
        }
        return _AT_FinaliseUtilityLibrary();
    }

    int AT_InitialiseLibrary() {
        if (!_AT_InitialiseLibrary) {
            throw std::logic_error("Function AT_InitialiseLibrary not loaded");
        }
        return _AT_InitialiseLibrary();
    }

    int AT_FinaliseLibrary() {
        if (!_AT_FinaliseLibrary) {
            throw std::logic_error("Function AT_FinaliseLibrary not loaded");
        }
        return _AT_FinaliseLibrary();
    }

    int AT_GetInt(AT_H Hndl, const AT_WC* Feature, AT_64* Value) {
        if (!_AT_GetInt) {
            throw std::logic_error("Function AT_GetInt not loaded");
        }
        return _AT_GetInt(Hndl, Feature, Value);
    }

    int AT_Open(int Index, AT_H* Hndl) {
        if (!_AT_Open) {
            throw std::logic_error("Function AT_Open not loaded");
        }
        return _AT_Open(Index, Hndl);
    }

    int AT_Close(AT_H Hndl) {
        if (!_AT_Close) {
            throw std::logic_error("Function AT_Close not loaded");
        }
        return _AT_Close(Hndl);
    }

    int AT_GetString(AT_H Hndl, const AT_WC* Feature, AT_WC* String, int StringLength) {
        if (!_AT_GetString) {
            throw std::logic_error("Function AT_GetString not loaded");
        }
        return _AT_GetString(Hndl, Feature, String, StringLength);
    }

    int AT_SetEnumeratedString(AT_H Hndl, const AT_WC* Feature, const AT_WC* String) {
        if (!_AT_SetEnumeratedString) {
            throw std::logic_error("Function AT_SetEnumeratedString not loaded");
        }
        return _AT_SetEnumeratedString(Hndl, Feature, String);
    }

    int AT_GetEnumIndex(AT_H Hndl, const AT_WC* Feature, int* Index) {
        if (!_AT_GetEnumIndex) {
            throw std::logic_error("Function AT_GetEnumIndex not loaded");
        }
        return _AT_GetEnumIndex(Hndl, Feature, Index);
    }

    int AT_GetEnumCount(AT_H Hndl, const AT_WC* Feature, int* Count) {
        if (!_AT_GetEnumCount) {
            throw std::logic_error("Function AT_GetEnumCount not loaded");
        }
        return _AT_GetEnumCount(Hndl, Feature, Count);
    }

    int AT_GetEnumStringByIndex(AT_H Hndl, const AT_WC* Feature, int Index, AT_WC* String, int StringLength) {
        if (!_AT_GetEnumStringByIndex) {
            throw std::logic_error("Function AT_GetEnumStringByIndex not loaded");
        }
        return _AT_GetEnumStringByIndex(Hndl, Feature, Index, String, StringLength);
    }

    int AT_SetFloat(AT_H Hndl, const AT_WC* Feature, double Value) {
        if (!_AT_SetFloat) {
            throw std::logic_error("Function AT_SetFloat not loaded");
        }
        return _AT_SetFloat(Hndl, Feature, Value);
    }

    int AT_GetFloat(AT_H Hndl, const AT_WC* Feature, double* Value) {
        if (!_AT_GetFloat) {
            throw std::logic_error("Function AT_GetFloat not loaded");
        }
        return _AT_GetFloat(Hndl, Feature, Value);
    }

    int AT_GetFloatMin(AT_H Hndl, const AT_WC* Feature, double* Value) {
        if (!_AT_GetFloatMin) {
            throw std::logic_error("Function AT_GetFloatMin not loaded");
        }
        return _AT_GetFloatMin(Hndl, Feature, Value);
    }

    int AT_GetFloatMax(AT_H Hndl, const AT_WC* Feature, double* Value) {
        if (!_AT_GetFloatMax) {
            throw std::logic_error("Function AT_GetFloatMax not loaded");
        }
        return _AT_GetFloatMax(Hndl, Feature, Value);
    }

    int AT_SetInt(AT_H Hndl, const AT_WC* Feature, AT_64 Value) {
        if (!_AT_SetInt) {
            throw std::logic_error("Function AT_SetInt not loaded");
        }
        return _AT_SetInt(Hndl, Feature, Value);
    }

    int AT_GetIntMin(AT_H Hndl, const AT_WC* Feature, AT_64* Value) {
        if (!_AT_GetIntMin) {
            throw std::logic_error("Function AT_GetIntMin not loaded");
        }
        return _AT_GetIntMin(Hndl, Feature, Value);
    }

    int AT_GetIntMax(AT_H Hndl, const AT_WC* Feature, AT_64* Value) {
        if (!_AT_GetIntMax) {
            throw std::logic_error("Function AT_GetIntMax not loaded");
        }
        return _AT_GetIntMax(Hndl, Feature, Value);
    }

    int AT_GetBool(AT_H Hndl, const AT_WC* Feature, AT_BOOL* Value) {
        if (!_AT_GetBool) {
            throw std::logic_error("Function AT_GetBool not loaded");
        }
        return _AT_GetBool(Hndl, Feature, Value);
    }

    int AT_SetBool(AT_H Hndl, const AT_WC* Feature, AT_BOOL Value) {
        if (!_AT_SetBool) {
            throw std::logic_error("Function AT_SetBool not loaded");
        }
        return _AT_SetBool(Hndl, Feature, Value);
    }

    int AT_Command(AT_H Hndl, const AT_WC* Command) {
        if (!_AT_Command) {
            throw std::logic_error("Function AT_Command not loaded");
        }
        return _AT_Command(Hndl, Command);
    }

    int AT_WaitBuffer(AT_H Hndl, AT_U8** Buffer, int* BufSize, unsigned int TimeoutMillis) {
        if (!_AT_WaitBuffer) {
            throw std::logic_error("Function AT_WaitBuffer not loaded");
        }
        return _AT_WaitBuffer(Hndl, Buffer, BufSize, TimeoutMillis);
    }

    int AT_QueueBuffer(AT_H Hndl, AT_U8* Buffer, int BufferSize) {
        if (!_AT_QueueBuffer) {
            throw std::logic_error("Function AT_QueueBuffer not loaded");
        }
        return _AT_QueueBuffer(Hndl, Buffer, BufferSize);
    }

    int AT_Flush(AT_H Hndl) {
        if (!_AT_Flush) {
            throw std::logic_error("Function AT_Flush not loaded");
        }
        return _AT_Flush(Hndl);
    }

private:
    DLL_HANDLE _hAtCoreDll;
    DLL_HANDLE _hAtUtilityDll;
    bool _allFunctionsLoaded;

    // Function pointers
    AT_ConvertBufferFunc _AT_ConvertBuffer;
    AT_InitialiseUtilityLibraryFunc _AT_InitialiseUtilityLibrary;
    AT_FinaliseUtilityLibraryFunc _AT_FinaliseUtilityLibrary;
    AT_InitialiseLibraryFunc _AT_InitialiseLibrary;
    AT_FinaliseLibraryFunc _AT_FinaliseLibrary;
    AT_GetIntFunc _AT_GetInt;
    AT_OpenFunc _AT_Open;
    AT_CloseFunc _AT_Close;
    AT_GetStringFunc _AT_GetString;
    AT_SetEnumeratedStringFunc _AT_SetEnumeratedString;
    AT_GetEnumIndexFunc _AT_GetEnumIndex;
    AT_GetEnumCountFunc _AT_GetEnumCount;
    AT_GetEnumStringByIndexFunc _AT_GetEnumStringByIndex;
    AT_SetFloatFunc _AT_SetFloat;
    AT_GetFloatFunc _AT_GetFloat;
    AT_GetFloatMinFunc _AT_GetFloatMin;
    AT_GetFloatMaxFunc _AT_GetFloatMax;
    AT_SetIntFunc _AT_SetInt;
    AT_GetIntMinFunc _AT_GetIntMin;
    AT_GetIntMaxFunc _AT_GetIntMax;
    AT_GetBoolFunc _AT_GetBool;
    AT_SetBoolFunc _AT_SetBool;
    AT_CommandFunc _AT_Command;
    AT_WaitBufferFunc _AT_WaitBuffer;
    AT_QueueBufferFunc _AT_QueueBuffer;
    AT_FlushFunc _AT_Flush;

    void loadAtCoreFunctions() {
        if (!_hAtCoreDll) {
            _allFunctionsLoaded = false;
            return;
        }

        _AT_InitialiseLibrary = reinterpret_cast<AT_InitialiseLibraryFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_InitialiseLibrary"));
        _AT_FinaliseLibrary = reinterpret_cast<AT_FinaliseLibraryFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_FinaliseLibrary"));
        _AT_GetInt = reinterpret_cast<AT_GetIntFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetInt"));
        _AT_Open = reinterpret_cast<AT_OpenFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_Open"));
        _AT_Close = reinterpret_cast<AT_CloseFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_Close"));
        _AT_GetString = reinterpret_cast<AT_GetStringFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetString"));
        _AT_SetEnumeratedString = reinterpret_cast<AT_SetEnumeratedStringFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_SetEnumeratedString"));
        _AT_GetEnumIndex = reinterpret_cast<AT_GetEnumIndexFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetEnumIndex"));
        _AT_GetEnumCount = reinterpret_cast<AT_GetEnumCountFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetEnumCount"));
        _AT_GetEnumStringByIndex = reinterpret_cast<AT_GetEnumStringByIndexFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetEnumStringByIndex"));
        _AT_SetFloat = reinterpret_cast<AT_SetFloatFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_SetFloat"));
        _AT_GetFloat = reinterpret_cast<AT_GetFloatFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetFloat"));
        _AT_GetFloatMin = reinterpret_cast<AT_GetFloatMinFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetFloatMin"));
        _AT_GetFloatMax = reinterpret_cast<AT_GetFloatMaxFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetFloatMax"));
        _AT_SetInt = reinterpret_cast<AT_SetIntFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_SetInt"));
        _AT_GetIntMin = reinterpret_cast<AT_GetIntMinFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetIntMin"));
        _AT_GetIntMax = reinterpret_cast<AT_GetIntMaxFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetIntMax"));
        _AT_GetBool = reinterpret_cast<AT_GetBoolFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_GetBool"));
        _AT_SetBool = reinterpret_cast<AT_SetBoolFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_SetBool"));
        _AT_Command = reinterpret_cast<AT_CommandFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_Command"));
        _AT_WaitBuffer = reinterpret_cast<AT_WaitBufferFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_WaitBuffer"));
        _AT_QueueBuffer = reinterpret_cast<AT_QueueBufferFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_QueueBuffer"));
        _AT_Flush = reinterpret_cast<AT_FlushFunc>(GET_PROC_ADDRESS(_hAtCoreDll, "AT_Flush"));

        if (!_AT_InitialiseLibrary || !_AT_FinaliseLibrary || !_AT_GetInt || !_AT_Open || !_AT_Close || !_AT_GetString ||
            !_AT_SetEnumeratedString || !_AT_GetEnumIndex || !_AT_GetEnumCount || !_AT_GetEnumStringByIndex || !_AT_SetFloat ||
            !_AT_GetFloat || !_AT_GetFloatMin || !_AT_GetFloatMax || !_AT_SetInt || !_AT_GetIntMin || !_AT_GetIntMax ||
            !_AT_GetBool || !_AT_SetBool || !_AT_Command || !_AT_WaitBuffer || !_AT_QueueBuffer || !_AT_Flush) {
            _allFunctionsLoaded = false;
        }
    }

    void loadAtUtilityFunctions() {
        if (!_hAtUtilityDll) {
            _allFunctionsLoaded = false;
            return;
        }

        _AT_ConvertBuffer = reinterpret_cast<AT_ConvertBufferFunc>(GET_PROC_ADDRESS(_hAtUtilityDll, "AT_ConvertBuffer"));
        _AT_InitialiseUtilityLibrary = reinterpret_cast<AT_InitialiseUtilityLibraryFunc>(GET_PROC_ADDRESS(_hAtUtilityDll, "AT_InitialiseUtilityLibrary"));
        _AT_FinaliseUtilityLibrary = reinterpret_cast<AT_FinaliseUtilityLibraryFunc>(GET_PROC_ADDRESS(_hAtUtilityDll, "AT_FinaliseUtilityLibrary"));

        if (!_AT_ConvertBuffer || !_AT_InitialiseUtilityLibrary || !_AT_FinaliseUtilityLibrary) {
            _allFunctionsLoaded = false;
        }
    }
};

inline AndorSDK3APIWrapper GetAndorSDK3APIWrapper() {
    return AndorSDK3APIWrapper();
}

#endif
