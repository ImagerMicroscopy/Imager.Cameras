#include <string>
#include <cstdint>
#include <vector>
#include <functional> // For std::function
#include <stdexcept>  // For std::logic_error

// Include the IDS Peak Comfort C API header for type definitions first, as it might
// have internal dependencies or specific ordering requirements.
#include "ids_peak_comfort_c/ids_peak_comfort_c.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h> // For dlopen, dlsym, dlclose on Linux
#endif

// Define PEAK_ERROR macro if not already defined (it's often in ids_peak_comfort_c.h)
#ifndef PEAK_ERROR
#define PEAK_ERROR(status) ((status) != peak_status_SUCCESS)
#endif

class IDSPeakAPIWrapper {
public:
    IDSPeakAPIWrapper() :
        m_isLibraryLoaded(false),
#ifdef _WIN32
        m_hModule(nullptr)
#else
        m_hModule(nullptr)
#endif
    {
        loadLibraryAndFunctions();
    }

    ~IDSPeakAPIWrapper() {
        unloadLibrary();
    }

    // Delete copy constructor and assignment operator to prevent copying
    IDSPeakAPIWrapper(const IDSPeakAPIWrapper&) = delete;
    IDSPeakAPIWrapper& operator=(const IDSPeakAPIWrapper&) = delete;

    // Member to check if all required functions are loaded
    bool areAllFunctionsLoaded() const {
        return m_isLibraryLoaded &&
               // Check each function pointer for validity
               peak_Library_Init_ptr &&
               peak_Library_Exit_ptr &&
               peak_CameraList_Update_ptr &&
               peak_CameraList_Get_ptr &&
               peak_Camera_GetAccessStatus_ptr &&
               peak_Camera_Open_ptr &&
               peak_Camera_ResetToDefaultSettings_ptr &&
               peak_Camera_Close_ptr &&
               peak_FrameRate_Get_ptr &&
               peak_PixelClock_GetAccessStatus_ptr &&
               peak_PixelClock_HasRange_ptr &&
               peak_PixelClock_GetRange_ptr &&
               peak_PixelClock_Set_ptr &&
               peak_PixelClock_Get_ptr &&
               peak_PixelClock_GetList_ptr &&
               peak_Gain_GetRange_ptr &&
               peak_Gain_Set_ptr &&
               peak_Gain_Get_ptr &&
               peak_ROI_Size_Get_ptr &&
               peak_FrameRate_GetRange_ptr &&
               peak_FrameRate_Set_ptr &&
               peak_ExposureTime_GetRange_ptr &&
               peak_ExposureTime_Set_ptr &&
               peak_ExposureTime_Get_ptr &&
               peak_Acquisition_Start_ptr &&
               peak_Acquisition_Stop_ptr &&
               peak_Acquisition_WaitForFrame_ptr &&
               peak_Frame_IsComplete_ptr &&
               peak_Frame_GetInfo_ptr &&
               peak_Frame_Release_ptr;
    }

