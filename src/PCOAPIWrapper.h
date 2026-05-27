#ifndef PCOAPIWRAPPER_H
#define PCOAPIWRAPPER_H

#include <iostream>
#include <string>
#include <map>
#include <stdexcept>

#ifdef __linux__
    #define PCO_LINUX
#endif
#include "PCO/pco_linux_defs.h"
#include "PCO/pco_err.h"
#include "PCO/sc2_sdkstructures.h"
#include "PCO/sc2_sdkaddendum.h"
#include "PCO/sc2_camexport.h"
#include "PCO/sc2_defs.h"

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

class PCOAPIWrapper {
public:
    decltype(&::PCO_GetGeneral) PCO_GetGeneral = nullptr;
    decltype(&::PCO_GetCameraType) PCO_GetCameraType = nullptr;
    decltype(&::PCO_GetCameraHealthStatus) PCO_GetCameraHealthStatus = nullptr;
    decltype(&::PCO_ResetSettingsToDefault) PCO_ResetSettingsToDefault = nullptr;
    decltype(&::PCO_GetCameraDescription) PCO_GetCameraDescription = nullptr;
    decltype(&::PCO_GetCameraName) PCO_GetCameraName = nullptr;
    decltype(&::PCO_GetSizes) PCO_GetSizes = nullptr;
    decltype(&::PCO_GetPixelRate) PCO_GetPixelRate = nullptr;
    decltype(&::PCO_SetPixelRate) PCO_SetPixelRate = nullptr;
    decltype(&::PCO_ArmCamera) PCO_ArmCamera = nullptr;
    decltype(&::PCO_GetDelayExposureTime) PCO_GetDelayExposureTime = nullptr;
    decltype(&::PCO_SetDelayExposureTime) PCO_SetDelayExposureTime = nullptr;
    decltype(&::PCO_SetRecordingState) PCO_SetRecordingState = nullptr;
    decltype(&::PCO_SetAcquireModeEx) PCO_SetAcquireModeEx = nullptr;
    decltype(&::PCO_SetStorageMode) PCO_SetStorageMode = nullptr;
    decltype(&::PCO_SetTimestampMode) PCO_SetTimestampMode = nullptr;
    decltype(&::PCO_SetAcquireControl) PCO_SetAcquireControl = nullptr;
    decltype(&::PCO_AddBufferExtern) PCO_AddBufferExtern = nullptr;
#ifdef __linux__
    decltype(&::PCO_WaitforNextBufferAdr) PCO_WaitforNextBufferAdr = nullptr;
#endif
    decltype(&::PCO_GetErrorTextSDK) PCO_GetErrorTextSDK = nullptr;
    decltype(&::PCO_GetCOCRuntime) PCO_GetCOCRuntime = nullptr;
    decltype(&::PCO_CloseCamera) PCO_CloseCamera = nullptr;
    decltype(&::PCO_SetMetaDataMode) PCO_SetMetaDataMode = nullptr;
    decltype(&::PCO_OpenCamera) PCO_OpenCamera = nullptr;
    decltype(&::PCO_CancelImages) PCO_CancelImages = nullptr;
    decltype(&::PCO_SetImageParameters) PCO_SetImageParameters = nullptr;


