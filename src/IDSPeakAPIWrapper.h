#ifndef IDSPEAKAPIWRAPPER_H
#define IDSPEAKAPIWRAPPER_H

#include <string>
#include <stdexcept>

// Include the IDS Peak Comfort C API header for type definitions first, as it might
// have internal dependencies or specific ordering requirements.
#include "ids_peak_comfort_c/ids_peak_comfort_c.h"

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

// Define PEAK_ERROR macro if not already defined (it's often in ids_peak_comfort_c.h)
#ifndef PEAK_ERROR
#define PEAK_ERROR(status) ((status) != peak_status_SUCCESS)
#endif

class IDSPeakAPIWrapper {
public:
    // Library functions
    decltype(&::peak_Library_Init) peak_Library_Init = nullptr;
    decltype(&::peak_Library_Exit) peak_Library_Exit = nullptr;
    decltype(&::peak_CameraList_Update) peak_CameraList_Update = nullptr;
    decltype(&::peak_CameraList_Get) peak_CameraList_Get = nullptr;

    // Camera functions
    decltype(&::peak_Camera_GetAccessStatus) peak_Camera_GetAccessStatus = nullptr;
    decltype(&::peak_Camera_Open) peak_Camera_Open = nullptr;
    decltype(&::peak_Camera_ResetToDefaultSettings) peak_Camera_ResetToDefaultSettings = nullptr;
    decltype(&::peak_Camera_Close) peak_Camera_Close = nullptr;

    // FrameRate functions
    decltype(&::peak_FrameRate_Get) peak_FrameRate_Get = nullptr;
    decltype(&::peak_FrameRate_GetRange) peak_FrameRate_GetRange = nullptr;
    decltype(&::peak_FrameRate_Set) peak_FrameRate_Set = nullptr;

    // PixelClock functions
    decltype(&::peak_PixelClock_GetAccessStatus) peak_PixelClock_GetAccessStatus = nullptr;
    decltype(&::peak_PixelClock_HasRange) peak_PixelClock_HasRange = nullptr;
    decltype(&::peak_PixelClock_GetRange) peak_PixelClock_GetRange = nullptr;
    decltype(&::peak_PixelClock_Set) peak_PixelClock_Set = nullptr;
    decltype(&::peak_PixelClock_Get) peak_PixelClock_Get = nullptr;
    decltype(&::peak_PixelClock_GetList) peak_PixelClock_GetList = nullptr;

    // Gain functions
    decltype(&::peak_Gain_GetRange) peak_Gain_GetRange = nullptr;
    decltype(&::peak_Gain_Set) peak_Gain_Set = nullptr;
    decltype(&::peak_Gain_Get) peak_Gain_Get = nullptr;

    // ROI functions
    decltype(&::peak_ROI_Size_Get) peak_ROI_Size_Get = nullptr;

    // ExposureTime functions
    decltype(&::peak_ExposureTime_GetRange) peak_ExposureTime_GetRange = nullptr;
    decltype(&::peak_ExposureTime_Set) peak_ExposureTime_Set = nullptr;
    decltype(&::peak_ExposureTime_Get) peak_ExposureTime_Get = nullptr;

    // Acquisition functions
    decltype(&::peak_Acquisition_Start) peak_Acquisition_Start = nullptr;
    decltype(&::peak_Acquisition_Stop) peak_Acquisition_Stop = nullptr;
    decltype(&::peak_Acquisition_WaitForFrame) peak_Acquisition_WaitForFrame = nullptr;

    // Frame functions
    decltype(&::peak_Frame_IsComplete) peak_Frame_IsComplete = nullptr;
    decltype(&::peak_Frame_GetInfo) peak_Frame_GetInfo = nullptr;
    decltype(&::peak_Frame_Release) peak_Frame_Release = nullptr;

