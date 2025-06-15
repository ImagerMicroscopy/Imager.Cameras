#ifndef PCOAPIWRAPPER_H
#define PCOAPIWRAPPER_H

#include <iostream>
#include <string>
#include <map>
#include <stdexcept>

#include "windows.h"
#include "PCO/pco_err.h"
#include "PCO/sc2_SDKStructures.h"
#include "PCO/sc2_SDKAddendum.h"
#include "PCO/sc2_CamExport.h"
#include "PCO/sc2_Defs.h"

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
    typedef return_type(__stdcall* name##_func)(__VA_ARGS__);

// Define function pointer types for each function in the library
DEFINE_FUNC_PTR(PCO_GetGeneral, int, HANDLE, PCO_General*);
DEFINE_FUNC_PTR(PCO_GetCameraType, int, HANDLE, PCO_CameraType*);
DEFINE_FUNC_PTR(PCO_GetCameraHealthStatus, int, HANDLE, DWORD*, DWORD*, DWORD*);
DEFINE_FUNC_PTR(PCO_ResetSettingsToDefault, int, HANDLE);
DEFINE_FUNC_PTR(PCO_GetCameraDescription, int, HANDLE, PCO_Description*);
DEFINE_FUNC_PTR(PCO_GetCameraName, int, HANDLE, char*, WORD);
DEFINE_FUNC_PTR(PCO_GetSizes, int, HANDLE, WORD*, WORD*, WORD*, WORD*);
DEFINE_FUNC_PTR(PCO_GetPixelRate, int, HANDLE, DWORD*);
DEFINE_FUNC_PTR(PCO_SetPixelRate, int, HANDLE, DWORD);
DEFINE_FUNC_PTR(PCO_ArmCamera, int, HANDLE);
DEFINE_FUNC_PTR(PCO_GetDelayExposureTime, int, HANDLE, DWORD*, DWORD*, WORD*, WORD*);
DEFINE_FUNC_PTR(PCO_SetDelayExposureTime, int, HANDLE, DWORD, DWORD, WORD, WORD);
DEFINE_FUNC_PTR(PCO_SetRecordingState, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_SetAcquireModeEx, int, HANDLE, WORD, DWORD, DWORD*);
DEFINE_FUNC_PTR(PCO_SetStorageMode, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_SetTimestampMode, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_SetAcquireControl, int, HANDLE, DWORD, DWORD*, WORD);
DEFINE_FUNC_PTR(PCO_AddBufferExtern, int, HANDLE, HANDLE, WORD, DWORD, DWORD, DWORD, void*, DWORD, DWORD*);
DEFINE_FUNC_PTR(PCO_GetErrorTextSDK, void, DWORD, char*, DWORD);
DEFINE_FUNC_PTR(PCO_GetCOCRuntime, int, HANDLE, DWORD*, DWORD*);
DEFINE_FUNC_PTR(PCO_CloseCamera, int, HANDLE);
DEFINE_FUNC_PTR(PCO_SetMetaDataMode, int, HANDLE, WORD, WORD*, WORD*);
DEFINE_FUNC_PTR(PCO_OpenCamera, int, HANDLE*, WORD);
DEFINE_FUNC_PTR(PCO_CancelImages, int, HANDLE);
DEFINE_FUNC_PTR(PCO_SetImageParameters, int, HANDLE, WORD, WORD, DWORD, void*, int);

class PCOAPIWrapper {
public:
    PCOAPIWrapper(const std::string& libraryPath) : _allFunctionsLoaded(true) {
        _dllHandle = LOAD_LIBRARY(libraryPath.c_str());
        if (!_dllHandle) {
            _allFunctionsLoaded = false;
            return;
        }

        // Initialize function pointers
        functionMap = {
            {"PCO_GetGeneral", &_PCO_GetGeneral},
            {"PCO_GetCameraType", &_PCO_GetCameraType},
            {"PCO_GetCameraHealthStatus", &_PCO_GetCameraHealthStatus},
            {"PCO_ResetSettingsToDefault", &_PCO_ResetSettingsToDefault},
            {"PCO_GetCameraDescription", &_PCO_GetCameraDescription},
            {"PCO_GetCameraName", &_PCO_GetCameraName},
            {"PCO_GetSizes", &_PCO_GetSizes},
            {"PCO_GetPixelRate", &_PCO_GetPixelRate},
            {"PCO_SetPixelRate", &_PCO_SetPixelRate},
            {"PCO_ArmCamera", &_PCO_ArmCamera},
            {"PCO_GetDelayExposureTime", &_PCO_GetDelayExposureTime},
            {"PCO_SetDelayExposureTime", &_PCO_SetDelayExposureTime},
            {"PCO_SetRecordingState", &_PCO_SetRecordingState},
            {"PCO_SetAcquireModeEx", &_PCO_SetAcquireModeEx},
            {"PCO_SetStorageMode", &_PCO_SetStorageMode},
            {"PCO_SetTimestampMode", &_PCO_SetTimestampMode},
            {"PCO_SetAcquireControl", &_PCO_SetAcquireControl},
            {"PCO_AddBufferExtern", &_PCO_AddBufferExtern},
            {"PCO_GetErrorTextSDK", &_PCO_GetErrorTextSDK},
            {"PCO_GetCOCRuntime", &_PCO_GetCOCRuntime},
            {"PCO_CloseCamera", &_PCO_CloseCamera},
            {"PCO_SetMetaDataMode", &_PCO_SetMetaDataMode},
            {"PCO_OpenCamera", &_PCO_OpenCamera},
            {"PCO_CancelImages", &_PCO_CancelImages},
            {"PCO_SetImageParameters", &_PCO_SetImageParameters}
        };

        for (const auto& func : functionMap) {
            *func.second = GET_PROC_ADDRESS(_dllHandle, func.first.c_str());
            if (!*func.second) {
                _allFunctionsLoaded = false;
            }
        }
    }

    ~PCOAPIWrapper() {
        if (_dllHandle) {
            FREE_LIBRARY(_dllHandle);
        }
    }

    bool areAllFunctionsLoaded() const {
        return _allFunctionsLoaded;
    }

    // Wrapper methods for each function
    int PCO_GetGeneral(HANDLE ph, PCO_General* strGeneral) {
        if (!_PCO_GetGeneral) {
            throw std::logic_error("Function PCO_GetGeneral is not loaded.");
        }
        return reinterpret_cast<PCO_GetGeneral_func>(_PCO_GetGeneral)(ph, strGeneral);
    }

    int PCO_GetCameraType(HANDLE ph, PCO_CameraType* strCamType) {
        if (!_PCO_GetCameraType) {
            throw std::logic_error("Function PCO_GetCameraType is not loaded.");
        }
        return reinterpret_cast<PCO_GetCameraType_func>(_PCO_GetCameraType)(ph, strCamType);
    }

    int PCO_GetCameraHealthStatus(HANDLE ph, DWORD* dwWarn, DWORD* dwErr, DWORD* dwStatus) {
        if (!_PCO_GetCameraHealthStatus) {
            throw std::logic_error("Function PCO_GetCameraHealthStatus is not loaded.");
        }
        return reinterpret_cast<PCO_GetCameraHealthStatus_func>(_PCO_GetCameraHealthStatus)(ph, dwWarn, dwErr, dwStatus);
    }

    int PCO_ResetSettingsToDefault(HANDLE ph) {
        if (!_PCO_ResetSettingsToDefault) {
            throw std::logic_error("Function PCO_ResetSettingsToDefault is not loaded.");
        }
        return reinterpret_cast<PCO_ResetSettingsToDefault_func>(_PCO_ResetSettingsToDefault)(ph);
    }

    int PCO_GetCameraDescription(HANDLE ph, PCO_Description* strDescription) {
        if (!_PCO_GetCameraDescription) {
            throw std::logic_error("Function PCO_GetCameraDescription is not loaded.");
        }
        return reinterpret_cast<PCO_GetCameraDescription_func>(_PCO_GetCameraDescription)(ph, strDescription);
    }

    int PCO_GetCameraName(HANDLE ph, char* szCameraName, WORD wSZCameraNameLen) {
        if (!_PCO_GetCameraName) {
            throw std::logic_error("Function PCO_GetCameraName is not loaded.");
        }
        return reinterpret_cast<PCO_GetCameraName_func>(_PCO_GetCameraName)(ph, szCameraName, wSZCameraNameLen);
    }

    int PCO_GetSizes(HANDLE ph, WORD* wXResAct, WORD* wYResAct, WORD* wXResMax, WORD* wYResMax) {
        if (!_PCO_GetSizes) {
            throw std::logic_error("Function PCO_GetSizes is not loaded.");
        }
        return reinterpret_cast<PCO_GetSizes_func>(_PCO_GetSizes)(ph, wXResAct, wYResAct, wXResMax, wYResMax);
    }

    int PCO_GetPixelRate(HANDLE ph, DWORD* dwPixelRate) {
        if (!_PCO_GetPixelRate) {
            throw std::logic_error("Function PCO_GetPixelRate is not loaded.");
        }
        return reinterpret_cast<PCO_GetPixelRate_func>(_PCO_GetPixelRate)(ph, dwPixelRate);
    }

    int PCO_SetPixelRate(HANDLE ph, DWORD dwPixelRate) {
        if (!_PCO_SetPixelRate) {
            throw std::logic_error("Function PCO_SetPixelRate is not loaded.");
        }
        return reinterpret_cast<PCO_SetPixelRate_func>(_PCO_SetPixelRate)(ph, dwPixelRate);
    }

    int PCO_ArmCamera(HANDLE ph) {
        if (!_PCO_ArmCamera) {
            throw std::logic_error("Function PCO_ArmCamera is not loaded.");
        }
        return reinterpret_cast<PCO_ArmCamera_func>(_PCO_ArmCamera)(ph);
    }

    int PCO_GetDelayExposureTime(HANDLE ph, DWORD* dwDelay, DWORD* dwExposure, WORD* wTimeBaseDelay, WORD* wTimeBaseExposure) {
        if (!_PCO_GetDelayExposureTime) {
            throw std::logic_error("Function PCO_GetDelayExposureTime is not loaded.");
        }
        return reinterpret_cast<PCO_GetDelayExposureTime_func>(_PCO_GetDelayExposureTime)(ph, dwDelay, dwExposure, wTimeBaseDelay, wTimeBaseExposure);
    }

    int PCO_SetDelayExposureTime(HANDLE ph, DWORD dwDelay, DWORD dwExposure, WORD wTimeBaseDelay, WORD wTimeBaseExposure) {
        if (!_PCO_SetDelayExposureTime) {
            throw std::logic_error("Function PCO_SetDelayExposureTime is not loaded.");
        }
        return reinterpret_cast<PCO_SetDelayExposureTime_func>(_PCO_SetDelayExposureTime)(ph, dwDelay, dwExposure, wTimeBaseDelay, wTimeBaseExposure);
    }

    int PCO_SetRecordingState(HANDLE ph, WORD wRecState) {
        if (!_PCO_SetRecordingState) {
            throw std::logic_error("Function PCO_SetRecordingState is not loaded.");
        }
        return reinterpret_cast<PCO_SetRecordingState_func>(_PCO_SetRecordingState)(ph, wRecState);
    }

    int PCO_SetAcquireModeEx(HANDLE ph, WORD wAcquMode, DWORD dwNumberImages, DWORD* dwReserved) {
        if (!_PCO_SetAcquireModeEx) {
            throw std::logic_error("Function PCO_SetAcquireModeEx is not loaded.");
        }
        return reinterpret_cast<PCO_SetAcquireModeEx_func>(_PCO_SetAcquireModeEx)(ph, wAcquMode, dwNumberImages, dwReserved);
    }

    int PCO_SetStorageMode(HANDLE ph, WORD wStorageMode) {
        if (!_PCO_SetStorageMode) {
            throw std::logic_error("Function PCO_SetStorageMode is not loaded.");
        }
        return reinterpret_cast<PCO_SetStorageMode_func>(_PCO_SetStorageMode)(ph, wStorageMode);
    }

    int PCO_SetTimestampMode(HANDLE ph, WORD wTimeStampMode) {
        if (!_PCO_SetTimestampMode) {
            throw std::logic_error("Function PCO_SetTimestampMode is not loaded.");
        }
        return reinterpret_cast<PCO_SetTimestampMode_func>(_PCO_SetTimestampMode)(ph, wTimeStampMode);
    }

    int PCO_SetAcquireControl(HANDLE ph, DWORD dwAcquCtrlFlags, DWORD* dwReserved, WORD wNumReserved) {
        if (!_PCO_SetAcquireControl) {
            throw std::logic_error("Function PCO_SetAcquireControl is not loaded.");
        }
        return reinterpret_cast<PCO_SetAcquireControl_func>(_PCO_SetAcquireControl)(ph, dwAcquCtrlFlags, dwReserved, wNumReserved);
    }

    int PCO_AddBufferExtern(HANDLE ph, HANDLE hEvent, WORD wActSeg, DWORD dw1stImage, DWORD dwLastImage, DWORD dwSynch, void* pBuf, DWORD dwLen, DWORD* dwStatus) {
        if (!_PCO_AddBufferExtern) {
            throw std::logic_error("Function PCO_AddBufferExtern is not loaded.");
        }
        return reinterpret_cast<PCO_AddBufferExtern_func>(_PCO_AddBufferExtern)(ph, hEvent, wActSeg, dw1stImage, dwLastImage, dwSynch, pBuf, dwLen, dwStatus);
    }

    void PCO_GetErrorTextSDK(DWORD dwError, char* pszErrorString, DWORD dwErrorStringLength) {
        if (!_PCO_GetErrorTextSDK) {
            throw std::logic_error("Function PCO_GetErrorTextSDK is not loaded.");
        }
        reinterpret_cast<PCO_GetErrorTextSDK_func>(_PCO_GetErrorTextSDK)(dwError, pszErrorString, dwErrorStringLength);
    }

    int PCO_GetCOCRuntime(HANDLE ph, DWORD* dwTime_s, DWORD* dwTime_ns) {
        if (!_PCO_GetCOCRuntime) {
            throw std::logic_error("Function PCO_GetCOCRuntime is not loaded.");
        }
        return reinterpret_cast<PCO_GetCOCRuntime_func>(_PCO_GetCOCRuntime)(ph, dwTime_s, dwTime_ns);
    }

    int PCO_CloseCamera(HANDLE ph) {
        if (!_PCO_CloseCamera) {
            throw std::logic_error("Function PCO_CloseCamera is not loaded.");
        }
        return reinterpret_cast<PCO_CloseCamera_func>(_PCO_CloseCamera)(ph);
    }

    int PCO_SetMetaDataMode(HANDLE ph, WORD wMetaDataMode, WORD* wMetaDataSize, WORD* wMetaDataVersion) {
        if (!_PCO_SetMetaDataMode) {
            throw std::logic_error("Function PCO_SetMetaDataMode is not loaded.");
        }
        return reinterpret_cast<PCO_SetMetaDataMode_func>(_PCO_SetMetaDataMode)(ph, wMetaDataMode, wMetaDataSize, wMetaDataVersion);
    }

    int PCO_OpenCamera(HANDLE* ph, WORD wCamNum) {
        if (!_PCO_OpenCamera) {
            throw std::logic_error("Function PCO_OpenCamera is not loaded.");
        }
        return reinterpret_cast<PCO_OpenCamera_func>(_PCO_OpenCamera)(ph, wCamNum);
    }

    int PCO_CancelImages(HANDLE ph) {
        if (!_PCO_CancelImages) {
            throw std::logic_error("Function PCO_CancelImages is not loaded.");
        }
        return reinterpret_cast<PCO_CancelImages_func>(_PCO_CancelImages)(ph);
    }

    int PCO_SetImageParameters(HANDLE ph, WORD wxres, WORD wyres, DWORD dwflags, void* param, int ilen) {
        if (!_PCO_SetImageParameters) {
            throw std::logic_error("Function PCO_SetImageParameters is not loaded.");
        }
        return reinterpret_cast<PCO_SetImageParameters_func>(_PCO_SetImageParameters)(ph, wxres, wyres, dwflags, param, ilen);
    }

private:
    LIB_HANDLE _dllHandle = nullptr;
    std::map<std::string, void**> functionMap;
    bool _allFunctionsLoaded = false;

    // Function pointers with leading underscore
    void* _PCO_GetGeneral = nullptr;
    void* _PCO_GetCameraType = nullptr;
    void* _PCO_GetCameraHealthStatus = nullptr;
    void* _PCO_ResetSettingsToDefault = nullptr;
    void* _PCO_GetCameraDescription = nullptr;
    void* _PCO_GetCameraName = nullptr;
    void* _PCO_GetSizes = nullptr;
    void* _PCO_GetPixelRate = nullptr;
    void* _PCO_SetPixelRate = nullptr;
    void* _PCO_ArmCamera = nullptr;
    void* _PCO_GetDelayExposureTime = nullptr;
    void* _PCO_SetDelayExposureTime = nullptr;
    void* _PCO_SetRecordingState = nullptr;
    void* _PCO_SetAcquireModeEx = nullptr;
    void* _PCO_SetStorageMode = nullptr;
    void* _PCO_SetTimestampMode = nullptr;
    void* _PCO_SetAcquireControl = nullptr;
    void* _PCO_AddBufferExtern = nullptr;
    void* _PCO_GetErrorTextSDK = nullptr;
    void* _PCO_GetCOCRuntime = nullptr;
    void* _PCO_CloseCamera = nullptr;
    void* _PCO_SetMetaDataMode = nullptr;
    void* _PCO_OpenCamera = nullptr;
    void* _PCO_CancelImages = nullptr;
    void* _PCO_SetImageParameters = nullptr;
};

inline PCOAPIWrapper GetPCOAPIWrapper() {
    return PCOAPIWrapper("sc2_cam");
}

#endif // PCOAPIWRAPPER_H
