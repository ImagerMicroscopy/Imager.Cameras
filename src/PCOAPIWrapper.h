

#ifndef PCOAPIWRAPPER_H
#define PCOAPIWRAPPER_H

#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <any>
#include <stdexcept>

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

// Define types and structures
typedef void* HANDLE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef short SHORT;
typedef unsigned char BYTE;

// Macro to define function pointer types
#define DEFINE_FUNC_PTR(name, return_type, ...) \
    using name##_func = return_type(__stdcall*)(__VA_ARGS__);

// Define function pointer types for each function in the library
DEFINE_FUNC_PTR(PCO_GetGeneral, int, HANDLE, void*);
DEFINE_FUNC_PTR(PCO_GetCameraType, int, HANDLE, void*);
DEFINE_FUNC_PTR(PCO_GetCameraHealthStatus, int, HANDLE, DWORD*, DWORD*, DWORD*);
DEFINE_FUNC_PTR(PCO_ResetSettingsToDefault, int, HANDLE);
DEFINE_FUNC_PTR(PCO_GetTemperature, int, HANDLE, SHORT*, SHORT*, SHORT*);
DEFINE_FUNC_PTR(PCO_GetInfoString, int, HANDLE, DWORD, char*, WORD);
DEFINE_FUNC_PTR(PCO_GetCameraName, int, HANDLE, char*, WORD);
DEFINE_FUNC_PTR(PCO_GetFirmwareInfo, int, HANDLE, WORD, void*);
DEFINE_FUNC_PTR(PCO_GetCameraSetup, int, HANDLE, WORD*, DWORD*, WORD*);
DEFINE_FUNC_PTR(PCO_SetCameraSetup, int, HANDLE, WORD, DWORD*, WORD);
DEFINE_FUNC_PTR(PCO_RebootCamera, int, HANDLE);
DEFINE_FUNC_PTR(PCO_GetShutterMode, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetShutterMode, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetPowerSaveMode, int, HANDLE, WORD*, WORD*);
DEFINE_FUNC_PTR(PCO_SetPowerSaveMode, int, HANDLE, WORD, WORD);
DEFINE_FUNC_PTR(PCO_GetBatteryStatus, int, HANDLE, WORD*, WORD*, WORD*, WORD);
DEFINE_FUNC_PTR(PCO_GetFanControlParameters, int, HANDLE, WORD*, WORD*);
DEFINE_FUNC_PTR(PCO_SetFanControlParameters, int, HANDLE, WORD, WORD);
DEFINE_FUNC_PTR(PCO_GetSensorStruct, int, HANDLE, void*);
DEFINE_FUNC_PTR(PCO_SetSensorStruct, int, HANDLE, void*);
DEFINE_FUNC_PTR(PCO_GetCameraDescription, int, HANDLE, void*);
DEFINE_FUNC_PTR(PCO_GetCameraDescriptionEx, int, HANDLE, void*, WORD);
DEFINE_FUNC_PTR(PCO_GetSensorFormat, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetSensorFormat, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetSizes, int, HANDLE, WORD*, WORD*, WORD*, WORD*);
DEFINE_FUNC_PTR(PCO_GetROI, int, HANDLE, WORD*, WORD*, WORD*, WORD*);
DEFINE_FUNC_PTR(PCO_SetROI, int, HANDLE, WORD, WORD, WORD, WORD);
DEFINE_FUNC_PTR(PCO_GetBinning, int, HANDLE, WORD*, WORD*);
DEFINE_FUNC_PTR(PCO_SetBinning, int, HANDLE, WORD, WORD);
DEFINE_FUNC_PTR(PCO_GetPixelRate, int, HANDLE, DWORD*);
DEFINE_FUNC_PTR(PCO_SetPixelRate, int, HANDLE, DWORD);
DEFINE_FUNC_PTR(PCO_GetConversionFactor, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetConversionFactor, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetDoubleImageMode, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetDoubleImageMode, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetADCOperation, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetADCOperation, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetIRSensitivity, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetIRSensitivity, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetCoolingSetpoints, int, HANDLE, WORD, WORD*, SHORT*);
DEFINE_FUNC_PTR(PCO_GetCoolingSetpointTemperature, int, HANDLE, SHORT*);
DEFINE_FUNC_PTR(PCO_SetCoolingSetpointTemperature, int, HANDLE, SHORT);
DEFINE_FUNC_PTR(PCO_GetOffsetMode, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetOffsetMode, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetNoiseFilterMode, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetNoiseFilterMode, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetHWIOSignalCount, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_GetHWIOSignalDescriptor, int, HANDLE, WORD, void*);
DEFINE_FUNC_PTR(PCO_GetColorCorrectionMatrix, int, HANDLE, double*);
DEFINE_FUNC_PTR(PCO_GetDSNUAdjustMode, int, HANDLE, WORD*, WORD*);
DEFINE_FUNC_PTR(PCO_SetDSNUAdjustMode, int, HANDLE, WORD, WORD);
DEFINE_FUNC_PTR(PCO_InitDSNUAdjustment, int, HANDLE, WORD, WORD);
DEFINE_FUNC_PTR(PCO_GetCDIMode, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetCDIMode, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetLookupTableInfo, int, HANDLE, WORD, WORD*, char*, WORD, WORD*, BYTE*, BYTE*, WORD*);
DEFINE_FUNC_PTR(PCO_GetActiveLookupTable, int, HANDLE, WORD*, WORD*);
DEFINE_FUNC_PTR(PCO_SetActiveLookupTable, int, HANDLE, WORD*, WORD*);
DEFINE_FUNC_PTR(PCO_GetIntensifiedGatingMode, int, HANDLE, WORD*, WORD*);
DEFINE_FUNC_PTR(PCO_SetIntensifiedGatingMode, int, HANDLE, WORD, WORD);
DEFINE_FUNC_PTR(PCO_GetIntensifiedMCP, int, HANDLE, WORD*, WORD*, DWORD*, DWORD*, DWORD*);
DEFINE_FUNC_PTR(PCO_SetIntensifiedMCP, int, HANDLE, WORD, WORD, WORD, DWORD, DWORD, DWORD);
DEFINE_FUNC_PTR(PCO_GetSensorDarkOffset, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_GetTimingStruct, int, HANDLE, void*);
DEFINE_FUNC_PTR(PCO_SetTimingStruct, int, HANDLE, void*);
DEFINE_FUNC_PTR(PCO_GetDelayExposureTime, int, HANDLE, DWORD*, DWORD*, WORD*, WORD*);
DEFINE_FUNC_PTR(PCO_SetDelayExposureTime, int, HANDLE, DWORD, DWORD, WORD, WORD);
DEFINE_FUNC_PTR(PCO_GetDelayExposureTimeTable, int, HANDLE, DWORD*, DWORD*, WORD*, WORD*, WORD);
DEFINE_FUNC_PTR(PCO_SetDelayExposureTimeTable, int, HANDLE, DWORD*, DWORD*, WORD, WORD, WORD);
DEFINE_FUNC_PTR(PCO_GetTriggerMode, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetTriggerMode, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_ForceTrigger, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_GetCameraBusyStatus, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_GetPowerDownMode, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetPowerDownMode, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetUserPowerDownTime, int, HANDLE, DWORD*);
DEFINE_FUNC_PTR(PCO_SetUserPowerDownTime, int, HANDLE, DWORD);
DEFINE_FUNC_PTR(PCO_GetExpTrigSignalStatus, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_GetCOCRuntime, int, HANDLE, DWORD*, DWORD*);
DEFINE_FUNC_PTR(PCO_GetFPSExposureMode, int, HANDLE, WORD*, DWORD*);
DEFINE_FUNC_PTR(PCO_SetFPSExposureMode, int, HANDLE, WORD, DWORD*);
DEFINE_FUNC_PTR(PCO_GetModulationMode, int, HANDLE, WORD*, DWORD*, WORD*, DWORD*, LONG*);
DEFINE_FUNC_PTR(PCO_SetModulationMode, int, HANDLE, WORD, DWORD, WORD, DWORD, LONG);
DEFINE_FUNC_PTR(PCO_GetFrameRate, int, HANDLE, WORD*, DWORD*, DWORD*);
DEFINE_FUNC_PTR(PCO_SetFrameRate, int, HANDLE, WORD*, WORD, DWORD*, DWORD*);
DEFINE_FUNC_PTR(PCO_GetStorageStruct, int, HANDLE, void*);
DEFINE_FUNC_PTR(PCO_SetStorageStruct, int, HANDLE, void*);
DEFINE_FUNC_PTR(PCO_GetCameraRamSize, int, HANDLE, DWORD*, WORD*);
DEFINE_FUNC_PTR(PCO_GetCameraRamSegmentSize, int, HANDLE, DWORD*);
DEFINE_FUNC_PTR(PCO_SetCameraRamSegmentSize, int, HANDLE, DWORD*);
DEFINE_FUNC_PTR(PCO_ClearRamSegment, int, HANDLE);
DEFINE_FUNC_PTR(PCO_GetActiveRamSegment, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetActiveRamSegment, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetCompressionMode, int, HANDLE, WORD*, DWORD*, WORD);
DEFINE_FUNC_PTR(PCO_SetCompressionMode, int, HANDLE, WORD, DWORD*, WORD);
DEFINE_FUNC_PTR(PCO_GetMaxNumberOfImagesInSegment, int, HANDLE, DWORD*);
DEFINE_FUNC_PTR(PCO_GetRecordingStruct, int, HANDLE, void*);
DEFINE_FUNC_PTR(PCO_SetRecordingStruct, int, HANDLE, void*);
DEFINE_FUNC_PTR(PCO_GetStorageMode, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetStorageMode, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetRecorderSubmode, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetRecorderSubmode, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetRecordingState, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetRecordingState, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_ArmCamera, int, HANDLE);
DEFINE_FUNC_PTR(PCO_GetAcquireMode, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetAcquireMode, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetAcquireModeEx, int, HANDLE, WORD*, DWORD*, DWORD*);
DEFINE_FUNC_PTR(PCO_SetAcquireModeEx, int, HANDLE, WORD, DWORD, DWORD*);
DEFINE_FUNC_PTR(PCO_GetAcquireControl, int, HANDLE, DWORD*, DWORD*, WORD);
DEFINE_FUNC_PTR(PCO_SetAcquireControl, int, HANDLE, DWORD, DWORD*, WORD);
DEFINE_FUNC_PTR(PCO_GetAcqEnblSignalStatus, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetDateTime, int, HANDLE, BYTE, BYTE, WORD, WORD, BYTE, BYTE);
DEFINE_FUNC_PTR(PCO_GetTimestampMode, int, HANDLE, WORD*);
DEFINE_FUNC_PTR(PCO_SetTimestampMode, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_GetRecordStopEvent, int, HANDLE, WORD*, DWORD*);
DEFINE_FUNC_PTR(PCO_SetRecordStopEvent, int, HANDLE, WORD, DWORD);
DEFINE_FUNC_PTR(PCO_StopRecord, int, HANDLE, WORD*, DWORD*);
DEFINE_FUNC_PTR(PCO_GetImage, int, HANDLE, WORD, DWORD, DWORD, SHORT);
DEFINE_FUNC_PTR(PCO_GetImageEx, int, HANDLE, WORD, DWORD, DWORD, SHORT, WORD, WORD, WORD);
DEFINE_FUNC_PTR(PCO_GetPendingBuffer, int, HANDLE, int*);
DEFINE_FUNC_PTR(PCO_CheckDeviceAvailability, int, HANDLE, WORD);
DEFINE_FUNC_PTR(PCO_SetTransferParameter, int, HANDLE, void*, int);
DEFINE_FUNC_PTR(PCO_GetTransferParameter, int, HANDLE, void*, int);
DEFINE_FUNC_PTR(PCO_SetTransferParametersAuto, int, HANDLE, void*, int);
DEFINE_FUNC_PTR(PCO_CamLinkSetImageParameters, int, HANDLE, WORD, WORD);
DEFINE_FUNC_PTR(PCO_SetImageParameters, int, HANDLE, WORD, WORD, DWORD, void*, int);
DEFINE_FUNC_PTR(PCO_SetTimeouts, int, HANDLE, void*, unsigned int);
DEFINE_FUNC_PTR(PCO_GetImageTransferMode, int, HANDLE, void*, int);
DEFINE_FUNC_PTR(PCO_SetImageTransferMode, int, HANDLE, void*, int);
DEFINE_FUNC_PTR(PCO_AddBufferExtern, int, HANDLE, HANDLE, WORD, DWORD, DWORD, DWORD, void*, DWORD, DWORD*);
DEFINE_FUNC_PTR(PCO_GetMetaData, int, HANDLE, SHORT, void*, DWORD, DWORD);
DEFINE_FUNC_PTR(PCO_GetMetaDataExtern, int, HANDLE, void*, void*, DWORD, DWORD);
DEFINE_FUNC_PTR(PCO_GetTimeStamp, int, HANDLE, SHORT, WORD*, void*);
DEFINE_FUNC_PTR(PCO_GetDeviceStatus, int, HANDLE, WORD, DWORD*, WORD);
DEFINE_FUNC_PTR(PCO_ControlCommandCall, int, HANDLE, void*, unsigned int, void*, unsigned int);
DEFINE_FUNC_PTR(PCO_ResetLib, int);
DEFINE_FUNC_PTR(PCO_InitializeLib, int);
DEFINE_FUNC_PTR(PCO_CleanupLib, int);
DEFINE_FUNC_PTR(PCO_EnableSoftROI, int, HANDLE, WORD, void*, int);
DEFINE_FUNC_PTR(PCO_GetAPIManagement, int, HANDLE, WORD*, void*);
DEFINE_FUNC_PTR(PCO_GetErrorTextSDK, void, DWORD, char*, DWORD);
DEFINE_FUNC_PTR(PCO_GetVersionInfoSC2_Cam, int, char*, int, char*, int, int*, int*, int*, int*);
DEFINE_FUNC_PTR(PCO_CloseCamera, int, HANDLE);
DEFINE_FUNC_PTR(PCO_CancelImages, int, HANDLE);
DEFINE_FUNC_PTR(PCO_SetMetaDataMode, int, HANDLE, WORD, WORD*, WORD*);
DEFINE_FUNC_PTR(PCO_OpenCamera, int, HANDLE*, WORD);