    IDSPeakAPIWrapper() {
        const char* libraryName =
#ifdef _WIN32
            "peak_comfort_c.dll";
#else
            "libpeak_comfort_c.so";
#endif

        _hModule = LOAD_LIBRARY(libraryName);
        if (!_hModule) {
            // we don't have the library, so the runtime probably isn't installed.
            _allFunctionsLoaded = false;
            return;
        }

        // Library functions
        peak_Library_Init = reinterpret_cast<decltype(peak_Library_Init)>(GET_PROC_ADDRESS(_hModule, "peak_Library_Init"));
        if (!peak_Library_Init) throw std::runtime_error("Failed to load function: peak_Library_Init");
        peak_Library_Exit = reinterpret_cast<decltype(peak_Library_Exit)>(GET_PROC_ADDRESS(_hModule, "peak_Library_Exit"));
        if (!peak_Library_Exit) throw std::runtime_error("Failed to load function: peak_Library_Exit");
        peak_CameraList_Update = reinterpret_cast<decltype(peak_CameraList_Update)>(GET_PROC_ADDRESS(_hModule, "peak_CameraList_Update"));
        if (!peak_CameraList_Update) throw std::runtime_error("Failed to load function: peak_CameraList_Update");
        peak_CameraList_Get = reinterpret_cast<decltype(peak_CameraList_Get)>(GET_PROC_ADDRESS(_hModule, "peak_CameraList_Get"));
        if (!peak_CameraList_Get) throw std::runtime_error("Failed to load function: peak_CameraList_Get");

        // Camera functions
        peak_Camera_GetAccessStatus = reinterpret_cast<decltype(peak_Camera_GetAccessStatus)>(GET_PROC_ADDRESS(_hModule, "peak_Camera_GetAccessStatus"));
        if (!peak_Camera_GetAccessStatus) throw std::runtime_error("Failed to load function: peak_Camera_GetAccessStatus");
        peak_Camera_Open = reinterpret_cast<decltype(peak_Camera_Open)>(GET_PROC_ADDRESS(_hModule, "peak_Camera_Open"));
        if (!peak_Camera_Open) throw std::runtime_error("Failed to load function: peak_Camera_Open");
        peak_Camera_ResetToDefaultSettings = reinterpret_cast<decltype(peak_Camera_ResetToDefaultSettings)>(GET_PROC_ADDRESS(_hModule, "peak_Camera_ResetToDefaultSettings"));
        if (!peak_Camera_ResetToDefaultSettings) throw std::runtime_error("Failed to load function: peak_Camera_ResetToDefaultSettings");
        peak_Camera_Close = reinterpret_cast<decltype(peak_Camera_Close)>(GET_PROC_ADDRESS(_hModule, "peak_Camera_Close"));
        if (!peak_Camera_Close) throw std::runtime_error("Failed to load function: peak_Camera_Close");

        // FrameRate functions
        peak_FrameRate_Get = reinterpret_cast<decltype(peak_FrameRate_Get)>(GET_PROC_ADDRESS(_hModule, "peak_FrameRate_Get"));
        if (!peak_FrameRate_Get) throw std::runtime_error("Failed to load function: peak_FrameRate_Get");
        peak_FrameRate_GetRange = reinterpret_cast<decltype(peak_FrameRate_GetRange)>(GET_PROC_ADDRESS(_hModule, "peak_FrameRate_GetRange"));
        if (!peak_FrameRate_GetRange) throw std::runtime_error("Failed to load function: peak_FrameRate_GetRange");
        peak_FrameRate_Set = reinterpret_cast<decltype(peak_FrameRate_Set)>(GET_PROC_ADDRESS(_hModule, "peak_FrameRate_Set"));
        if (!peak_FrameRate_Set) throw std::runtime_error("Failed to load function: peak_FrameRate_Set");

        // PixelClock functions
        peak_PixelClock_GetAccessStatus = reinterpret_cast<decltype(peak_PixelClock_GetAccessStatus)>(GET_PROC_ADDRESS(_hModule, "peak_PixelClock_GetAccessStatus"));
        if (!peak_PixelClock_GetAccessStatus) throw std::runtime_error("Failed to load function: peak_PixelClock_GetAccessStatus");
        peak_PixelClock_HasRange = reinterpret_cast<decltype(peak_PixelClock_HasRange)>(GET_PROC_ADDRESS(_hModule, "peak_PixelClock_HasRange"));
        if (!peak_PixelClock_HasRange) throw std::runtime_error("Failed to load function: peak_PixelClock_HasRange");
        peak_PixelClock_GetRange = reinterpret_cast<decltype(peak_PixelClock_GetRange)>(GET_PROC_ADDRESS(_hModule, "peak_PixelClock_GetRange"));
        if (!peak_PixelClock_GetRange) throw std::runtime_error("Failed to load function: peak_PixelClock_GetRange");
        peak_PixelClock_Set = reinterpret_cast<decltype(peak_PixelClock_Set)>(GET_PROC_ADDRESS(_hModule, "peak_PixelClock_Set"));
        if (!peak_PixelClock_Set) throw std::runtime_error("Failed to load function: peak_PixelClock_Set");
        peak_PixelClock_Get = reinterpret_cast<decltype(peak_PixelClock_Get)>(GET_PROC_ADDRESS(_hModule, "peak_PixelClock_Get"));
        if (!peak_PixelClock_Get) throw std::runtime_error("Failed to load function: peak_PixelClock_Get");
        peak_PixelClock_GetList = reinterpret_cast<decltype(peak_PixelClock_GetList)>(GET_PROC_ADDRESS(_hModule, "peak_PixelClock_GetList"));
        if (!peak_PixelClock_GetList) throw std::runtime_error("Failed to load function: peak_PixelClock_GetList");

        // Gain functions
        peak_Gain_GetRange = reinterpret_cast<decltype(peak_Gain_GetRange)>(GET_PROC_ADDRESS(_hModule, "peak_Gain_GetRange"));
        if (!peak_Gain_GetRange) throw std::runtime_error("Failed to load function: peak_Gain_GetRange");
        peak_Gain_Set = reinterpret_cast<decltype(peak_Gain_Set)>(GET_PROC_ADDRESS(_hModule, "peak_Gain_Set"));
        if (!peak_Gain_Set) throw std::runtime_error("Failed to load function: peak_Gain_Set");
        peak_Gain_Get = reinterpret_cast<decltype(peak_Gain_Get)>(GET_PROC_ADDRESS(_hModule, "peak_Gain_Get"));
        if (!peak_Gain_Get) throw std::runtime_error("Failed to load function: peak_Gain_Get");

        // ROI functions
        peak_ROI_Size_Get = reinterpret_cast<decltype(peak_ROI_Size_Get)>(GET_PROC_ADDRESS(_hModule, "peak_ROI_Size_Get"));
        if (!peak_ROI_Size_Get) throw std::runtime_error("Failed to load function: peak_ROI_Size_Get");

        // ExposureTime functions
        peak_ExposureTime_GetRange = reinterpret_cast<decltype(peak_ExposureTime_GetRange)>(GET_PROC_ADDRESS(_hModule, "peak_ExposureTime_GetRange"));
        if (!peak_ExposureTime_GetRange) throw std::runtime_error("Failed to load function: peak_ExposureTime_GetRange");
        peak_ExposureTime_Set = reinterpret_cast<decltype(peak_ExposureTime_Set)>(GET_PROC_ADDRESS(_hModule, "peak_ExposureTime_Set"));
        if (!peak_ExposureTime_Set) throw std::runtime_error("Failed to load function: peak_ExposureTime_Set");
        peak_ExposureTime_Get = reinterpret_cast<decltype(peak_ExposureTime_Get)>(GET_PROC_ADDRESS(_hModule, "peak_ExposureTime_Get"));
        if (!peak_ExposureTime_Get) throw std::runtime_error("Failed to load function: peak_ExposureTime_Get");

        // Acquisition functions
        peak_Acquisition_Start = reinterpret_cast<decltype(peak_Acquisition_Start)>(GET_PROC_ADDRESS(_hModule, "peak_Acquisition_Start"));
        if (!peak_Acquisition_Start) throw std::runtime_error("Failed to load function: peak_Acquisition_Start");
        peak_Acquisition_Stop = reinterpret_cast<decltype(peak_Acquisition_Stop)>(GET_PROC_ADDRESS(_hModule, "peak_Acquisition_Stop"));
        if (!peak_Acquisition_Stop) throw std::runtime_error("Failed to load function: peak_Acquisition_Stop");
        peak_Acquisition_WaitForFrame = reinterpret_cast<decltype(peak_Acquisition_WaitForFrame)>(GET_PROC_ADDRESS(_hModule, "peak_Acquisition_WaitForFrame"));
        if (!peak_Acquisition_WaitForFrame) throw std::runtime_error("Failed to load function: peak_Acquisition_WaitForFrame");

        // Frame functions
        peak_Frame_IsComplete = reinterpret_cast<decltype(peak_Frame_IsComplete)>(GET_PROC_ADDRESS(_hModule, "peak_Frame_IsComplete"));
        if (!peak_Frame_IsComplete) throw std::runtime_error("Failed to load function: peak_Frame_IsComplete");
        peak_Frame_GetInfo = reinterpret_cast<decltype(peak_Frame_GetInfo)>(GET_PROC_ADDRESS(_hModule, "peak_Frame_GetInfo"));
        if (!peak_Frame_GetInfo) throw std::runtime_error("Failed to load function: peak_Frame_GetInfo");
        peak_Frame_Release = reinterpret_cast<decltype(peak_Frame_Release)>(GET_PROC_ADDRESS(_hModule, "peak_Frame_Release"));
        if (!peak_Frame_Release) throw std::runtime_error("Failed to load function: peak_Frame_Release");

        _allFunctionsLoaded = true;
    }

    ~IDSPeakAPIWrapper() {
        if (_hModule) {
            FREE_LIBRARY(_hModule);
        }
    }

    IDSPeakAPIWrapper(const IDSPeakAPIWrapper&) = delete;
    IDSPeakAPIWrapper& operator=(const IDSPeakAPIWrapper&) = delete;

    bool areAllFunctionsLoaded() const { return _allFunctionsLoaded; }

private:
    LIB_HANDLE _hModule = nullptr;
    bool _allFunctionsLoaded = false;
};

#endif