    PCOAPIWrapper(const std::string& libraryPath) {
        _dllHandle = LOAD_LIBRARY(libraryPath.c_str());
        if (!_dllHandle) {
            throw std::runtime_error("Failed to load library: " + libraryPath);
        }
        PCO_GetGeneral = reinterpret_cast<decltype(PCO_GetGeneral)>(GET_PROC_ADDRESS(_dllHandle, "PCO_GetGeneral"));
        if (!PCO_GetGeneral) throw std::runtime_error("Failed to load function: PCO_GetGeneral");
        PCO_GetCameraType = reinterpret_cast<decltype(PCO_GetCameraType)>(GET_PROC_ADDRESS(_dllHandle, "PCO_GetCameraType"));
        if (!PCO_GetCameraType) throw std::runtime_error("Failed to load function: PCO_GetCameraType");
        PCO_GetCameraHealthStatus = reinterpret_cast<decltype(PCO_GetCameraHealthStatus)>(GET_PROC_ADDRESS(_dllHandle, "PCO_GetCameraHealthStatus"));
        if (!PCO_GetCameraHealthStatus) throw std::runtime_error("Failed to load function: PCO_GetCameraHealthStatus");
        PCO_ResetSettingsToDefault = reinterpret_cast<decltype(PCO_ResetSettingsToDefault)>(GET_PROC_ADDRESS(_dllHandle, "PCO_ResetSettingsToDefault"));
        if (!PCO_ResetSettingsToDefault) throw std::runtime_error("Failed to load function: PCO_ResetSettingsToDefault");
        PCO_GetCameraDescription = reinterpret_cast<decltype(PCO_GetCameraDescription)>(GET_PROC_ADDRESS(_dllHandle, "PCO_GetCameraDescription"));
        if (!PCO_GetCameraDescription) throw std::runtime_error("Failed to load function: PCO_GetCameraDescription");
        PCO_GetCameraName = reinterpret_cast<decltype(PCO_GetCameraName)>(GET_PROC_ADDRESS(_dllHandle, "PCO_GetCameraName"));
        if (!PCO_GetCameraName) throw std::runtime_error("Failed to load function: PCO_GetCameraName");
        PCO_GetSizes = reinterpret_cast<decltype(PCO_GetSizes)>(GET_PROC_ADDRESS(_dllHandle, "PCO_GetSizes"));
        if (!PCO_GetSizes) throw std::runtime_error("Failed to load function: PCO_GetSizes");
        PCO_GetPixelRate = reinterpret_cast<decltype(PCO_GetPixelRate)>(GET_PROC_ADDRESS(_dllHandle, "PCO_GetPixelRate"));
        if (!PCO_GetPixelRate) throw std::runtime_error("Failed to load function: PCO_GetPixelRate");
        PCO_SetPixelRate = reinterpret_cast<decltype(PCO_SetPixelRate)>(GET_PROC_ADDRESS(_dllHandle, "PCO_SetPixelRate"));
        if (!PCO_SetPixelRate) throw std::runtime_error("Failed to load function: PCO_SetPixelRate");
        PCO_ArmCamera = reinterpret_cast<decltype(PCO_ArmCamera)>(GET_PROC_ADDRESS(_dllHandle, "PCO_ArmCamera"));
        if (!PCO_ArmCamera) throw std::runtime_error("Failed to load function: PCO_ArmCamera");
        PCO_GetDelayExposureTime = reinterpret_cast<decltype(PCO_GetDelayExposureTime)>(GET_PROC_ADDRESS(_dllHandle, "PCO_GetDelayExposureTime"));
        if (!PCO_GetDelayExposureTime) throw std::runtime_error("Failed to load function: PCO_GetDelayExposureTime");
        PCO_SetDelayExposureTime = reinterpret_cast<decltype(PCO_SetDelayExposureTime)>(GET_PROC_ADDRESS(_dllHandle, "PCO_SetDelayExposureTime"));
        if (!PCO_SetDelayExposureTime) throw std::runtime_error("Failed to load function: PCO_SetDelayExposureTime");
        PCO_SetRecordingState = reinterpret_cast<decltype(PCO_SetRecordingState)>(GET_PROC_ADDRESS(_dllHandle, "PCO_SetRecordingState"));
        if (!PCO_SetRecordingState) throw std::runtime_error("Failed to load function: PCO_SetRecordingState");
        PCO_SetAcquireModeEx = reinterpret_cast<decltype(PCO_SetAcquireModeEx)>(GET_PROC_ADDRESS(_dllHandle, "PCO_SetAcquireModeEx"));
        if (!PCO_SetAcquireModeEx) throw std::runtime_error("Failed to load function: PCO_SetAcquireModeEx");
        PCO_SetStorageMode = reinterpret_cast<decltype(PCO_SetStorageMode)>(GET_PROC_ADDRESS(_dllHandle, "PCO_SetStorageMode"));
        if (!PCO_SetStorageMode) throw std::runtime_error("Failed to load function: PCO_SetStorageMode");
        PCO_SetTimestampMode = reinterpret_cast<decltype(PCO_SetTimestampMode)>(GET_PROC_ADDRESS(_dllHandle, "PCO_SetTimestampMode"));
        if (!PCO_SetTimestampMode) throw std::runtime_error("Failed to load function: PCO_SetTimestampMode");
        PCO_SetAcquireControl = reinterpret_cast<decltype(PCO_SetAcquireControl)>(GET_PROC_ADDRESS(_dllHandle, "PCO_SetAcquireControl"));
        if (!PCO_SetAcquireControl) throw std::runtime_error("Failed to load function: PCO_SetAcquireControl");
        PCO_AddBufferExtern = reinterpret_cast<decltype(PCO_AddBufferExtern)>(GET_PROC_ADDRESS(_dllHandle, "PCO_AddBufferExtern"));
        if (!PCO_AddBufferExtern) throw std::runtime_error("Failed to load function: PCO_AddBufferExtern");
#ifdef __linux__
        PCO_WaitforNextBufferAdr = reinterpret_cast<decltype(PCO_WaitforNextBufferAdr)>(GET_PROC_ADDRESS(_dllHandle, "PCO_WaitforNextBufferAdr"));
        if (!PCO_WaitforNextBufferAdr) throw std::runtime_error("Failed to load function: PCO_WaitforNextBufferAdr");
#endif
        PCO_GetErrorTextSDK = reinterpret_cast<decltype(PCO_GetErrorTextSDK)>(GET_PROC_ADDRESS(_dllHandle, "PCO_GetErrorTextSDK"));
        if (!PCO_GetErrorTextSDK) throw std::runtime_error("Failed to load function: PCO_GetErrorTextSDK");
        PCO_GetCOCRuntime = reinterpret_cast<decltype(PCO_GetCOCRuntime)>(GET_PROC_ADDRESS(_dllHandle, "PCO_GetCOCRuntime"));
        if (!PCO_GetCOCRuntime) throw std::runtime_error("Failed to load function: PCO_GetCOCRuntime");
        PCO_CloseCamera = reinterpret_cast<decltype(PCO_CloseCamera)>(GET_PROC_ADDRESS(_dllHandle, "PCO_CloseCamera"));
        if (!PCO_CloseCamera) throw std::runtime_error("Failed to load function: PCO_CloseCamera");
        PCO_SetMetaDataMode = reinterpret_cast<decltype(PCO_SetMetaDataMode)>(GET_PROC_ADDRESS(_dllHandle, "PCO_SetMetaDataMode"));
        if (!PCO_SetMetaDataMode) throw std::runtime_error("Failed to load function: PCO_SetMetaDataMode");
        PCO_OpenCamera = reinterpret_cast<decltype(PCO_OpenCamera)>(GET_PROC_ADDRESS(_dllHandle, "PCO_OpenCamera"));
        if (!PCO_OpenCamera) throw std::runtime_error("Failed to load function: PCO_OpenCamera");
        PCO_CancelImages = reinterpret_cast<decltype(PCO_CancelImages)>(GET_PROC_ADDRESS(_dllHandle, "PCO_CancelImages"));
        if (!PCO_CancelImages) throw std::runtime_error("Failed to load function: PCO_CancelImages");
        PCO_SetImageParameters = reinterpret_cast<decltype(PCO_SetImageParameters)>(GET_PROC_ADDRESS(_dllHandle, "PCO_SetImageParameters"));
        if (!PCO_SetImageParameters) throw std::runtime_error("Failed to load function: PCO_SetImageParameters");

        _allFunctionsLoaded = true;
    }

    ~PCOAPIWrapper() {
        if (_dllHandle) {
            FREE_LIBRARY(_dllHandle);
        }
    }

    PCOAPIWrapper(const PCOAPIWrapper&) = delete;
    PCOAPIWrapper& operator=(const PCOAPIWrapper&) = delete;

    bool areAllFunctionsLoaded() const {return _allFunctionsLoaded;}


private:
    LIB_HANDLE _dllHandle = nullptr;

    bool _allFunctionsLoaded = false;
};

inline PCOAPIWrapper GetPCOAPIWrapper() {
    return PCOAPIWrapper("sc2_cam");
}

#endif // PCOAPIWRAPPER_H