// Define the wrapper class
class PCOAPIWrapper {
public:
    bool allFunctionsLoaded;

    PCOAPIWrapper(const std::string& libraryPath) : allFunctionsLoaded(true) {
        handle = LOAD_LIBRARY(libraryPath.c_str());
        if (!handle) {
            allFunctionsLoaded = false;
            return;
        }

        functionMap = {
            {"PCO_GetGeneral", std::make_pair(std::any(PCO_GetGeneral_func()), (void**)&_PCO_GetGeneral)},
            {"PCO_GetCameraType", std::make_pair(std::any(PCO_GetCameraType_func()), (void**)&_PCO_GetCameraType)},
            {"PCO_GetCameraHealthStatus", std::make_pair(std::any(PCO_GetCameraHealthStatus_func()), (void**)&_PCO_GetCameraHealthStatus)},
            {"PCO_ResetSettingsToDefault", std::make_pair(std::any(PCO_ResetSettingsToDefault_func()), (void**)&_PCO_ResetSettingsToDefault)},
            {"PCO_GetTemperature", std::make_pair(std::any(PCO_GetTemperature_func()), (void**)&_PCO_GetTemperature)},
            {"PCO_GetInfoString", std::make_pair(std::any(PCO_GetInfoString_func()), (void**)&_PCO_GetInfoString)},
            {"PCO_GetCameraName", std::make_pair(std::any(PCO_GetCameraName_func()), (void**)&_PCO_GetCameraName)},
            {"PCO_GetFirmwareInfo", std::make_pair(std::any(PCO_GetFirmwareInfo_func()), (void**)&_PCO_GetFirmwareInfo)},
            {"PCO_GetCameraSetup", std::make_pair(std::any(PCO_GetCameraSetup_func()), (void**)&_PCO_GetCameraSetup)},
            {"PCO_SetCameraSetup", std::make_pair(std::any(PCO_SetCameraSetup_func()), (void**)&_PCO_SetCameraSetup)},
            {"PCO_RebootCamera", std::make_pair(std::any(PCO_RebootCamera_func()), (void**)&_PCO_RebootCamera)},
            {"PCO_GetShutterMode", std::make_pair(std::any(PCO_GetShutterMode_func()), (void**)&_PCO_GetShutterMode)},
            {"PCO_SetShutterMode", std::make_pair(std::any(PCO_SetShutterMode_func()), (void**)&_PCO_SetShutterMode)},
            {"PCO_GetPowerSaveMode", std::make_pair(std::any(PCO_GetPowerSaveMode_func()), (void**)&_PCO_GetPowerSaveMode)},
            {"PCO_SetPowerSaveMode", std::make_pair(std::any(PCO_SetPowerSaveMode_func()), (void**)&_PCO_SetPowerSaveMode)},
            {"PCO_GetBatteryStatus", std::make_pair(std::any(PCO_GetBatteryStatus_func()), (void**)&_PCO_GetBatteryStatus)},
            {"PCO_GetFanControlParameters", std::make_pair(std::any(PCO_GetFanControlParameters_func()), (void**)&_PCO_GetFanControlParameters)},
            {"PCO_SetFanControlParameters", std::make_pair(std::any(PCO_SetFanControlParameters_func()), (void**)&_PCO_SetFanControlParameters)},
            {"PCO_GetSensorStruct", std::make_pair(std::any(PCO_GetSensorStruct_func()), (void**)&_PCO_GetSensorStruct)},
            {"PCO_SetSensorStruct", std::make_pair(std::any(PCO_SetSensorStruct_func()), (void**)&_PCO_SetSensorStruct)},
            {"PCO_GetCameraDescription", std::make_pair(std::any(PCO_GetCameraDescription_func()), (void**)&_PCO_GetCameraDescription)},
            {"PCO_GetCameraDescriptionEx", std::make_pair(std::any(PCO_GetCameraDescriptionEx_func()), (void**)&_PCO_GetCameraDescriptionEx)},
            {"PCO_GetSensorFormat", std::make_pair(std::any(PCO_GetSensorFormat_func()), (void**)&_PCO_GetSensorFormat)},
            {"PCO_SetSensorFormat", std::make_pair(std::any(PCO_SetSensorFormat_func()), (void**)&_PCO_SetSensorFormat)},
            {"PCO_GetSizes", std::make_pair(std::any(PCO_GetSizes_func()), (void**)&_PCO_GetSizes)},
            {"PCO_GetROI", std::make_pair(std::any(PCO_GetROI_func()), (void**)&_PCO_GetROI)},
            {"PCO_SetROI", std::make_pair(std::any(PCO_SetROI_func()), (void**)&_PCO_SetROI)},
            {"PCO_GetBinning", std::make_pair(std::any(PCO_GetBinning_func()), (void**)&_PCO_GetBinning)},
            {"PCO_SetBinning", std::make_pair(std::any(PCO_SetBinning_func()), (void**)&_PCO_SetBinning)},
            {"PCO_GetPixelRate", std::make_pair(std::any(PCO_GetPixelRate_func()), (void**)&_PCO_GetPixelRate)},
            {"PCO_SetPixelRate", std::make_pair(std::any(PCO_SetPixelRate_func()), (void**)&_PCO_SetPixelRate)},
            {"PCO_GetConversionFactor", std::make_pair(std::any(PCO_GetConversionFactor_func()), (void**)&_PCO_GetConversionFactor)},
            {"PCO_SetConversionFactor", std::make_pair(std::any(PCO_SetConversionFactor_func()), (void**)&_PCO_SetConversionFactor)},
            {"PCO_GetDoubleImageMode", std::make_pair(std::any(PCO_GetDoubleImageMode_func()), (void**)&_PCO_GetDoubleImageMode)},
            {"PCO_SetDoubleImageMode", std::make_pair(std::any(PCO_SetDoubleImageMode_func()), (void**)&_PCO_SetDoubleImageMode)},
            {"PCO_GetADCOperation", std::make_pair(std::any(PCO_GetADCOperation_func()), (void**)&_PCO_GetADCOperation)},
            {"PCO_SetADCOperation", std::make_pair(std::any(PCO_SetADCOperation_func()), (void**)&_PCO_SetADCOperation)},
            {"PCO_GetIRSensitivity", std::make_pair(std::any(PCO_GetIRSensitivity_func()), (void**)&_PCO_GetIRSensitivity)},
            {"PCO_SetIRSensitivity", std::make_pair(std::any(PCO_SetIRSensitivity_func()), (void**)&_PCO_SetIRSensitivity)},
            {"PCO_GetCoolingSetpoints", std::make_pair(std::any(PCO_GetCoolingSetpoints_func()), (void**)&_PCO_GetCoolingSetpoints)},
            {"PCO_GetCoolingSetpointTemperature", std::make_pair(std::any(PCO_GetCoolingSetpointTemperature_func()), (void**)&_PCO_GetCoolingSetpointTemperature)},
            {"PCO_SetCoolingSetpointTemperature", std::make_pair(std::any(PCO_SetCoolingSetpointTemperature_func()), (void**)&_PCO_SetCoolingSetpointTemperature)},
            {"PCO_GetOffsetMode", std::make_pair(std::any(PCO_GetOffsetMode_func()), (void**)&_PCO_GetOffsetMode)},
            {"PCO_SetOffsetMode", std::make_pair(std::any(PCO_SetOffsetMode_func()), (void**)&_PCO_SetOffsetMode)},
            {"PCO_GetNoiseFilterMode", std::make_pair(std::any(PCO_GetNoiseFilterMode_func()), (void**)&_PCO_GetNoiseFilterMode)},
            {"PCO_SetNoiseFilterMode", std::make_pair(std::any(PCO_SetNoiseFilterMode_func()), (void**)&_PCO_SetNoiseFilterMode)},
            {"PCO_GetHWIOSignalCount", std::make_pair(std::any(PCO_GetHWIOSignalCount_func()), (void**)&_PCO_GetHWIOSignalCount)},
            {"PCO_GetHWIOSignalDescriptor", std::make_pair(std::any(PCO_GetHWIOSignalDescriptor_func()), (void**)&_PCO_GetHWIOSignalDescriptor)},
            {"PCO_GetColorCorrectionMatrix", std::make_pair(std::any(PCO_GetColorCorrectionMatrix_func()), (void**)&_PCO_GetColorCorrectionMatrix)},
            {"PCO_GetDSNUAdjustMode", std::make_pair(std::any(PCO_GetDSNUAdjustMode_func()), (void**)&_PCO_GetDSNUAdjustMode)},
            {"PCO_SetDSNUAdjustMode", std::make_pair(std::any(PCO_SetDSNUAdjustMode_func()), (void**)&_PCO_SetDSNUAdjustMode)},
            {"PCO_InitDSNUAdjustment", std::make_pair(std::any(PCO_InitDSNUAdjustment_func()), (void**)&_PCO_InitDSNUAdjustment)},
            {"PCO_GetCDIMode", std::make_pair(std::any(PCO_GetCDIMode_func()), (void**)&_PCO_GetCDIMode)},
            {"PCO_SetCDIMode", std::make_pair(std::any(PCO_SetCDIMode_func()), (void**)&_PCO_SetCDIMode)},
            {"PCO_GetLookupTableInfo", std::make_pair(std::any(PCO_GetLookupTableInfo_func()), (void**)&_PCO_GetLookupTableInfo)},
            {"PCO_GetActiveLookupTable", std::make_pair(std::any(PCO_GetActiveLookupTable_func()), (void**)&_PCO_GetActiveLookupTable)},
            {"PCO_SetActiveLookupTable", std::make_pair(std::any(PCO_SetActiveLookupTable_func()), (void**)&_PCO_SetActiveLookupTable)},
            {"PCO_GetIntensifiedGatingMode", std::make_pair(std::any(PCO_GetIntensifiedGatingMode_func()), (void**)&_PCO_GetIntensifiedGatingMode)},
            {"PCO_SetIntensifiedGatingMode", std::make_pair(std::any(PCO_SetIntensifiedGatingMode_func()), (void**)&_PCO_SetIntensifiedGatingMode)},
            {"PCO_GetIntensifiedMCP", std::make_pair(std::any(PCO_GetIntensifiedMCP_func()), (void**)&_PCO_GetIntensifiedMCP)},
            {"PCO_SetIntensifiedMCP", std::make_pair(std::any(PCO_SetIntensifiedMCP_func()), (void**)&_PCO_SetIntensifiedMCP)},
            {"PCO_GetSensorDarkOffset", std::make_pair(std::any(PCO_GetSensorDarkOffset_func()), (void**)&_PCO_GetSensorDarkOffset)},
            {"PCO_GetTimingStruct", std::make_pair(std::any(PCO_GetTimingStruct_func()), (void**)&_PCO_GetTimingStruct)},
            {"PCO_SetTimingStruct", std::make_pair(std::any(PCO_SetTimingStruct_func()), (void**)&_PCO_SetTimingStruct)},
            {"PCO_GetDelayExposureTime", std::make_pair(std::any(PCO_GetDelayExposureTime_func()), (void**)&_PCO_GetDelayExposureTime)},
            {"PCO_SetDelayExposureTime", std::make_pair(std::any(PCO_SetDelayExposureTime_func()), (void**)&_PCO_SetDelayExposureTime)},
            {"PCO_GetDelayExposureTimeTable", std::make_pair(std::any(PCO_GetDelayExposureTimeTable_func()), (void**)&_PCO_GetDelayExposureTimeTable)},
            {"PCO_SetDelayExposureTimeTable", std::make_pair(std::any(PCO_SetDelayExposureTimeTable_func()), (void**)&_PCO_SetDelayExposureTimeTable)},
            {"PCO_GetTriggerMode", std::make_pair(std::any(PCO_GetTriggerMode_func()), (void**)&_PCO_GetTriggerMode)},
            {"PCO_SetTriggerMode", std::make_pair(std::any(PCO_SetTriggerMode_func()), (void**)&_PCO_SetTriggerMode)},
            {"PCO_ForceTrigger", std::make_pair(std::any(PCO_ForceTrigger_func()), (void**)&_PCO_ForceTrigger)},
            {"PCO_GetCameraBusyStatus", std::make_pair(std::any(PCO_GetCameraBusyStatus_func()), (void**)&_PCO_GetCameraBusyStatus)},
            {"PCO_GetPowerDownMode", std::make_pair(std::any(PCO_GetPowerDownMode_func()), (void**)&_PCO_GetPowerDownMode)},
            {"PCO_SetPowerDownMode", std::make_pair(std::any(PCO_SetPowerDownMode_func()), (void**)&_PCO_SetPowerDownMode)},
            {"PCO_GetUserPowerDownTime", std::make_pair(std::any(PCO_GetUserPowerDownTime_func()), (void**)&_PCO_GetUserPowerDownTime)},
            {"PCO_SetUserPowerDownTime", std::make_pair(std::any(PCO_SetUserPowerDownTime_func()), (void**)&_PCO_SetUserPowerDownTime)},
            {"PCO_GetExpTrigSignalStatus", std::make_pair(std::any(PCO_GetExpTrigSignalStatus_func()), (void**)&_PCO_GetExpTrigSignalStatus)},
            {"PCO_GetCOCRuntime", std::make_pair(std::any(PCO_GetCOCRuntime_func()), (void**)&_PCO_GetCOCRuntime)},
            {"PCO_GetFPSExposureMode", std::make_pair(std::any(PCO_GetFPSExposureMode_func()), (void**)&_PCO_GetFPSExposureMode)},
            {"PCO_SetFPSExposureMode", std::make_pair(std::any(PCO_SetFPSExposureMode_func()), (void**)&_PCO_SetFPSExposureMode)},
            {"PCO_GetModulationMode", std::make_pair(std::any(PCO_GetModulationMode_func()), (void**)&_PCO_GetModulationMode)},
            {"PCO_SetModulationMode", std::make_pair(std::any(PCO_SetModulationMode_func()), (void**)&_PCO_SetModulationMode)},
            {"PCO_GetFrameRate", std::make_pair(std::any(PCO_GetFrameRate_func()), (void**)&_PCO_GetFrameRate)},
            {"PCO_SetFrameRate", std::make_pair(std::any(PCO_SetFrameRate_func()), (void**)&_PCO_SetFrameRate)},
            {"PCO_GetStorageStruct", std::make_pair(std::any(PCO_GetStorageStruct_func()), (void**)&_PCO_GetStorageStruct)},
            {"PCO_SetStorageStruct", std::make_pair(std::any(PCO_SetStorageStruct_func()), (void**)&_PCO_SetStorageStruct)},
            {"PCO_GetCameraRamSize", std::make_pair(std::any(PCO_GetCameraRamSize_func()), (void**)&_PCO_GetCameraRamSize)},
            {"PCO_GetCameraRamSegmentSize", std::make_pair(std::any(PCO_GetCameraRamSegmentSize_func()), (void**)&_PCO_GetCameraRamSegmentSize)},
            {"PCO_SetCameraRamSegmentSize", std::make_pair(std::any(PCO_SetCameraRamSegmentSize_func()), (void**)&_PCO_SetCameraRamSegmentSize)},
            {"PCO_ClearRamSegment", std::make_pair(std::any(PCO_ClearRamSegment_func()), (void**)&_PCO_ClearRamSegment)},
            {"PCO_GetActiveRamSegment", std::make_pair(std::any(PCO_GetActiveRamSegment_func()), (void**)&_PCO_GetActiveRamSegment)},
            {"PCO_SetActiveRamSegment", std::make_pair(std::any(PCO_SetActiveRamSegment_func()), (void**)&_PCO_SetActiveRamSegment)},
            {"PCO_GetCompressionMode", std::make_pair(std::any(PCO_GetCompressionMode_func()), (void**)&_PCO_GetCompressionMode)},
            {"PCO_SetCompressionMode", std::make_pair(std::any(PCO_SetCompressionMode_func()), (void**)&_PCO_SetCompressionMode)},
            {"PCO_GetMaxNumberOfImagesInSegment", std::make_pair(std::any(PCO_GetMaxNumberOfImagesInSegment_func()), (void**)&_PCO_GetMaxNumberOfImagesInSegment)},
            {"PCO_GetRecordingStruct", std::make_pair(std::any(PCO_GetRecordingStruct_func()), (void**)&_PCO_GetRecordingStruct)},
            {"PCO_SetRecordingStruct", std::make_pair(std::any(PCO_SetRecordingStruct_func()), (void**)&_PCO_SetRecordingStruct)},
            {"PCO_GetStorageMode", std::make_pair(std::any(PCO_GetStorageMode_func()), (void**)&_PCO_GetStorageMode)},
            {"PCO_SetStorageMode", std::make_pair(std::any(PCO_SetStorageMode_func()), (void**)&_PCO_SetStorageMode)},
            {"PCO_GetRecorderSubmode", std::make_pair(std::any(PCO_GetRecorderSubmode_func()), (void**)&_PCO_GetRecorderSubmode)},
            {"PCO_SetRecorderSubmode", std::make_pair(std::any(PCO_SetRecorderSubmode_func()), (void**)&_PCO_SetRecorderSubmode)},
            {"PCO_GetRecordingState", std::make_pair(std::any(PCO_GetRecordingState_func()), (void**)&_PCO_GetRecordingState)},
            {"PCO_SetRecordingState", std::make_pair(std::any(PCO_SetRecordingState_func()), (void**)&_PCO_SetRecordingState)},
            {"PCO_ArmCamera", std::make_pair(std::any(PCO_ArmCamera_func()), (void**)&_PCO_ArmCamera)},
            {"PCO_GetAcquireMode", std::make_pair(std::any(PCO_GetAcquireMode_func()), (void**)&_PCO_GetAcquireMode)},
            {"PCO_SetAcquireMode", std::make_pair(std::any(PCO_SetAcquireMode_func()), (void**)&_PCO_SetAcquireMode)},
            {"PCO_GetAcquireModeEx", std::make_pair(std::any(PCO_GetAcquireModeEx_func()), (void**)&_PCO_GetAcquireModeEx)},
            {"PCO_SetAcquireModeEx", std::make_pair(std::any(PCO_SetAcquireModeEx_func()), (void**)&_PCO_SetAcquireModeEx)},
            {"PCO_GetAcquireControl", std::make_pair(std::any(PCO_GetAcquireControl_func()), (void**)&_PCO_GetAcquireControl)},
            {"PCO_SetAcquireControl", std::make_pair(std::any(PCO_SetAcquireControl_func()), (void**)&_PCO_SetAcquireControl)},
            {"PCO_GetAcqEnblSignalStatus", std::make_pair(std::any(PCO_GetAcqEnblSignalStatus_func()), (void**)&_PCO_GetAcqEnblSignalStatus)},
            {"PCO_SetDateTime", std::make_pair(std::any(PCO_SetDateTime_func()), (void**)&_PCO_SetDateTime)},
            {"PCO_GetTimestampMode", std::make_pair(std::any(PCO_GetTimestampMode_func()), (void**)&_PCO_GetTimestampMode)},
            {"PCO_SetTimestampMode", std::make_pair(std::any(PCO_SetTimestampMode_func()), (void**)&_PCO_SetTimestampMode)},
            {"PCO_GetRecordStopEvent", std::make_pair(std::any(PCO_GetRecordStopEvent_func()), (void**)&_PCO_GetRecordStopEvent)},
            {"PCO_SetRecordStopEvent", std::make_pair(std::any(PCO_SetRecordStopEvent_func()), (void**)&_PCO_SetRecordStopEvent)},
            {"PCO_StopRecord", std::make_pair(std::any(PCO_StopRecord_func()), (void**)&_PCO_StopRecord)},
            {"PCO_GetImage", std::make_pair(std::any(PCO_GetImage_func()), (void**)&_PCO_GetImage)},
            {"PCO_GetImageEx", std::make_pair(std::any(PCO_GetImageEx_func()), (void**)&_PCO_GetImageEx)},
            {"PCO_GetPendingBuffer", std::make_pair(std::any(PCO_GetPendingBuffer_func()), (void**)&_PCO_GetPendingBuffer)},
            {"PCO_CheckDeviceAvailability", std::make_pair(std::any(PCO_CheckDeviceAvailability_func()), (void**)&_PCO_CheckDeviceAvailability)},
            {"PCO_SetTransferParameter", std::make_pair(std::any(PCO_SetTransferParameter_func()), (void**)&_PCO_SetTransferParameter)},
            {"PCO_GetTransferParameter", std::make_pair(std::any(PCO_GetTransferParameter_func()), (void**)&_PCO_GetTransferParameter)},
            {"PCO_SetTransferParametersAuto", std::make_pair(std::any(PCO_SetTransferParametersAuto_func()), (void**)&_PCO_SetTransferParametersAuto)},
            {"PCO_CamLinkSetImageParameters", std::make_pair(std::any(PCO_CamLinkSetImageParameters_func()), (void**)&_PCO_CamLinkSetImageParameters)},
            {"PCO_SetImageParameters", std::make_pair(std::any(PCO_SetImageParameters_func()), (void**)&_PCO_SetImageParameters)},
            {"PCO_SetTimeouts", std::make_pair(std::any(PCO_SetTimeouts_func()), (void**)&_PCO_SetTimeouts)},
            {"PCO_GetImageTransferMode", std::make_pair(std::any(PCO_GetImageTransferMode_func()), (void**)&_PCO_GetImageTransferMode)},
            {"PCO_SetImageTransferMode", std::make_pair(std::any(PCO_SetImageTransferMode_func()), (void**)&_PCO_SetImageTransferMode)},
            {"PCO_AddBufferExtern", std::make_pair(std::any(PCO_AddBufferExtern_func()), (void**)&_PCO_AddBufferExtern)},
            {"PCO_GetMetaData", std::make_pair(std::any(PCO_GetMetaData_func()), (void**)&_PCO_GetMetaData)},
            {"PCO_GetMetaDataExtern", std::make_pair(std::any(PCO_GetMetaDataExtern_func()), (void**)&_PCO_GetMetaDataExtern)},
            {"PCO_GetTimeStamp", std::make_pair(std::any(PCO_GetTimeStamp_func()), (void**)&_PCO_GetTimeStamp)},
            {"PCO_GetDeviceStatus", std::make_pair(std::any(PCO_GetDeviceStatus_func()), (void**)&_PCO_GetDeviceStatus)},
            {"PCO_ControlCommandCall", std::make_pair(std::any(PCO_ControlCommandCall_func()), (void**)&_PCO_ControlCommandCall)},
            {"PCO_ResetLib", std::make_pair(std::any(PCO_ResetLib_func()), (void**)&_PCO_ResetLib)},
            {"PCO_InitializeLib", std::make_pair(std::any(PCO_InitializeLib_func()), (void**)&_PCO_InitializeLib)},
            {"PCO_CleanupLib", std::make_pair(std::any(PCO_CleanupLib_func()), (void**)&_PCO_CleanupLib)},
            {"PCO_EnableSoftROI", std::make_pair(std::any(PCO_EnableSoftROI_func()), (void**)&_PCO_EnableSoftROI)},
            {"PCO_GetAPIManagement", std::make_pair(std::any(PCO_GetAPIManagement_func()), (void**)&_PCO_GetAPIManagement)},
            {"PCO_GetErrorTextSDK", std::make_pair(std::any(PCO_GetErrorTextSDK_func()), (void**)&_PCO_GetErrorTextSDK)},
            {"PCO_GetVersionInfoSC2_Cam", std::make_pair(std::any(PCO_GetVersionInfoSC2_Cam_func()), (void**)&_PCO_GetVersionInfoSC2_Cam)},
            {"PCO_CloseCamera", std::make_pair(std::any(PCO_CloseCamera_func()), (void**)&_PCO_CloseCamera)},
            {"PCO_CancelImages", std::make_pair(std::any(PCO_CancelImages_func()), (void**)&_PCO_CancelImages)},
            {"PCO_SetMetaDataMode", std::make_pair(std::any(PCO_SetMetaDataMode_func()), (void**)&_PCO_SetMetaDataMode)},
            {"PCO_OpenCamera", std::make_pair(std::any(PCO_OpenCamera_func()), (void**)&_PCO_OpenCamera)}
        };

        for (auto& func : functionMap) {
            void* funcPtr = GET_PROC_ADDRESS(handle, func.first.c_str());
            if (!funcPtr) {
                allFunctionsLoaded = false;
                continue;
            }
            *func.second.second = funcPtr;
        }
    }