    // Explicit public wrapper methods (formerly PEAK_FN_IMPLEMENT_WRAPPER)
    peak_status peak_Library_Init() {
        if (peak_Library_Init_ptr) {
            return peak_Library_Init_ptr();
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Library_Init");
    }

    peak_status peak_Library_Exit() {
        if (peak_Library_Exit_ptr) {
            return peak_Library_Exit_ptr();
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Library_Exit");
    }

    peak_status peak_CameraList_Update(size_t* cameraCount) {
        if (peak_CameraList_Update_ptr) {
            return peak_CameraList_Update_ptr(cameraCount);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_CameraList_Update");
    }

    peak_status peak_CameraList_Get(peak_camera_descriptor* camDescriptors, size_t* numCameras) {
        if (peak_CameraList_Get_ptr) {
            return peak_CameraList_Get_ptr(camDescriptors, numCameras);
        }
        if (numCameras) *numCameras = 0; // Initialize out parameter before throwing
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_CameraList_Get");
    }

    peak_access_status peak_Camera_GetAccessStatus(peak_camera_id cameraID) {
        if (peak_Camera_GetAccessStatus_ptr) {
            return peak_Camera_GetAccessStatus_ptr(cameraID);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Camera_GetAccessStatus");
    }

    peak_status peak_Camera_Open(peak_camera_id cameraID, peak_camera_handle* hCam) {
        if (peak_Camera_Open_ptr) {
            return peak_Camera_Open_ptr(cameraID, hCam);
        }
        if (hCam) *hCam = nullptr; // Initialize out parameter before throwing
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Camera_Open");
    }

    peak_status peak_Camera_ResetToDefaultSettings(peak_camera_handle hCam) {
        if (peak_Camera_ResetToDefaultSettings_ptr) {
            return peak_Camera_ResetToDefaultSettings_ptr(hCam);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Camera_ResetToDefaultSettings");
    }

    peak_status peak_Camera_Close(peak_camera_handle hCam) {
        if (peak_Camera_Close_ptr) {
            return peak_Camera_Close_ptr(hCam);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Camera_Close");
    }

    peak_status peak_FrameRate_Get(peak_camera_handle hCam, double* frameRate) {
        if (peak_FrameRate_Get_ptr) {
            return peak_FrameRate_Get_ptr(hCam, frameRate);
        }
        if (frameRate) *frameRate = 0.0; // Initialize out parameter before throwing
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_FrameRate_Get");
    }

    peak_status peak_FrameRate_GetRange(peak_camera_handle hCam, double* minFrameRate, double* maxFrameRate, double* frameRateStep) {
        if (peak_FrameRate_GetRange_ptr) {
            return peak_FrameRate_GetRange_ptr(hCam, minFrameRate, maxFrameRate, frameRateStep);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_FrameRate_GetRange");
    }

    peak_status peak_FrameRate_Set(peak_camera_handle hCam, double frameRate) {
        if (peak_FrameRate_Set_ptr) {
            return peak_FrameRate_Set_ptr(hCam, frameRate);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_FrameRate_Set");
    }

    peak_access_status peak_PixelClock_GetAccessStatus(peak_camera_handle hCam) {
        if (peak_PixelClock_GetAccessStatus_ptr) {
            return peak_PixelClock_GetAccessStatus_ptr(hCam);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_PixelClock_GetAccessStatus");
    }

    peak_bool peak_PixelClock_HasRange(peak_camera_handle hCam) {
        if (peak_PixelClock_HasRange_ptr) {
            return peak_PixelClock_HasRange_ptr(hCam);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_PixelClock_HasRange");
    }

    peak_status peak_PixelClock_GetRange(peak_camera_handle hCam, double* minPixelClock_MHz, double* maxPixelClock_MHz, double* incPixelClock_MHz) {
        if (peak_PixelClock_GetRange_ptr) {
            return peak_PixelClock_GetRange_ptr(hCam, minPixelClock_MHz, maxPixelClock_MHz, incPixelClock_MHz);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_PixelClock_GetRange");
    }

    peak_status peak_PixelClock_Set(peak_camera_handle hCam, double pixelClock_MHz) {
        if (peak_PixelClock_Set_ptr) {
            return peak_PixelClock_Set_ptr(hCam, pixelClock_MHz);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_PixelClock_Set");
    }

    peak_status peak_PixelClock_Get(peak_camera_handle hCam, double* pixelClock_MHz) {
        if (peak_PixelClock_Get_ptr) {
            return peak_PixelClock_Get_ptr(hCam, pixelClock_MHz);
        }
        if (pixelClock_MHz) *pixelClock_MHz = 0.0; // Initialize out parameter before throwing
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_PixelClock_Get");
    }

    peak_status peak_PixelClock_GetList(peak_camera_handle hCam, double* availableClocks, size_t* numClocks) {
        if (peak_PixelClock_GetList_ptr) {
            return peak_PixelClock_GetList_ptr(hCam, availableClocks, numClocks);
        }
        if (numClocks) *numClocks = 0; // Initialize out parameter before throwing
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_PixelClock_GetList");
    }

    peak_status peak_Gain_GetRange(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double* minGain, double* maxGain, double* incGain) {
        if (peak_Gain_GetRange_ptr) {
            return peak_Gain_GetRange_ptr(hCam, gainType, gainChannel, minGain, maxGain, incGain);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Gain_GetRange");
    }

    peak_status peak_Gain_Set(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double gain) {
        if (peak_Gain_Set_ptr) {
            return peak_Gain_Set_ptr(hCam, gainType, gainChannel, gain);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Gain_Set");
    }

    peak_status peak_Gain_Get(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double* gain) {
        if (peak_Gain_Get_ptr) {
            return peak_Gain_Get_ptr(hCam, gainType, gainChannel, gain);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Gain_Get");
    }

    peak_status peak_ROI_Size_Get(peak_camera_handle hCam, peak_size* size) {
        if (peak_ROI_Size_Get_ptr) {
            return peak_ROI_Size_Get_ptr(hCam, size);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_ROI_Size_Get");
    }

    peak_status peak_ExposureTime_GetRange(peak_camera_handle hCam, double* minExp, double* maxExp, double* expStep) {
        if (peak_ExposureTime_GetRange_ptr) {
            return peak_ExposureTime_GetRange_ptr(hCam, minExp, maxExp, expStep);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_ExposureTime_GetRange");
    }

    peak_status peak_ExposureTime_Set(peak_camera_handle hCam, double exposureTime_us) {
        if (peak_ExposureTime_Set_ptr) {
            return peak_ExposureTime_Set_ptr(hCam, exposureTime_us);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_ExposureTime_Set");
    }

    peak_status peak_ExposureTime_Get(peak_camera_handle hCam, double* exposureTime_us) {
        if (peak_ExposureTime_Get_ptr) {
            return peak_ExposureTime_Get_ptr(hCam, exposureTime_us);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_ExposureTime_Get");
    }

    peak_status peak_Acquisition_Start(peak_camera_handle hCam, int32_t numFrames) {
        if (peak_Acquisition_Start_ptr) {
            return peak_Acquisition_Start_ptr(hCam, numFrames);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Acquisition_Start");
    }

    peak_status peak_Acquisition_Stop(peak_camera_handle hCam) {
        if (peak_Acquisition_Stop_ptr) {
            return peak_Acquisition_Stop_ptr(hCam);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Acquisition_Stop");
    }

    peak_status peak_Acquisition_WaitForFrame(peak_camera_handle hCam, int32_t timeoutMs, peak_frame_handle* hFrame) {
        if (peak_Acquisition_WaitForFrame_ptr) {
            return peak_Acquisition_WaitForFrame_ptr(hCam, timeoutMs, hFrame);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Acquisition_WaitForFrame");
    }

    peak_bool peak_Frame_IsComplete(peak_frame_handle hFrame) {
        if (peak_Frame_IsComplete_ptr) {
            return peak_Frame_IsComplete_ptr(hFrame);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Frame_IsComplete");
    }

    peak_status peak_Frame_GetInfo(peak_frame_handle hFrame, peak_frame_info* info) {
        if (peak_Frame_GetInfo_ptr) {
            return peak_Frame_GetInfo_ptr(hFrame, info);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Frame_GetInfo");
    }

    peak_status peak_Frame_Release(peak_camera_handle hCam, peak_frame_handle hFrame) {
        if (peak_Frame_Release_ptr) {
            return peak_Frame_Release_ptr(hCam, hFrame);
        }
        throw std::logic_error("Attempt to execute unloaded IDS Peak Comfort C API function: peak_Frame_Release");
    }


private:
    // Platform-specific library handle
#ifdef _WIN32
    HMODULE m_hModule;
#else
    void* m_hModule;
#endif

    bool m_isLibraryLoaded;

    // Explicitly declared function pointer typedefs and member variables
    // Library functions
    using F_peak_Library_Init = peak_status (PEAK_CALLCONV*)();
    F_peak_Library_Init peak_Library_Init_ptr;

    using F_peak_Library_Exit = peak_status (PEAK_CALLCONV*)();
    F_peak_Library_Exit peak_Library_Exit_ptr;

    using F_peak_CameraList_Update = peak_status (PEAK_CALLCONV*)(size_t* cameraCount);
    F_peak_CameraList_Update peak_CameraList_Update_ptr;

    using F_peak_CameraList_Get = peak_status (PEAK_CALLCONV*)(peak_camera_descriptor* camDescriptors, size_t* numCameras);
    F_peak_CameraList_Get peak_CameraList_Get_ptr;

    // Camera functions
    using F_peak_Camera_GetAccessStatus = peak_access_status (PEAK_CALLCONV*)(peak_camera_id cameraID);
    F_peak_Camera_GetAccessStatus peak_Camera_GetAccessStatus_ptr;

    using F_peak_Camera_Open = peak_status (PEAK_CALLCONV*)(peak_camera_id cameraID, peak_camera_handle* hCam);
    F_peak_Camera_Open peak_Camera_Open_ptr;

    using F_peak_Camera_ResetToDefaultSettings = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam);
    F_peak_Camera_ResetToDefaultSettings peak_Camera_ResetToDefaultSettings_ptr;

    using F_peak_Camera_Close = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam);
    F_peak_Camera_Close peak_Camera_Close_ptr;

    // FrameRate functions
    using F_peak_FrameRate_Get = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, double* frameRate);
    F_peak_FrameRate_Get peak_FrameRate_Get_ptr;

    using F_peak_FrameRate_GetRange = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, double* minFrameRate, double* maxFrameRate, double* frameRateStep);
    F_peak_FrameRate_GetRange peak_FrameRate_GetRange_ptr;

    using F_peak_FrameRate_Set = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, double frameRate);
    F_peak_FrameRate_Set peak_FrameRate_Set_ptr;

    // PixelClock functions
    using F_peak_PixelClock_GetAccessStatus = peak_access_status (PEAK_CALLCONV*)(peak_camera_handle hCam);
    F_peak_PixelClock_GetAccessStatus peak_PixelClock_GetAccessStatus_ptr;

    using F_peak_PixelClock_HasRange = peak_bool (PEAK_CALLCONV*)(peak_camera_handle hCam);
    F_peak_PixelClock_HasRange peak_PixelClock_HasRange_ptr;

    using F_peak_PixelClock_GetRange = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, double* minPixelClock_MHz, double* maxPixelClock_MHz, double* incPixelClock_MHz);
    F_peak_PixelClock_GetRange peak_PixelClock_GetRange_ptr;

    using F_peak_PixelClock_Set = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, double pixelClock_MHz);
    F_peak_PixelClock_Set peak_PixelClock_Set_ptr;

    using F_peak_PixelClock_Get = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, double* pixelClock_MHz);
    F_peak_PixelClock_Get peak_PixelClock_Get_ptr;

    using F_peak_PixelClock_GetList = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, double* availableClocks, size_t* numClocks);
    F_peak_PixelClock_GetList peak_PixelClock_GetList_ptr;

    // Gain functions
    using F_peak_Gain_GetRange = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double* minGain, double* maxGain, double* incGain);
    F_peak_Gain_GetRange peak_Gain_GetRange_ptr;

    using F_peak_Gain_Set = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double gain);
    F_peak_Gain_Set peak_Gain_Set_ptr;

    using F_peak_Gain_Get = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double* gain);
    F_peak_Gain_Get peak_Gain_Get_ptr;

    // ROI functions
    using F_peak_ROI_Size_Get = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, peak_size* size);
    F_peak_ROI_Size_Get peak_ROI_Size_Get_ptr;

    // ExposureTime functions
    using F_peak_ExposureTime_GetRange = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, double* minExp, double* maxExp, double* expStep);
    F_peak_ExposureTime_GetRange peak_ExposureTime_GetRange_ptr;

    using F_peak_ExposureTime_Set = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, double exposureTime_us);
    F_peak_ExposureTime_Set peak_ExposureTime_Set_ptr;

    using F_peak_ExposureTime_Get = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, double* exposureTime_us);
    F_peak_ExposureTime_Get peak_ExposureTime_Get_ptr;

    // Acquisition functions
    using F_peak_Acquisition_Start = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, int32_t numFrames);
    F_peak_Acquisition_Start peak_Acquisition_Start_ptr;

    using F_peak_Acquisition_Stop = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam);
    F_peak_Acquisition_Stop peak_Acquisition_Stop_ptr;

    using F_peak_Acquisition_WaitForFrame = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, int32_t timeoutMs, peak_frame_handle* hFrame);
    F_peak_Acquisition_WaitForFrame peak_Acquisition_WaitForFrame_ptr;

    // Frame functions
    using F_peak_Frame_IsComplete = peak_bool (PEAK_CALLCONV*)(peak_frame_handle hFrame);
    F_peak_Frame_IsComplete peak_Frame_IsComplete_ptr;

    using F_peak_Frame_GetInfo = peak_status (PEAK_CALLCONV*)(peak_frame_handle hFrame, peak_frame_info* info);
    F_peak_Frame_GetInfo peak_Frame_GetInfo_ptr;

    using F_peak_Frame_Release = peak_status (PEAK_CALLCONV*)(peak_camera_handle hCam, peak_frame_handle hFrame);
    F_peak_Frame_Release peak_Frame_Release_ptr;


    // Helper to get function pointer, no exceptions/logging on failure
    template<typename T>
    T getFunction(const char* funcName) {
#ifdef _WIN32
        return reinterpret_cast<T>(GetProcAddress(m_hModule, funcName));
#else
        return reinterpret_cast<T>(dlsym(m_hModule, funcName));
#endif
    }

    void loadLibraryAndFunctions() {
        const char* libraryName =
#ifdef _WIN32
            "peak_comfort_c.dll";
#else
            "libpeak_comfort_c.so";
#endif

        // Load the library
#ifdef _WIN32
        m_hModule = LoadLibraryA(libraryName);
#else
        m_hModule = dlopen(libraryName, RTLD_LAZY);
#endif

        if (m_hModule) {
            m_isLibraryLoaded = true; // Tentatively set to true, will be refined below

            // Attempt to load all required function pointers
            // Library functions
            peak_Library_Init_ptr = getFunction<F_peak_Library_Init>("peak_Library_Init");
            peak_Library_Exit_ptr = getFunction<F_peak_Library_Exit>("peak_Library_Exit");
            peak_CameraList_Update_ptr = getFunction<F_peak_CameraList_Update>("peak_CameraList_Update");
            peak_CameraList_Get_ptr = getFunction<F_peak_CameraList_Get>("peak_CameraList_Get");

            // Camera functions
            peak_Camera_GetAccessStatus_ptr = getFunction<F_peak_Camera_GetAccessStatus>("peak_Camera_GetAccessStatus");
            peak_Camera_Open_ptr = getFunction<F_peak_Camera_Open>("peak_Camera_Open");
            peak_Camera_ResetToDefaultSettings_ptr = getFunction<F_peak_Camera_ResetToDefaultSettings>("peak_Camera_ResetToDefaultSettings");
            peak_Camera_Close_ptr = getFunction<F_peak_Camera_Close>("peak_Camera_Close");

            // FrameRate functions
            peak_FrameRate_Get_ptr = getFunction<F_peak_FrameRate_Get>("peak_FrameRate_Get");
            peak_FrameRate_GetRange_ptr = getFunction<F_peak_FrameRate_GetRange>("peak_FrameRate_GetRange");
            peak_FrameRate_Set_ptr = getFunction<F_peak_FrameRate_Set>("peak_FrameRate_Set");

            // PixelClock functions
            peak_PixelClock_GetAccessStatus_ptr = getFunction<F_peak_PixelClock_GetAccessStatus>("peak_PixelClock_GetAccessStatus");
            peak_PixelClock_HasRange_ptr = getFunction<F_peak_PixelClock_HasRange>("peak_PixelClock_HasRange");
            peak_PixelClock_GetRange_ptr = getFunction<F_peak_PixelClock_GetRange>("peak_PixelClock_GetRange");
            peak_PixelClock_Set_ptr = getFunction<F_peak_PixelClock_Set>("peak_PixelClock_Set");
            peak_PixelClock_Get_ptr = getFunction<F_peak_PixelClock_Get>("peak_PixelClock_Get");
            peak_PixelClock_GetList_ptr = getFunction<F_peak_PixelClock_GetList>("peak_PixelClock_GetList");

            // Gain functions
            peak_Gain_GetRange_ptr = getFunction<F_peak_Gain_GetRange>("peak_Gain_GetRange");
            peak_Gain_Set_ptr = getFunction<F_peak_Gain_Set>("peak_Gain_Set");
            peak_Gain_Get_ptr = getFunction<F_peak_Gain_Get>("peak_Gain_Get");

            // ROI functions
            peak_ROI_Size_Get_ptr = getFunction<F_peak_ROI_Size_Get>("peak_ROI_Size_Get");

            // ExposureTime functions
            peak_ExposureTime_GetRange_ptr = getFunction<F_peak_ExposureTime_GetRange>("peak_ExposureTime_GetRange");
            peak_ExposureTime_Set_ptr = getFunction<F_peak_ExposureTime_Set>("peak_ExposureTime_Set");
            peak_ExposureTime_Get_ptr = getFunction<F_peak_ExposureTime_Get>("peak_ExposureTime_Get");

            // Acquisition functions
            peak_Acquisition_Start_ptr = getFunction<F_peak_Acquisition_Start>("peak_Acquisition_Start");
            peak_Acquisition_Stop_ptr = getFunction<F_peak_Acquisition_Stop>("peak_Acquisition_Stop");
            peak_Acquisition_WaitForFrame_ptr = getFunction<F_peak_Acquisition_WaitForFrame>("peak_Acquisition_WaitForFrame");

            // Frame functions
            peak_Frame_IsComplete_ptr = getFunction<F_peak_Frame_IsComplete>("peak_Frame_IsComplete");
            peak_Frame_GetInfo_ptr = getFunction<F_peak_Frame_GetInfo>("peak_Frame_GetInfo");
            peak_Frame_Release_ptr = getFunction<F_peak_Frame_Release>("peak_Frame_Release");

            // After attempting to load all, check if all were actually loaded.
            // If not all functions were found, set m_isLibraryLoaded to false
            // This is important because the library might load but not all symbols exist.
            if (!areAllFunctionsLoaded()) {
                unloadLibrary(); // Unload if not all symbols could be resolved
            }

        } else {
            m_isLibraryLoaded = false;
        }
    }

    void unloadLibrary() {
        if (m_hModule) {
#ifdef _WIN32
            FreeLibrary(m_hModule);
#else
            dlclose(m_hModule);
#endif
            m_hModule = nullptr;
        }
        m_isLibraryLoaded = false;
    }
};