    ~PCOAPIWrapper() {
        if (handle) {
            FREE_LIBRARY(handle);
        }
    }

    // Wrapper methods for each function
    int PCO_GetGeneral(HANDLE ph, void* strGeneral) {
        if (!_PCO_GetGeneral) {
            throw std::logic_error("Function PCO_GetGeneral is not loaded.");
        }
        return _PCO_GetGeneral(ph, strGeneral);
    }

    int PCO_GetCameraType(HANDLE ph, void* strCamType) {
        if (!_PCO_GetCameraType) {
            throw std::logic_error("Function PCO_GetCameraType is not loaded.");
        }
        return _PCO_GetCameraType(ph, strCamType);
    }

    int PCO_GetCameraHealthStatus(HANDLE ph, DWORD* dwWarn, DWORD* dwErr, DWORD* dwStatus) {
        if (!_PCO_GetCameraHealthStatus) {
            throw std::logic_error("Function PCO_GetCameraHealthStatus is not loaded.");
        }
        return _PCO_GetCameraHealthStatus(ph, dwWarn, dwErr, dwStatus);
    }

    int PCO_ResetSettingsToDefault(HANDLE ph) {
        if (!_PCO_ResetSettingsToDefault) {
            throw std::logic_error("Function PCO_ResetSettingsToDefault is not loaded.");
        }
        return _PCO_ResetSettingsToDefault(ph);
    }

    int PCO_GetTemperature(HANDLE ph, SHORT* sCCDTemp, SHORT* sCamTemp, SHORT* sPowTemp) {
        if (!_PCO_GetTemperature) {
            throw std::logic_error("Function PCO_GetTemperature is not loaded.");
        }
        return _PCO_GetTemperature(ph, sCCDTemp, sCamTemp, sPowTemp);
    }

    int PCO_GetInfoString(HANDLE ph, DWORD dwinfotype, char* buf_in, WORD size_in) {
        if (!_PCO_GetInfoString) {
            throw std::logic_error("Function PCO_GetInfoString is not loaded.");
        }
        return _PCO_GetInfoString(ph, dwinfotype, buf_in, size_in);
    }

    int PCO_GetCameraName(HANDLE ph, char* szCameraName, WORD wSZCameraNameLen) {
        if (!_PCO_GetCameraName) {
            throw std::logic_error("Function PCO_GetCameraName is not loaded.");
        }
        return _PCO_GetCameraName(ph, szCameraName, wSZCameraNameLen);
    }

    int PCO_GetFirmwareInfo(HANDLE ph, WORD wDeviceBlock, void* pstrFirmWareVersion) {
        if (!_PCO_GetFirmwareInfo) {
            throw std::logic_error("Function PCO_GetFirmwareInfo is not loaded.");
        }
        return _PCO_GetFirmwareInfo(ph, wDeviceBlock, pstrFirmWareVersion);
    }

    int PCO_GetCameraSetup(HANDLE ph, WORD* wType, DWORD* dwSetup, WORD* wLen) {
        if (!_PCO_GetCameraSetup) {
            throw std::logic_error("Function PCO_GetCameraSetup is not loaded.");
        }
        return _PCO_GetCameraSetup(ph, wType, dwSetup, wLen);
    }

    int PCO_SetCameraSetup(HANDLE ph, WORD wType, DWORD* dwSetup, WORD wLen) {
        if (!_PCO_SetCameraSetup) {
            throw std::logic_error("Function PCO_SetCameraSetup is not loaded.");
        }
        return _PCO_SetCameraSetup(ph, wType, dwSetup, wLen);
    }

    int PCO_RebootCamera(HANDLE ph) {
        if (!_PCO_RebootCamera) {
            throw std::logic_error("Function PCO_RebootCamera is not loaded.");
        }
        return _PCO_RebootCamera(ph);
    }

    int PCO_GetShutterMode(HANDLE ph, WORD* wShuttermode) {
        if (!_PCO_GetShutterMode) {
            throw std::logic_error("Function PCO_GetShutterMode is not loaded.");
        }
        return _PCO_GetShutterMode(ph, wShuttermode);
    }

    int PCO_SetShutterMode(HANDLE ph, WORD wShuttermode) {
        if (!_PCO_SetShutterMode) {
            throw std::logic_error("Function PCO_SetShutterMode is not loaded.");
        }
        return _PCO_SetShutterMode(ph, wShuttermode);
    }

    int PCO_GetPowerSaveMode(HANDLE ph, WORD* wMode, WORD* wDelayMinutes) {
        if (!_PCO_GetPowerSaveMode) {
            throw std::logic_error("Function PCO_GetPowerSaveMode is not loaded.");
        }
        return _PCO_GetPowerSaveMode(ph, wMode, wDelayMinutes);
    }

    int PCO_SetPowerSaveMode(HANDLE ph, WORD wMode, WORD wDelayMinutes) {
        if (!_PCO_SetPowerSaveMode) {
            throw std::logic_error("Function PCO_SetPowerSaveMode is not loaded.");
        }
        return _PCO_SetPowerSaveMode(ph, wMode, wDelayMinutes);
    }

    int PCO_GetBatteryStatus(HANDLE ph, WORD* wBatteryType, WORD* wBatteryLevel, WORD* wPowerStatus, WORD wNumReserved) {
        if (!_PCO_GetBatteryStatus) {
            throw std::logic_error("Function PCO_GetBatteryStatus is not loaded.");
        }
        return _PCO_GetBatteryStatus(ph, wBatteryType, wBatteryLevel, wPowerStatus, wNumReserved);
    }

    int PCO_GetFanControlParameters(HANDLE hCam, WORD* wMode, WORD* wValue) {
        if (!_PCO_GetFanControlParameters) {
            throw std::logic_error("Function PCO_GetFanControlParameters is not loaded.");
        }
        return _PCO_GetFanControlParameters(hCam, wMode, wValue);
    }

    int PCO_SetFanControlParameters(HANDLE hCam, WORD wMode, WORD wValue) {
        if (!_PCO_SetFanControlParameters) {
            throw std::logic_error("Function PCO_SetFanControlParameters is not loaded.");
        }
        return _PCO_SetFanControlParameters(hCam, wMode, wValue);
    }

    int PCO_GetSensorStruct(HANDLE ph, void* strSensor) {
        if (!_PCO_GetSensorStruct) {
            throw std::logic_error("Function PCO_GetSensorStruct is not loaded.");
        }
        return _PCO_GetSensorStruct(ph, strSensor);
    }

    int PCO_SetSensorStruct(HANDLE ph, void* strSensor) {
        if (!_PCO_SetSensorStruct) {
            throw std::logic_error("Function PCO_SetSensorStruct is not loaded.");
        }
        return _PCO_SetSensorStruct(ph, strSensor);
    }

    int PCO_GetCameraDescription(HANDLE ph, void* strDescription) {
        if (!_PCO_GetCameraDescription) {
            throw std::logic_error("Function PCO_GetCameraDescription is not loaded.");
        }
        return _PCO_GetCameraDescription(ph, strDescription);
    }

    int PCO_GetCameraDescriptionEx(HANDLE ph, void* strDescription, WORD wType) {
        if (!_PCO_GetCameraDescriptionEx) {
            throw std::logic_error("Function PCO_GetCameraDescriptionEx is not loaded.");
        }
        return _PCO_GetCameraDescriptionEx(ph, strDescription, wType);
    }

    int PCO_GetSensorFormat(HANDLE ph, WORD* wSensor) {
        if (!_PCO_GetSensorFormat) {
            throw std::logic_error("Function PCO_GetSensorFormat is not loaded.");
        }
        return _PCO_GetSensorFormat(ph, wSensor);
    }

    int PCO_SetSensorFormat(HANDLE ph, WORD wSensor) {
        if (!_PCO_SetSensorFormat) {
            throw std::logic_error("Function PCO_SetSensorFormat is not loaded.");
        }
        return _PCO_SetSensorFormat(ph, wSensor);
    }

    int PCO_GetSizes(HANDLE ph, WORD* wXResAct, WORD* wYResAct, WORD* wXResMax, WORD* wYResMax) {
        if (!_PCO_GetSizes) {
            throw std::logic_error("Function PCO_GetSizes is not loaded.");
        }
        return _PCO_GetSizes(ph, wXResAct, wYResAct, wXResMax, wYResMax);
    }

    int PCO_GetROI(HANDLE ph, WORD* wRoiX0, WORD* wRoiY0, WORD* wRoiX1, WORD* wRoiY1) {
        if (!_PCO_GetROI) {
            throw std::logic_error("Function PCO_GetROI is not loaded.");
        }
        return _PCO_GetROI(ph, wRoiX0, wRoiY0, wRoiX1, wRoiY1);
    }

    int PCO_SetROI(HANDLE ph, WORD wRoiX0, WORD wRoiY0, WORD wRoiX1, WORD wRoiY1) {
        if (!_PCO_SetROI) {
            throw std::logic_error("Function PCO_SetROI is not loaded.");
        }
        return _PCO_SetROI(ph, wRoiX0, wRoiY0, wRoiX1, wRoiY1);
    }

    int PCO_GetBinning(HANDLE ph, WORD* wBinHorz, WORD* wBinVert) {
        if (!_PCO_GetBinning) {
            throw std::logic_error("Function PCO_GetBinning is not loaded.");
        }
        return _PCO_GetBinning(ph, wBinHorz, wBinVert);
    }

    int PCO_SetBinning(HANDLE ph, WORD wBinHorz, WORD wBinVert) {
        if (!_PCO_SetBinning) {
            throw std::logic_error("Function PCO_SetBinning is not loaded.");
        }
        return _PCO_SetBinning(ph, wBinHorz, wBinVert);
    }

    int PCO_GetPixelRate(HANDLE ph, DWORD* dwPixelRate) {
        if (!_PCO_GetPixelRate) {
            throw std::logic_error("Function PCO_GetPixelRate is not loaded.");
        }
        return _PCO_GetPixelRate(ph, dwPixelRate);
    }

    int PCO_SetPixelRate(HANDLE ph, DWORD dwPixelRate) {
        if (!_PCO_SetPixelRate) {
            throw std::logic_error("Function PCO_SetPixelRate is not loaded.");
        }
        return _PCO_SetPixelRate(ph, dwPixelRate);
    }

    int PCO_GetConversionFactor(HANDLE ph, WORD* wConvFact) {
        if (!_PCO_GetConversionFactor) {
            throw std::logic_error("Function PCO_GetConversionFactor is not loaded.");
        }
        return _PCO_GetConversionFactor(ph, wConvFact);
    }

    int PCO_SetConversionFactor(HANDLE ph, WORD wConvFact) {
        if (!_PCO_SetConversionFactor) {
            throw std::logic_error("Function PCO_SetConversionFactor is not loaded.");
        }
        return _PCO_SetConversionFactor(ph, wConvFact);
    }

    int PCO_GetDoubleImageMode(HANDLE ph, WORD* wDoubleImage) {
        if (!_PCO_GetDoubleImageMode) {
            throw std::logic_error("Function PCO_GetDoubleImageMode is not loaded.");
        }
        return _PCO_GetDoubleImageMode(ph, wDoubleImage);
    }

    int PCO_SetDoubleImageMode(HANDLE ph, WORD wDoubleImage) {
        if (!_PCO_SetDoubleImageMode) {
            throw std::logic_error("Function PCO_SetDoubleImageMode is not loaded.");
        }
        return _PCO_SetDoubleImageMode(ph, wDoubleImage);
    }

    int PCO_GetADCOperation(HANDLE ph, WORD* wADCOperation) {
        if (!_PCO_GetADCOperation) {
            throw std::logic_error("Function PCO_GetADCOperation is not loaded.");
        }
        return _PCO_GetADCOperation(ph, wADCOperation);
    }

    int PCO_SetADCOperation(HANDLE ph, WORD wADCOperation) {
        if (!_PCO_SetADCOperation) {
            throw std::logic_error("Function PCO_SetADCOperation is not loaded.");
        }
        return _PCO_SetADCOperation(ph, wADCOperation);
    }

    int PCO_GetIRSensitivity(HANDLE ph, WORD* wIR) {
        if (!_PCO_GetIRSensitivity) {
            throw std::logic_error("Function PCO_GetIRSensitivity is not loaded.");
        }
        return _PCO_GetIRSensitivity(ph, wIR);
    }

    int PCO_SetIRSensitivity(HANDLE ph, WORD wIR) {
        if (!_PCO_SetIRSensitivity) {
            throw std::logic_error("Function PCO_SetIRSensitivity is not loaded.");
        }
        return _PCO_SetIRSensitivity(ph, wIR);
    }

    int PCO_GetCoolingSetpoints(HANDLE ph, WORD wBlockID, WORD* wNumSetPoints, SHORT* sCoolSetpoints) {
        if (!_PCO_GetCoolingSetpoints) {
            throw std::logic_error("Function PCO_GetCoolingSetpoints is not loaded.");
        }
        return _PCO_GetCoolingSetpoints(ph, wBlockID, wNumSetPoints, sCoolSetpoints);
    }

    int PCO_GetCoolingSetpointTemperature(HANDLE ph, SHORT* sCoolSet) {
        if (!_PCO_GetCoolingSetpointTemperature) {
            throw std::logic_error("Function PCO_GetCoolingSetpointTemperature is not loaded.");
        }
        return _PCO_GetCoolingSetpointTemperature(ph, sCoolSet);
    }

    int PCO_SetCoolingSetpointTemperature(HANDLE ph, SHORT sCoolSet) {
        if (!_PCO_SetCoolingSetpointTemperature) {
            throw std::logic_error("Function PCO_SetCoolingSetpointTemperature is not loaded.");
        }
        return _PCO_SetCoolingSetpointTemperature(ph, sCoolSet);
    }

    int PCO_GetOffsetMode(HANDLE ph, WORD* wOffsetRegulation) {
        if (!_PCO_GetOffsetMode) {
            throw std::logic_error("Function PCO_GetOffsetMode is not loaded.");
        }
        return _PCO_GetOffsetMode(ph, wOffsetRegulation);
    }

    int PCO_SetOffsetMode(HANDLE ph, WORD wOffsetRegulation) {
        if (!_PCO_SetOffsetMode) {
            throw std::logic_error("Function PCO_SetOffsetMode is not loaded.");
        }
        return _PCO_SetOffsetMode(ph, wOffsetRegulation);
    }

    int PCO_GetNoiseFilterMode(HANDLE ph, WORD* wNoiseFilterMode) {
        if (!_PCO_GetNoiseFilterMode) {
            throw std::logic_error("Function PCO_GetNoiseFilterMode is not loaded.");
        }
        return _PCO_GetNoiseFilterMode(ph, wNoiseFilterMode);
    }

    int PCO_SetNoiseFilterMode(HANDLE ph, WORD wNoiseFilterMode) {
        if (!_PCO_SetNoiseFilterMode) {
            throw std::logic_error("Function PCO_SetNoiseFilterMode is not loaded.");
        }
        return _PCO_SetNoiseFilterMode(ph, wNoiseFilterMode);
    }

    int PCO_GetHWIOSignalCount(HANDLE ph, WORD* wNumSignals) {
        if (!_PCO_GetHWIOSignalCount) {
            throw std::logic_error("Function PCO_GetHWIOSignalCount is not loaded.");
        }
        return _PCO_GetHWIOSignalCount(ph, wNumSignals);
    }

    int PCO_GetHWIOSignalDescriptor(HANDLE ph, WORD wSignalNum, void* pstrSignal) {
        if (!_PCO_GetHWIOSignalDescriptor) {
            throw std::logic_error("Function PCO_GetHWIOSignalDescriptor is not loaded.");
        }
        return _PCO_GetHWIOSignalDescriptor(ph, wSignalNum, pstrSignal);
    }

    int PCO_GetColorCorrectionMatrix(HANDLE ph, double* pdMatrix) {
        if (!_PCO_GetColorCorrectionMatrix) {
            throw std::logic_error("Function PCO_GetColorCorrectionMatrix is not loaded.");
        }
        return _PCO_GetColorCorrectionMatrix(ph, pdMatrix);
    }

    int PCO_GetDSNUAdjustMode(HANDLE ph, WORD* wDSNUAdjustMode, WORD* wReserved) {
        if (!_PCO_GetDSNUAdjustMode) {
            throw std::logic_error("Function PCO_GetDSNUAdjustMode is not loaded.");
        }
        return _PCO_GetDSNUAdjustMode(ph, wDSNUAdjustMode, wReserved);
    }

    int PCO_SetDSNUAdjustMode(HANDLE ph, WORD wDSNUAdjustMode, WORD wReserved) {
        if (!_PCO_SetDSNUAdjustMode) {
            throw std::logic_error("Function PCO_SetDSNUAdjustMode is not loaded.");
        }
        return _PCO_SetDSNUAdjustMode(ph, wDSNUAdjustMode, wReserved);
    }

    int PCO_InitDSNUAdjustment(HANDLE ph, WORD wDSNUAdjustMode, WORD wReserved) {
        if (!_PCO_InitDSNUAdjustment) {
            throw std::logic_error("Function PCO_InitDSNUAdjustment is not loaded.");
        }
        return _PCO_InitDSNUAdjustment(ph, wDSNUAdjustMode, wReserved);
    }

    int PCO_GetCDIMode(HANDLE ph, WORD* wCDIMode) {
        if (!_PCO_GetCDIMode) {
            throw std::logic_error("Function PCO_GetCDIMode is not loaded.");
        }
        return _PCO_GetCDIMode(ph, wCDIMode);
    }

    int PCO_SetCDIMode(HANDLE ph, WORD wCDIMode) {
        if (!_PCO_SetCDIMode) {
            throw std::logic_error("Function PCO_SetCDIMode is not loaded.");
        }
        return _PCO_SetCDIMode(ph, wCDIMode);
    }

    int PCO_GetLookupTableInfo(HANDLE ph, WORD wLUTNum, WORD* wNumberOfLuts, char* Description, WORD wDescLen, WORD* wIdentifier, BYTE* bInputWidth, BYTE* bOutputWidth, WORD* wFormat) {
        if (!_PCO_GetLookupTableInfo) {
            throw std::logic_error("Function PCO_GetLookupTableInfo is not loaded.");
        }
        return _PCO_GetLookupTableInfo(ph, wLUTNum, wNumberOfLuts, Description, wDescLen, wIdentifier, bInputWidth, bOutputWidth, wFormat);
    }

    int PCO_GetActiveLookupTable(HANDLE ph, WORD* wIdentifier, WORD* wParameter) {
        if (!_PCO_GetActiveLookupTable) {
            throw std::logic_error("Function PCO_GetActiveLookupTable is not loaded.");
        }
        return _PCO_GetActiveLookupTable(ph, wIdentifier, wParameter);
    }

    int PCO_SetActiveLookupTable(HANDLE ph, WORD* wIdentifier, WORD* wParameter) {
        if (!_PCO_SetActiveLookupTable) {
            throw std::logic_error("Function PCO_SetActiveLookupTable is not loaded.");
        }
        return _PCO_SetActiveLookupTable(ph, wIdentifier, wParameter);
    }

    int PCO_GetIntensifiedGatingMode(HANDLE ph, WORD* wIntensifiedGatingMode, WORD* wReserved) {
        if (!_PCO_GetIntensifiedGatingMode) {
            throw std::logic_error("Function PCO_GetIntensifiedGatingMode is not loaded.");
        }
        return _PCO_GetIntensifiedGatingMode(ph, wIntensifiedGatingMode, wReserved);
    }

    int PCO_SetIntensifiedGatingMode(HANDLE ph, WORD wIntensifiedGatingMode, WORD wReserved) {
        if (!_PCO_SetIntensifiedGatingMode) {
            throw std::logic_error("Function PCO_SetIntensifiedGatingMode is not loaded.");
        }
        return _PCO_SetIntensifiedGatingMode(ph, wIntensifiedGatingMode, wReserved);
    }

    int PCO_GetIntensifiedMCP(HANDLE ph, WORD* wIntensifiedVoltage, WORD* wReserved, DWORD* dwIntensifiedPhosphorDecay_us, DWORD* dwReserved1, DWORD* dwReserved2) {
        if (!_PCO_GetIntensifiedMCP) {
            throw std::logic_error("Function PCO_GetIntensifiedMCP is not loaded.");
        }
        return _PCO_GetIntensifiedMCP(ph, wIntensifiedVoltage, wReserved, dwIntensifiedPhosphorDecay_us, dwReserved1, dwReserved2);
    }

    int PCO_SetIntensifiedMCP(HANDLE ph, WORD wIntensifiedVoltage, WORD wFlags, WORD wReserved, DWORD dwIntensifiedPhosphorDecay_us, DWORD dwReserved1, DWORD dwReserved2) {
        if (!_PCO_SetIntensifiedMCP) {
            throw std::logic_error("Function PCO_SetIntensifiedMCP is not loaded.");
        }
        return _PCO_SetIntensifiedMCP(ph, wIntensifiedVoltage, wFlags, wReserved, dwIntensifiedPhosphorDecay_us, dwReserved1, dwReserved2);
    }

    int PCO_GetSensorDarkOffset(HANDLE ph, WORD* pwDarkOffset) {
        if (!_PCO_GetSensorDarkOffset) {
            throw std::logic_error("Function PCO_GetSensorDarkOffset is not loaded.");
        }
        return _PCO_GetSensorDarkOffset(ph, pwDarkOffset);
    }

    int PCO_GetTimingStruct(HANDLE ph, void* strTiming) {
        if (!_PCO_GetTimingStruct) {
            throw std::logic_error("Function PCO_GetTimingStruct is not loaded.");
        }
        return _PCO_GetTimingStruct(ph, strTiming);
    }

    int PCO_SetTimingStruct(HANDLE ph, void* strTiming) {
        if (!_PCO_SetTimingStruct) {
            throw std::logic_error("Function PCO_SetTimingStruct is not loaded.");
        }
        return _PCO_SetTimingStruct(ph, strTiming);
    }

    int PCO_GetDelayExposureTime(HANDLE ph, DWORD* dwDelay, DWORD* dwExposure, WORD* wTimeBaseDelay, WORD* wTimeBaseExposure) {
        if (!_PCO_GetDelayExposureTime) {
            throw std::logic_error("Function PCO_GetDelayExposureTime is not loaded.");
        }
        return _PCO_GetDelayExposureTime(ph, dwDelay, dwExposure, wTimeBaseDelay, wTimeBaseExposure);
    }

    int PCO_SetDelayExposureTime(HANDLE ph, DWORD dwDelay, DWORD dwExposure, WORD wTimeBaseDelay, WORD wTimeBaseExposure) {
        if (!_PCO_SetDelayExposureTime) {
            throw std::logic_error("Function PCO_SetDelayExposureTime is not loaded.");
        }
        return _PCO_SetDelayExposureTime(ph, dwDelay, dwExposure, wTimeBaseDelay, wTimeBaseExposure);
    }

    int PCO_GetDelayExposureTimeTable(HANDLE ph, DWORD* dwDelay, DWORD* dwExposure, WORD* wTimeBaseDelay, WORD* wTimeBaseExposure, WORD wCount) {
        if (!_PCO_GetDelayExposureTimeTable) {
            throw std::logic_error("Function PCO_GetDelayExposureTimeTable is not loaded.");
        }
        return _PCO_GetDelayExposureTimeTable(ph, dwDelay, dwExposure, wTimeBaseDelay, wTimeBaseExposure, wCount);
    }

    int PCO_SetDelayExposureTimeTable(HANDLE ph, DWORD* dwDelay, DWORD* dwExposure, WORD wTimeBaseDelay, WORD wTimeBaseExposure, WORD wCount) {
        if (!_PCO_SetDelayExposureTimeTable) {
            throw std::logic_error("Function PCO_SetDelayExposureTimeTable is not loaded.");
        }
        return _PCO_SetDelayExposureTimeTable(ph, dwDelay, dwExposure, wTimeBaseDelay, wTimeBaseExposure, wCount);
    }

    int PCO_GetTriggerMode(HANDLE ph, WORD* wTriggerMode) {
        if (!_PCO_GetTriggerMode) {
            throw std::logic_error("Function PCO_GetTriggerMode is not loaded.");
        }
        return _PCO_GetTriggerMode(ph, wTriggerMode);
    }

    int PCO_SetTriggerMode(HANDLE ph, WORD wTriggerMode) {
        if (!_PCO_SetTriggerMode) {
            throw std::logic_error("Function PCO_SetTriggerMode is not loaded.");
        }
        return _PCO_SetTriggerMode(ph, wTriggerMode);
    }

    int PCO_ForceTrigger(HANDLE ph, WORD* wTriggered) {
        if (!_PCO_ForceTrigger) {
            throw std::logic_error("Function PCO_ForceTrigger is not loaded.");
        }
        return _PCO_ForceTrigger(ph, wTriggered);
    }

    int PCO_GetCameraBusyStatus(HANDLE ph, WORD* wCameraBusyState) {
        if (!_PCO_GetCameraBusyStatus) {
            throw std::logic_error("Function PCO_GetCameraBusyStatus is not loaded.");
        }
        return _PCO_GetCameraBusyStatus(ph, wCameraBusyState);
    }

    int PCO_GetPowerDownMode(HANDLE ph, WORD* wPowerDownMode) {
        if (!_PCO_GetPowerDownMode) {
            throw std::logic_error("Function PCO_GetPowerDownMode is not loaded.");
        }
        return _PCO_GetPowerDownMode(ph, wPowerDownMode);
    }

    int PCO_SetPowerDownMode(HANDLE ph, WORD wPowerDownMode) {
        if (!_PCO_SetPowerDownMode) {
            throw std::logic_error("Function PCO_SetPowerDownMode is not loaded.");
        }
        return _PCO_SetPowerDownMode(ph, wPowerDownMode);
    }

    int PCO_GetUserPowerDownTime(HANDLE ph, DWORD* dwPowerDownTime) {
        if (!_PCO_GetUserPowerDownTime) {
            throw std::logic_error("Function PCO_GetUserPowerDownTime is not loaded.");
        }
        return _PCO_GetUserPowerDownTime(ph, dwPowerDownTime);
    }

    int PCO_SetUserPowerDownTime(HANDLE ph, DWORD dwPowerDownTime) {
        if (!_PCO_SetUserPowerDownTime) {
            throw std::logic_error("Function PCO_SetUserPowerDownTime is not loaded.");
        }
        return _PCO_SetUserPowerDownTime(ph, dwPowerDownTime);
    }

    int PCO_GetExpTrigSignalStatus(HANDLE ph, WORD* wExpTrgSignal) {
        if (!_PCO_GetExpTrigSignalStatus) {
            throw std::logic_error("Function PCO_GetExpTrigSignalStatus is not loaded.");
        }
        return _PCO_GetExpTrigSignalStatus(ph, wExpTrgSignal);
    }

    int PCO_GetCOCRuntime(HANDLE ph, DWORD* dwTime_s, DWORD* dwTime_ns) {
        if (!_PCO_GetCOCRuntime) {
            throw std::logic_error("Function PCO_GetCOCRuntime is not loaded.");
        }
        return _PCO_GetCOCRuntime(ph, dwTime_s, dwTime_ns);
    }

    int PCO_GetFPSExposureMode(HANDLE ph, WORD* wFPSExposureMode, DWORD* dwFPSExposureTime) {
        if (!_PCO_GetFPSExposureMode) {
            throw std::logic_error("Function PCO_GetFPSExposureMode is not loaded.");
        }
        return _PCO_GetFPSExposureMode(ph, wFPSExposureMode, dwFPSExposureTime);
    }

    int PCO_SetFPSExposureMode(HANDLE ph, WORD wFPSExposureMode, DWORD* dwFPSExposureTime) {
        if (!_PCO_SetFPSExposureMode) {
            throw std::logic_error("Function PCO_SetFPSExposureMode is not loaded.");
        }
        return _PCO_SetFPSExposureMode(ph, wFPSExposureMode, dwFPSExposureTime);
    }

    int PCO_GetModulationMode(HANDLE ph, WORD* wModulationMode, DWORD* dwPeriodicalTime, WORD* wTimebasePeriodical, DWORD* dwNumberOfExposures, LONG* lMonitorOffset) {
        if (!_PCO_GetModulationMode) {
            throw std::logic_error("Function PCO_GetModulationMode is not loaded.");
        }
        return _PCO_GetModulationMode(ph, wModulationMode, dwPeriodicalTime, wTimebasePeriodical, dwNumberOfExposures, lMonitorOffset);
    }

    int PCO_SetModulationMode(HANDLE ph, WORD wModulationMode, DWORD dwPeriodicalTime, WORD wTimebasePeriodical, DWORD dwNumberOfExposures, LONG lMonitorOffset) {
        if (!_PCO_SetModulationMode) {
            throw std::logic_error("Function PCO_SetModulationMode is not loaded.");
        }
        return _PCO_SetModulationMode(ph, wModulationMode, dwPeriodicalTime, wTimebasePeriodical, dwNumberOfExposures, lMonitorOffset);
    }

    int PCO_GetFrameRate(HANDLE ph, WORD* wFrameRateStatus, DWORD* dwFrameRate, DWORD* dwFrameRateExposure) {
        if (!_PCO_GetFrameRate) {
            throw std::logic_error("Function PCO_GetFrameRate is not loaded.");
        }
        return _PCO_GetFrameRate(ph, wFrameRateStatus, dwFrameRate, dwFrameRateExposure);
    }

    int PCO_SetFrameRate(HANDLE ph, WORD* wFrameRateStatus, WORD wFrameRateMode, DWORD* dwFrameRate, DWORD* dwFrameRateExposure) {
        if (!_PCO_SetFrameRate) {
            throw std::logic_error("Function PCO_SetFrameRate is not loaded.");
        }
        return _PCO_SetFrameRate(ph, wFrameRateStatus, wFrameRateMode, dwFrameRate, dwFrameRateExposure);
    }

    int PCO_GetStorageStruct(HANDLE ph, void* strStorage) {
        if (!_PCO_GetStorageStruct) {
            throw std::logic_error("Function PCO_GetStorageStruct is not loaded.");
        }
        return _PCO_GetStorageStruct(ph, strStorage);
    }

    int PCO_SetStorageStruct(HANDLE ph, void* strStorage) {
        if (!_PCO_SetStorageStruct) {
            throw std::logic_error("Function PCO_SetStorageStruct is not loaded.");
        }
        return _PCO_SetStorageStruct(ph, strStorage);
    }

    int PCO_GetCameraRamSize(HANDLE ph, DWORD* dwRamSize, WORD* wPageSize) {
        if (!_PCO_GetCameraRamSize) {
            throw std::logic_error("Function PCO_GetCameraRamSize is not loaded.");
        }
        return _PCO_GetCameraRamSize(ph, dwRamSize, wPageSize);
    }

    int PCO_GetCameraRamSegmentSize(HANDLE ph, DWORD* dwRamSegSize) {
        if (!_PCO_GetCameraRamSegmentSize) {
            throw std::logic_error("Function PCO_GetCameraRamSegmentSize is not loaded.");
        }
        return _PCO_GetCameraRamSegmentSize(ph, dwRamSegSize);
    }

    int PCO_SetCameraRamSegmentSize(HANDLE ph, DWORD* dwRamSegSize) {
        if (!_PCO_SetCameraRamSegmentSize) {
            throw std::logic_error("Function PCO_SetCameraRamSegmentSize is not loaded.");
        }
        return _PCO_SetCameraRamSegmentSize(ph, dwRamSegSize);
    }

    int PCO_ClearRamSegment(HANDLE ph) {
        if (!_PCO_ClearRamSegment) {
            throw std::logic_error("Function PCO_ClearRamSegment is not loaded.");
        }
        return _PCO_ClearRamSegment(ph);
    }

    int PCO_GetActiveRamSegment(HANDLE ph, WORD* wActSeg) {
        if (!_PCO_GetActiveRamSegment) {
            throw std::logic_error("Function PCO_GetActiveRamSegment is not loaded.");
        }
        return _PCO_GetActiveRamSegment(ph, wActSeg);
    }

    int PCO_SetActiveRamSegment(HANDLE ph, WORD wActSeg) {
        if (!_PCO_SetActiveRamSegment) {
            throw std::logic_error("Function PCO_SetActiveRamSegment is not loaded.");
        }
        return _PCO_SetActiveRamSegment(ph, wActSeg);
    }

    int PCO_GetCompressionMode(HANDLE ph, WORD* wCompressionMode, DWORD* pdwReserved, WORD wReservedLen) {
        if (!_PCO_GetCompressionMode) {
            throw std::logic_error("Function PCO_GetCompressionMode is not loaded.");
        }
        return _PCO_GetCompressionMode(ph, wCompressionMode, pdwReserved, wReservedLen);
    }

    int PCO_SetCompressionMode(HANDLE ph, WORD wCompressionMode, DWORD* pdwReserved, WORD wReservedLen) {
        if (!_PCO_SetCompressionMode) {
            throw std::logic_error("Function PCO_SetCompressionMode is not loaded.");
        }
        return _PCO_SetCompressionMode(ph, wCompressionMode, pdwReserved, wReservedLen);
    }

    int PCO_GetMaxNumberOfImagesInSegment(HANDLE ph, DWORD* dwMaxNumberImages) {
        if (!_PCO_GetMaxNumberOfImagesInSegment) {
            throw std::logic_error("Function PCO_GetMaxNumberOfImagesInSegment is not loaded.");
        }
        return _PCO_GetMaxNumberOfImagesInSegment(ph, dwMaxNumberImages);
    }

    int PCO_GetRecordingStruct(HANDLE ph, void* strRecording) {
        if (!_PCO_GetRecordingStruct) {
            throw std::logic_error("Function PCO_GetRecordingStruct is not loaded.");
        }
        return _PCO_GetRecordingStruct(ph, strRecording);
    }

    int PCO_SetRecordingStruct(HANDLE ph, void* strRecording) {
        if (!_PCO_SetRecordingStruct) {
            throw std::logic_error("Function PCO_SetRecordingStruct is not loaded.");
        }
        return _PCO_SetRecordingStruct(ph, strRecording);
    }

    int PCO_GetStorageMode(HANDLE ph, WORD* wStorageMode) {
        if (!_PCO_GetStorageMode) {
            throw std::logic_error("Function PCO_GetStorageMode is not loaded.");
        }
        return _PCO_GetStorageMode(ph, wStorageMode);
    }

    int PCO_SetStorageMode(HANDLE ph, WORD wStorageMode) {
        if (!_PCO_SetStorageMode) {
            throw std::logic_error("Function PCO_SetStorageMode is not loaded.");
        }
        return _PCO_SetStorageMode(ph, wStorageMode);
    }

    int PCO_GetRecorderSubmode(HANDLE ph, WORD* wRecSubmode) {
        if (!_PCO_GetRecorderSubmode) {
            throw std::logic_error("Function PCO_GetRecorderSubmode is not loaded.");
        }
        return _PCO_GetRecorderSubmode(ph, wRecSubmode);
    }

    int PCO_SetRecorderSubmode(HANDLE ph, WORD wRecSubmode) {
        if (!_PCO_SetRecorderSubmode) {
            throw std::logic_error("Function PCO_SetRecorderSubmode is not loaded.");
        }
        return _PCO_SetRecorderSubmode(ph, wRecSubmode);
    }

    int PCO_GetRecordingState(HANDLE ph, WORD* wRecState) {
        if (!_PCO_GetRecordingState) {
            throw std::logic_error("Function PCO_GetRecordingState is not loaded.");
        }
        return _PCO_GetRecordingState(ph, wRecState);
    }

    int PCO_SetRecordingState(HANDLE ph, WORD wRecState) {
        if (!_PCO_SetRecordingState) {
            throw std::logic_error("Function PCO_SetRecordingState is not loaded.");
        }
        return _PCO_SetRecordingState(ph, wRecState);
    }

    int PCO_ArmCamera(HANDLE ph) {
        if (!_PCO_ArmCamera) {
            throw std::logic_error("Function PCO_ArmCamera is not loaded.");
        }
        return _PCO_ArmCamera(ph);
    }

    int PCO_GetAcquireMode(HANDLE ph, WORD* wAcquMode) {
        if (!_PCO_GetAcquireMode) {
            throw std::logic_error("Function PCO_GetAcquireMode is not loaded.");
        }
        return _PCO_GetAcquireMode(ph, wAcquMode);
    }

    int PCO_SetAcquireMode(HANDLE ph, WORD wAcquMode) {
        if (!_PCO_SetAcquireMode) {
            throw std::logic_error("Function PCO_SetAcquireMode is not loaded.");
        }
        return _PCO_SetAcquireMode(ph, wAcquMode);
    }

    int PCO_GetAcquireModeEx(HANDLE ph, WORD* wAcquMode, DWORD* dwNumberImages, DWORD* dwReserved) {
        if (!_PCO_GetAcquireModeEx) {
            throw std::logic_error("Function PCO_GetAcquireModeEx is not loaded.");
        }
        return _PCO_GetAcquireModeEx(ph, wAcquMode, dwNumberImages, dwReserved);
    }

    int PCO_SetAcquireModeEx(HANDLE ph, WORD wAcquMode, DWORD dwNumberImages, DWORD* dwReserved) {
        if (!_PCO_SetAcquireModeEx) {
            throw std::logic_error("Function PCO_SetAcquireModeEx is not loaded.");
        }
        return _PCO_SetAcquireModeEx(ph, wAcquMode, dwNumberImages, dwReserved);
    }

    int PCO_GetAcquireControl(HANDLE ph, DWORD* dwAcquCtrlFlags, DWORD* dwReserved, WORD wNumReserved) {
        if (!_PCO_GetAcquireControl) {
            throw std::logic_error("Function PCO_GetAcquireControl is not loaded.");
        }
        return _PCO_GetAcquireControl(ph, dwAcquCtrlFlags, dwReserved, wNumReserved);
    }

    int PCO_SetAcquireControl(HANDLE ph, DWORD dwAcquCtrlFlags, DWORD* dwReserved, WORD wNumReserved) {
        if (!_PCO_SetAcquireControl) {
            throw std::logic_error("Function PCO_SetAcquireControl is not loaded.");
        }
        return _PCO_SetAcquireControl(ph, dwAcquCtrlFlags, dwReserved, wNumReserved);
    }

    int PCO_GetAcqEnblSignalStatus(HANDLE ph, WORD* wAcquEnableState) {
        if (!_PCO_GetAcqEnblSignalStatus) {
            throw std::logic_error("Function PCO_GetAcqEnblSignalStatus is not loaded.");
        }
        return _PCO_GetAcqEnblSignalStatus(ph, wAcquEnableState);
    }

    int PCO_SetDateTime(HANDLE ph, BYTE ucDay, BYTE ucMonth, WORD wYear, WORD wHour, BYTE ucMin, BYTE ucSec) {
        if (!_PCO_SetDateTime) {
            throw std::logic_error("Function PCO_SetDateTime is not loaded.");
        }
        return _PCO_SetDateTime(ph, ucDay, ucMonth, wYear, wHour, ucMin, ucSec);
    }

    int PCO_GetTimestampMode(HANDLE ph, WORD* wTimeStampMode) {
        if (!_PCO_GetTimestampMode) {
            throw std::logic_error("Function PCO_GetTimestampMode is not loaded.");
        }
        return _PCO_GetTimestampMode(ph, wTimeStampMode);
    }

    int PCO_SetTimestampMode(HANDLE ph, WORD wTimeStampMode) {
        if (!_PCO_SetTimestampMode) {
            throw std::logic_error("Function PCO_SetTimestampMode is not loaded.");
        }
        return _PCO_SetTimestampMode(ph, wTimeStampMode);
    }

    int PCO_GetRecordStopEvent(HANDLE ph, WORD* wRecordStopEventMode, DWORD* dwRecordStopDelayImages) {
        if (!_PCO_GetRecordStopEvent) {
            throw std::logic_error("Function PCO_GetRecordStopEvent is not loaded.");
        }
        return _PCO_GetRecordStopEvent(ph, wRecordStopEventMode, dwRecordStopDelayImages);
    }

    int PCO_SetRecordStopEvent(HANDLE ph, WORD wRecordStopEventMode, DWORD dwRecordStopDelayImages) {
        if (!_PCO_SetRecordStopEvent) {
            throw std::logic_error("Function PCO_SetRecordStopEvent is not loaded.");
        }
        return _PCO_SetRecordStopEvent(ph, wRecordStopEventMode, dwRecordStopDelayImages);
    }

    int PCO_StopRecord(HANDLE ph, WORD* wReserved0, DWORD* dwReserved1) {
        if (!_PCO_StopRecord) {
            throw std::logic_error("Function PCO_StopRecord is not loaded.");
        }
        return _PCO_StopRecord(ph, wReserved0, dwReserved1);
    }

    int PCO_GetImage(HANDLE ph, WORD wSegment, DWORD dw1stImage, DWORD dwLastImage, SHORT sBufNr) {
        if (!_PCO_GetImage) {
            throw std::logic_error("Function PCO_GetImage is not loaded.");
        }
        return _PCO_GetImage(ph, wSegment, dw1stImage, dwLastImage, sBufNr);
    }

    int PCO_GetImageEx(HANDLE ph, WORD wSegment, DWORD dw1stImage, DWORD dwLastImage, SHORT sBufNr, WORD wXRes, WORD wYRes, WORD wBitPerPixel) {
        if (!_PCO_GetImageEx) {
            throw std::logic_error("Function PCO_GetImageEx is not loaded.");
        }
        return _PCO_GetImageEx(ph, wSegment, dw1stImage, dwLastImage, sBufNr, wXRes, wYRes, wBitPerPixel);
    }

    int PCO_GetPendingBuffer(HANDLE ph, int* count) {
        if (!_PCO_GetPendingBuffer) {
            throw std::logic_error("Function PCO_GetPendingBuffer is not loaded.");
        }
        return _PCO_GetPendingBuffer(ph, count);
    }

    int PCO_CheckDeviceAvailability(HANDLE ph, WORD wNum) {
        if (!_PCO_CheckDeviceAvailability) {
            throw std::logic_error("Function PCO_CheckDeviceAvailability is not loaded.");
        }
        return _PCO_CheckDeviceAvailability(ph, wNum);
    }

    int PCO_SetTransferParameter(HANDLE ph, void* buffer, int ilen) {
        if (!_PCO_SetTransferParameter) {
            throw std::logic_error("Function PCO_SetTransferParameter is not loaded.");
        }
        return _PCO_SetTransferParameter(ph, buffer, ilen);
    }

    int PCO_GetTransferParameter(HANDLE ph, void* buffer, int ilen) {
        if (!_PCO_GetTransferParameter) {
            throw std::logic_error("Function PCO_GetTransferParameter is not loaded.");
        }
        return _PCO_GetTransferParameter(ph, buffer, ilen);
    }

    int PCO_SetTransferParametersAuto(HANDLE ph, void* buffer, int ilen) {
        if (!_PCO_SetTransferParametersAuto) {
            throw std::logic_error("Function PCO_SetTransferParametersAuto is not loaded.");
        }
        return _PCO_SetTransferParametersAuto(ph, buffer, ilen);
    }

    int PCO_CamLinkSetImageParameters(HANDLE ph, WORD wxres, WORD wyres) {
        if (!_PCO_CamLinkSetImageParameters) {
            throw std::logic_error("Function PCO_CamLinkSetImageParameters is not loaded.");
        }
        return _PCO_CamLinkSetImageParameters(ph, wxres, wyres);
    }

    int PCO_SetImageParameters(HANDLE ph, WORD wxres, WORD wyres, DWORD dwflags, void* param, int ilen) {
        if (!_PCO_SetImageParameters) {
            throw std::logic_error("Function PCO_SetImageParameters is not loaded.");
        }
        return _PCO_SetImageParameters(ph, wxres, wyres, dwflags, param, ilen);
    }

    int PCO_SetTimeouts(HANDLE ph, void* buffer, unsigned int size_in) {
        if (!_PCO_SetTimeouts) {
            throw std::logic_error("Function PCO_SetTimeouts is not loaded.");
        }
        return _PCO_SetTimeouts(ph, buffer, size_in);
    }

    int PCO_GetImageTransferMode(HANDLE ph, void* param, int ilen) {
        if (!_PCO_GetImageTransferMode) {
            throw std::logic_error("Function PCO_GetImageTransferMode is not loaded.");
        }
        return _PCO_GetImageTransferMode(ph, param, ilen);
    }

    int PCO_SetImageTransferMode(HANDLE ph, void* param, int ilen) {
        if (!_PCO_SetImageTransferMode) {
            throw std::logic_error("Function PCO_SetImageTransferMode is not loaded.");
        }
        return _PCO_SetImageTransferMode(ph, param, ilen);
    }

    int PCO_AddBufferExtern(HANDLE ph, HANDLE hEvent, WORD wActSeg, DWORD dw1stImage, DWORD dwLastImage, DWORD dwSynch, void* pBuf, DWORD dwLen, DWORD* dwStatus) {
        if (!_PCO_AddBufferExtern) {
            throw std::logic_error("Function PCO_AddBufferExtern is not loaded.");
        }
        return _PCO_AddBufferExtern(ph, hEvent, wActSeg, dw1stImage, dwLastImage, dwSynch, pBuf, dwLen, dwStatus);
    }

    int PCO_GetMetaData(HANDLE ph, SHORT sBufNr, void* pMetaData, DWORD dwReserved1, DWORD dwReserved2) {
        if (!_PCO_GetMetaData) {
            throw std::logic_error("Function PCO_GetMetaData is not loaded.");
        }
        return _PCO_GetMetaData(ph, sBufNr, pMetaData, dwReserved1, dwReserved2);
    }

    int PCO_GetMetaDataExtern(HANDLE ph, void* pBuf, void* pMetaData, DWORD dwReserved1, DWORD dwReserved2) {
        if (!_PCO_GetMetaDataExtern) {
            throw std::logic_error("Function PCO_GetMetaDataExtern is not loaded.");
        }
        return _PCO_GetMetaDataExtern(ph, pBuf, pMetaData, dwReserved1, dwReserved2);
    }

    int PCO_GetTimeStamp(HANDLE ph, SHORT sBufNr, WORD* wBuf, void* strTimeStamp) {
        if (!_PCO_GetTimeStamp) {
            throw std::logic_error("Function PCO_GetTimeStamp is not loaded.");
        }
        return _PCO_GetTimeStamp(ph, sBufNr, wBuf, strTimeStamp);
    }

    int PCO_GetDeviceStatus(HANDLE ph, WORD wNum, DWORD* dwStatus, WORD wStatusLen) {
        if (!_PCO_GetDeviceStatus) {
            throw std::logic_error("Function PCO_GetDeviceStatus is not loaded.");
        }
        return _PCO_GetDeviceStatus(ph, wNum, dwStatus, wStatusLen);
    }

    int PCO_ControlCommandCall(HANDLE ph, void* buf_in, unsigned int size_in, void* buf_out, unsigned int size_out) {
        if (!_PCO_ControlCommandCall) {
            throw std::logic_error("Function PCO_ControlCommandCall is not loaded.");
        }
        return _PCO_ControlCommandCall(ph, buf_in, size_in, buf_out, size_out);
    }

    int PCO_ResetLib() {
        if (!_PCO_ResetLib) {
            throw std::logic_error("Function PCO_ResetLib is not loaded.");
        }
        return _PCO_ResetLib();
    }

    int PCO_InitializeLib() {
        if (!_PCO_InitializeLib) {
            throw std::logic_error("Function PCO_InitializeLib is not loaded.");
        }
        return _PCO_InitializeLib();
    }

    int PCO_CleanupLib() {
        if (!_PCO_CleanupLib) {
            throw std::logic_error("Function PCO_CleanupLib is not loaded.");
        }
        return _PCO_CleanupLib();
    }

    int PCO_EnableSoftROI(HANDLE ph, WORD wSoftROIFlags, void* param, int ilen) {
        if (!_PCO_EnableSoftROI) {
            throw std::logic_error("Function PCO_EnableSoftROI is not loaded.");
        }
        return _PCO_EnableSoftROI(ph, wSoftROIFlags, param, ilen);
    }

    int PCO_GetAPIManagement(HANDLE ph, WORD* wFlags, void* pstrApi) {
        if (!_PCO_GetAPIManagement) {
            throw std::logic_error("Function PCO_GetAPIManagement is not loaded.");
        }
        return _PCO_GetAPIManagement(ph, wFlags, pstrApi);
    }

    void PCO_GetErrorTextSDK(DWORD dwError, char* pszErrorString, DWORD dwErrorStringLength) {
        if (!_PCO_GetErrorTextSDK) {
            throw std::logic_error("Function PCO_GetErrorTextSDK is not loaded.");
        }
        _PCO_GetErrorTextSDK(dwError, pszErrorString, dwErrorStringLength);
    }

    int PCO_GetVersionInfoSC2_Cam(char* pszName, int iNameLength, char* pszPath, int iPathLength, int* piMajor, int* piMinor, int* piPatch, int* piBuild) {
        if (!_PCO_GetVersionInfoSC2_Cam) {
            throw std::logic_error("Function PCO_GetVersionInfoSC2_Cam is not loaded.");
        }
        return _PCO_GetVersionInfoSC2_Cam(pszName, iNameLength, pszPath, iPathLength, piMajor, piMinor, piPatch, piBuild);
    }

    int PCO_CloseCamera(HANDLE ph) {
        if (!_PCO_CloseCamera) {
            throw std::logic_error("Function PCO_CloseCamera is not loaded.");
        }
        return _PCO_CloseCamera(ph);
    }

    int PCO_CancelImages(HANDLE ph) {
        if (!_PCO_CancelImages) {
            throw std::logic_error("Function PCO_CancelImages is not loaded.");
        }
        return _PCO_CancelImages(ph);
    }

    int PCO_SetMetaDataMode(HANDLE ph, WORD wMetaDataMode, WORD* wMetaDataSize, WORD* wMetaDataVersion) {
        if (!_PCO_SetMetaDataMode) {
            throw std::logic_error("Function PCO_SetMetaDataMode is not loaded.");
        }
        return _PCO_SetMetaDataMode(ph, wMetaDataMode, wMetaDataSize, wMetaDataVersion);
    }

    int PCO_OpenCamera(HANDLE* ph, WORD wCamNum) {
        if (!_PCO_OpenCamera) {
            throw std::logic_error("Function PCO_OpenCamera is not loaded.");
        }
        return _PCO_OpenCamera(ph, wCamNum);
    }

private:
    LIB_HANDLE handle;
    std::map<std::string, std::pair<std::any, void**>> functionMap;

    // Function pointers with leading underscore
    PCO_GetGeneral_func _PCO_GetGeneral = nullptr;
    PCO_GetCameraType_func _PCO_GetCameraType = nullptr;
    PCO_GetCameraHealthStatus_func _PCO_GetCameraHealthStatus = nullptr;
    PCO_ResetSettingsToDefault_func _PCO_ResetSettingsToDefault = nullptr;
    PCO_GetTemperature_func _PCO_GetTemperature = nullptr;
    PCO_GetInfoString_func _PCO_GetInfoString = nullptr;
    PCO_GetCameraName_func _PCO_GetCameraName = nullptr;
    PCO_GetFirmwareInfo_func _PCO_GetFirmwareInfo = nullptr;
    PCO_GetCameraSetup_func _PCO_GetCameraSetup = nullptr;
    PCO_SetCameraSetup_func _PCO_SetCameraSetup = nullptr;
    PCO_RebootCamera_func _PCO_RebootCamera = nullptr;
    PCO_GetShutterMode_func _PCO_GetShutterMode = nullptr;
    PCO_SetShutterMode_func _PCO_SetShutterMode = nullptr;
    PCO_GetPowerSaveMode_func _PCO_GetPowerSaveMode = nullptr;
    PCO_SetPowerSaveMode_func _PCO_SetPowerSaveMode = nullptr;
    PCO_GetBatteryStatus_func _PCO_GetBatteryStatus = nullptr;
    PCO_GetFanControlParameters_func _PCO_GetFanControlParameters = nullptr;
    PCO_SetFanControlParameters_func _PCO_SetFanControlParameters = nullptr;
    PCO_GetSensorStruct_func _PCO_GetSensorStruct = nullptr;
    PCO_SetSensorStruct_func _PCO_SetSensorStruct = nullptr;
    PCO_GetCameraDescription_func _PCO_GetCameraDescription = nullptr;
    PCO_GetCameraDescriptionEx_func _PCO_GetCameraDescriptionEx = nullptr;
    PCO_GetSensorFormat_func _PCO_GetSensorFormat = nullptr;
    PCO_SetSensorFormat_func _PCO_SetSensorFormat = nullptr;
    PCO_GetSizes_func _PCO_GetSizes = nullptr;
    PCO_GetROI_func _PCO_GetROI = nullptr;
    PCO_SetROI_func _PCO_SetROI = nullptr;
    PCO_GetBinning_func _PCO_GetBinning = nullptr;
    PCO_SetBinning_func _PCO_SetBinning = nullptr;
    PCO_GetPixelRate_func _PCO_GetPixelRate = nullptr;
    PCO_SetPixelRate_func _PCO_SetPixelRate = nullptr;
    PCO_GetConversionFactor_func _PCO_GetConversionFactor = nullptr;
    PCO_SetConversionFactor_func _PCO_SetConversionFactor = nullptr;
    PCO_GetDoubleImageMode_func _PCO_GetDoubleImageMode = nullptr;
    PCO_SetDoubleImageMode_func _PCO_SetDoubleImageMode = nullptr;
    PCO_GetADCOperation_func _PCO_GetADCOperation = nullptr;
    PCO_SetADCOperation_func _PCO_SetADCOperation = nullptr;
    PCO_GetIRSensitivity_func _PCO_GetIRSensitivity = nullptr;
    PCO_SetIRSensitivity_func _PCO_SetIRSensitivity = nullptr;
    PCO_GetCoolingSetpoints_func _PCO_GetCoolingSetpoints = nullptr;
    PCO_GetCoolingSetpointTemperature_func _PCO_GetCoolingSetpointTemperature = nullptr;
    PCO_SetCoolingSetpointTemperature_func _PCO_SetCoolingSetpointTemperature = nullptr;
    PCO_GetOffsetMode_func _PCO_GetOffsetMode = nullptr;
    PCO_SetOffsetMode_func _PCO_SetOffsetMode = nullptr;
    PCO_GetNoiseFilterMode_func _PCO_GetNoiseFilterMode = nullptr;
    PCO_SetNoiseFilterMode_func _PCO_SetNoiseFilterMode = nullptr;
    PCO_GetHWIOSignalCount_func _PCO_GetHWIOSignalCount = nullptr;
    PCO_GetHWIOSignalDescriptor_func _PCO_GetHWIOSignalDescriptor = nullptr;
    PCO_GetColorCorrectionMatrix_func _PCO_GetColorCorrectionMatrix = nullptr;
    PCO_GetDSNUAdjustMode_func _PCO_GetDSNUAdjustMode = nullptr;
    PCO_SetDSNUAdjustMode_func _PCO_SetDSNUAdjustMode = nullptr;
    PCO_InitDSNUAdjustment_func _PCO_InitDSNUAdjustment = nullptr;
    PCO_GetCDIMode_func _PCO_GetCDIMode = nullptr;
    PCO_SetCDIMode_func _PCO_SetCDIMode = nullptr;
    PCO_GetLookupTableInfo_func _PCO_GetLookupTableInfo = nullptr;
    PCO_GetActiveLookupTable_func _PCO_GetActiveLookupTable = nullptr;
    PCO_SetActiveLookupTable_func _PCO_SetActiveLookupTable = nullptr;
    PCO_GetIntensifiedGatingMode_func _PCO_GetIntensifiedGatingMode = nullptr;
    PCO_SetIntensifiedGatingMode_func _PCO_SetIntensifiedGatingMode = nullptr;
    PCO_GetIntensifiedMCP_func _PCO_GetIntensifiedMCP = nullptr;
    PCO_SetIntensifiedMCP_func _PCO_SetIntensifiedMCP = nullptr;
    PCO_GetSensorDarkOffset_func _PCO_GetSensorDarkOffset = nullptr;
    PCO_GetTimingStruct_func _PCO_GetTimingStruct = nullptr;
    PCO_SetTimingStruct_func _PCO_SetTimingStruct = nullptr;
    PCO_GetDelayExposureTime_func _PCO_GetDelayExposureTime = nullptr;
    PCO_SetDelayExposureTime_func _PCO_SetDelayExposureTime = nullptr;
    PCO_GetDelayExposureTimeTable_func _PCO_GetDelayExposureTimeTable = nullptr;
    PCO_SetDelayExposureTimeTable_func _PCO_SetDelayExposureTimeTable = nullptr;
    PCO_GetTriggerMode_func _PCO_GetTriggerMode = nullptr;
    PCO_SetTriggerMode_func _PCO_SetTriggerMode = nullptr;
    PCO_ForceTrigger_func _PCO_ForceTrigger = nullptr;
    PCO_GetCameraBusyStatus_func _PCO_GetCameraBusyStatus = nullptr;
    PCO_GetPowerDownMode_func _PCO_GetPowerDownMode = nullptr;
    PCO_SetPowerDownMode_func _PCO_SetPowerDownMode = nullptr;
    PCO_GetUserPowerDownTime_func _PCO_GetUserPowerDownTime = nullptr;
    PCO_SetUserPowerDownTime_func _PCO_SetUserPowerDownTime = nullptr;
    PCO_GetExpTrigSignalStatus_func _PCO_GetExpTrigSignalStatus = nullptr;
    PCO_GetCOCRuntime_func _PCO_GetCOCRuntime = nullptr;
    PCO_GetFPSExposureMode_func _PCO_GetFPSExposureMode = nullptr;
    PCO_SetFPSExposureMode_func _PCO_SetFPSExposureMode = nullptr;
    PCO_GetModulationMode_func _PCO_GetModulationMode = nullptr;
    PCO_SetModulationMode_func _PCO_SetModulationMode = nullptr;
    PCO_GetFrameRate_func _PCO_GetFrameRate = nullptr;
    PCO_SetFrameRate_func _PCO_SetFrameRate = nullptr;
    PCO_GetStorageStruct_func _PCO_GetStorageStruct = nullptr;
    PCO_SetStorageStruct_func _PCO_SetStorageStruct = nullptr;
    PCO_GetCameraRamSize_func _PCO_GetCameraRamSize = nullptr;
    PCO_GetCameraRamSegmentSize_func _PCO_GetCameraRamSegmentSize = nullptr;
    PCO_SetCameraRamSegmentSize_func _PCO_SetCameraRamSegmentSize = nullptr;
    PCO_ClearRamSegment_func _PCO_ClearRamSegment = nullptr;
    PCO_GetActiveRamSegment_func _PCO_GetActiveRamSegment = nullptr;
    PCO_SetActiveRamSegment_func _PCO_SetActiveRamSegment = nullptr;
    PCO_GetCompressionMode_func _PCO_GetCompressionMode = nullptr;
    PCO_SetCompressionMode_func _PCO_SetCompressionMode = nullptr;
    PCO_GetMaxNumberOfImagesInSegment_func _PCO_GetMaxNumberOfImagesInSegment = nullptr;
    PCO_GetRecordingStruct_func _PCO_GetRecordingStruct = nullptr;
    PCO_SetRecordingStruct_func _PCO_SetRecordingStruct = nullptr;
    PCO_GetStorageMode_func _PCO_GetStorageMode = nullptr;
    PCO_SetStorageMode_func _PCO_SetStorageMode = nullptr;
    PCO_GetRecorderSubmode_func _PCO_GetRecorderSubmode = nullptr;
    PCO_SetRecorderSubmode_func _PCO_SetRecorderSubmode = nullptr;
    PCO_GetRecordingState_func _PCO_GetRecordingState = nullptr;
    PCO_SetRecordingState_func _PCO_SetRecordingState = nullptr;
    PCO_ArmCamera_func _PCO_ArmCamera = nullptr;
    PCO_GetAcquireMode_func _PCO_GetAcquireMode = nullptr;
    PCO_SetAcquireMode_func _PCO_SetAcquireMode = nullptr;
    PCO_GetAcquireModeEx_func _PCO_GetAcquireModeEx = nullptr;
    PCO_SetAcquireModeEx_func _PCO_SetAcquireModeEx = nullptr;
    PCO_GetAcquireControl_func _PCO_GetAcquireControl = nullptr;
    PCO_SetAcquireControl_func _PCO_SetAcquireControl = nullptr;
    PCO_GetAcqEnblSignalStatus_func _PCO_GetAcqEnblSignalStatus = nullptr;
    PCO_SetDateTime_func _PCO_SetDateTime = nullptr;
    PCO_GetTimestampMode_func _PCO_GetTimestampMode = nullptr;
    PCO_SetTimestampMode_func _PCO_SetTimestampMode = nullptr;
    PCO_GetRecordStopEvent_func _PCO_GetRecordStopEvent = nullptr;
    PCO_SetRecordStopEvent_func _PCO_SetRecordStopEvent = nullptr;
    PCO_StopRecord_func _PCO_StopRecord = nullptr;
    PCO_GetImage_func _PCO_GetImage = nullptr;
    PCO_GetImageEx_func _PCO_GetImageEx = nullptr;
    PCO_GetPendingBuffer_func _PCO_GetPendingBuffer = nullptr;
    PCO_CheckDeviceAvailability_func _PCO_CheckDeviceAvailability = nullptr;
    PCO_SetTransferParameter_func _PCO_SetTransferParameter = nullptr;
    PCO_GetTransferParameter_func _PCO_GetTransferParameter = nullptr;
    PCO_SetTransferParametersAuto_func _PCO_SetTransferParametersAuto = nullptr;
    PCO_CamLinkSetImageParameters_func _PCO_CamLinkSetImageParameters = nullptr;
    PCO_SetImageParameters_func _PCO_SetImageParameters = nullptr;
    PCO_SetTimeouts_func _PCO_SetTimeouts = nullptr;
    PCO_GetImageTransferMode_func _PCO_GetImageTransferMode = nullptr;
    PCO_SetImageTransferMode_func _PCO_SetImageTransferMode = nullptr;
    PCO_AddBufferExtern_func _PCO_AddBufferExtern = nullptr;
    PCO_GetMetaData_func _PCO_GetMetaData = nullptr;
    PCO_GetMetaDataExtern_func _PCO_GetMetaDataExtern = nullptr;
    PCO_GetTimeStamp_func _PCO_GetTimeStamp = nullptr;
    PCO_GetDeviceStatus_func _PCO_GetDeviceStatus = nullptr;
    PCO_ControlCommandCall_func _PCO_ControlCommandCall = nullptr;
    PCO_ResetLib_func _PCO_ResetLib = nullptr;
    PCO_InitializeLib_func _PCO_InitializeLib = nullptr;
    PCO_CleanupLib_func _PCO_CleanupLib = nullptr;
    PCO_EnableSoftROI_func _PCO_EnableSoftROI = nullptr;
    PCO_GetAPIManagement_func _PCO_GetAPIManagement = nullptr;
    PCO_GetErrorTextSDK_func _PCO_GetErrorTextSDK = nullptr;
    PCO_GetVersionInfoSC2_Cam_func _PCO_GetVersionInfoSC2_Cam = nullptr;
    PCO_CloseCamera_func _PCO_CloseCamera = nullptr;
    PCO_CancelImages_func _PCO_CancelImages = nullptr;
    PCO_SetMetaDataMode_func _PCO_SetMetaDataMode = nullptr;
    PCO_OpenCamera_func _PCO_OpenCamera = nullptr;

};

PCOAPIWrapper GetPCOAPIWrapper();

#endif //PCOAPIWRAPPER_H
