/*!
 * \file    ids_peak_comfort_c_dynamic_loader.h
 *
 * \author  IDS Imaging Development Systems GmbH
 * \date    2019-05-01
 * \since   1.0
 *
 * Copyright (C) 2022 - 2025, IDS Imaging Development Systems GmbH.
 */
#pragma once

#include "ids_peak_comfort_c.h"
            
#include <string>
#include <cstdint>

#ifdef __linux__
    #include <dlfcn.h>
#else
    #include <vector>
    #include <windows.h>
    #include <tchar.h>
#endif
 
#include <stdexcept>

#undef PEAK_EXPORT
#define PEAK_EXPORT
#undef PEAK_CALLCONV
#define PEAK_CALLCONV

namespace peak_comfortC
{
namespace dynamic
{

typedef PEAK_API_STATUS (*dyn_peak_Library_Init)();
typedef PEAK_API_STATUS (*dyn_peak_Library_Exit)();
typedef PEAK_API_STATUS (*dyn_peak_Library_GetVersion)(uint32_t * majorVersion, uint32_t * minorVersion, uint32_t * subminorVersion, uint32_t * patchVersion);
typedef PEAK_API_STATUS (*dyn_peak_Library_GetLastError)(peak_status * lastErrorCode, char * lastErrorMessage, size_t * lastErrorMessageSize);
typedef PEAK_API_BOOL (*dyn_peak_Library_InterfaceTechnology_IsSupported)(peak_interface_technology interfaceTech);
typedef PEAK_API_STATUS (*dyn_peak_Reconnect_Enable)(peak_interface_technology interfaceTech, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_Reconnect_IsEnabled)(peak_interface_technology interfaceTech);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Reconnect_GetAccessStatus)(peak_interface_technology interfaceTech);
typedef PEAK_API_STATUS (*dyn_peak_CameraList_Update)(size_t * cameraCount);
typedef PEAK_API_STATUS (*dyn_peak_CameraList_Get)(peak_camera_descriptor * cameraList, size_t * cameraCount);
typedef PEAK_API_CAMERA_ID (*dyn_peak_Camera_ID_FromHandle)(peak_camera_handle hCam);
typedef PEAK_API_CAMERA_ID (*dyn_peak_Camera_ID_FromSerialNumber)(const char * serialNumber);
typedef PEAK_API_CAMERA_ID (*dyn_peak_Camera_ID_FromUserDefinedName)(const char * userDefinedName);
typedef PEAK_API_CAMERA_ID (*dyn_peak_Camera_ID_FromMAC)(peak_mac_address macAddress);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Camera_GetAccessStatus)(peak_camera_id cameraID);
typedef PEAK_API_STATUS (*dyn_peak_Camera_GetDescriptor)(peak_camera_id cameraID, peak_camera_descriptor * cameraDescriptor);
typedef PEAK_API_STATUS (*dyn_peak_Camera_Open)(peak_camera_id cameraID, peak_camera_handle * hCam);
typedef PEAK_API_STATUS (*dyn_peak_Camera_OpenFirstAvailable)(peak_camera_handle * hCam);
typedef PEAK_API_STATUS (*dyn_peak_Camera_Close)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Camera_ResetToDefaultSettings)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Camera_UserDefinedName_Set)(peak_camera_handle hCam, const char * userDefinedName);
typedef PEAK_API_STATUS (*dyn_peak_Camera_UserDefinedName_Get)(peak_camera_handle hCam, char * userDefinedName, size_t * userDefinedNameSize);
typedef PEAK_API_BOOL (*dyn_peak_Camera_IsConnected)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Camera_Reboot)(peak_camera_handle hCam);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_EthernetConfig_GetAccessStatus)(peak_camera_id cameraID);
typedef PEAK_API_STATUS (*dyn_peak_EthernetConfig_GetInfo)(peak_camera_id cameraID, peak_ethernet_info * ethernetInfo);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_EthernetConfig_DHCP_GetAccessStatus)(peak_camera_id cameraID);
typedef PEAK_API_STATUS (*dyn_peak_EthernetConfig_DHCP_Enable)(peak_camera_id cameraID, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_EthernetConfig_DHCP_IsEnabled)(peak_camera_id cameraID);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_EthernetConfig_PersistentIP_GetAccessStatus)(peak_camera_id cameraID);
typedef PEAK_API_STATUS (*dyn_peak_EthernetConfig_PersistentIP_Set)(peak_camera_id cameraID, peak_ip_config persistentIP);
typedef PEAK_API_STATUS (*dyn_peak_EthernetConfig_PersistentIP_Get)(peak_camera_id cameraID, peak_ip_config * persistentIP);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_Start)(peak_camera_handle hCam, uint32_t numberOfFrames);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_Stop)(peak_camera_handle hCam);
typedef PEAK_API_BOOL (*dyn_peak_Acquisition_IsStarted)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_WaitForFrame)(peak_camera_handle hCam, uint32_t timeout_ms, peak_frame_handle * hFrame);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_GetInfo)(peak_camera_handle hCam, peak_acquisition_info * acquisitionInfo);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_Buffer_GetRequiredSize)(peak_camera_handle hCam, size_t * requiredBufferSize);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_Buffer_GetRequiredCount)(peak_camera_handle hCam, size_t * requiredBufferCount);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_Buffer_Announce)(peak_camera_handle hCam, uint8_t * memoryAddress, size_t memorySize, void * userContext);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_Buffer_Revoke)(peak_camera_handle hCam, uint8_t * memoryAddress);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_Buffer_RevokeAll)(peak_camera_handle hCam);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Acquisition_BufferHandling_Mode_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_BufferHandling_Mode_GetList)(peak_camera_handle hCam, peak_buffer_handling_mode * bufferHandlingModeList, size_t * bufferHandlingModesCount);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_BufferHandling_Mode_Set)(peak_camera_handle hCam, peak_buffer_handling_mode mode);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_BufferHandling_Mode_Get)(peak_camera_handle hCam, peak_buffer_handling_mode * mode);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Acquisition_LossHandling_Mode_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_LossHandling_Mode_GetList)(peak_camera_handle hCam, peak_loss_handling_mode * lossHandlingModeList, size_t * lossHandlingModesCount);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_LossHandling_Mode_Set)(peak_camera_handle hCam, peak_loss_handling_mode mode);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_LossHandling_Mode_Get)(peak_camera_handle hCam, peak_loss_handling_mode * mode);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Acquisition_LossHandling_Extent_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_LossHandling_Extent_GetRange)(peak_camera_handle hCam, int64_t * minExtent, int64_t * maxExtent, int64_t * incExtent);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_LossHandling_Extent_Set)(peak_camera_handle hCam, int64_t extent);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_LossHandling_Extent_Get)(peak_camera_handle hCam, int64_t * extent);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Acquisition_LossHandling_FrameAbortTimeout_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_LossHandling_FrameAbortTimeout_GetRange)(peak_camera_handle hCam, int64_t * minTimeout, int64_t * maxTimeout, int64_t * incTimeout);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_LossHandling_FrameAbortTimeout_Set)(peak_camera_handle hCam, int64_t timeout);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_LossHandling_FrameAbortTimeout_Get)(peak_camera_handle hCam, int64_t * timeout);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Acquisition_LossHandling_ResendRequestTimeout_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_LossHandling_ResendRequestTimeout_GetRange)(peak_camera_handle hCam, int64_t * minTimeout, int64_t * maxTimeout, int64_t * incTimeout);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_LossHandling_ResendRequestTimeout_Set)(peak_camera_handle hCam, int64_t timeout);
typedef PEAK_API_STATUS (*dyn_peak_Acquisition_LossHandling_ResendRequestTimeout_Get)(peak_camera_handle hCam, int64_t * timeout);
typedef PEAK_API_STATUS (*dyn_peak_PixelFormat_GetInfo)(peak_pixel_format pixelFormat, peak_pixel_format_info * pixelFormatInfo);
typedef PEAK_API_STATUS (*dyn_peak_Frame_Release)(peak_camera_handle hCam, peak_frame_handle hFrame);
typedef PEAK_API_STATUS (*dyn_peak_Frame_GetInfo)(peak_frame_handle hFrame, peak_frame_info * frameInfo);
typedef PEAK_API_STATUS (*dyn_peak_Frame_Type_Get)(peak_frame_handle hFrame, peak_frame_type * frameType);
typedef PEAK_API_STATUS (*dyn_peak_Frame_Buffer_Get)(peak_frame_handle hFrame, peak_buffer * buffer);
typedef PEAK_API_STATUS (*dyn_peak_Frame_ID_Get)(peak_frame_handle hFrame, uint64_t * frameID);
typedef PEAK_API_STATUS (*dyn_peak_Frame_Timestamp_Get)(peak_frame_handle hFrame, uint64_t * timestamp_ns);
typedef PEAK_API_STATUS (*dyn_peak_Frame_ROI_Get)(peak_frame_handle hFrame, peak_roi * roi);
typedef PEAK_API_STATUS (*dyn_peak_Frame_PixelFormat_Get)(peak_frame_handle hFrame, peak_pixel_format * pixelFormat);
typedef PEAK_API_BOOL (*dyn_peak_Frame_IsComplete)(peak_frame_handle hFrame);
typedef PEAK_API_STATUS (*dyn_peak_Frame_BytesExpected_Get)(peak_frame_handle hFrame, size_t * bytesExpected);
typedef PEAK_API_STATUS (*dyn_peak_Frame_BytesWritten_Get)(peak_frame_handle hFrame, size_t * bytesWritten);
typedef PEAK_API_STATUS (*dyn_peak_Frame_ProcessingTime_Get)(peak_frame_handle hFrame, uint32_t * processingTime_ms);
typedef PEAK_API_STATUS (*dyn_peak_Frame_Save)(peak_frame_handle hFrame, const char * fileName);
typedef PEAK_API_BOOL (*dyn_peak_Frame_HasChunks)(peak_frame_handle hFrame);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_CameraSettings_ParameterSet_GetAccessStatus)(peak_camera_handle hCam, peak_parameter_set parameterSet);
typedef PEAK_API_STATUS (*dyn_peak_CameraSettings_ParameterSet_GetList)(peak_camera_handle hCam, peak_parameter_set * parameterSetList, size_t * parameterSetCount);
typedef PEAK_API_STATUS (*dyn_peak_CameraSettings_ParameterSet_Store)(peak_camera_handle hCam, peak_parameter_set parameterSet);
typedef PEAK_API_STATUS (*dyn_peak_CameraSettings_ParameterSet_Apply)(peak_camera_handle hCam, peak_parameter_set parameterSet);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_CameraSettings_ParameterSet_Startup_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_CameraSettings_ParameterSet_Startup_Set)(peak_camera_handle hCam, peak_parameter_set parameterSet);
typedef PEAK_API_STATUS (*dyn_peak_CameraSettings_ParameterSet_Startup_Get)(peak_camera_handle hCam, peak_parameter_set * parameterSet);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_CameraSettings_DiskFile_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_CameraSettings_DiskFile_Store)(peak_camera_handle hCam, const char * file);
typedef PEAK_API_STATUS (*dyn_peak_CameraSettings_DiskFile_Apply)(peak_camera_handle hCam, const char * file);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_FrameRate_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_FrameRate_GetRange)(peak_camera_handle hCam, double * minFrameRate_fps, double * maxFrameRate_fps, double * incFrameRate_fps);
typedef PEAK_API_STATUS (*dyn_peak_FrameRate_Set)(peak_camera_handle hCam, double frameRate_fps);
typedef PEAK_API_STATUS (*dyn_peak_FrameRate_Get)(peak_camera_handle hCam, double * frameRate_fps);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_ExposureTime_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_ExposureTime_GetRange)(peak_camera_handle hCam, double * minExposureTime_us, double * maxExposureTime_us, double * incExposureTime_us);
typedef PEAK_API_STATUS (*dyn_peak_ExposureTime_Set)(peak_camera_handle hCam, double exposureTime_us);
typedef PEAK_API_STATUS (*dyn_peak_ExposureTime_Get)(peak_camera_handle hCam, double * exposureTime_us);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_ShutterMode_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_ShutterMode_GetList)(peak_camera_handle hCam, peak_shutter_mode * shutterModeList, size_t * shutterModeCount);
typedef PEAK_API_STATUS (*dyn_peak_ShutterMode_Set)(peak_camera_handle hCam, peak_shutter_mode shutterMode);
typedef PEAK_API_STATUS (*dyn_peak_ShutterMode_Get)(peak_camera_handle hCam, peak_shutter_mode * shutterMode);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_PixelClock_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_BOOL (*dyn_peak_PixelClock_HasRange)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_PixelClock_GetRange)(peak_camera_handle hCam, double * minPixelClock_MHz, double * maxPixelClock_MHz, double * incPixelClock_MHz);
typedef PEAK_API_STATUS (*dyn_peak_PixelClock_GetList)(peak_camera_handle hCam, double * pixelClockList, size_t * pixelClockCount);
typedef PEAK_API_STATUS (*dyn_peak_PixelClock_Set)(peak_camera_handle hCam, double pixelClock_MHz);
typedef PEAK_API_STATUS (*dyn_peak_PixelClock_Get)(peak_camera_handle hCam, double * pixelClock_MHz);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_IOChannel_GetAccessStatus)(peak_camera_handle hCam, peak_io_channel ioChannel);
typedef  (*dyn_PEAK_API_STATUS_DEPRECATED)("Use peak_IOChannel_GetListForDirection instead") peak_IOChannel_GetList(peak_camera_handlehCam,peak_io_channel*ioChannelList,size_t*ioChannelCount);
typedef PEAK_API_STATUS (*dyn_peak_IOChannel_GetListForDirection)(peak_camera_handle hCam, peak_io_direction direction, peak_io_channel * ioChannelList, size_t * ioChannelCount);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_IOChannel_Direction_GetAccessStatus)(peak_camera_handle hCam, peak_io_channel ioChannel);
typedef PEAK_API_STATUS (*dyn_peak_IOChannel_Direction_Get)(peak_camera_handle hCam, peak_io_channel ioChannel, peak_io_direction * direction);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_IOChannel_Type_GetAccessStatus)(peak_camera_handle hCam, peak_io_channel ioChannel);
typedef PEAK_API_STATUS (*dyn_peak_IOChannel_Type_Get)(peak_camera_handle hCam, peak_io_channel ioChannel, peak_io_type * type);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_IOChannel_Level_GetAccessStatus)(peak_camera_handle hCam, peak_io_channel ioChannel);
typedef PEAK_API_BOOL (*dyn_peak_IOChannel_Level_IsHigh)(peak_camera_handle hCam, peak_io_channel ioChannel);
typedef PEAK_API_STATUS (*dyn_peak_IOChannel_Level_SetHigh)(peak_camera_handle hCam, peak_io_channel ioChannel, peak_bool high);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_IOChannel_Inverter_GetAccessStatus)(peak_camera_handle hCam, peak_io_channel ioChannel);
typedef PEAK_API_STATUS (*dyn_peak_IOChannel_Inverter_Enable)(peak_camera_handle hCam, peak_io_channel ioChannel, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_IOChannel_Inverter_IsEnabled)(peak_camera_handle hCam, peak_io_channel ioChannel);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_IOChannel_NoiseFilter_GetAccessStatus)(peak_camera_handle hCam, peak_io_channel ioChannel);
typedef PEAK_API_STATUS (*dyn_peak_IOChannel_NoiseFilter_Enable)(peak_camera_handle hCam, peak_io_channel ioChannel, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_IOChannel_NoiseFilter_IsEnabled)(peak_camera_handle hCam, peak_io_channel ioChannel);
typedef PEAK_API_STATUS (*dyn_peak_IOChannel_NoiseFilter_Duration_GetRange)(peak_camera_handle hCam, peak_io_channel ioChannel, double * minNoiseFilterDuration_us, double * maxNoiseFilterDuration_us, double * incNoiseFilterDuration_us);
typedef PEAK_API_STATUS (*dyn_peak_IOChannel_NoiseFilter_Duration_Set)(peak_camera_handle hCam, peak_io_channel ioChannel, double noiseFilterDuration_us);
typedef PEAK_API_STATUS (*dyn_peak_IOChannel_NoiseFilter_Duration_Get)(peak_camera_handle hCam, peak_io_channel ioChannel, double * noiseFilterDuration_us);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Trigger_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Enable)(peak_camera_handle hCam, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_Trigger_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_BOOL (*dyn_peak_Trigger_IsExecutable)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Execute)(peak_camera_handle hCam);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Trigger_Mode_GetAccessStatus)(peak_camera_handle hCam, peak_trigger_mode triggerMode);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Mode_Set)(peak_camera_handle hCam, peak_trigger_mode triggerMode);
typedef  (*dyn_PEAK_API_STATUS_DEPRECATED)("Use peak_Trigger_Mode_Config_Get instead") peak_Trigger_Mode_Get(peak_camera_handlehCam,peak_trigger_mode*triggerMode);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Mode_Config_Get)(peak_camera_handle hCam, peak_trigger_mode * triggerMode);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Trigger_Edge_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Edge_GetList)(peak_camera_handle hCam, peak_trigger_edge * triggerEdgeList, size_t * triggerEdgeCount);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Edge_Set)(peak_camera_handle hCam, peak_trigger_edge triggerEdge);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Edge_Get)(peak_camera_handle hCam, peak_trigger_edge * triggerEdge);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Trigger_Delay_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Delay_GetRange)(peak_camera_handle hCam, double * minTriggerDelay_us, double * maxTriggerDelay_us, double * incTriggerDelay_us);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Delay_Set)(peak_camera_handle hCam, double triggerDelay_us);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Delay_Get)(peak_camera_handle hCam, double * triggerDelay_us);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Trigger_Divider_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Divider_GetRange)(peak_camera_handle hCam, uint32_t * minTriggerDivider, uint32_t * maxTriggerDivider, uint32_t * incTriggerDivider);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Divider_Set)(peak_camera_handle hCam, uint32_t triggerDivider);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Divider_Get)(peak_camera_handle hCam, uint32_t * triggerDivider);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Trigger_Burst_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Burst_GetRange)(peak_camera_handle hCam, uint32_t * minTriggerBurst, uint32_t * maxTriggerBurst, uint32_t * incTriggerBurst);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Burst_Set)(peak_camera_handle hCam, uint32_t triggerBurst);
typedef PEAK_API_STATUS (*dyn_peak_Trigger_Burst_Get)(peak_camera_handle hCam, uint32_t * triggerBurst);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Flash_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Flash_Enable)(peak_camera_handle hCam, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_Flash_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Flash_Mode_GetAccessStatus)(peak_camera_handle hCam, peak_flash_mode flashMode);
typedef PEAK_API_STATUS (*dyn_peak_Flash_Mode_Set)(peak_camera_handle hCam, peak_flash_mode flashMode);
typedef  (*dyn_PEAK_API_STATUS_DEPRECATED)("Use peak_Flash_Mode_Config_Get instead") peak_Flash_Mode_Get(peak_camera_handlehCam,peak_flash_mode*flashMode);
typedef PEAK_API_STATUS (*dyn_peak_Flash_Mode_Config_Get)(peak_camera_handle hCam, peak_flash_mode * flashMode);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Flash_StartDelay_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Flash_StartDelay_GetRange)(peak_camera_handle hCam, double * minFlashStartDelay_us, double * maxFlashStartDelay_us, double * incFlashStartDelay_us);
typedef PEAK_API_STATUS (*dyn_peak_Flash_StartDelay_Set)(peak_camera_handle hCam, double flashStartDelay_us);
typedef PEAK_API_STATUS (*dyn_peak_Flash_StartDelay_Get)(peak_camera_handle hCam, double * flashStartDelay_us);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Flash_EndDelay_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Flash_EndDelay_GetRange)(peak_camera_handle hCam, double * minFlashEndDelay_us, double * maxFlashEndDelay_us, double * incFlashEndDelay_us);
typedef PEAK_API_STATUS (*dyn_peak_Flash_EndDelay_Set)(peak_camera_handle hCam, double flashEndDelay_us);
typedef PEAK_API_STATUS (*dyn_peak_Flash_EndDelay_Get)(peak_camera_handle hCam, double * flashEndDelay_us);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Flash_Duration_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Flash_Duration_GetRange)(peak_camera_handle hCam, double * minFlashDuration_us, double * maxFlashDuration_us, double * incFlashDuration_us);
typedef PEAK_API_STATUS (*dyn_peak_Flash_Duration_Set)(peak_camera_handle hCam, double flashDuration_us);
typedef PEAK_API_STATUS (*dyn_peak_Flash_Duration_Get)(peak_camera_handle hCam, double * flashDuration_us);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Focus_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Focus_GetRange)(peak_camera_handle hCam, uint32_t * minFocus, uint32_t * maxFocus, uint32_t * incFocus);
typedef PEAK_API_STATUS (*dyn_peak_Focus_Set)(peak_camera_handle hCam, uint32_t focus);
typedef PEAK_API_STATUS (*dyn_peak_Focus_Get)(peak_camera_handle hCam, uint32_t * focus);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_PixelFormat_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_PixelFormat_GetList)(peak_camera_handle hCam, peak_pixel_format * pixelFormatList, size_t * pixelFormatCount);
typedef PEAK_API_STATUS (*dyn_peak_PixelFormat_Set)(peak_camera_handle hCam, peak_pixel_format pixelFormat);
typedef PEAK_API_STATUS (*dyn_peak_PixelFormat_Get)(peak_camera_handle hCam, peak_pixel_format * pixelFormat);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Gain_GetAccessStatus)(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel);
typedef PEAK_API_STATUS (*dyn_peak_Gain_GetChannelList)(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel * gainChannelList, size_t * gainChannelCount);
typedef PEAK_API_STATUS (*dyn_peak_Gain_GetRange)(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double * minGain, double * maxGain, double * incGain);
typedef PEAK_API_STATUS (*dyn_peak_Gain_Set)(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double gain);
typedef PEAK_API_STATUS (*dyn_peak_Gain_Get)(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double * gain);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Gamma_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Gamma_GetRange)(peak_camera_handle hCam, double * minGamma, double * maxGamma, double * incGamma);
typedef PEAK_API_STATUS (*dyn_peak_Gamma_Set)(peak_camera_handle hCam, double gamma);
typedef PEAK_API_STATUS (*dyn_peak_Gamma_Get)(peak_camera_handle hCam, double * gamma);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_ColorCorrection_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_ColorCorrection_Mode_GetList)(peak_camera_handle hCam, peak_color_correction_mode * colorCorrectionModeList, size_t * colorCorrectionModeCount);
typedef PEAK_API_STATUS (*dyn_peak_ColorCorrection_Mode_Set)(peak_camera_handle hCam, peak_color_correction_mode colorCorrectionMode);
typedef PEAK_API_STATUS (*dyn_peak_ColorCorrection_Mode_Get)(peak_camera_handle hCam, peak_color_correction_mode * colorCorrectionMode);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_ColorCorrection_Matrix_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_ColorCorrection_Matrix_GetRange)(peak_camera_handle hCam, double * minMatrixElementValue, double * maxMatrixElementValue, double * incMatrixElementValue);
typedef PEAK_API_STATUS (*dyn_peak_ColorCorrection_Matrix_Set)(peak_camera_handle hCam, peak_matrix colorCorrectionMatrix);
typedef PEAK_API_STATUS (*dyn_peak_ColorCorrection_Matrix_Get)(peak_camera_handle hCam, peak_matrix * colorCorrectionMatrix);
typedef PEAK_API_STATUS (*dyn_peak_ColorCorrection_Enable)(peak_camera_handle hCam, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_ColorCorrection_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_AutoBrightness_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_AutoBrightness_Target_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_Target_GetRange)(peak_camera_handle hCam, uint32_t * minAutoBrightnessTarget, uint32_t * maxAutoBrightnessTarget, uint32_t * incAutoBrightnessTarget);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_Target_Set)(peak_camera_handle hCam, uint32_t autoBrightnessTarget);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_Target_Get)(peak_camera_handle hCam, uint32_t * autoBrightnessTarget);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_AutoBrightness_TargetTolerance_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_TargetTolerance_GetRange)(peak_camera_handle hCam, uint32_t * minAutoBrightnessTargetTolerance, uint32_t * maxAutoBrightnessTargetTolerance, uint32_t * incAutoBrightnessTargetTolerance);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_TargetTolerance_Set)(peak_camera_handle hCam, uint32_t autoBrightnessTargetTolerance);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_TargetTolerance_Get)(peak_camera_handle hCam, uint32_t * autoBrightnessTargetTolerance);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_AutoBrightness_TargetPercentile_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_TargetPercentile_GetRange)(peak_camera_handle hCam, double * minAutoBrightnessTargetPercentile, double * maxAutoBrightnessTargetPercentile, double * incAutoBrightnessTargetPercentile);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_TargetPercentile_Set)(peak_camera_handle hCam, double autoBrightnessTargetPercentile);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_TargetPercentile_Get)(peak_camera_handle hCam, double * autoBrightnessTargetPercentile);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_AutoBrightness_ROI_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_ROI_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_roi_mode autoBrightnessROIMode);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_ROI_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_roi_mode * autoBrightnessROIMode);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_ROI_Offset_GetRange)(peak_camera_handle hCam, peak_position * minAutoBrightnessROIOffset, peak_position * maxAutoBrightnessROIOffset, peak_position * incAutoBrightnessROIOffset);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_ROI_Size_GetRange)(peak_camera_handle hCam, peak_size * minAutoBrightnessROISize, peak_size * maxAutoBrightnessROISize, peak_size * incAutoBrightnessROISize);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_ROI_Set)(peak_camera_handle hCam, peak_roi autoBrightnessROI);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_ROI_Get)(peak_camera_handle hCam, peak_roi * autoBrightnessROI);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_AutoBrightness_Exposure_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_Exposure_Mode_GetList)(peak_camera_handle hCam, peak_auto_feature_mode * modeList, size_t * modeListSize);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_Exposure_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_mode autoExposureMode);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_Exposure_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_mode * autoExposureMode);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_AutoBrightness_Gain_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_Gain_Mode_GetList)(peak_camera_handle hCam, peak_auto_feature_mode * modeList, size_t * modeListSize);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_Gain_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode);
typedef PEAK_API_STATUS (*dyn_peak_AutoBrightness_Gain_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_AutoWhiteBalance_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_AutoWhiteBalance_ROI_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_AutoWhiteBalance_ROI_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_roi_mode autoWhiteBalanceROIMode);
typedef PEAK_API_STATUS (*dyn_peak_AutoWhiteBalance_ROI_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_roi_mode * autoWhiteBalanceROIMode);
typedef PEAK_API_STATUS (*dyn_peak_AutoWhiteBalance_ROI_Offset_GetRange)(peak_camera_handle hCam, peak_position * minAutoWhiteBalanceROIOffset, peak_position * maxAutoWhiteBalanceROIOffset, peak_position * incAutoWhiteBalanceROIOffset);
typedef PEAK_API_STATUS (*dyn_peak_AutoWhiteBalance_ROI_Size_GetRange)(peak_camera_handle hCam, peak_size * minAutoWhiteBalanceROISize, peak_size * maxAutoWhiteBalanceROISize, peak_size * incAutoWhiteBalanceROISize);
typedef PEAK_API_STATUS (*dyn_peak_AutoWhiteBalance_ROI_Set)(peak_camera_handle hCam, peak_roi autoWhiteBalanceROI);
typedef PEAK_API_STATUS (*dyn_peak_AutoWhiteBalance_ROI_Get)(peak_camera_handle hCam, peak_roi * autoWhiteBalanceROI);
typedef PEAK_API_STATUS (*dyn_peak_AutoWhiteBalance_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_mode autoWhiteBalanceMode);
typedef PEAK_API_STATUS (*dyn_peak_AutoWhiteBalance_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_mode * autoWhiteBalanceMode);
typedef PEAK_API_STATUS (*dyn_peak_AutoWhiteBalance_Mode_GetList)(peak_camera_handle hCam, peak_auto_feature_mode * modeList, size_t * modeListSize);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_ROI_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_ROI_Offset_GetRange)(peak_camera_handle hCam, peak_position * minROIOffset, peak_position * maxROIOffset, peak_position * incROIOffset);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_ROI_Offset_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_ROI_Offset_Set)(peak_camera_handle hCam, peak_position position);
typedef PEAK_API_STATUS (*dyn_peak_ROI_Offset_Get)(peak_camera_handle hCam, peak_position * position);
typedef PEAK_API_STATUS (*dyn_peak_ROI_Size_GetRange)(peak_camera_handle hCam, peak_size * minROISize, peak_size * maxROISize, peak_size * incROISize);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_ROI_Size_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_ROI_Size_Set)(peak_camera_handle hCam, peak_size size);
typedef PEAK_API_STATUS (*dyn_peak_ROI_Size_Get)(peak_camera_handle hCam, peak_size * size);
typedef PEAK_API_STATUS (*dyn_peak_ROI_Set)(peak_camera_handle hCam, peak_roi roi);
typedef PEAK_API_STATUS (*dyn_peak_ROI_Get)(peak_camera_handle hCam, peak_roi * roi);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Binning_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Binning_FactorX_GetList)(peak_camera_handle hCam, uint32_t * binningFactorXList, size_t * binningFactorXCount);
typedef PEAK_API_STATUS (*dyn_peak_Binning_FactorY_GetList)(peak_camera_handle hCam, uint32_t * binningFactorYList, size_t * binningFactorYCount);
typedef PEAK_API_STATUS (*dyn_peak_Binning_Set)(peak_camera_handle hCam, uint32_t binningFactorX, uint32_t binningFactorY);
typedef PEAK_API_STATUS (*dyn_peak_Binning_Get)(peak_camera_handle hCam, uint32_t * binningFactorX, uint32_t * binningFactorY);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_BinningManual_GetAccessStatus)(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine);
typedef PEAK_API_STATUS (*dyn_peak_BinningManual_FactorX_GetList)(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * binningFactorXList, size_t * binningFactorXCount);
typedef PEAK_API_STATUS (*dyn_peak_BinningManual_FactorY_GetList)(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * binningFactorYList, size_t * binningFactorYCount);
typedef PEAK_API_STATUS (*dyn_peak_BinningManual_Set)(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t binningFactorX, uint32_t binningFactorY);
typedef PEAK_API_STATUS (*dyn_peak_BinningManual_Get)(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * binningFactorX, uint32_t * binningFactorY);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Decimation_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Decimation_FactorX_GetList)(peak_camera_handle hCam, uint32_t * decimationFactorXList, size_t * decimationFactorXCount);
typedef PEAK_API_STATUS (*dyn_peak_Decimation_FactorY_GetList)(peak_camera_handle hCam, uint32_t * decimationFactorYList, size_t * decimationFactorYCount);
typedef PEAK_API_STATUS (*dyn_peak_Decimation_Set)(peak_camera_handle hCam, uint32_t decimationFactorX, uint32_t decimationFactorY);
typedef PEAK_API_STATUS (*dyn_peak_Decimation_Get)(peak_camera_handle hCam, uint32_t * decimationFactorX, uint32_t * decimationFactorY);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_DecimationManual_GetAccessStatus)(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine);
typedef PEAK_API_STATUS (*dyn_peak_DecimationManual_FactorX_GetList)(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * decimationFactorXList, size_t * decimationFactorXCount);
typedef PEAK_API_STATUS (*dyn_peak_DecimationManual_FactorY_GetList)(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * decimationFactorYList, size_t * decimationFactorYCount);
typedef PEAK_API_STATUS (*dyn_peak_DecimationManual_Set)(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t decimationFactorX, uint32_t decimationFactorY);
typedef PEAK_API_STATUS (*dyn_peak_DecimationManual_Get)(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * decimationFactorX, uint32_t * decimationFactorY);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Scaling_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Scaling_FactorX_GetRange)(peak_camera_handle hCam, double * minScalingFactorX, double * maxScalingFactorX, double * incScalingFactorX);
typedef PEAK_API_STATUS (*dyn_peak_Scaling_FactorY_GetRange)(peak_camera_handle hCam, double * minScalingFactorY, double * maxScalingFactorY, double * incScalingFactorY);
typedef PEAK_API_STATUS (*dyn_peak_Scaling_Set)(peak_camera_handle hCam, double scalingFactorX, double scalingFactorY);
typedef PEAK_API_STATUS (*dyn_peak_Scaling_Get)(peak_camera_handle hCam, double * scalingFactorX, double * scalingFactorY);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Mirror_LeftRight_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Mirror_LeftRight_Enable)(peak_camera_handle hCam, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_Mirror_LeftRight_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Mirror_UpDown_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Mirror_UpDown_Enable)(peak_camera_handle hCam, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_Mirror_UpDown_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_CameraMemory_Area_GetAccessStatus)(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea);
typedef PEAK_API_STATUS (*dyn_peak_CameraMemory_Area_GetList)(peak_camera_handle hCam, peak_camera_memory_area * cameraMemoryAreaList, size_t * cameraMemoryAreaCount);
typedef PEAK_API_STATUS (*dyn_peak_CameraMemory_Area_Size_Get)(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea, size_t * cameraMemoryAreaSize);
typedef PEAK_API_STATUS (*dyn_peak_CameraMemory_Area_Data_Clear)(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea);
typedef PEAK_API_STATUS (*dyn_peak_CameraMemory_Area_Data_Write)(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea, const uint8_t * data, size_t dataSize);
typedef PEAK_API_STATUS (*dyn_peak_CameraMemory_Area_Data_Read)(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea, uint8_t * data, size_t dataSize);
typedef PEAK_API_STATUS (*dyn_peak_GFA_EnableWriteAccess)(peak_camera_handle hCam, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_GFA_IsWriteAccessEnabled)(peak_camera_handle hCam);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_GFA_Feature_GetAccessStatus)(peak_camera_handle hCam, peak_gfa_module module, const char * featureName);
typedef PEAK_API_BOOL (*dyn_peak_GFA_Float_HasRange)(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Float_GetRange)(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName, double * minFloatValue, double * maxFloatValue, double * incFloatValue);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Float_GetList)(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName, double * floatList, size_t * floatCount);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Float_Set)(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName, double floatValue);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Float_Get)(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName, double * floatValue);
typedef PEAK_API_BOOL (*dyn_peak_GFA_Integer_HasRange)(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Integer_GetRange)(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName, int64_t * minIntegerValue, int64_t * maxIntegerValue, int64_t * incIntegerValue);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Integer_GetList)(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName, int64_t * integerList, size_t * integerCount);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Integer_Set)(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName, int64_t integerValue);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Integer_Get)(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName, int64_t * integerValue);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Boolean_Set)(peak_camera_handle hCam, peak_gfa_module module, const char * booleanFeatureName, peak_bool booleanValue);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Boolean_Get)(peak_camera_handle hCam, peak_gfa_module module, const char * booleanFeatureName, peak_bool * booleanValue);
typedef PEAK_API_STATUS (*dyn_peak_GFA_String_Set)(peak_camera_handle hCam, peak_gfa_module module, const char * stringFeatureName, const char * stringValue);
typedef PEAK_API_STATUS (*dyn_peak_GFA_String_Get)(peak_camera_handle hCam, peak_gfa_module module, const char * stringFeatureName, char * stringValue, size_t * stringValueSize);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Command_Execute)(peak_camera_handle hCam, peak_gfa_module module, const char * commandFeatureName);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Command_WaitForDone)(peak_camera_handle hCam, peak_gfa_module module, const char * commandFeatureName, uint32_t timeout_ms);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Enumeration_GetList)(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, peak_gfa_enumeration_entry * enumerationEntryList, size_t * enumerationEntryCount);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_GFA_EnumerationEntry_GetAccessStatus)(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, const peak_gfa_enumeration_entry * enumerationEntry);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_GFA_EnumerationEntry_GetAccessStatusBySymbolicValue)(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, const char * enumerationEntrySymbolicValue);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_GFA_EnumerationEntry_GetAccessStatusByIntegerValue)(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, int64_t enumerationEntryIntegerValue);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Enumeration_Set)(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, const peak_gfa_enumeration_entry * enumerationEntry);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Enumeration_SetBySymbolicValue)(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, const char * enumerationEntrySymbolicValue);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Enumeration_SetByIntegerValue)(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, int64_t enumerationEntryIntegerValue);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Enumeration_Get)(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, peak_gfa_enumeration_entry * enumerationEntry);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Register_Set)(peak_camera_handle hCam, peak_gfa_module module, const char * registerFeatureName, const uint8_t * registerValue, size_t registerValueSize);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Register_Get)(peak_camera_handle hCam, peak_gfa_module module, const char * registerFeatureName, uint8_t * registerValue, size_t * registerValueSize);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Data_Write)(peak_camera_handle hCam, peak_gfa_module module, uint64_t address, const uint8_t * data, size_t dataSize);
typedef PEAK_API_STATUS (*dyn_peak_GFA_Data_Read)(peak_camera_handle hCam, peak_gfa_module module, uint64_t address, uint8_t * data, size_t dataSize);
typedef PEAK_API_STATUS (*dyn_peak_IPL_PixelFormat_GetList)(peak_camera_handle hCam, peak_pixel_format inputPixelFormat, peak_pixel_format * outputPixelFormatList, size_t * outputPixelFormatCount);
typedef PEAK_API_STATUS (*dyn_peak_IPL_PixelFormat_Set)(peak_camera_handle hCam, peak_pixel_format pixelFormat);
typedef PEAK_API_STATUS (*dyn_peak_IPL_PixelFormat_Get)(peak_camera_handle hCam, peak_pixel_format * pixelFormat);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Gain_GetRange)(peak_camera_handle hCam, peak_gain_channel gainChannel, double * minGain, double * maxGain, double * incGain);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Gain_Set)(peak_camera_handle hCam, peak_gain_channel gainChannel, double gain);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Gain_Get)(peak_camera_handle hCam, peak_gain_channel gainChannel, double * gain);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Gamma_GetRange)(peak_camera_handle hCam, double * minGamma, double * maxGamma, double * incGamma);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Gamma_Set)(peak_camera_handle hCam, double gamma);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Gamma_Get)(peak_camera_handle hCam, double * gamma);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ColorCorrection_Matrix_Set)(peak_camera_handle hCam, peak_matrix colorCorrectionMatrix);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ColorCorrection_Matrix_Get)(peak_camera_handle hCam, peak_matrix * colorCorrectionMatrix);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ColorCorrection_Saturation_Get)(peak_camera_handle hCam, double * saturation);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ColorCorrection_Saturation_Set)(peak_camera_handle hCam, double saturation);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ColorCorrection_Saturation_GetRange)(peak_camera_handle hCam, double * minSaturation, double * maxSaturation, double * incSaturation);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Get)(peak_camera_handle hCam, peak_chromatic_adaption_color_space * colorSpace);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Set)(peak_camera_handle hCam, peak_chromatic_adaption_color_space colorSpace);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Get)(peak_camera_handle hCam, peak_chromatic_adaption_algorithm * algorithm);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Set)(peak_camera_handle hCam, peak_chromatic_adaption_algorithm algorithm);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Get)(peak_camera_handle hCam, uint32_t * colorTemperature);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Set)(peak_camera_handle hCam, uint32_t colorTemperature);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_GetRange)(peak_camera_handle hCam, uint32_t * minColorTemperature, uint32_t * maxColorTemperature, uint32_t * incColorTemperature);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ColorCorrection_ChromaticAdaption_Enable)(peak_camera_handle hCam, peak_bool enable);
typedef PEAK_API_BOOL (*dyn_peak_IPL_ColorCorrection_ChromaticAdaption_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ColorCorrection_Enable)(peak_camera_handle hCam, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_IPL_ColorCorrection_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_Target_GetRange)(peak_camera_handle hCam, uint32_t * minAutoBrightnessTarget, uint32_t * maxAutoBrightnessTarget, uint32_t * incAutoBrightnessTarget);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_Target_Set)(peak_camera_handle hCam, uint32_t autoBrightnessTarget);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_Target_Get)(peak_camera_handle hCam, uint32_t * autoBrightnessTarget);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_TargetTolerance_GetRange)(peak_camera_handle hCam, uint32_t * minAutoBrightnessTargetTolerance, uint32_t * maxAutoBrightnessTargetTolerance, uint32_t * incAutoBrightnessTargetTolerance);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_TargetTolerance_Set)(peak_camera_handle hCam, uint32_t autoBrightnessTargetTolerance);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_TargetTolerance_Get)(peak_camera_handle hCam, uint32_t * autoBrightnessTargetTolerance);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_TargetPercentile_GetRange)(peak_camera_handle hCam, double * minAutoBrightnessTargetPercentile, double * maxAutoBrightnessTargetPercentile, double * incAutoBrightnessTargetPercentile);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_TargetPercentile_Set)(peak_camera_handle hCam, double autoBrightnessTargetPercentile);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_TargetPercentile_Get)(peak_camera_handle hCam, double * autoBrightnessTargetPercentile);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_ROI_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_roi_mode autoBrightnessROIMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_ROI_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_roi_mode * autoBrightnessROIMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_ROI_Offset_GetRange)(peak_camera_handle hCam, peak_position * minAutoBrightnessROIOffset, peak_position * maxAutoBrightnessROIOffset, peak_position * incAutoBrightnessROIOffset);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_ROI_Size_GetRange)(peak_camera_handle hCam, peak_size * minAutoBrightnessROISize, peak_size * maxAutoBrightnessROISize, peak_size * incAutoBrightnessROISize);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_ROI_Set)(peak_camera_handle hCam, peak_roi autoBrightnessROI);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_ROI_Get)(peak_camera_handle hCam, peak_roi * autoBrightnessROI);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_Exposure_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_mode autoExposureMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_Exposure_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_mode * autoExposureMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_ExposureLimit_Get)(peak_camera_handle hCam, peak_double_limit * exposureLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_ExposureLimit_Set)(peak_camera_handle hCam, peak_double_limit exposureLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_Gain_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_Gain_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainAnalog_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainAnalog_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainDigital_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainDigital_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainCombined_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainCombined_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainHost_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainHost_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_Algorithm_Set)(peak_camera_handle hCam, peak_auto_feature_brightness_algorithm algorithm);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_Algorithm_Get)(peak_camera_handle hCam, peak_auto_feature_brightness_algorithm * algorithm);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_AverageLast_Get)(peak_camera_handle hCam, uint32_t * lastAverage);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainLimit_Set)(peak_camera_handle hCam, peak_double_limit gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainLimit_Get)(peak_camera_handle hCam, peak_double_limit * gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainLimit_GetRange)(peak_camera_handle hCam, peak_double_limit * gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainAnalogLimit_Set)(peak_camera_handle hCam, peak_double_limit gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainAnalogLimit_Get)(peak_camera_handle hCam, peak_double_limit * gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainAnalogLimit_GetRange)(peak_camera_handle hCam, peak_double_limit * gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainDigitalLimit_Set)(peak_camera_handle hCam, peak_double_limit gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainDigitalLimit_Get)(peak_camera_handle hCam, peak_double_limit * gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainDigitalLimit_GetRange)(peak_camera_handle hCam, peak_double_limit * gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainCombinedLimit_Set)(peak_camera_handle hCam, peak_double_limit gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainCombinedLimit_Get)(peak_camera_handle hCam, peak_double_limit * gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainCombinedLimit_GetRange)(peak_camera_handle hCam, peak_double_limit * gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainHostLimit_Set)(peak_camera_handle hCam, peak_double_limit gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainHostLimit_Get)(peak_camera_handle hCam, peak_double_limit * gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_GainHostLimit_GetRange)(peak_camera_handle hCam, peak_double_limit * gainLimit);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_SkipFrames_Set)(peak_camera_handle hCam, uint32_t skipFrames);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_SkipFrames_Get)(peak_camera_handle hCam, uint32_t * skipFrames);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoBrightness_SkipFrames_GetRange)(peak_camera_handle hCam, uint32_t * skipFramesMin, uint32_t * skipFramesMax, uint32_t * skipFramesInc);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoWhiteBalance_ROI_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_roi_mode autoWhiteBalanceROIMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoWhiteBalance_ROI_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_roi_mode * autoWhiteBalanceROIMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoWhiteBalance_ROI_Offset_GetRange)(peak_camera_handle hCam, peak_position * minAutoWhiteBalanceROIOffset, peak_position * maxAutoWhiteBalanceROIOffset, peak_position * incAutoWhiteBalanceROIOffset);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoWhiteBalance_ROI_Size_GetRange)(peak_camera_handle hCam, peak_size * minAutoWhiteBalanceROISize, peak_size * maxAutoWhiteBalanceROISize, peak_size * incAutoWhiteBalanceROISize);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoWhiteBalance_ROI_Set)(peak_camera_handle hCam, peak_roi autoWhiteBalanceROI);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoWhiteBalance_ROI_Get)(peak_camera_handle hCam, peak_roi * autoWhiteBalanceROI);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoWhiteBalance_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_mode autoWhiteBalanceMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoWhiteBalance_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_mode * autoWhiteBalanceMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoWhiteBalance_SkipFrames_Set)(peak_camera_handle hCam, uint32_t skipFrames);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoWhiteBalance_SkipFrames_Get)(peak_camera_handle hCam, uint32_t * skipFrames);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoWhiteBalance_SkipFrames_GetRange)(peak_camera_handle hCam, uint32_t * skipFramesMin, uint32_t * skipFramesMax, uint32_t * skipFramesInc);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_IPL_AutoFocus_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoFocus_ROI_Set)(peak_camera_handle hCam, const peak_focus_roi * autoFocusROIList, size_t autoFocusROICount);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoFocus_ROI_Get)(peak_camera_handle hCam, peak_focus_roi * autoFocusROIList, size_t * autoFocusROICount);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoFocus_Mode_Set)(peak_camera_handle hCam, peak_auto_feature_mode autoFocusMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoFocus_Mode_Get)(peak_camera_handle hCam, peak_auto_feature_mode * autoFocusMode);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoFocus_SearchAlgorithm_Set)(peak_camera_handle hCam, peak_auto_focus_search_algorithm searchAlgorithm);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoFocus_SearchAlgorithm_Get)(peak_camera_handle hCam, peak_auto_focus_search_algorithm * searchAlgorithm);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoFocus_SharpnessAlgorithm_Set)(peak_camera_handle hCam, peak_sharpness_algorithm sharpnessAlgorithm);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoFocus_SharpnessAlgorithm_Get)(peak_camera_handle hCam, peak_sharpness_algorithm * sharpnessAlgorithm);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoFocus_Range_Set)(peak_camera_handle hCam, uint32_t rangeBegin, uint32_t rangeEnd);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoFocus_Range_Get)(peak_camera_handle hCam, uint32_t * rangeBegin, uint32_t * rangeEnd);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoFocus_Hysteresis_Set)(peak_camera_handle hCam, uint8_t hysteresis);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoFocus_Hysteresis_Get)(peak_camera_handle hCam, uint8_t * hysteresis);
typedef PEAK_API_STATUS (*dyn_peak_IPL_AutoFocus_Hysteresis_GetRange)(peak_camera_handle hCam, uint8_t * minHysteresis, uint8_t * maxHysteresis, uint8_t * incHysteresis);
typedef PEAK_API_STATUS (*dyn_peak_IPL_HotpixelCorrection_Sensitivity_Set)(peak_camera_handle hCam, peak_hotpixel_correction_sensitivity hotpixelCorrectionSensitivity);
typedef PEAK_API_STATUS (*dyn_peak_IPL_HotpixelCorrection_Sensitivity_Get)(peak_camera_handle hCam, peak_hotpixel_correction_sensitivity * hotpixelCorrectionSensitivity);
typedef PEAK_API_STATUS (*dyn_peak_IPL_HotpixelCorrection_GetList)(peak_camera_handle hCam, peak_position * hotpixelList, size_t * hotpixelCount);
typedef PEAK_API_STATUS (*dyn_peak_IPL_HotpixelCorrection_SetList)(peak_camera_handle hCam, const peak_position * hotpixelList, size_t hotpixelCount);
typedef PEAK_API_STATUS (*dyn_peak_IPL_HotpixelCorrection_ResetList)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_IPL_HotpixelCorrection_Enable)(peak_camera_handle hCam, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_IPL_HotpixelCorrection_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Mirror_UpDown_Enable)(peak_camera_handle hCam, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_IPL_Mirror_UpDown_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Mirror_LeftRight_Enable)(peak_camera_handle hCam, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_IPL_Mirror_LeftRight_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ProcessFrame)(peak_camera_handle hCam, peak_frame_handle hFrame, peak_frame_handle * hResultFrame);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ProcessFrameInplace)(peak_camera_handle hCam, peak_frame_handle hFrame);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ReadImage)(peak_camera_handle hCam, const char * path, peak_frame_handle * hFrame);
typedef PEAK_API_STATUS (*dyn_peak_IPL_EdgeEnhancement_Enable)(peak_camera_handle hCam, peak_bool enable);
typedef PEAK_API_BOOL (*dyn_peak_IPL_EdgeEnhancement_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_IPL_EdgeEnhancement_Factor_Set)(peak_camera_handle hCam, uint32_t factor);
typedef PEAK_API_STATUS (*dyn_peak_IPL_EdgeEnhancement_Factor_Get)(peak_camera_handle hCam, uint32_t * factor);
typedef PEAK_API_STATUS (*dyn_peak_IPL_EdgeEnhancement_Factor_GetDefault)(peak_camera_handle hCam, uint32_t * defaultFactor);
typedef PEAK_API_STATUS (*dyn_peak_IPL_EdgeEnhancement_Factor_GetRange)(peak_camera_handle hCam, uint32_t * minFactor, uint32_t * maxFactor, uint32_t * incFactor);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Sharpness_Measure)(peak_frame_handle hFrame, peak_roi roi, peak_sharpness_algorithm sharpnessAlgorithm, double * calculatedValue);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Sharpness_GetList)(peak_sharpness_algorithm sharpnessAlgorithm, peak_pixel_format * pixelFormatList, size_t * pixelFormatSize);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Rotation_Angle_Set)(peak_camera_handle hCam, int32_t rotationAngle);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Rotation_Angle_Get)(peak_camera_handle hCam, int32_t * rotationAngle);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Histogram_ProcessFrame)(peak_frame_handle hFrame, peak_histogram_handle * hHistogram);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Histogram_Release)(peak_histogram_handle hHistogram);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Histogram_Channel_GetCount)(peak_histogram_handle hHistogram, size_t * numChannels);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Histogram_Channel_GetInfo)(peak_histogram_handle hHistogram, size_t channel, peak_histogram_channel_info * channelInfo);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Histogram_Channel_GetBinArray)(peak_histogram_handle hHistogram, size_t channel, uint64_t * binArray, size_t * binArraySize);
typedef PEAK_API_STATUS (*dyn_peak_VideoWriter_Open)(peak_video_handle * hVideo, const char * fileName, peak_video_container container, peak_video_encoder encoder);
typedef PEAK_API_STATUS (*dyn_peak_VideoWriter_Close)(peak_video_handle hVideo);
typedef PEAK_API_STATUS (*dyn_peak_VideoWriter_AddFrame)(peak_video_handle hVideo, peak_frame_handle hFrame);
typedef PEAK_API_STATUS (*dyn_peak_VideoWriter_Container_GetEncoderList)(peak_video_container container, peak_video_encoder * encoderList, size_t * encoderCount);
typedef PEAK_API_STATUS (*dyn_peak_VideoWriter_Encoder_GetPixelFormatList)(peak_video_encoder encoder, peak_pixel_format * pixelFormatList, size_t * pixelFormatCount);
typedef PEAK_API_STATUS (*dyn_peak_VideoWriter_Encoder_GetContainerList)(peak_video_encoder encoder, peak_video_container * containerList, size_t * containerCount);
typedef PEAK_API_STATUS (*dyn_peak_VideoWriter_GetInfo)(peak_video_handle hVideo, peak_video_info * videoInfo);
typedef PEAK_API_STATUS (*dyn_peak_VideoWriter_Container_Option_Set)(peak_video_handle hVideo, peak_video_container_option containerOption, const void * value, size_t count);
typedef PEAK_API_STATUS (*dyn_peak_VideoWriter_Container_Option_Get)(peak_video_handle hVideo, peak_video_container_option containerOption, void * value, size_t count, size_t * outCount);
typedef PEAK_API_STATUS (*dyn_peak_VideoWriter_Encoder_Option_Set)(peak_video_handle hVideo, peak_video_encoder_option encoderOption, const void * value, size_t count);
typedef PEAK_API_STATUS (*dyn_peak_VideoWriter_Encoder_Option_Get)(peak_video_handle hVideo, peak_video_encoder_option encoderOption, void * value, size_t count, size_t * outCount);
typedef PEAK_API_STATUS (*dyn_peak_VideoWriter_WaitUntilQueueEmpty)(peak_video_handle hVideo, int32_t timeout_ms);
typedef PEAK_API_STATUS (*dyn_peak_Inference_CNN_Open)(const char * path, peak_inference_handle * hInference);
typedef PEAK_API_STATUS (*dyn_peak_Inference_CNN_Close)(peak_inference_handle hInference);
typedef PEAK_API_STATUS (*dyn_peak_Inference_CNN_ProcessFrame)(peak_inference_handle hInference, peak_frame_handle hFrame, peak_inference_result_handle * hInferenceHandle);
typedef PEAK_API_STATUS (*dyn_peak_Inference_CNN_Info_Get)(peak_inference_handle hInference, peak_inference_info * info);
typedef PEAK_API_STATUS (*dyn_peak_Inference_Result_Get)(peak_inference_result_handle hInferenceHandle, peak_inference_result_data * result);
typedef PEAK_API_STATUS (*dyn_peak_Inference_Result_Release)(peak_inference_result_handle hInferenceHandle);
typedef PEAK_API_STATUS (*dyn_peak_Inference_Result_Classification_GetList)(peak_inference_result_handle hInferenceHandle, peak_inference_result_classification * resultList, size_t * resultCount);
typedef PEAK_API_STATUS (*dyn_peak_Inference_Result_Detection_GetList)(peak_inference_result_handle hInferenceHandle, peak_inference_result_detection * resultList, size_t * resultCount);
typedef PEAK_API_STATUS (*dyn_peak_Inference_Statistics_Get)(peak_inference_handle hInference, peak_inference_statistics * statistics);
typedef PEAK_API_STATUS (*dyn_peak_Inference_Statistics_Reset)(peak_inference_handle hInference);
typedef PEAK_API_STATUS (*dyn_peak_Inference_ConfidenceThreshold_Get)(peak_inference_handle hInference, uint32_t * threshold);
typedef PEAK_API_STATUS (*dyn_peak_Inference_ConfidenceThreshold_GetRange)(peak_inference_handle hInference, uint32_t * minThreshold, uint32_t * maxThreshold, uint32_t * incThreshold);
typedef PEAK_API_STATUS (*dyn_peak_Inference_ConfidenceThreshold_Set)(peak_inference_handle hInference, uint32_t threshold);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_Create)(peak_message_queue_handle * hMessageQueue);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_Destroy)(peak_message_queue_handle hMessageQueue);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_EnableMessage)(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type messageType);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_DisableMessage)(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type messageType);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_EnableMessageList)(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, const peak_message_type * messageTypesArray, size_t messageTypesArraySize);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_DisableMessageList)(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, const peak_message_type * messageTypesArray, size_t messageTypesArraySize);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_EnabledMessages_GetList)(peak_message_queue_handle hMessageQueue, peak_message_type * messageTypesArray, size_t * messageTypesArraySize);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_SetMode)(peak_message_queue_handle hMessageQueue, peak_message_queue_mode queueMode);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_GetMode)(peak_message_queue_handle hMessageQueue, peak_message_queue_mode * queueMode);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_Start)(peak_message_queue_handle hMessageQueue);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_Stop)(peak_message_queue_handle hMessageQueue);
typedef PEAK_API_BOOL (*dyn_peak_MessageQueue_IsStarted)(peak_message_queue_handle hMessageQueue);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_WaitForMessage)(peak_message_queue_handle hMessageQueue, uint32_t timeout_ms, peak_message_handle * hMessage);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_Flush)(peak_message_queue_handle hMessageQueue);
typedef PEAK_API_BOOL (*dyn_peak_MessageQueue_IsMessageSupported)(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type messageType);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_Statistics_Get)(peak_message_queue_handle hMessageQueue, peak_message_queue_statistics_info * messageQueueInfo);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_Statistics_Reset)(peak_message_queue_handle hMessageQueue);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_MaxQueueSize_Set)(peak_message_queue_handle hMessageQueue, size_t messageQueueMaxSize);
typedef PEAK_API_STATUS (*dyn_peak_MessageQueue_MaxQueueSize_Get)(peak_message_queue_handle hMessageQueue, size_t * messageQueueMaxSize);
typedef PEAK_API_STATUS (*dyn_peak_Message_Release)(peak_message_handle hMessage);
typedef PEAK_API_STATUS (*dyn_peak_Message_GetInfo)(peak_message_handle hMessage, peak_message_info * messageInfo);
typedef PEAK_API_STATUS (*dyn_peak_Message_Type_Get)(peak_message_handle hMessage, peak_message_type * messageType);
typedef PEAK_API_STATUS (*dyn_peak_Message_CameraHandle_Get)(peak_message_handle hMessage, peak_camera_handle * hCam);
typedef PEAK_API_STATUS (*dyn_peak_Message_ID_Get)(peak_message_handle hMessage, uint64_t * messageID);
typedef PEAK_API_STATUS (*dyn_peak_Message_HostTimestamp_Get)(peak_message_handle hMessage, uint64_t * hostTimestamp_ns);
typedef PEAK_API_STATUS (*dyn_peak_Message_Data_Type_Get)(peak_message_handle hMessage, peak_message_data_type * messageType);
typedef PEAK_API_STATUS (*dyn_peak_Message_Data_RemoteDevice_Get)(peak_message_handle hMessage, peak_message_data_remote_device * message);
typedef PEAK_API_STATUS (*dyn_peak_Message_Data_RemoteDeviceError_Get)(peak_message_handle hMessage, peak_message_data_remote_device_error * message);
typedef PEAK_API_STATUS (*dyn_peak_Message_Data_RemoteDeviceDropped_Get)(peak_message_handle hMessage, peak_message_data_remote_device_dropped * message);
typedef PEAK_API_STATUS (*dyn_peak_Message_Data_RemoteDeviceFrame_Get)(peak_message_handle hMessage, peak_message_data_remote_device_frame * message);
typedef PEAK_API_STATUS (*dyn_peak_Message_Data_RemoteDeviceTemperature_Get)(peak_message_handle hMessage, peak_message_data_remote_device_temperature * message);
typedef PEAK_API_STATUS (*dyn_peak_Message_Data_AutoFocusData_Get)(peak_message_handle hMessage, peak_message_data_autofocus * message);
typedef PEAK_API_STATUS (*dyn_peak_Message_Data_DeviceFound_Get)(peak_message_handle hMessage, peak_message_data_device_found * message);
typedef PEAK_API_STATUS (*dyn_peak_Message_Data_DeviceLost_Get)(peak_message_handle hMessage, peak_message_data_device_lost * message);
typedef PEAK_API_STATUS (*dyn_peak_Message_Data_DeviceReconnected_Get)(peak_message_handle hMessage, peak_message_data_device_reconnected * message);
typedef PEAK_API_STATUS (*dyn_peak_Message_Data_DeviceDisconnected_Get)(peak_message_handle hMessage, peak_message_data_device_disconnected * message);
typedef PEAK_API_STATUS (*dyn_peak_Message_Data_FirmwareUpdate_Get)(peak_message_handle hMessage, peak_message_data_firmware_update * message);
typedef PEAK_API_STATUS (*dyn_peak_I2C_Create)(peak_camera_handle hCam, peak_i2c_handle * hI2C);
typedef PEAK_API_STATUS (*dyn_peak_I2C_Destroy)(peak_i2c_handle hI2C);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_I2C_GetAccessStatus)(peak_i2c_handle hI2C);
typedef PEAK_API_STATUS (*dyn_peak_I2C_Mode_GetList)(peak_i2c_handle hI2C, peak_i2c_mode * modeList, size_t * modeListSize);
typedef PEAK_API_STATUS (*dyn_peak_I2C_Mode_Set)(peak_i2c_handle hI2C, peak_i2c_mode mode);
typedef PEAK_API_STATUS (*dyn_peak_I2C_Mode_Get)(peak_i2c_handle hI2C, peak_i2c_mode * mode);
typedef PEAK_API_STATUS (*dyn_peak_I2C_DeviceAddress_GetRange)(peak_i2c_handle hI2C, uint32_t * minAddress, uint32_t * maxAddress);
typedef PEAK_API_STATUS (*dyn_peak_I2C_DeviceAddress_Set)(peak_i2c_handle hI2C, uint32_t address);
typedef PEAK_API_STATUS (*dyn_peak_I2C_DeviceAddress_Get)(peak_i2c_handle hI2C, uint32_t * address);
typedef PEAK_API_STATUS (*dyn_peak_I2C_RegisterAddress_Length_GetList)(peak_i2c_handle hI2C, peak_i2c_register_address_length * i2cLengthList, size_t * i2cLengthCount);
typedef PEAK_API_STATUS (*dyn_peak_I2C_RegisterAddress_Length_Set)(peak_i2c_handle hI2C, peak_i2c_register_address_length length);
typedef PEAK_API_STATUS (*dyn_peak_I2C_RegisterAddress_Length_Get)(peak_i2c_handle hI2C, peak_i2c_register_address_length * length);
typedef PEAK_API_STATUS (*dyn_peak_I2C_RegisterAddress_Endianness_Set)(peak_i2c_handle hI2C, peak_endianness endianness);
typedef PEAK_API_STATUS (*dyn_peak_I2C_RegisterAddress_Endianness_Get)(peak_i2c_handle hI2C, peak_endianness * endianness);
typedef PEAK_API_STATUS (*dyn_peak_I2C_RegisterAddress_Set)(peak_i2c_handle hI2C, uint32_t address);
typedef PEAK_API_STATUS (*dyn_peak_I2C_RegisterAddress_Get)(peak_i2c_handle hI2C, uint32_t * address);
typedef PEAK_API_STATUS (*dyn_peak_I2C_AckPolling_Enable)(peak_i2c_handle hI2C, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_I2C_AckPolling_IsEnabled)(peak_i2c_handle hI2C);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_I2C_AckPolling_Timeout_GetAccessStatus)(peak_i2c_handle hI2C);
typedef PEAK_API_STATUS (*dyn_peak_I2C_AckPolling_Timeout_GetRange)(peak_i2c_handle hI2C, uint32_t * minTimeout_ms, uint32_t * maxTimeout_ms, uint32_t * incTimeout_ms);
typedef PEAK_API_STATUS (*dyn_peak_I2C_AckPolling_Timeout_Set)(peak_i2c_handle hI2C, uint32_t timeout_ms);
typedef PEAK_API_STATUS (*dyn_peak_I2C_AckPolling_Timeout_Get)(peak_i2c_handle hI2C, uint32_t * timeout_ms);
typedef PEAK_API_STATUS (*dyn_peak_I2C_Data_Write)(peak_i2c_handle hI2C, const uint8_t * data, size_t dataSize);
typedef PEAK_API_STATUS (*dyn_peak_I2C_Data_Read)(peak_i2c_handle hI2C, size_t maxDataSize, uint8_t * data, size_t * dataSize);
typedef PEAK_API_STATUS (*dyn_peak_I2C_Data_MaxSize_Get)(peak_i2c_handle hI2C, size_t * maxDataSize);
typedef PEAK_API_STATUS (*dyn_peak_I2C_OperationStatus_Get)(peak_i2c_handle hI2C, peak_i2c_operation_status * operationStatus);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ImageWriter_Create)(peak_imagewriter_handle * hImageWriter);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ImageWriter_Destroy)(peak_imagewriter_handle hImageWriter);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ImageWriter_Save)(peak_imagewriter_handle hImageWriter, peak_frame_handle hFrame, const char * fileName);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ImageWriter_Format_Set)(peak_imagewriter_handle hImageWriter, peak_imagefile_format imageFormat);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ImageWriter_Format_Get)(peak_imagewriter_handle hImageWriter, peak_imagefile_format * imageFormat);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ImageWriter_Compression_Set)(peak_imagewriter_handle hImageWriter, uint32_t compression);
typedef PEAK_API_STATUS (*dyn_peak_IPL_ImageWriter_Compression_Get)(peak_imagewriter_handle hImageWriter, uint32_t * compression);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Binning_FactorX_GetList)(peak_camera_handle hCam, uint32_t * binningFactorXList, size_t * binningFactorXCount);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Binning_FactorY_GetList)(peak_camera_handle hCam, uint32_t * binningFactorYList, size_t * binningFactorYCount);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Binning_Set)(peak_camera_handle hCam, uint32_t binningFactorX, uint32_t binningFactorY);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Binning_Get)(peak_camera_handle hCam, uint32_t * binningFactorX, uint32_t * binningFactorY);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Decimation_FactorX_GetList)(peak_camera_handle hCam, uint32_t * decimationFactorXList, size_t * decimationFactorXCount);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Decimation_FactorY_GetList)(peak_camera_handle hCam, uint32_t * decimationFactorYList, size_t * decimationFactorYCount);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Decimation_Set)(peak_camera_handle hCam, uint32_t decimationFactorX, uint32_t decimationFactorY);
typedef PEAK_API_STATUS (*dyn_peak_IPL_Decimation_Get)(peak_camera_handle hCam, uint32_t * decimationFactorX, uint32_t * decimationFactorY);
typedef PEAK_API_STATUS (*dyn_peak_FirmwareUpdate_CompatibleCameraList_Get)(const char * gufFileName, peak_camera_descriptor * cameraList, size_t * cameraCount);
typedef PEAK_API_STATUS (*dyn_peak_FirmwareUpdate_Execute)(peak_camera_id cameraID, const char * gufFileName);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_TestPattern_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_TestPattern_Set)(peak_camera_handle hCam, peak_test_pattern pattern);
typedef PEAK_API_STATUS (*dyn_peak_TestPattern_Get)(peak_camera_handle hCam, peak_test_pattern * pattern);
typedef PEAK_API_STATUS (*dyn_peak_TestPattern_GetList)(peak_camera_handle hCam, peak_test_pattern * testPatternList, size_t * testPatternCount);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_LED_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_LED_Target_GetList)(peak_camera_handle hCam, peak_led_target * targetList, size_t * targetCount);
typedef PEAK_API_STATUS (*dyn_peak_LED_Mode_GetList)(peak_camera_handle hCam, peak_led_target target, peak_led_mode * modeList, size_t * ledModeCount);
typedef PEAK_API_STATUS (*dyn_peak_LED_Set)(peak_camera_handle hCam, peak_led_target target, peak_led_mode mode);
typedef PEAK_API_STATUS (*dyn_peak_LED_Get)(peak_camera_handle hCam, peak_led_target target, peak_led_mode * mode);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_BlackLevel_Auto_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_BlackLevel_Auto_Enable)(peak_camera_handle hCam, peak_bool enable);
typedef PEAK_API_BOOL (*dyn_peak_BlackLevel_Auto_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_BlackLevel_Offset_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_BlackLevel_Offset_Set)(peak_camera_handle hCam, double offset);
typedef PEAK_API_STATUS (*dyn_peak_BlackLevel_Offset_Get)(peak_camera_handle hCam, double * offset);
typedef PEAK_API_STATUS (*dyn_peak_BlackLevel_Offset_GetRange)(peak_camera_handle hCam, double * min, double * max, double * inc);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Bandwidth_LinkSpeed_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Bandwidth_LinkSpeed_Get)(peak_camera_handle hCam, int64_t * linkSpeed_Bps);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Bandwidth_ThroughputLimit_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Bandwidth_ThroughputLimit_GetRange)(peak_camera_handle hCam, int64_t * minThroughputLimit_Bps, int64_t * maxThroughputLimit_Bps, int64_t * incThroughputLimit_Bps);
typedef PEAK_API_STATUS (*dyn_peak_Bandwidth_ThroughputLimit_Set)(peak_camera_handle hCam, int64_t throughputLimit_Bps);
typedef PEAK_API_STATUS (*dyn_peak_Bandwidth_ThroughputLimit_Get)(peak_camera_handle hCam, int64_t * throughputLimit_Bps);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Bandwidth_ThroughputFrameRateLimit_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Bandwidth_ThroughputFrameRateLimit_Get)(peak_camera_handle hCam, double * throughputFrameRateLimit_fps);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Bandwidth_ThroughputCalculated_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Bandwidth_ThroughputCalculated_Get)(peak_camera_handle hCam, int64_t * throughputCalculated_Bps);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_IPO_GetAccessStatus)(peak_interface_technology interfaceTech);
typedef PEAK_API_BOOL (*dyn_peak_IPO_IsEnabled)(peak_interface_technology interfaceTech);
typedef PEAK_API_STATUS (*dyn_peak_IPO_Enable)(peak_interface_technology interfaceTech, peak_bool enabled);
typedef PEAK_API_STATUS (*dyn_peak_IPL_DigitalBlack_GetRange)(peak_camera_handle hCam, double * minDigitalBlack, double * maxDigitalBlack);
typedef PEAK_API_STATUS (*dyn_peak_IPL_DigitalBlack_Set)(peak_camera_handle hCam, double digitalBlack);
typedef PEAK_API_STATUS (*dyn_peak_IPL_DigitalBlack_Get)(peak_camera_handle hCam, double * digitalBlack);
typedef PEAK_API_STATUS (*dyn_peak_IPL_LUT_Enable)(peak_camera_handle hCam, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_IPL_LUT_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_IPL_LUT_Preset_Set)(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_preset preset);
typedef PEAK_API_STATUS (*dyn_peak_IPL_LUT_Value_Set)(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_channel channel, uint32_t index, uint32_t value);
typedef PEAK_API_STATUS (*dyn_peak_IPL_LUT_Value_Get)(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_channel channel, uint32_t index, uint32_t * value);
typedef PEAK_API_STATUS (*dyn_peak_IPL_LUT_ValueList_Set)(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_channel channel, uint32_t * values, size_t size);
typedef PEAK_API_STATUS (*dyn_peak_IPL_LUT_ValueList_Get)(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_channel channel, uint32_t * values, size_t * size);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Chunks_GetAccessStatus)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_Enable)(peak_camera_handle hCam, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_Chunks_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_AutoUpdate_Enable)(peak_camera_handle hCam, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_Chunks_AutoUpdate_IsEnabled)(peak_camera_handle hCam);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_Update)(peak_camera_handle hCam, peak_frame_handle hFrame);
typedef PEAK_API_ACCESS_STATUS (*dyn_peak_Chunks_Type_GetAccessStatus)(peak_camera_handle hCam, peak_chunks_type type);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_Type_Enable)(peak_camera_handle hCam, peak_chunks_type type, peak_bool enabled);
typedef PEAK_API_BOOL (*dyn_peak_Chunks_Type_IsEnabled)(peak_camera_handle hCam, peak_chunks_type type);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_Type_Supported_GetList)(peak_camera_handle hCam, peak_chunks_type * chunksTypesSupported, size_t * chunksTypesSize);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_FrameInfo_Get)(peak_camera_handle hCam, peak_chunks_frame_info * data);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_Exposure_Get)(peak_camera_handle hCam, peak_chunks_exposure * data);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_Gain_Get)(peak_camera_handle hCam, peak_chunks_gain * data);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_Sequencer_Get)(peak_camera_handle hCam, peak_chunks_sequencer * data);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_Timestamp_Get)(peak_camera_handle hCam, peak_chunks_timestamp * data);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_ExposureTrigger_Get)(peak_camera_handle hCam, peak_chunks_exposure_trigger * data);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_UsableROI_Get)(peak_camera_handle hCam, peak_chunks_usable_roi * data);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_LineStatus_Get)(peak_camera_handle hCam, peak_chunks_line_status * data);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_AutoFeature_Get)(peak_camera_handle hCam, peak_chunks_autofeature * data);
typedef PEAK_API_STATUS (*dyn_peak_Chunks_PTPStatus_Get)(peak_camera_handle hCam, peak_chunks_ptp_status * data);


class LoadLibraryException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};
                        
class DynamicLoader
{
private:
    DynamicLoader();
    
    static DynamicLoader& instance()
    {
        static DynamicLoader dynamicLoader{};
        return dynamicLoader;
    }
    bool loadLib(const char* file);
    void unload();
    bool setPointers(bool load);

public:
    ~DynamicLoader();
    
    static bool isLoaded();
    
    static PEAK_API_STATUS peak_Library_Init();
    static PEAK_API_STATUS peak_Library_Exit();
    static PEAK_API_STATUS peak_Library_GetVersion(uint32_t * majorVersion, uint32_t * minorVersion, uint32_t * subminorVersion, uint32_t * patchVersion);
    static PEAK_API_STATUS peak_Library_GetLastError(peak_status * lastErrorCode, char * lastErrorMessage, size_t * lastErrorMessageSize);
    static PEAK_API_BOOL peak_Library_InterfaceTechnology_IsSupported(peak_interface_technology interfaceTech);
    static PEAK_API_STATUS peak_Reconnect_Enable(peak_interface_technology interfaceTech, peak_bool enabled);
    static PEAK_API_BOOL peak_Reconnect_IsEnabled(peak_interface_technology interfaceTech);
    static PEAK_API_ACCESS_STATUS peak_Reconnect_GetAccessStatus(peak_interface_technology interfaceTech);
    static PEAK_API_STATUS peak_CameraList_Update(size_t * cameraCount);
    static PEAK_API_STATUS peak_CameraList_Get(peak_camera_descriptor * cameraList, size_t * cameraCount);
    static PEAK_API_CAMERA_ID peak_Camera_ID_FromHandle(peak_camera_handle hCam);
    static PEAK_API_CAMERA_ID peak_Camera_ID_FromSerialNumber(const char * serialNumber);
    static PEAK_API_CAMERA_ID peak_Camera_ID_FromUserDefinedName(const char * userDefinedName);
    static PEAK_API_CAMERA_ID peak_Camera_ID_FromMAC(peak_mac_address macAddress);
    static PEAK_API_ACCESS_STATUS peak_Camera_GetAccessStatus(peak_camera_id cameraID);
    static PEAK_API_STATUS peak_Camera_GetDescriptor(peak_camera_id cameraID, peak_camera_descriptor * cameraDescriptor);
    static PEAK_API_STATUS peak_Camera_Open(peak_camera_id cameraID, peak_camera_handle * hCam);
    static PEAK_API_STATUS peak_Camera_OpenFirstAvailable(peak_camera_handle * hCam);
    static PEAK_API_STATUS peak_Camera_Close(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Camera_ResetToDefaultSettings(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Camera_UserDefinedName_Set(peak_camera_handle hCam, const char * userDefinedName);
    static PEAK_API_STATUS peak_Camera_UserDefinedName_Get(peak_camera_handle hCam, char * userDefinedName, size_t * userDefinedNameSize);
    static PEAK_API_BOOL peak_Camera_IsConnected(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Camera_Reboot(peak_camera_handle hCam);
    static PEAK_API_ACCESS_STATUS peak_EthernetConfig_GetAccessStatus(peak_camera_id cameraID);
    static PEAK_API_STATUS peak_EthernetConfig_GetInfo(peak_camera_id cameraID, peak_ethernet_info * ethernetInfo);
    static PEAK_API_ACCESS_STATUS peak_EthernetConfig_DHCP_GetAccessStatus(peak_camera_id cameraID);
    static PEAK_API_STATUS peak_EthernetConfig_DHCP_Enable(peak_camera_id cameraID, peak_bool enabled);
    static PEAK_API_BOOL peak_EthernetConfig_DHCP_IsEnabled(peak_camera_id cameraID);
    static PEAK_API_ACCESS_STATUS peak_EthernetConfig_PersistentIP_GetAccessStatus(peak_camera_id cameraID);
    static PEAK_API_STATUS peak_EthernetConfig_PersistentIP_Set(peak_camera_id cameraID, peak_ip_config persistentIP);
    static PEAK_API_STATUS peak_EthernetConfig_PersistentIP_Get(peak_camera_id cameraID, peak_ip_config * persistentIP);
    static PEAK_API_STATUS peak_Acquisition_Start(peak_camera_handle hCam, uint32_t numberOfFrames);
    static PEAK_API_STATUS peak_Acquisition_Stop(peak_camera_handle hCam);
    static PEAK_API_BOOL peak_Acquisition_IsStarted(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Acquisition_WaitForFrame(peak_camera_handle hCam, uint32_t timeout_ms, peak_frame_handle * hFrame);
    static PEAK_API_STATUS peak_Acquisition_GetInfo(peak_camera_handle hCam, peak_acquisition_info * acquisitionInfo);
    static PEAK_API_STATUS peak_Acquisition_Buffer_GetRequiredSize(peak_camera_handle hCam, size_t * requiredBufferSize);
    static PEAK_API_STATUS peak_Acquisition_Buffer_GetRequiredCount(peak_camera_handle hCam, size_t * requiredBufferCount);
    static PEAK_API_STATUS peak_Acquisition_Buffer_Announce(peak_camera_handle hCam, uint8_t * memoryAddress, size_t memorySize, void * userContext);
    static PEAK_API_STATUS peak_Acquisition_Buffer_Revoke(peak_camera_handle hCam, uint8_t * memoryAddress);
    static PEAK_API_STATUS peak_Acquisition_Buffer_RevokeAll(peak_camera_handle hCam);
    static PEAK_API_ACCESS_STATUS peak_Acquisition_BufferHandling_Mode_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Acquisition_BufferHandling_Mode_GetList(peak_camera_handle hCam, peak_buffer_handling_mode * bufferHandlingModeList, size_t * bufferHandlingModesCount);
    static PEAK_API_STATUS peak_Acquisition_BufferHandling_Mode_Set(peak_camera_handle hCam, peak_buffer_handling_mode mode);
    static PEAK_API_STATUS peak_Acquisition_BufferHandling_Mode_Get(peak_camera_handle hCam, peak_buffer_handling_mode * mode);
    static PEAK_API_ACCESS_STATUS peak_Acquisition_LossHandling_Mode_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Acquisition_LossHandling_Mode_GetList(peak_camera_handle hCam, peak_loss_handling_mode * lossHandlingModeList, size_t * lossHandlingModesCount);
    static PEAK_API_STATUS peak_Acquisition_LossHandling_Mode_Set(peak_camera_handle hCam, peak_loss_handling_mode mode);
    static PEAK_API_STATUS peak_Acquisition_LossHandling_Mode_Get(peak_camera_handle hCam, peak_loss_handling_mode * mode);
    static PEAK_API_ACCESS_STATUS peak_Acquisition_LossHandling_Extent_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Acquisition_LossHandling_Extent_GetRange(peak_camera_handle hCam, int64_t * minExtent, int64_t * maxExtent, int64_t * incExtent);
    static PEAK_API_STATUS peak_Acquisition_LossHandling_Extent_Set(peak_camera_handle hCam, int64_t extent);
    static PEAK_API_STATUS peak_Acquisition_LossHandling_Extent_Get(peak_camera_handle hCam, int64_t * extent);
    static PEAK_API_ACCESS_STATUS peak_Acquisition_LossHandling_FrameAbortTimeout_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Acquisition_LossHandling_FrameAbortTimeout_GetRange(peak_camera_handle hCam, int64_t * minTimeout, int64_t * maxTimeout, int64_t * incTimeout);
    static PEAK_API_STATUS peak_Acquisition_LossHandling_FrameAbortTimeout_Set(peak_camera_handle hCam, int64_t timeout);
    static PEAK_API_STATUS peak_Acquisition_LossHandling_FrameAbortTimeout_Get(peak_camera_handle hCam, int64_t * timeout);
    static PEAK_API_ACCESS_STATUS peak_Acquisition_LossHandling_ResendRequestTimeout_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Acquisition_LossHandling_ResendRequestTimeout_GetRange(peak_camera_handle hCam, int64_t * minTimeout, int64_t * maxTimeout, int64_t * incTimeout);
    static PEAK_API_STATUS peak_Acquisition_LossHandling_ResendRequestTimeout_Set(peak_camera_handle hCam, int64_t timeout);
    static PEAK_API_STATUS peak_Acquisition_LossHandling_ResendRequestTimeout_Get(peak_camera_handle hCam, int64_t * timeout);
    static PEAK_API_STATUS peak_PixelFormat_GetInfo(peak_pixel_format pixelFormat, peak_pixel_format_info * pixelFormatInfo);
    static PEAK_API_STATUS peak_Frame_Release(peak_camera_handle hCam, peak_frame_handle hFrame);
    static PEAK_API_STATUS peak_Frame_GetInfo(peak_frame_handle hFrame, peak_frame_info * frameInfo);
    static PEAK_API_STATUS peak_Frame_Type_Get(peak_frame_handle hFrame, peak_frame_type * frameType);
    static PEAK_API_STATUS peak_Frame_Buffer_Get(peak_frame_handle hFrame, peak_buffer * buffer);
    static PEAK_API_STATUS peak_Frame_ID_Get(peak_frame_handle hFrame, uint64_t * frameID);
    static PEAK_API_STATUS peak_Frame_Timestamp_Get(peak_frame_handle hFrame, uint64_t * timestamp_ns);
    static PEAK_API_STATUS peak_Frame_ROI_Get(peak_frame_handle hFrame, peak_roi * roi);
    static PEAK_API_STATUS peak_Frame_PixelFormat_Get(peak_frame_handle hFrame, peak_pixel_format * pixelFormat);
    static PEAK_API_BOOL peak_Frame_IsComplete(peak_frame_handle hFrame);
    static PEAK_API_STATUS peak_Frame_BytesExpected_Get(peak_frame_handle hFrame, size_t * bytesExpected);
    static PEAK_API_STATUS peak_Frame_BytesWritten_Get(peak_frame_handle hFrame, size_t * bytesWritten);
    static PEAK_API_STATUS peak_Frame_ProcessingTime_Get(peak_frame_handle hFrame, uint32_t * processingTime_ms);
    static PEAK_API_STATUS peak_Frame_Save(peak_frame_handle hFrame, const char * fileName);
    static PEAK_API_BOOL peak_Frame_HasChunks(peak_frame_handle hFrame);
    static PEAK_API_ACCESS_STATUS peak_CameraSettings_ParameterSet_GetAccessStatus(peak_camera_handle hCam, peak_parameter_set parameterSet);
    static PEAK_API_STATUS peak_CameraSettings_ParameterSet_GetList(peak_camera_handle hCam, peak_parameter_set * parameterSetList, size_t * parameterSetCount);
    static PEAK_API_STATUS peak_CameraSettings_ParameterSet_Store(peak_camera_handle hCam, peak_parameter_set parameterSet);
    static PEAK_API_STATUS peak_CameraSettings_ParameterSet_Apply(peak_camera_handle hCam, peak_parameter_set parameterSet);
    static PEAK_API_ACCESS_STATUS peak_CameraSettings_ParameterSet_Startup_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_CameraSettings_ParameterSet_Startup_Set(peak_camera_handle hCam, peak_parameter_set parameterSet);
    static PEAK_API_STATUS peak_CameraSettings_ParameterSet_Startup_Get(peak_camera_handle hCam, peak_parameter_set * parameterSet);
    static PEAK_API_ACCESS_STATUS peak_CameraSettings_DiskFile_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_CameraSettings_DiskFile_Store(peak_camera_handle hCam, const char * file);
    static PEAK_API_STATUS peak_CameraSettings_DiskFile_Apply(peak_camera_handle hCam, const char * file);
    static PEAK_API_ACCESS_STATUS peak_FrameRate_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_FrameRate_GetRange(peak_camera_handle hCam, double * minFrameRate_fps, double * maxFrameRate_fps, double * incFrameRate_fps);
    static PEAK_API_STATUS peak_FrameRate_Set(peak_camera_handle hCam, double frameRate_fps);
    static PEAK_API_STATUS peak_FrameRate_Get(peak_camera_handle hCam, double * frameRate_fps);
    static PEAK_API_ACCESS_STATUS peak_ExposureTime_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_ExposureTime_GetRange(peak_camera_handle hCam, double * minExposureTime_us, double * maxExposureTime_us, double * incExposureTime_us);
    static PEAK_API_STATUS peak_ExposureTime_Set(peak_camera_handle hCam, double exposureTime_us);
    static PEAK_API_STATUS peak_ExposureTime_Get(peak_camera_handle hCam, double * exposureTime_us);
    static PEAK_API_ACCESS_STATUS peak_ShutterMode_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_ShutterMode_GetList(peak_camera_handle hCam, peak_shutter_mode * shutterModeList, size_t * shutterModeCount);
    static PEAK_API_STATUS peak_ShutterMode_Set(peak_camera_handle hCam, peak_shutter_mode shutterMode);
    static PEAK_API_STATUS peak_ShutterMode_Get(peak_camera_handle hCam, peak_shutter_mode * shutterMode);
    static PEAK_API_ACCESS_STATUS peak_PixelClock_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_BOOL peak_PixelClock_HasRange(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_PixelClock_GetRange(peak_camera_handle hCam, double * minPixelClock_MHz, double * maxPixelClock_MHz, double * incPixelClock_MHz);
    static PEAK_API_STATUS peak_PixelClock_GetList(peak_camera_handle hCam, double * pixelClockList, size_t * pixelClockCount);
    static PEAK_API_STATUS peak_PixelClock_Set(peak_camera_handle hCam, double pixelClock_MHz);
    static PEAK_API_STATUS peak_PixelClock_Get(peak_camera_handle hCam, double * pixelClock_MHz);
    static PEAK_API_ACCESS_STATUS peak_IOChannel_GetAccessStatus(peak_camera_handle hCam, peak_io_channel ioChannel);
    static  PEAK_API_STATUS_DEPRECATED("Use peak_IOChannel_GetListForDirection instead") peak_IOChannel_GetList(peak_camera_handlehCam,peak_io_channel*ioChannelList,size_t*ioChannelCount);
    static PEAK_API_STATUS peak_IOChannel_GetListForDirection(peak_camera_handle hCam, peak_io_direction direction, peak_io_channel * ioChannelList, size_t * ioChannelCount);
    static PEAK_API_ACCESS_STATUS peak_IOChannel_Direction_GetAccessStatus(peak_camera_handle hCam, peak_io_channel ioChannel);
    static PEAK_API_STATUS peak_IOChannel_Direction_Get(peak_camera_handle hCam, peak_io_channel ioChannel, peak_io_direction * direction);
    static PEAK_API_ACCESS_STATUS peak_IOChannel_Type_GetAccessStatus(peak_camera_handle hCam, peak_io_channel ioChannel);
    static PEAK_API_STATUS peak_IOChannel_Type_Get(peak_camera_handle hCam, peak_io_channel ioChannel, peak_io_type * type);
    static PEAK_API_ACCESS_STATUS peak_IOChannel_Level_GetAccessStatus(peak_camera_handle hCam, peak_io_channel ioChannel);
    static PEAK_API_BOOL peak_IOChannel_Level_IsHigh(peak_camera_handle hCam, peak_io_channel ioChannel);
    static PEAK_API_STATUS peak_IOChannel_Level_SetHigh(peak_camera_handle hCam, peak_io_channel ioChannel, peak_bool high);
    static PEAK_API_ACCESS_STATUS peak_IOChannel_Inverter_GetAccessStatus(peak_camera_handle hCam, peak_io_channel ioChannel);
    static PEAK_API_STATUS peak_IOChannel_Inverter_Enable(peak_camera_handle hCam, peak_io_channel ioChannel, peak_bool enabled);
    static PEAK_API_BOOL peak_IOChannel_Inverter_IsEnabled(peak_camera_handle hCam, peak_io_channel ioChannel);
    static PEAK_API_ACCESS_STATUS peak_IOChannel_NoiseFilter_GetAccessStatus(peak_camera_handle hCam, peak_io_channel ioChannel);
    static PEAK_API_STATUS peak_IOChannel_NoiseFilter_Enable(peak_camera_handle hCam, peak_io_channel ioChannel, peak_bool enabled);
    static PEAK_API_BOOL peak_IOChannel_NoiseFilter_IsEnabled(peak_camera_handle hCam, peak_io_channel ioChannel);
    static PEAK_API_STATUS peak_IOChannel_NoiseFilter_Duration_GetRange(peak_camera_handle hCam, peak_io_channel ioChannel, double * minNoiseFilterDuration_us, double * maxNoiseFilterDuration_us, double * incNoiseFilterDuration_us);
    static PEAK_API_STATUS peak_IOChannel_NoiseFilter_Duration_Set(peak_camera_handle hCam, peak_io_channel ioChannel, double noiseFilterDuration_us);
    static PEAK_API_STATUS peak_IOChannel_NoiseFilter_Duration_Get(peak_camera_handle hCam, peak_io_channel ioChannel, double * noiseFilterDuration_us);
    static PEAK_API_ACCESS_STATUS peak_Trigger_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Trigger_Enable(peak_camera_handle hCam, peak_bool enabled);
    static PEAK_API_BOOL peak_Trigger_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_BOOL peak_Trigger_IsExecutable(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Trigger_Execute(peak_camera_handle hCam);
    static PEAK_API_ACCESS_STATUS peak_Trigger_Mode_GetAccessStatus(peak_camera_handle hCam, peak_trigger_mode triggerMode);
    static PEAK_API_STATUS peak_Trigger_Mode_Set(peak_camera_handle hCam, peak_trigger_mode triggerMode);
    static  PEAK_API_STATUS_DEPRECATED("Use peak_Trigger_Mode_Config_Get instead") peak_Trigger_Mode_Get(peak_camera_handlehCam,peak_trigger_mode*triggerMode);
    static PEAK_API_STATUS peak_Trigger_Mode_Config_Get(peak_camera_handle hCam, peak_trigger_mode * triggerMode);
    static PEAK_API_ACCESS_STATUS peak_Trigger_Edge_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Trigger_Edge_GetList(peak_camera_handle hCam, peak_trigger_edge * triggerEdgeList, size_t * triggerEdgeCount);
    static PEAK_API_STATUS peak_Trigger_Edge_Set(peak_camera_handle hCam, peak_trigger_edge triggerEdge);
    static PEAK_API_STATUS peak_Trigger_Edge_Get(peak_camera_handle hCam, peak_trigger_edge * triggerEdge);
    static PEAK_API_ACCESS_STATUS peak_Trigger_Delay_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Trigger_Delay_GetRange(peak_camera_handle hCam, double * minTriggerDelay_us, double * maxTriggerDelay_us, double * incTriggerDelay_us);
    static PEAK_API_STATUS peak_Trigger_Delay_Set(peak_camera_handle hCam, double triggerDelay_us);
    static PEAK_API_STATUS peak_Trigger_Delay_Get(peak_camera_handle hCam, double * triggerDelay_us);
    static PEAK_API_ACCESS_STATUS peak_Trigger_Divider_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Trigger_Divider_GetRange(peak_camera_handle hCam, uint32_t * minTriggerDivider, uint32_t * maxTriggerDivider, uint32_t * incTriggerDivider);
    static PEAK_API_STATUS peak_Trigger_Divider_Set(peak_camera_handle hCam, uint32_t triggerDivider);
    static PEAK_API_STATUS peak_Trigger_Divider_Get(peak_camera_handle hCam, uint32_t * triggerDivider);
    static PEAK_API_ACCESS_STATUS peak_Trigger_Burst_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Trigger_Burst_GetRange(peak_camera_handle hCam, uint32_t * minTriggerBurst, uint32_t * maxTriggerBurst, uint32_t * incTriggerBurst);
    static PEAK_API_STATUS peak_Trigger_Burst_Set(peak_camera_handle hCam, uint32_t triggerBurst);
    static PEAK_API_STATUS peak_Trigger_Burst_Get(peak_camera_handle hCam, uint32_t * triggerBurst);
    static PEAK_API_ACCESS_STATUS peak_Flash_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Flash_Enable(peak_camera_handle hCam, peak_bool enabled);
    static PEAK_API_BOOL peak_Flash_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_ACCESS_STATUS peak_Flash_Mode_GetAccessStatus(peak_camera_handle hCam, peak_flash_mode flashMode);
    static PEAK_API_STATUS peak_Flash_Mode_Set(peak_camera_handle hCam, peak_flash_mode flashMode);
    static  PEAK_API_STATUS_DEPRECATED("Use peak_Flash_Mode_Config_Get instead") peak_Flash_Mode_Get(peak_camera_handlehCam,peak_flash_mode*flashMode);
    static PEAK_API_STATUS peak_Flash_Mode_Config_Get(peak_camera_handle hCam, peak_flash_mode * flashMode);
    static PEAK_API_ACCESS_STATUS peak_Flash_StartDelay_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Flash_StartDelay_GetRange(peak_camera_handle hCam, double * minFlashStartDelay_us, double * maxFlashStartDelay_us, double * incFlashStartDelay_us);
    static PEAK_API_STATUS peak_Flash_StartDelay_Set(peak_camera_handle hCam, double flashStartDelay_us);
    static PEAK_API_STATUS peak_Flash_StartDelay_Get(peak_camera_handle hCam, double * flashStartDelay_us);
    static PEAK_API_ACCESS_STATUS peak_Flash_EndDelay_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Flash_EndDelay_GetRange(peak_camera_handle hCam, double * minFlashEndDelay_us, double * maxFlashEndDelay_us, double * incFlashEndDelay_us);
    static PEAK_API_STATUS peak_Flash_EndDelay_Set(peak_camera_handle hCam, double flashEndDelay_us);
    static PEAK_API_STATUS peak_Flash_EndDelay_Get(peak_camera_handle hCam, double * flashEndDelay_us);
    static PEAK_API_ACCESS_STATUS peak_Flash_Duration_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Flash_Duration_GetRange(peak_camera_handle hCam, double * minFlashDuration_us, double * maxFlashDuration_us, double * incFlashDuration_us);
    static PEAK_API_STATUS peak_Flash_Duration_Set(peak_camera_handle hCam, double flashDuration_us);
    static PEAK_API_STATUS peak_Flash_Duration_Get(peak_camera_handle hCam, double * flashDuration_us);
    static PEAK_API_ACCESS_STATUS peak_Focus_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Focus_GetRange(peak_camera_handle hCam, uint32_t * minFocus, uint32_t * maxFocus, uint32_t * incFocus);
    static PEAK_API_STATUS peak_Focus_Set(peak_camera_handle hCam, uint32_t focus);
    static PEAK_API_STATUS peak_Focus_Get(peak_camera_handle hCam, uint32_t * focus);
    static PEAK_API_ACCESS_STATUS peak_PixelFormat_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_PixelFormat_GetList(peak_camera_handle hCam, peak_pixel_format * pixelFormatList, size_t * pixelFormatCount);
    static PEAK_API_STATUS peak_PixelFormat_Set(peak_camera_handle hCam, peak_pixel_format pixelFormat);
    static PEAK_API_STATUS peak_PixelFormat_Get(peak_camera_handle hCam, peak_pixel_format * pixelFormat);
    static PEAK_API_ACCESS_STATUS peak_Gain_GetAccessStatus(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel);
    static PEAK_API_STATUS peak_Gain_GetChannelList(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel * gainChannelList, size_t * gainChannelCount);
    static PEAK_API_STATUS peak_Gain_GetRange(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double * minGain, double * maxGain, double * incGain);
    static PEAK_API_STATUS peak_Gain_Set(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double gain);
    static PEAK_API_STATUS peak_Gain_Get(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double * gain);
    static PEAK_API_ACCESS_STATUS peak_Gamma_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Gamma_GetRange(peak_camera_handle hCam, double * minGamma, double * maxGamma, double * incGamma);
    static PEAK_API_STATUS peak_Gamma_Set(peak_camera_handle hCam, double gamma);
    static PEAK_API_STATUS peak_Gamma_Get(peak_camera_handle hCam, double * gamma);
    static PEAK_API_ACCESS_STATUS peak_ColorCorrection_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_ColorCorrection_Mode_GetList(peak_camera_handle hCam, peak_color_correction_mode * colorCorrectionModeList, size_t * colorCorrectionModeCount);
    static PEAK_API_STATUS peak_ColorCorrection_Mode_Set(peak_camera_handle hCam, peak_color_correction_mode colorCorrectionMode);
    static PEAK_API_STATUS peak_ColorCorrection_Mode_Get(peak_camera_handle hCam, peak_color_correction_mode * colorCorrectionMode);
    static PEAK_API_ACCESS_STATUS peak_ColorCorrection_Matrix_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_ColorCorrection_Matrix_GetRange(peak_camera_handle hCam, double * minMatrixElementValue, double * maxMatrixElementValue, double * incMatrixElementValue);
    static PEAK_API_STATUS peak_ColorCorrection_Matrix_Set(peak_camera_handle hCam, peak_matrix colorCorrectionMatrix);
    static PEAK_API_STATUS peak_ColorCorrection_Matrix_Get(peak_camera_handle hCam, peak_matrix * colorCorrectionMatrix);
    static PEAK_API_STATUS peak_ColorCorrection_Enable(peak_camera_handle hCam, peak_bool enabled);
    static PEAK_API_BOOL peak_ColorCorrection_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_ACCESS_STATUS peak_AutoBrightness_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_ACCESS_STATUS peak_AutoBrightness_Target_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_AutoBrightness_Target_GetRange(peak_camera_handle hCam, uint32_t * minAutoBrightnessTarget, uint32_t * maxAutoBrightnessTarget, uint32_t * incAutoBrightnessTarget);
    static PEAK_API_STATUS peak_AutoBrightness_Target_Set(peak_camera_handle hCam, uint32_t autoBrightnessTarget);
    static PEAK_API_STATUS peak_AutoBrightness_Target_Get(peak_camera_handle hCam, uint32_t * autoBrightnessTarget);
    static PEAK_API_ACCESS_STATUS peak_AutoBrightness_TargetTolerance_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_AutoBrightness_TargetTolerance_GetRange(peak_camera_handle hCam, uint32_t * minAutoBrightnessTargetTolerance, uint32_t * maxAutoBrightnessTargetTolerance, uint32_t * incAutoBrightnessTargetTolerance);
    static PEAK_API_STATUS peak_AutoBrightness_TargetTolerance_Set(peak_camera_handle hCam, uint32_t autoBrightnessTargetTolerance);
    static PEAK_API_STATUS peak_AutoBrightness_TargetTolerance_Get(peak_camera_handle hCam, uint32_t * autoBrightnessTargetTolerance);
    static PEAK_API_ACCESS_STATUS peak_AutoBrightness_TargetPercentile_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_AutoBrightness_TargetPercentile_GetRange(peak_camera_handle hCam, double * minAutoBrightnessTargetPercentile, double * maxAutoBrightnessTargetPercentile, double * incAutoBrightnessTargetPercentile);
    static PEAK_API_STATUS peak_AutoBrightness_TargetPercentile_Set(peak_camera_handle hCam, double autoBrightnessTargetPercentile);
    static PEAK_API_STATUS peak_AutoBrightness_TargetPercentile_Get(peak_camera_handle hCam, double * autoBrightnessTargetPercentile);
    static PEAK_API_ACCESS_STATUS peak_AutoBrightness_ROI_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_AutoBrightness_ROI_Mode_Set(peak_camera_handle hCam, peak_auto_feature_roi_mode autoBrightnessROIMode);
    static PEAK_API_STATUS peak_AutoBrightness_ROI_Mode_Get(peak_camera_handle hCam, peak_auto_feature_roi_mode * autoBrightnessROIMode);
    static PEAK_API_STATUS peak_AutoBrightness_ROI_Offset_GetRange(peak_camera_handle hCam, peak_position * minAutoBrightnessROIOffset, peak_position * maxAutoBrightnessROIOffset, peak_position * incAutoBrightnessROIOffset);
    static PEAK_API_STATUS peak_AutoBrightness_ROI_Size_GetRange(peak_camera_handle hCam, peak_size * minAutoBrightnessROISize, peak_size * maxAutoBrightnessROISize, peak_size * incAutoBrightnessROISize);
    static PEAK_API_STATUS peak_AutoBrightness_ROI_Set(peak_camera_handle hCam, peak_roi autoBrightnessROI);
    static PEAK_API_STATUS peak_AutoBrightness_ROI_Get(peak_camera_handle hCam, peak_roi * autoBrightnessROI);
    static PEAK_API_ACCESS_STATUS peak_AutoBrightness_Exposure_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_AutoBrightness_Exposure_Mode_GetList(peak_camera_handle hCam, peak_auto_feature_mode * modeList, size_t * modeListSize);
    static PEAK_API_STATUS peak_AutoBrightness_Exposure_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoExposureMode);
    static PEAK_API_STATUS peak_AutoBrightness_Exposure_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoExposureMode);
    static PEAK_API_ACCESS_STATUS peak_AutoBrightness_Gain_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_AutoBrightness_Gain_Mode_GetList(peak_camera_handle hCam, peak_auto_feature_mode * modeList, size_t * modeListSize);
    static PEAK_API_STATUS peak_AutoBrightness_Gain_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode);
    static PEAK_API_STATUS peak_AutoBrightness_Gain_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode);
    static PEAK_API_ACCESS_STATUS peak_AutoWhiteBalance_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_ACCESS_STATUS peak_AutoWhiteBalance_ROI_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_AutoWhiteBalance_ROI_Mode_Set(peak_camera_handle hCam, peak_auto_feature_roi_mode autoWhiteBalanceROIMode);
    static PEAK_API_STATUS peak_AutoWhiteBalance_ROI_Mode_Get(peak_camera_handle hCam, peak_auto_feature_roi_mode * autoWhiteBalanceROIMode);
    static PEAK_API_STATUS peak_AutoWhiteBalance_ROI_Offset_GetRange(peak_camera_handle hCam, peak_position * minAutoWhiteBalanceROIOffset, peak_position * maxAutoWhiteBalanceROIOffset, peak_position * incAutoWhiteBalanceROIOffset);
    static PEAK_API_STATUS peak_AutoWhiteBalance_ROI_Size_GetRange(peak_camera_handle hCam, peak_size * minAutoWhiteBalanceROISize, peak_size * maxAutoWhiteBalanceROISize, peak_size * incAutoWhiteBalanceROISize);
    static PEAK_API_STATUS peak_AutoWhiteBalance_ROI_Set(peak_camera_handle hCam, peak_roi autoWhiteBalanceROI);
    static PEAK_API_STATUS peak_AutoWhiteBalance_ROI_Get(peak_camera_handle hCam, peak_roi * autoWhiteBalanceROI);
    static PEAK_API_STATUS peak_AutoWhiteBalance_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoWhiteBalanceMode);
    static PEAK_API_STATUS peak_AutoWhiteBalance_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoWhiteBalanceMode);
    static PEAK_API_STATUS peak_AutoWhiteBalance_Mode_GetList(peak_camera_handle hCam, peak_auto_feature_mode * modeList, size_t * modeListSize);
    static PEAK_API_ACCESS_STATUS peak_ROI_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_ROI_Offset_GetRange(peak_camera_handle hCam, peak_position * minROIOffset, peak_position * maxROIOffset, peak_position * incROIOffset);
    static PEAK_API_ACCESS_STATUS peak_ROI_Offset_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_ROI_Offset_Set(peak_camera_handle hCam, peak_position position);
    static PEAK_API_STATUS peak_ROI_Offset_Get(peak_camera_handle hCam, peak_position * position);
    static PEAK_API_STATUS peak_ROI_Size_GetRange(peak_camera_handle hCam, peak_size * minROISize, peak_size * maxROISize, peak_size * incROISize);
    static PEAK_API_ACCESS_STATUS peak_ROI_Size_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_ROI_Size_Set(peak_camera_handle hCam, peak_size size);
    static PEAK_API_STATUS peak_ROI_Size_Get(peak_camera_handle hCam, peak_size * size);
    static PEAK_API_STATUS peak_ROI_Set(peak_camera_handle hCam, peak_roi roi);
    static PEAK_API_STATUS peak_ROI_Get(peak_camera_handle hCam, peak_roi * roi);
    static PEAK_API_ACCESS_STATUS peak_Binning_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Binning_FactorX_GetList(peak_camera_handle hCam, uint32_t * binningFactorXList, size_t * binningFactorXCount);
    static PEAK_API_STATUS peak_Binning_FactorY_GetList(peak_camera_handle hCam, uint32_t * binningFactorYList, size_t * binningFactorYCount);
    static PEAK_API_STATUS peak_Binning_Set(peak_camera_handle hCam, uint32_t binningFactorX, uint32_t binningFactorY);
    static PEAK_API_STATUS peak_Binning_Get(peak_camera_handle hCam, uint32_t * binningFactorX, uint32_t * binningFactorY);
    static PEAK_API_ACCESS_STATUS peak_BinningManual_GetAccessStatus(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine);
    static PEAK_API_STATUS peak_BinningManual_FactorX_GetList(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * binningFactorXList, size_t * binningFactorXCount);
    static PEAK_API_STATUS peak_BinningManual_FactorY_GetList(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * binningFactorYList, size_t * binningFactorYCount);
    static PEAK_API_STATUS peak_BinningManual_Set(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t binningFactorX, uint32_t binningFactorY);
    static PEAK_API_STATUS peak_BinningManual_Get(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * binningFactorX, uint32_t * binningFactorY);
    static PEAK_API_ACCESS_STATUS peak_Decimation_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Decimation_FactorX_GetList(peak_camera_handle hCam, uint32_t * decimationFactorXList, size_t * decimationFactorXCount);
    static PEAK_API_STATUS peak_Decimation_FactorY_GetList(peak_camera_handle hCam, uint32_t * decimationFactorYList, size_t * decimationFactorYCount);
    static PEAK_API_STATUS peak_Decimation_Set(peak_camera_handle hCam, uint32_t decimationFactorX, uint32_t decimationFactorY);
    static PEAK_API_STATUS peak_Decimation_Get(peak_camera_handle hCam, uint32_t * decimationFactorX, uint32_t * decimationFactorY);
    static PEAK_API_ACCESS_STATUS peak_DecimationManual_GetAccessStatus(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine);
    static PEAK_API_STATUS peak_DecimationManual_FactorX_GetList(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * decimationFactorXList, size_t * decimationFactorXCount);
    static PEAK_API_STATUS peak_DecimationManual_FactorY_GetList(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * decimationFactorYList, size_t * decimationFactorYCount);
    static PEAK_API_STATUS peak_DecimationManual_Set(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t decimationFactorX, uint32_t decimationFactorY);
    static PEAK_API_STATUS peak_DecimationManual_Get(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * decimationFactorX, uint32_t * decimationFactorY);
    static PEAK_API_ACCESS_STATUS peak_Scaling_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Scaling_FactorX_GetRange(peak_camera_handle hCam, double * minScalingFactorX, double * maxScalingFactorX, double * incScalingFactorX);
    static PEAK_API_STATUS peak_Scaling_FactorY_GetRange(peak_camera_handle hCam, double * minScalingFactorY, double * maxScalingFactorY, double * incScalingFactorY);
    static PEAK_API_STATUS peak_Scaling_Set(peak_camera_handle hCam, double scalingFactorX, double scalingFactorY);
    static PEAK_API_STATUS peak_Scaling_Get(peak_camera_handle hCam, double * scalingFactorX, double * scalingFactorY);
    static PEAK_API_ACCESS_STATUS peak_Mirror_LeftRight_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Mirror_LeftRight_Enable(peak_camera_handle hCam, peak_bool enabled);
    static PEAK_API_BOOL peak_Mirror_LeftRight_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_ACCESS_STATUS peak_Mirror_UpDown_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Mirror_UpDown_Enable(peak_camera_handle hCam, peak_bool enabled);
    static PEAK_API_BOOL peak_Mirror_UpDown_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_ACCESS_STATUS peak_CameraMemory_Area_GetAccessStatus(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea);
    static PEAK_API_STATUS peak_CameraMemory_Area_GetList(peak_camera_handle hCam, peak_camera_memory_area * cameraMemoryAreaList, size_t * cameraMemoryAreaCount);
    static PEAK_API_STATUS peak_CameraMemory_Area_Size_Get(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea, size_t * cameraMemoryAreaSize);
    static PEAK_API_STATUS peak_CameraMemory_Area_Data_Clear(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea);
    static PEAK_API_STATUS peak_CameraMemory_Area_Data_Write(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea, const uint8_t * data, size_t dataSize);
    static PEAK_API_STATUS peak_CameraMemory_Area_Data_Read(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea, uint8_t * data, size_t dataSize);
    static PEAK_API_STATUS peak_GFA_EnableWriteAccess(peak_camera_handle hCam, peak_bool enabled);
    static PEAK_API_BOOL peak_GFA_IsWriteAccessEnabled(peak_camera_handle hCam);
    static PEAK_API_ACCESS_STATUS peak_GFA_Feature_GetAccessStatus(peak_camera_handle hCam, peak_gfa_module module, const char * featureName);
    static PEAK_API_BOOL peak_GFA_Float_HasRange(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName);
    static PEAK_API_STATUS peak_GFA_Float_GetRange(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName, double * minFloatValue, double * maxFloatValue, double * incFloatValue);
    static PEAK_API_STATUS peak_GFA_Float_GetList(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName, double * floatList, size_t * floatCount);
    static PEAK_API_STATUS peak_GFA_Float_Set(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName, double floatValue);
    static PEAK_API_STATUS peak_GFA_Float_Get(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName, double * floatValue);
    static PEAK_API_BOOL peak_GFA_Integer_HasRange(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName);
    static PEAK_API_STATUS peak_GFA_Integer_GetRange(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName, int64_t * minIntegerValue, int64_t * maxIntegerValue, int64_t * incIntegerValue);
    static PEAK_API_STATUS peak_GFA_Integer_GetList(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName, int64_t * integerList, size_t * integerCount);
    static PEAK_API_STATUS peak_GFA_Integer_Set(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName, int64_t integerValue);
    static PEAK_API_STATUS peak_GFA_Integer_Get(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName, int64_t * integerValue);
    static PEAK_API_STATUS peak_GFA_Boolean_Set(peak_camera_handle hCam, peak_gfa_module module, const char * booleanFeatureName, peak_bool booleanValue);
    static PEAK_API_STATUS peak_GFA_Boolean_Get(peak_camera_handle hCam, peak_gfa_module module, const char * booleanFeatureName, peak_bool * booleanValue);
    static PEAK_API_STATUS peak_GFA_String_Set(peak_camera_handle hCam, peak_gfa_module module, const char * stringFeatureName, const char * stringValue);
    static PEAK_API_STATUS peak_GFA_String_Get(peak_camera_handle hCam, peak_gfa_module module, const char * stringFeatureName, char * stringValue, size_t * stringValueSize);
    static PEAK_API_STATUS peak_GFA_Command_Execute(peak_camera_handle hCam, peak_gfa_module module, const char * commandFeatureName);
    static PEAK_API_STATUS peak_GFA_Command_WaitForDone(peak_camera_handle hCam, peak_gfa_module module, const char * commandFeatureName, uint32_t timeout_ms);
    static PEAK_API_STATUS peak_GFA_Enumeration_GetList(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, peak_gfa_enumeration_entry * enumerationEntryList, size_t * enumerationEntryCount);
    static PEAK_API_ACCESS_STATUS peak_GFA_EnumerationEntry_GetAccessStatus(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, const peak_gfa_enumeration_entry * enumerationEntry);
    static PEAK_API_ACCESS_STATUS peak_GFA_EnumerationEntry_GetAccessStatusBySymbolicValue(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, const char * enumerationEntrySymbolicValue);
    static PEAK_API_ACCESS_STATUS peak_GFA_EnumerationEntry_GetAccessStatusByIntegerValue(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, int64_t enumerationEntryIntegerValue);
    static PEAK_API_STATUS peak_GFA_Enumeration_Set(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, const peak_gfa_enumeration_entry * enumerationEntry);
    static PEAK_API_STATUS peak_GFA_Enumeration_SetBySymbolicValue(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, const char * enumerationEntrySymbolicValue);
    static PEAK_API_STATUS peak_GFA_Enumeration_SetByIntegerValue(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, int64_t enumerationEntryIntegerValue);
    static PEAK_API_STATUS peak_GFA_Enumeration_Get(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, peak_gfa_enumeration_entry * enumerationEntry);
    static PEAK_API_STATUS peak_GFA_Register_Set(peak_camera_handle hCam, peak_gfa_module module, const char * registerFeatureName, const uint8_t * registerValue, size_t registerValueSize);
    static PEAK_API_STATUS peak_GFA_Register_Get(peak_camera_handle hCam, peak_gfa_module module, const char * registerFeatureName, uint8_t * registerValue, size_t * registerValueSize);
    static PEAK_API_STATUS peak_GFA_Data_Write(peak_camera_handle hCam, peak_gfa_module module, uint64_t address, const uint8_t * data, size_t dataSize);
    static PEAK_API_STATUS peak_GFA_Data_Read(peak_camera_handle hCam, peak_gfa_module module, uint64_t address, uint8_t * data, size_t dataSize);
    static PEAK_API_STATUS peak_IPL_PixelFormat_GetList(peak_camera_handle hCam, peak_pixel_format inputPixelFormat, peak_pixel_format * outputPixelFormatList, size_t * outputPixelFormatCount);
    static PEAK_API_STATUS peak_IPL_PixelFormat_Set(peak_camera_handle hCam, peak_pixel_format pixelFormat);
    static PEAK_API_STATUS peak_IPL_PixelFormat_Get(peak_camera_handle hCam, peak_pixel_format * pixelFormat);
    static PEAK_API_STATUS peak_IPL_Gain_GetRange(peak_camera_handle hCam, peak_gain_channel gainChannel, double * minGain, double * maxGain, double * incGain);
    static PEAK_API_STATUS peak_IPL_Gain_Set(peak_camera_handle hCam, peak_gain_channel gainChannel, double gain);
    static PEAK_API_STATUS peak_IPL_Gain_Get(peak_camera_handle hCam, peak_gain_channel gainChannel, double * gain);
    static PEAK_API_STATUS peak_IPL_Gamma_GetRange(peak_camera_handle hCam, double * minGamma, double * maxGamma, double * incGamma);
    static PEAK_API_STATUS peak_IPL_Gamma_Set(peak_camera_handle hCam, double gamma);
    static PEAK_API_STATUS peak_IPL_Gamma_Get(peak_camera_handle hCam, double * gamma);
    static PEAK_API_STATUS peak_IPL_ColorCorrection_Matrix_Set(peak_camera_handle hCam, peak_matrix colorCorrectionMatrix);
    static PEAK_API_STATUS peak_IPL_ColorCorrection_Matrix_Get(peak_camera_handle hCam, peak_matrix * colorCorrectionMatrix);
    static PEAK_API_STATUS peak_IPL_ColorCorrection_Saturation_Get(peak_camera_handle hCam, double * saturation);
    static PEAK_API_STATUS peak_IPL_ColorCorrection_Saturation_Set(peak_camera_handle hCam, double saturation);
    static PEAK_API_STATUS peak_IPL_ColorCorrection_Saturation_GetRange(peak_camera_handle hCam, double * minSaturation, double * maxSaturation, double * incSaturation);
    static PEAK_API_STATUS peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Get(peak_camera_handle hCam, peak_chromatic_adaption_color_space * colorSpace);
    static PEAK_API_STATUS peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Set(peak_camera_handle hCam, peak_chromatic_adaption_color_space colorSpace);
    static PEAK_API_STATUS peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Get(peak_camera_handle hCam, peak_chromatic_adaption_algorithm * algorithm);
    static PEAK_API_STATUS peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Set(peak_camera_handle hCam, peak_chromatic_adaption_algorithm algorithm);
    static PEAK_API_STATUS peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Get(peak_camera_handle hCam, uint32_t * colorTemperature);
    static PEAK_API_STATUS peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Set(peak_camera_handle hCam, uint32_t colorTemperature);
    static PEAK_API_STATUS peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_GetRange(peak_camera_handle hCam, uint32_t * minColorTemperature, uint32_t * maxColorTemperature, uint32_t * incColorTemperature);
    static PEAK_API_STATUS peak_IPL_ColorCorrection_ChromaticAdaption_Enable(peak_camera_handle hCam, peak_bool enable);
    static PEAK_API_BOOL peak_IPL_ColorCorrection_ChromaticAdaption_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_IPL_ColorCorrection_Enable(peak_camera_handle hCam, peak_bool enabled);
    static PEAK_API_BOOL peak_IPL_ColorCorrection_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_Target_GetRange(peak_camera_handle hCam, uint32_t * minAutoBrightnessTarget, uint32_t * maxAutoBrightnessTarget, uint32_t * incAutoBrightnessTarget);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_Target_Set(peak_camera_handle hCam, uint32_t autoBrightnessTarget);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_Target_Get(peak_camera_handle hCam, uint32_t * autoBrightnessTarget);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_TargetTolerance_GetRange(peak_camera_handle hCam, uint32_t * minAutoBrightnessTargetTolerance, uint32_t * maxAutoBrightnessTargetTolerance, uint32_t * incAutoBrightnessTargetTolerance);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_TargetTolerance_Set(peak_camera_handle hCam, uint32_t autoBrightnessTargetTolerance);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_TargetTolerance_Get(peak_camera_handle hCam, uint32_t * autoBrightnessTargetTolerance);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_TargetPercentile_GetRange(peak_camera_handle hCam, double * minAutoBrightnessTargetPercentile, double * maxAutoBrightnessTargetPercentile, double * incAutoBrightnessTargetPercentile);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_TargetPercentile_Set(peak_camera_handle hCam, double autoBrightnessTargetPercentile);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_TargetPercentile_Get(peak_camera_handle hCam, double * autoBrightnessTargetPercentile);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_ROI_Mode_Set(peak_camera_handle hCam, peak_auto_feature_roi_mode autoBrightnessROIMode);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_ROI_Mode_Get(peak_camera_handle hCam, peak_auto_feature_roi_mode * autoBrightnessROIMode);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_ROI_Offset_GetRange(peak_camera_handle hCam, peak_position * minAutoBrightnessROIOffset, peak_position * maxAutoBrightnessROIOffset, peak_position * incAutoBrightnessROIOffset);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_ROI_Size_GetRange(peak_camera_handle hCam, peak_size * minAutoBrightnessROISize, peak_size * maxAutoBrightnessROISize, peak_size * incAutoBrightnessROISize);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_ROI_Set(peak_camera_handle hCam, peak_roi autoBrightnessROI);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_ROI_Get(peak_camera_handle hCam, peak_roi * autoBrightnessROI);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_Exposure_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoExposureMode);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_Exposure_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoExposureMode);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_ExposureLimit_Get(peak_camera_handle hCam, peak_double_limit * exposureLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_ExposureLimit_Set(peak_camera_handle hCam, peak_double_limit exposureLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_Gain_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_Gain_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainAnalog_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainAnalog_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainDigital_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainDigital_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainCombined_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainCombined_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainHost_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainHost_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_Algorithm_Set(peak_camera_handle hCam, peak_auto_feature_brightness_algorithm algorithm);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_Algorithm_Get(peak_camera_handle hCam, peak_auto_feature_brightness_algorithm * algorithm);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_AverageLast_Get(peak_camera_handle hCam, uint32_t * lastAverage);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainLimit_Set(peak_camera_handle hCam, peak_double_limit gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainLimit_Get(peak_camera_handle hCam, peak_double_limit * gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainLimit_GetRange(peak_camera_handle hCam, peak_double_limit * gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainAnalogLimit_Set(peak_camera_handle hCam, peak_double_limit gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainAnalogLimit_Get(peak_camera_handle hCam, peak_double_limit * gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainAnalogLimit_GetRange(peak_camera_handle hCam, peak_double_limit * gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainDigitalLimit_Set(peak_camera_handle hCam, peak_double_limit gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainDigitalLimit_Get(peak_camera_handle hCam, peak_double_limit * gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainDigitalLimit_GetRange(peak_camera_handle hCam, peak_double_limit * gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainCombinedLimit_Set(peak_camera_handle hCam, peak_double_limit gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainCombinedLimit_Get(peak_camera_handle hCam, peak_double_limit * gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainCombinedLimit_GetRange(peak_camera_handle hCam, peak_double_limit * gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainHostLimit_Set(peak_camera_handle hCam, peak_double_limit gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainHostLimit_Get(peak_camera_handle hCam, peak_double_limit * gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_GainHostLimit_GetRange(peak_camera_handle hCam, peak_double_limit * gainLimit);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_SkipFrames_Set(peak_camera_handle hCam, uint32_t skipFrames);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_SkipFrames_Get(peak_camera_handle hCam, uint32_t * skipFrames);
    static PEAK_API_STATUS peak_IPL_AutoBrightness_SkipFrames_GetRange(peak_camera_handle hCam, uint32_t * skipFramesMin, uint32_t * skipFramesMax, uint32_t * skipFramesInc);
    static PEAK_API_STATUS peak_IPL_AutoWhiteBalance_ROI_Mode_Set(peak_camera_handle hCam, peak_auto_feature_roi_mode autoWhiteBalanceROIMode);
    static PEAK_API_STATUS peak_IPL_AutoWhiteBalance_ROI_Mode_Get(peak_camera_handle hCam, peak_auto_feature_roi_mode * autoWhiteBalanceROIMode);
    static PEAK_API_STATUS peak_IPL_AutoWhiteBalance_ROI_Offset_GetRange(peak_camera_handle hCam, peak_position * minAutoWhiteBalanceROIOffset, peak_position * maxAutoWhiteBalanceROIOffset, peak_position * incAutoWhiteBalanceROIOffset);
    static PEAK_API_STATUS peak_IPL_AutoWhiteBalance_ROI_Size_GetRange(peak_camera_handle hCam, peak_size * minAutoWhiteBalanceROISize, peak_size * maxAutoWhiteBalanceROISize, peak_size * incAutoWhiteBalanceROISize);
    static PEAK_API_STATUS peak_IPL_AutoWhiteBalance_ROI_Set(peak_camera_handle hCam, peak_roi autoWhiteBalanceROI);
    static PEAK_API_STATUS peak_IPL_AutoWhiteBalance_ROI_Get(peak_camera_handle hCam, peak_roi * autoWhiteBalanceROI);
    static PEAK_API_STATUS peak_IPL_AutoWhiteBalance_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoWhiteBalanceMode);
    static PEAK_API_STATUS peak_IPL_AutoWhiteBalance_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoWhiteBalanceMode);
    static PEAK_API_STATUS peak_IPL_AutoWhiteBalance_SkipFrames_Set(peak_camera_handle hCam, uint32_t skipFrames);
    static PEAK_API_STATUS peak_IPL_AutoWhiteBalance_SkipFrames_Get(peak_camera_handle hCam, uint32_t * skipFrames);
    static PEAK_API_STATUS peak_IPL_AutoWhiteBalance_SkipFrames_GetRange(peak_camera_handle hCam, uint32_t * skipFramesMin, uint32_t * skipFramesMax, uint32_t * skipFramesInc);
    static PEAK_API_ACCESS_STATUS peak_IPL_AutoFocus_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_IPL_AutoFocus_ROI_Set(peak_camera_handle hCam, const peak_focus_roi * autoFocusROIList, size_t autoFocusROICount);
    static PEAK_API_STATUS peak_IPL_AutoFocus_ROI_Get(peak_camera_handle hCam, peak_focus_roi * autoFocusROIList, size_t * autoFocusROICount);
    static PEAK_API_STATUS peak_IPL_AutoFocus_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoFocusMode);
    static PEAK_API_STATUS peak_IPL_AutoFocus_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoFocusMode);
    static PEAK_API_STATUS peak_IPL_AutoFocus_SearchAlgorithm_Set(peak_camera_handle hCam, peak_auto_focus_search_algorithm searchAlgorithm);
    static PEAK_API_STATUS peak_IPL_AutoFocus_SearchAlgorithm_Get(peak_camera_handle hCam, peak_auto_focus_search_algorithm * searchAlgorithm);
    static PEAK_API_STATUS peak_IPL_AutoFocus_SharpnessAlgorithm_Set(peak_camera_handle hCam, peak_sharpness_algorithm sharpnessAlgorithm);
    static PEAK_API_STATUS peak_IPL_AutoFocus_SharpnessAlgorithm_Get(peak_camera_handle hCam, peak_sharpness_algorithm * sharpnessAlgorithm);
    static PEAK_API_STATUS peak_IPL_AutoFocus_Range_Set(peak_camera_handle hCam, uint32_t rangeBegin, uint32_t rangeEnd);
    static PEAK_API_STATUS peak_IPL_AutoFocus_Range_Get(peak_camera_handle hCam, uint32_t * rangeBegin, uint32_t * rangeEnd);
    static PEAK_API_STATUS peak_IPL_AutoFocus_Hysteresis_Set(peak_camera_handle hCam, uint8_t hysteresis);
    static PEAK_API_STATUS peak_IPL_AutoFocus_Hysteresis_Get(peak_camera_handle hCam, uint8_t * hysteresis);
    static PEAK_API_STATUS peak_IPL_AutoFocus_Hysteresis_GetRange(peak_camera_handle hCam, uint8_t * minHysteresis, uint8_t * maxHysteresis, uint8_t * incHysteresis);
    static PEAK_API_STATUS peak_IPL_HotpixelCorrection_Sensitivity_Set(peak_camera_handle hCam, peak_hotpixel_correction_sensitivity hotpixelCorrectionSensitivity);
    static PEAK_API_STATUS peak_IPL_HotpixelCorrection_Sensitivity_Get(peak_camera_handle hCam, peak_hotpixel_correction_sensitivity * hotpixelCorrectionSensitivity);
    static PEAK_API_STATUS peak_IPL_HotpixelCorrection_GetList(peak_camera_handle hCam, peak_position * hotpixelList, size_t * hotpixelCount);
    static PEAK_API_STATUS peak_IPL_HotpixelCorrection_SetList(peak_camera_handle hCam, const peak_position * hotpixelList, size_t hotpixelCount);
    static PEAK_API_STATUS peak_IPL_HotpixelCorrection_ResetList(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_IPL_HotpixelCorrection_Enable(peak_camera_handle hCam, peak_bool enabled);
    static PEAK_API_BOOL peak_IPL_HotpixelCorrection_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_IPL_Mirror_UpDown_Enable(peak_camera_handle hCam, peak_bool enabled);
    static PEAK_API_BOOL peak_IPL_Mirror_UpDown_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_IPL_Mirror_LeftRight_Enable(peak_camera_handle hCam, peak_bool enabled);
    static PEAK_API_BOOL peak_IPL_Mirror_LeftRight_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_IPL_ProcessFrame(peak_camera_handle hCam, peak_frame_handle hFrame, peak_frame_handle * hResultFrame);
    static PEAK_API_STATUS peak_IPL_ProcessFrameInplace(peak_camera_handle hCam, peak_frame_handle hFrame);
    static PEAK_API_STATUS peak_IPL_ReadImage(peak_camera_handle hCam, const char * path, peak_frame_handle * hFrame);
    static PEAK_API_STATUS peak_IPL_EdgeEnhancement_Enable(peak_camera_handle hCam, peak_bool enable);
    static PEAK_API_BOOL peak_IPL_EdgeEnhancement_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_IPL_EdgeEnhancement_Factor_Set(peak_camera_handle hCam, uint32_t factor);
    static PEAK_API_STATUS peak_IPL_EdgeEnhancement_Factor_Get(peak_camera_handle hCam, uint32_t * factor);
    static PEAK_API_STATUS peak_IPL_EdgeEnhancement_Factor_GetDefault(peak_camera_handle hCam, uint32_t * defaultFactor);
    static PEAK_API_STATUS peak_IPL_EdgeEnhancement_Factor_GetRange(peak_camera_handle hCam, uint32_t * minFactor, uint32_t * maxFactor, uint32_t * incFactor);
    static PEAK_API_STATUS peak_IPL_Sharpness_Measure(peak_frame_handle hFrame, peak_roi roi, peak_sharpness_algorithm sharpnessAlgorithm, double * calculatedValue);
    static PEAK_API_STATUS peak_IPL_Sharpness_GetList(peak_sharpness_algorithm sharpnessAlgorithm, peak_pixel_format * pixelFormatList, size_t * pixelFormatSize);
    static PEAK_API_STATUS peak_IPL_Rotation_Angle_Set(peak_camera_handle hCam, int32_t rotationAngle);
    static PEAK_API_STATUS peak_IPL_Rotation_Angle_Get(peak_camera_handle hCam, int32_t * rotationAngle);
    static PEAK_API_STATUS peak_IPL_Histogram_ProcessFrame(peak_frame_handle hFrame, peak_histogram_handle * hHistogram);
    static PEAK_API_STATUS peak_IPL_Histogram_Release(peak_histogram_handle hHistogram);
    static PEAK_API_STATUS peak_IPL_Histogram_Channel_GetCount(peak_histogram_handle hHistogram, size_t * numChannels);
    static PEAK_API_STATUS peak_IPL_Histogram_Channel_GetInfo(peak_histogram_handle hHistogram, size_t channel, peak_histogram_channel_info * channelInfo);
    static PEAK_API_STATUS peak_IPL_Histogram_Channel_GetBinArray(peak_histogram_handle hHistogram, size_t channel, uint64_t * binArray, size_t * binArraySize);
    static PEAK_API_STATUS peak_VideoWriter_Open(peak_video_handle * hVideo, const char * fileName, peak_video_container container, peak_video_encoder encoder);
    static PEAK_API_STATUS peak_VideoWriter_Close(peak_video_handle hVideo);
    static PEAK_API_STATUS peak_VideoWriter_AddFrame(peak_video_handle hVideo, peak_frame_handle hFrame);
    static PEAK_API_STATUS peak_VideoWriter_Container_GetEncoderList(peak_video_container container, peak_video_encoder * encoderList, size_t * encoderCount);
    static PEAK_API_STATUS peak_VideoWriter_Encoder_GetPixelFormatList(peak_video_encoder encoder, peak_pixel_format * pixelFormatList, size_t * pixelFormatCount);
    static PEAK_API_STATUS peak_VideoWriter_Encoder_GetContainerList(peak_video_encoder encoder, peak_video_container * containerList, size_t * containerCount);
    static PEAK_API_STATUS peak_VideoWriter_GetInfo(peak_video_handle hVideo, peak_video_info * videoInfo);
    static PEAK_API_STATUS peak_VideoWriter_Container_Option_Set(peak_video_handle hVideo, peak_video_container_option containerOption, const void * value, size_t count);
    static PEAK_API_STATUS peak_VideoWriter_Container_Option_Get(peak_video_handle hVideo, peak_video_container_option containerOption, void * value, size_t count, size_t * outCount);
    static PEAK_API_STATUS peak_VideoWriter_Encoder_Option_Set(peak_video_handle hVideo, peak_video_encoder_option encoderOption, const void * value, size_t count);
    static PEAK_API_STATUS peak_VideoWriter_Encoder_Option_Get(peak_video_handle hVideo, peak_video_encoder_option encoderOption, void * value, size_t count, size_t * outCount);
    static PEAK_API_STATUS peak_VideoWriter_WaitUntilQueueEmpty(peak_video_handle hVideo, int32_t timeout_ms);
    static PEAK_API_STATUS peak_Inference_CNN_Open(const char * path, peak_inference_handle * hInference);
    static PEAK_API_STATUS peak_Inference_CNN_Close(peak_inference_handle hInference);
    static PEAK_API_STATUS peak_Inference_CNN_ProcessFrame(peak_inference_handle hInference, peak_frame_handle hFrame, peak_inference_result_handle * hInferenceHandle);
    static PEAK_API_STATUS peak_Inference_CNN_Info_Get(peak_inference_handle hInference, peak_inference_info * info);
    static PEAK_API_STATUS peak_Inference_Result_Get(peak_inference_result_handle hInferenceHandle, peak_inference_result_data * result);
    static PEAK_API_STATUS peak_Inference_Result_Release(peak_inference_result_handle hInferenceHandle);
    static PEAK_API_STATUS peak_Inference_Result_Classification_GetList(peak_inference_result_handle hInferenceHandle, peak_inference_result_classification * resultList, size_t * resultCount);
    static PEAK_API_STATUS peak_Inference_Result_Detection_GetList(peak_inference_result_handle hInferenceHandle, peak_inference_result_detection * resultList, size_t * resultCount);
    static PEAK_API_STATUS peak_Inference_Statistics_Get(peak_inference_handle hInference, peak_inference_statistics * statistics);
    static PEAK_API_STATUS peak_Inference_Statistics_Reset(peak_inference_handle hInference);
    static PEAK_API_STATUS peak_Inference_ConfidenceThreshold_Get(peak_inference_handle hInference, uint32_t * threshold);
    static PEAK_API_STATUS peak_Inference_ConfidenceThreshold_GetRange(peak_inference_handle hInference, uint32_t * minThreshold, uint32_t * maxThreshold, uint32_t * incThreshold);
    static PEAK_API_STATUS peak_Inference_ConfidenceThreshold_Set(peak_inference_handle hInference, uint32_t threshold);
    static PEAK_API_STATUS peak_MessageQueue_Create(peak_message_queue_handle * hMessageQueue);
    static PEAK_API_STATUS peak_MessageQueue_Destroy(peak_message_queue_handle hMessageQueue);
    static PEAK_API_STATUS peak_MessageQueue_EnableMessage(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type messageType);
    static PEAK_API_STATUS peak_MessageQueue_DisableMessage(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type messageType);
    static PEAK_API_STATUS peak_MessageQueue_EnableMessageList(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, const peak_message_type * messageTypesArray, size_t messageTypesArraySize);
    static PEAK_API_STATUS peak_MessageQueue_DisableMessageList(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, const peak_message_type * messageTypesArray, size_t messageTypesArraySize);
    static PEAK_API_STATUS peak_MessageQueue_EnabledMessages_GetList(peak_message_queue_handle hMessageQueue, peak_message_type * messageTypesArray, size_t * messageTypesArraySize);
    static PEAK_API_STATUS peak_MessageQueue_SetMode(peak_message_queue_handle hMessageQueue, peak_message_queue_mode queueMode);
    static PEAK_API_STATUS peak_MessageQueue_GetMode(peak_message_queue_handle hMessageQueue, peak_message_queue_mode * queueMode);
    static PEAK_API_STATUS peak_MessageQueue_Start(peak_message_queue_handle hMessageQueue);
    static PEAK_API_STATUS peak_MessageQueue_Stop(peak_message_queue_handle hMessageQueue);
    static PEAK_API_BOOL peak_MessageQueue_IsStarted(peak_message_queue_handle hMessageQueue);
    static PEAK_API_STATUS peak_MessageQueue_WaitForMessage(peak_message_queue_handle hMessageQueue, uint32_t timeout_ms, peak_message_handle * hMessage);
    static PEAK_API_STATUS peak_MessageQueue_Flush(peak_message_queue_handle hMessageQueue);
    static PEAK_API_BOOL peak_MessageQueue_IsMessageSupported(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type messageType);
    static PEAK_API_STATUS peak_MessageQueue_Statistics_Get(peak_message_queue_handle hMessageQueue, peak_message_queue_statistics_info * messageQueueInfo);
    static PEAK_API_STATUS peak_MessageQueue_Statistics_Reset(peak_message_queue_handle hMessageQueue);
    static PEAK_API_STATUS peak_MessageQueue_MaxQueueSize_Set(peak_message_queue_handle hMessageQueue, size_t messageQueueMaxSize);
    static PEAK_API_STATUS peak_MessageQueue_MaxQueueSize_Get(peak_message_queue_handle hMessageQueue, size_t * messageQueueMaxSize);
    static PEAK_API_STATUS peak_Message_Release(peak_message_handle hMessage);
    static PEAK_API_STATUS peak_Message_GetInfo(peak_message_handle hMessage, peak_message_info * messageInfo);
    static PEAK_API_STATUS peak_Message_Type_Get(peak_message_handle hMessage, peak_message_type * messageType);
    static PEAK_API_STATUS peak_Message_CameraHandle_Get(peak_message_handle hMessage, peak_camera_handle * hCam);
    static PEAK_API_STATUS peak_Message_ID_Get(peak_message_handle hMessage, uint64_t * messageID);
    static PEAK_API_STATUS peak_Message_HostTimestamp_Get(peak_message_handle hMessage, uint64_t * hostTimestamp_ns);
    static PEAK_API_STATUS peak_Message_Data_Type_Get(peak_message_handle hMessage, peak_message_data_type * messageType);
    static PEAK_API_STATUS peak_Message_Data_RemoteDevice_Get(peak_message_handle hMessage, peak_message_data_remote_device * message);
    static PEAK_API_STATUS peak_Message_Data_RemoteDeviceError_Get(peak_message_handle hMessage, peak_message_data_remote_device_error * message);
    static PEAK_API_STATUS peak_Message_Data_RemoteDeviceDropped_Get(peak_message_handle hMessage, peak_message_data_remote_device_dropped * message);
    static PEAK_API_STATUS peak_Message_Data_RemoteDeviceFrame_Get(peak_message_handle hMessage, peak_message_data_remote_device_frame * message);
    static PEAK_API_STATUS peak_Message_Data_RemoteDeviceTemperature_Get(peak_message_handle hMessage, peak_message_data_remote_device_temperature * message);
    static PEAK_API_STATUS peak_Message_Data_AutoFocusData_Get(peak_message_handle hMessage, peak_message_data_autofocus * message);
    static PEAK_API_STATUS peak_Message_Data_DeviceFound_Get(peak_message_handle hMessage, peak_message_data_device_found * message);
    static PEAK_API_STATUS peak_Message_Data_DeviceLost_Get(peak_message_handle hMessage, peak_message_data_device_lost * message);
    static PEAK_API_STATUS peak_Message_Data_DeviceReconnected_Get(peak_message_handle hMessage, peak_message_data_device_reconnected * message);
    static PEAK_API_STATUS peak_Message_Data_DeviceDisconnected_Get(peak_message_handle hMessage, peak_message_data_device_disconnected * message);
    static PEAK_API_STATUS peak_Message_Data_FirmwareUpdate_Get(peak_message_handle hMessage, peak_message_data_firmware_update * message);
    static PEAK_API_STATUS peak_I2C_Create(peak_camera_handle hCam, peak_i2c_handle * hI2C);
    static PEAK_API_STATUS peak_I2C_Destroy(peak_i2c_handle hI2C);
    static PEAK_API_ACCESS_STATUS peak_I2C_GetAccessStatus(peak_i2c_handle hI2C);
    static PEAK_API_STATUS peak_I2C_Mode_GetList(peak_i2c_handle hI2C, peak_i2c_mode * modeList, size_t * modeListSize);
    static PEAK_API_STATUS peak_I2C_Mode_Set(peak_i2c_handle hI2C, peak_i2c_mode mode);
    static PEAK_API_STATUS peak_I2C_Mode_Get(peak_i2c_handle hI2C, peak_i2c_mode * mode);
    static PEAK_API_STATUS peak_I2C_DeviceAddress_GetRange(peak_i2c_handle hI2C, uint32_t * minAddress, uint32_t * maxAddress);
    static PEAK_API_STATUS peak_I2C_DeviceAddress_Set(peak_i2c_handle hI2C, uint32_t address);
    static PEAK_API_STATUS peak_I2C_DeviceAddress_Get(peak_i2c_handle hI2C, uint32_t * address);
    static PEAK_API_STATUS peak_I2C_RegisterAddress_Length_GetList(peak_i2c_handle hI2C, peak_i2c_register_address_length * i2cLengthList, size_t * i2cLengthCount);
    static PEAK_API_STATUS peak_I2C_RegisterAddress_Length_Set(peak_i2c_handle hI2C, peak_i2c_register_address_length length);
    static PEAK_API_STATUS peak_I2C_RegisterAddress_Length_Get(peak_i2c_handle hI2C, peak_i2c_register_address_length * length);
    static PEAK_API_STATUS peak_I2C_RegisterAddress_Endianness_Set(peak_i2c_handle hI2C, peak_endianness endianness);
    static PEAK_API_STATUS peak_I2C_RegisterAddress_Endianness_Get(peak_i2c_handle hI2C, peak_endianness * endianness);
    static PEAK_API_STATUS peak_I2C_RegisterAddress_Set(peak_i2c_handle hI2C, uint32_t address);
    static PEAK_API_STATUS peak_I2C_RegisterAddress_Get(peak_i2c_handle hI2C, uint32_t * address);
    static PEAK_API_STATUS peak_I2C_AckPolling_Enable(peak_i2c_handle hI2C, peak_bool enabled);
    static PEAK_API_BOOL peak_I2C_AckPolling_IsEnabled(peak_i2c_handle hI2C);
    static PEAK_API_ACCESS_STATUS peak_I2C_AckPolling_Timeout_GetAccessStatus(peak_i2c_handle hI2C);
    static PEAK_API_STATUS peak_I2C_AckPolling_Timeout_GetRange(peak_i2c_handle hI2C, uint32_t * minTimeout_ms, uint32_t * maxTimeout_ms, uint32_t * incTimeout_ms);
    static PEAK_API_STATUS peak_I2C_AckPolling_Timeout_Set(peak_i2c_handle hI2C, uint32_t timeout_ms);
    static PEAK_API_STATUS peak_I2C_AckPolling_Timeout_Get(peak_i2c_handle hI2C, uint32_t * timeout_ms);
    static PEAK_API_STATUS peak_I2C_Data_Write(peak_i2c_handle hI2C, const uint8_t * data, size_t dataSize);
    static PEAK_API_STATUS peak_I2C_Data_Read(peak_i2c_handle hI2C, size_t maxDataSize, uint8_t * data, size_t * dataSize);
    static PEAK_API_STATUS peak_I2C_Data_MaxSize_Get(peak_i2c_handle hI2C, size_t * maxDataSize);
    static PEAK_API_STATUS peak_I2C_OperationStatus_Get(peak_i2c_handle hI2C, peak_i2c_operation_status * operationStatus);
    static PEAK_API_STATUS peak_IPL_ImageWriter_Create(peak_imagewriter_handle * hImageWriter);
    static PEAK_API_STATUS peak_IPL_ImageWriter_Destroy(peak_imagewriter_handle hImageWriter);
    static PEAK_API_STATUS peak_IPL_ImageWriter_Save(peak_imagewriter_handle hImageWriter, peak_frame_handle hFrame, const char * fileName);
    static PEAK_API_STATUS peak_IPL_ImageWriter_Format_Set(peak_imagewriter_handle hImageWriter, peak_imagefile_format imageFormat);
    static PEAK_API_STATUS peak_IPL_ImageWriter_Format_Get(peak_imagewriter_handle hImageWriter, peak_imagefile_format * imageFormat);
    static PEAK_API_STATUS peak_IPL_ImageWriter_Compression_Set(peak_imagewriter_handle hImageWriter, uint32_t compression);
    static PEAK_API_STATUS peak_IPL_ImageWriter_Compression_Get(peak_imagewriter_handle hImageWriter, uint32_t * compression);
    static PEAK_API_STATUS peak_IPL_Binning_FactorX_GetList(peak_camera_handle hCam, uint32_t * binningFactorXList, size_t * binningFactorXCount);
    static PEAK_API_STATUS peak_IPL_Binning_FactorY_GetList(peak_camera_handle hCam, uint32_t * binningFactorYList, size_t * binningFactorYCount);
    static PEAK_API_STATUS peak_IPL_Binning_Set(peak_camera_handle hCam, uint32_t binningFactorX, uint32_t binningFactorY);
    static PEAK_API_STATUS peak_IPL_Binning_Get(peak_camera_handle hCam, uint32_t * binningFactorX, uint32_t * binningFactorY);
    static PEAK_API_STATUS peak_IPL_Decimation_FactorX_GetList(peak_camera_handle hCam, uint32_t * decimationFactorXList, size_t * decimationFactorXCount);
    static PEAK_API_STATUS peak_IPL_Decimation_FactorY_GetList(peak_camera_handle hCam, uint32_t * decimationFactorYList, size_t * decimationFactorYCount);
    static PEAK_API_STATUS peak_IPL_Decimation_Set(peak_camera_handle hCam, uint32_t decimationFactorX, uint32_t decimationFactorY);
    static PEAK_API_STATUS peak_IPL_Decimation_Get(peak_camera_handle hCam, uint32_t * decimationFactorX, uint32_t * decimationFactorY);
    static PEAK_API_STATUS peak_FirmwareUpdate_CompatibleCameraList_Get(const char * gufFileName, peak_camera_descriptor * cameraList, size_t * cameraCount);
    static PEAK_API_STATUS peak_FirmwareUpdate_Execute(peak_camera_id cameraID, const char * gufFileName);
    static PEAK_API_ACCESS_STATUS peak_TestPattern_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_TestPattern_Set(peak_camera_handle hCam, peak_test_pattern pattern);
    static PEAK_API_STATUS peak_TestPattern_Get(peak_camera_handle hCam, peak_test_pattern * pattern);
    static PEAK_API_STATUS peak_TestPattern_GetList(peak_camera_handle hCam, peak_test_pattern * testPatternList, size_t * testPatternCount);
    static PEAK_API_ACCESS_STATUS peak_LED_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_LED_Target_GetList(peak_camera_handle hCam, peak_led_target * targetList, size_t * targetCount);
    static PEAK_API_STATUS peak_LED_Mode_GetList(peak_camera_handle hCam, peak_led_target target, peak_led_mode * modeList, size_t * ledModeCount);
    static PEAK_API_STATUS peak_LED_Set(peak_camera_handle hCam, peak_led_target target, peak_led_mode mode);
    static PEAK_API_STATUS peak_LED_Get(peak_camera_handle hCam, peak_led_target target, peak_led_mode * mode);
    static PEAK_API_ACCESS_STATUS peak_BlackLevel_Auto_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_BlackLevel_Auto_Enable(peak_camera_handle hCam, peak_bool enable);
    static PEAK_API_BOOL peak_BlackLevel_Auto_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_ACCESS_STATUS peak_BlackLevel_Offset_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_BlackLevel_Offset_Set(peak_camera_handle hCam, double offset);
    static PEAK_API_STATUS peak_BlackLevel_Offset_Get(peak_camera_handle hCam, double * offset);
    static PEAK_API_STATUS peak_BlackLevel_Offset_GetRange(peak_camera_handle hCam, double * min, double * max, double * inc);
    static PEAK_API_ACCESS_STATUS peak_Bandwidth_LinkSpeed_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Bandwidth_LinkSpeed_Get(peak_camera_handle hCam, int64_t * linkSpeed_Bps);
    static PEAK_API_ACCESS_STATUS peak_Bandwidth_ThroughputLimit_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Bandwidth_ThroughputLimit_GetRange(peak_camera_handle hCam, int64_t * minThroughputLimit_Bps, int64_t * maxThroughputLimit_Bps, int64_t * incThroughputLimit_Bps);
    static PEAK_API_STATUS peak_Bandwidth_ThroughputLimit_Set(peak_camera_handle hCam, int64_t throughputLimit_Bps);
    static PEAK_API_STATUS peak_Bandwidth_ThroughputLimit_Get(peak_camera_handle hCam, int64_t * throughputLimit_Bps);
    static PEAK_API_ACCESS_STATUS peak_Bandwidth_ThroughputFrameRateLimit_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Bandwidth_ThroughputFrameRateLimit_Get(peak_camera_handle hCam, double * throughputFrameRateLimit_fps);
    static PEAK_API_ACCESS_STATUS peak_Bandwidth_ThroughputCalculated_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Bandwidth_ThroughputCalculated_Get(peak_camera_handle hCam, int64_t * throughputCalculated_Bps);
    static PEAK_API_ACCESS_STATUS peak_IPO_GetAccessStatus(peak_interface_technology interfaceTech);
    static PEAK_API_BOOL peak_IPO_IsEnabled(peak_interface_technology interfaceTech);
    static PEAK_API_STATUS peak_IPO_Enable(peak_interface_technology interfaceTech, peak_bool enabled);
    static PEAK_API_STATUS peak_IPL_DigitalBlack_GetRange(peak_camera_handle hCam, double * minDigitalBlack, double * maxDigitalBlack);
    static PEAK_API_STATUS peak_IPL_DigitalBlack_Set(peak_camera_handle hCam, double digitalBlack);
    static PEAK_API_STATUS peak_IPL_DigitalBlack_Get(peak_camera_handle hCam, double * digitalBlack);
    static PEAK_API_STATUS peak_IPL_LUT_Enable(peak_camera_handle hCam, peak_bool enabled);
    static PEAK_API_BOOL peak_IPL_LUT_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_IPL_LUT_Preset_Set(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_preset preset);
    static PEAK_API_STATUS peak_IPL_LUT_Value_Set(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_channel channel, uint32_t index, uint32_t value);
    static PEAK_API_STATUS peak_IPL_LUT_Value_Get(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_channel channel, uint32_t index, uint32_t * value);
    static PEAK_API_STATUS peak_IPL_LUT_ValueList_Set(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_channel channel, uint32_t * values, size_t size);
    static PEAK_API_STATUS peak_IPL_LUT_ValueList_Get(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_channel channel, uint32_t * values, size_t * size);
    static PEAK_API_ACCESS_STATUS peak_Chunks_GetAccessStatus(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Chunks_Enable(peak_camera_handle hCam, peak_bool enabled);
    static PEAK_API_BOOL peak_Chunks_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Chunks_AutoUpdate_Enable(peak_camera_handle hCam, peak_bool enabled);
    static PEAK_API_BOOL peak_Chunks_AutoUpdate_IsEnabled(peak_camera_handle hCam);
    static PEAK_API_STATUS peak_Chunks_Update(peak_camera_handle hCam, peak_frame_handle hFrame);
    static PEAK_API_ACCESS_STATUS peak_Chunks_Type_GetAccessStatus(peak_camera_handle hCam, peak_chunks_type type);
    static PEAK_API_STATUS peak_Chunks_Type_Enable(peak_camera_handle hCam, peak_chunks_type type, peak_bool enabled);
    static PEAK_API_BOOL peak_Chunks_Type_IsEnabled(peak_camera_handle hCam, peak_chunks_type type);
    static PEAK_API_STATUS peak_Chunks_Type_Supported_GetList(peak_camera_handle hCam, peak_chunks_type * chunksTypesSupported, size_t * chunksTypesSize);
    static PEAK_API_STATUS peak_Chunks_FrameInfo_Get(peak_camera_handle hCam, peak_chunks_frame_info * data);
    static PEAK_API_STATUS peak_Chunks_Exposure_Get(peak_camera_handle hCam, peak_chunks_exposure * data);
    static PEAK_API_STATUS peak_Chunks_Gain_Get(peak_camera_handle hCam, peak_chunks_gain * data);
    static PEAK_API_STATUS peak_Chunks_Sequencer_Get(peak_camera_handle hCam, peak_chunks_sequencer * data);
    static PEAK_API_STATUS peak_Chunks_Timestamp_Get(peak_camera_handle hCam, peak_chunks_timestamp * data);
    static PEAK_API_STATUS peak_Chunks_ExposureTrigger_Get(peak_camera_handle hCam, peak_chunks_exposure_trigger * data);
    static PEAK_API_STATUS peak_Chunks_UsableROI_Get(peak_camera_handle hCam, peak_chunks_usable_roi * data);
    static PEAK_API_STATUS peak_Chunks_LineStatus_Get(peak_camera_handle hCam, peak_chunks_line_status * data);
    static PEAK_API_STATUS peak_Chunks_AutoFeature_Get(peak_camera_handle hCam, peak_chunks_autofeature * data);
    static PEAK_API_STATUS peak_Chunks_PTPStatus_Get(peak_camera_handle hCam, peak_chunks_ptp_status * data);
       
private:
    void* m_handle = nullptr;
    dyn_peak_Library_Init m_peak_Library_Init{};
    dyn_peak_Library_Exit m_peak_Library_Exit{};
    dyn_peak_Library_GetVersion m_peak_Library_GetVersion{};
    dyn_peak_Library_GetLastError m_peak_Library_GetLastError{};
    dyn_peak_Library_InterfaceTechnology_IsSupported m_peak_Library_InterfaceTechnology_IsSupported{};
    dyn_peak_Reconnect_Enable m_peak_Reconnect_Enable{};
    dyn_peak_Reconnect_IsEnabled m_peak_Reconnect_IsEnabled{};
    dyn_peak_Reconnect_GetAccessStatus m_peak_Reconnect_GetAccessStatus{};
    dyn_peak_CameraList_Update m_peak_CameraList_Update{};
    dyn_peak_CameraList_Get m_peak_CameraList_Get{};
    dyn_peak_Camera_ID_FromHandle m_peak_Camera_ID_FromHandle{};
    dyn_peak_Camera_ID_FromSerialNumber m_peak_Camera_ID_FromSerialNumber{};
    dyn_peak_Camera_ID_FromUserDefinedName m_peak_Camera_ID_FromUserDefinedName{};
    dyn_peak_Camera_ID_FromMAC m_peak_Camera_ID_FromMAC{};
    dyn_peak_Camera_GetAccessStatus m_peak_Camera_GetAccessStatus{};
    dyn_peak_Camera_GetDescriptor m_peak_Camera_GetDescriptor{};
    dyn_peak_Camera_Open m_peak_Camera_Open{};
    dyn_peak_Camera_OpenFirstAvailable m_peak_Camera_OpenFirstAvailable{};
    dyn_peak_Camera_Close m_peak_Camera_Close{};
    dyn_peak_Camera_ResetToDefaultSettings m_peak_Camera_ResetToDefaultSettings{};
    dyn_peak_Camera_UserDefinedName_Set m_peak_Camera_UserDefinedName_Set{};
    dyn_peak_Camera_UserDefinedName_Get m_peak_Camera_UserDefinedName_Get{};
    dyn_peak_Camera_IsConnected m_peak_Camera_IsConnected{};
    dyn_peak_Camera_Reboot m_peak_Camera_Reboot{};
    dyn_peak_EthernetConfig_GetAccessStatus m_peak_EthernetConfig_GetAccessStatus{};
    dyn_peak_EthernetConfig_GetInfo m_peak_EthernetConfig_GetInfo{};
    dyn_peak_EthernetConfig_DHCP_GetAccessStatus m_peak_EthernetConfig_DHCP_GetAccessStatus{};
    dyn_peak_EthernetConfig_DHCP_Enable m_peak_EthernetConfig_DHCP_Enable{};
    dyn_peak_EthernetConfig_DHCP_IsEnabled m_peak_EthernetConfig_DHCP_IsEnabled{};
    dyn_peak_EthernetConfig_PersistentIP_GetAccessStatus m_peak_EthernetConfig_PersistentIP_GetAccessStatus{};
    dyn_peak_EthernetConfig_PersistentIP_Set m_peak_EthernetConfig_PersistentIP_Set{};
    dyn_peak_EthernetConfig_PersistentIP_Get m_peak_EthernetConfig_PersistentIP_Get{};
    dyn_peak_Acquisition_Start m_peak_Acquisition_Start{};
    dyn_peak_Acquisition_Stop m_peak_Acquisition_Stop{};
    dyn_peak_Acquisition_IsStarted m_peak_Acquisition_IsStarted{};
    dyn_peak_Acquisition_WaitForFrame m_peak_Acquisition_WaitForFrame{};
    dyn_peak_Acquisition_GetInfo m_peak_Acquisition_GetInfo{};
    dyn_peak_Acquisition_Buffer_GetRequiredSize m_peak_Acquisition_Buffer_GetRequiredSize{};
    dyn_peak_Acquisition_Buffer_GetRequiredCount m_peak_Acquisition_Buffer_GetRequiredCount{};
    dyn_peak_Acquisition_Buffer_Announce m_peak_Acquisition_Buffer_Announce{};
    dyn_peak_Acquisition_Buffer_Revoke m_peak_Acquisition_Buffer_Revoke{};
    dyn_peak_Acquisition_Buffer_RevokeAll m_peak_Acquisition_Buffer_RevokeAll{};
    dyn_peak_Acquisition_BufferHandling_Mode_GetAccessStatus m_peak_Acquisition_BufferHandling_Mode_GetAccessStatus{};
    dyn_peak_Acquisition_BufferHandling_Mode_GetList m_peak_Acquisition_BufferHandling_Mode_GetList{};
    dyn_peak_Acquisition_BufferHandling_Mode_Set m_peak_Acquisition_BufferHandling_Mode_Set{};
    dyn_peak_Acquisition_BufferHandling_Mode_Get m_peak_Acquisition_BufferHandling_Mode_Get{};
    dyn_peak_Acquisition_LossHandling_Mode_GetAccessStatus m_peak_Acquisition_LossHandling_Mode_GetAccessStatus{};
    dyn_peak_Acquisition_LossHandling_Mode_GetList m_peak_Acquisition_LossHandling_Mode_GetList{};
    dyn_peak_Acquisition_LossHandling_Mode_Set m_peak_Acquisition_LossHandling_Mode_Set{};
    dyn_peak_Acquisition_LossHandling_Mode_Get m_peak_Acquisition_LossHandling_Mode_Get{};
    dyn_peak_Acquisition_LossHandling_Extent_GetAccessStatus m_peak_Acquisition_LossHandling_Extent_GetAccessStatus{};
    dyn_peak_Acquisition_LossHandling_Extent_GetRange m_peak_Acquisition_LossHandling_Extent_GetRange{};
    dyn_peak_Acquisition_LossHandling_Extent_Set m_peak_Acquisition_LossHandling_Extent_Set{};
    dyn_peak_Acquisition_LossHandling_Extent_Get m_peak_Acquisition_LossHandling_Extent_Get{};
    dyn_peak_Acquisition_LossHandling_FrameAbortTimeout_GetAccessStatus m_peak_Acquisition_LossHandling_FrameAbortTimeout_GetAccessStatus{};
    dyn_peak_Acquisition_LossHandling_FrameAbortTimeout_GetRange m_peak_Acquisition_LossHandling_FrameAbortTimeout_GetRange{};
    dyn_peak_Acquisition_LossHandling_FrameAbortTimeout_Set m_peak_Acquisition_LossHandling_FrameAbortTimeout_Set{};
    dyn_peak_Acquisition_LossHandling_FrameAbortTimeout_Get m_peak_Acquisition_LossHandling_FrameAbortTimeout_Get{};
    dyn_peak_Acquisition_LossHandling_ResendRequestTimeout_GetAccessStatus m_peak_Acquisition_LossHandling_ResendRequestTimeout_GetAccessStatus{};
    dyn_peak_Acquisition_LossHandling_ResendRequestTimeout_GetRange m_peak_Acquisition_LossHandling_ResendRequestTimeout_GetRange{};
    dyn_peak_Acquisition_LossHandling_ResendRequestTimeout_Set m_peak_Acquisition_LossHandling_ResendRequestTimeout_Set{};
    dyn_peak_Acquisition_LossHandling_ResendRequestTimeout_Get m_peak_Acquisition_LossHandling_ResendRequestTimeout_Get{};
    dyn_peak_PixelFormat_GetInfo m_peak_PixelFormat_GetInfo{};
    dyn_peak_Frame_Release m_peak_Frame_Release{};
    dyn_peak_Frame_GetInfo m_peak_Frame_GetInfo{};
    dyn_peak_Frame_Type_Get m_peak_Frame_Type_Get{};
    dyn_peak_Frame_Buffer_Get m_peak_Frame_Buffer_Get{};
    dyn_peak_Frame_ID_Get m_peak_Frame_ID_Get{};
    dyn_peak_Frame_Timestamp_Get m_peak_Frame_Timestamp_Get{};
    dyn_peak_Frame_ROI_Get m_peak_Frame_ROI_Get{};
    dyn_peak_Frame_PixelFormat_Get m_peak_Frame_PixelFormat_Get{};
    dyn_peak_Frame_IsComplete m_peak_Frame_IsComplete{};
    dyn_peak_Frame_BytesExpected_Get m_peak_Frame_BytesExpected_Get{};
    dyn_peak_Frame_BytesWritten_Get m_peak_Frame_BytesWritten_Get{};
    dyn_peak_Frame_ProcessingTime_Get m_peak_Frame_ProcessingTime_Get{};
    dyn_peak_Frame_Save m_peak_Frame_Save{};
    dyn_peak_Frame_HasChunks m_peak_Frame_HasChunks{};
    dyn_peak_CameraSettings_ParameterSet_GetAccessStatus m_peak_CameraSettings_ParameterSet_GetAccessStatus{};
    dyn_peak_CameraSettings_ParameterSet_GetList m_peak_CameraSettings_ParameterSet_GetList{};
    dyn_peak_CameraSettings_ParameterSet_Store m_peak_CameraSettings_ParameterSet_Store{};
    dyn_peak_CameraSettings_ParameterSet_Apply m_peak_CameraSettings_ParameterSet_Apply{};
    dyn_peak_CameraSettings_ParameterSet_Startup_GetAccessStatus m_peak_CameraSettings_ParameterSet_Startup_GetAccessStatus{};
    dyn_peak_CameraSettings_ParameterSet_Startup_Set m_peak_CameraSettings_ParameterSet_Startup_Set{};
    dyn_peak_CameraSettings_ParameterSet_Startup_Get m_peak_CameraSettings_ParameterSet_Startup_Get{};
    dyn_peak_CameraSettings_DiskFile_GetAccessStatus m_peak_CameraSettings_DiskFile_GetAccessStatus{};
    dyn_peak_CameraSettings_DiskFile_Store m_peak_CameraSettings_DiskFile_Store{};
    dyn_peak_CameraSettings_DiskFile_Apply m_peak_CameraSettings_DiskFile_Apply{};
    dyn_peak_FrameRate_GetAccessStatus m_peak_FrameRate_GetAccessStatus{};
    dyn_peak_FrameRate_GetRange m_peak_FrameRate_GetRange{};
    dyn_peak_FrameRate_Set m_peak_FrameRate_Set{};
    dyn_peak_FrameRate_Get m_peak_FrameRate_Get{};
    dyn_peak_ExposureTime_GetAccessStatus m_peak_ExposureTime_GetAccessStatus{};
    dyn_peak_ExposureTime_GetRange m_peak_ExposureTime_GetRange{};
    dyn_peak_ExposureTime_Set m_peak_ExposureTime_Set{};
    dyn_peak_ExposureTime_Get m_peak_ExposureTime_Get{};
    dyn_peak_ShutterMode_GetAccessStatus m_peak_ShutterMode_GetAccessStatus{};
    dyn_peak_ShutterMode_GetList m_peak_ShutterMode_GetList{};
    dyn_peak_ShutterMode_Set m_peak_ShutterMode_Set{};
    dyn_peak_ShutterMode_Get m_peak_ShutterMode_Get{};
    dyn_peak_PixelClock_GetAccessStatus m_peak_PixelClock_GetAccessStatus{};
    dyn_peak_PixelClock_HasRange m_peak_PixelClock_HasRange{};
    dyn_peak_PixelClock_GetRange m_peak_PixelClock_GetRange{};
    dyn_peak_PixelClock_GetList m_peak_PixelClock_GetList{};
    dyn_peak_PixelClock_Set m_peak_PixelClock_Set{};
    dyn_peak_PixelClock_Get m_peak_PixelClock_Get{};
    dyn_peak_IOChannel_GetAccessStatus m_peak_IOChannel_GetAccessStatus{};
    dyn_PEAK_API_STATUS_DEPRECATED m_PEAK_API_STATUS_DEPRECATED{};
    dyn_peak_IOChannel_GetListForDirection m_peak_IOChannel_GetListForDirection{};
    dyn_peak_IOChannel_Direction_GetAccessStatus m_peak_IOChannel_Direction_GetAccessStatus{};
    dyn_peak_IOChannel_Direction_Get m_peak_IOChannel_Direction_Get{};
    dyn_peak_IOChannel_Type_GetAccessStatus m_peak_IOChannel_Type_GetAccessStatus{};
    dyn_peak_IOChannel_Type_Get m_peak_IOChannel_Type_Get{};
    dyn_peak_IOChannel_Level_GetAccessStatus m_peak_IOChannel_Level_GetAccessStatus{};
    dyn_peak_IOChannel_Level_IsHigh m_peak_IOChannel_Level_IsHigh{};
    dyn_peak_IOChannel_Level_SetHigh m_peak_IOChannel_Level_SetHigh{};
    dyn_peak_IOChannel_Inverter_GetAccessStatus m_peak_IOChannel_Inverter_GetAccessStatus{};
    dyn_peak_IOChannel_Inverter_Enable m_peak_IOChannel_Inverter_Enable{};
    dyn_peak_IOChannel_Inverter_IsEnabled m_peak_IOChannel_Inverter_IsEnabled{};
    dyn_peak_IOChannel_NoiseFilter_GetAccessStatus m_peak_IOChannel_NoiseFilter_GetAccessStatus{};
    dyn_peak_IOChannel_NoiseFilter_Enable m_peak_IOChannel_NoiseFilter_Enable{};
    dyn_peak_IOChannel_NoiseFilter_IsEnabled m_peak_IOChannel_NoiseFilter_IsEnabled{};
    dyn_peak_IOChannel_NoiseFilter_Duration_GetRange m_peak_IOChannel_NoiseFilter_Duration_GetRange{};
    dyn_peak_IOChannel_NoiseFilter_Duration_Set m_peak_IOChannel_NoiseFilter_Duration_Set{};
    dyn_peak_IOChannel_NoiseFilter_Duration_Get m_peak_IOChannel_NoiseFilter_Duration_Get{};
    dyn_peak_Trigger_GetAccessStatus m_peak_Trigger_GetAccessStatus{};
    dyn_peak_Trigger_Enable m_peak_Trigger_Enable{};
    dyn_peak_Trigger_IsEnabled m_peak_Trigger_IsEnabled{};
    dyn_peak_Trigger_IsExecutable m_peak_Trigger_IsExecutable{};
    dyn_peak_Trigger_Execute m_peak_Trigger_Execute{};
    dyn_peak_Trigger_Mode_GetAccessStatus m_peak_Trigger_Mode_GetAccessStatus{};
    dyn_peak_Trigger_Mode_Set m_peak_Trigger_Mode_Set{};
    dyn_PEAK_API_STATUS_DEPRECATED m_PEAK_API_STATUS_DEPRECATED{};
    dyn_peak_Trigger_Mode_Config_Get m_peak_Trigger_Mode_Config_Get{};
    dyn_peak_Trigger_Edge_GetAccessStatus m_peak_Trigger_Edge_GetAccessStatus{};
    dyn_peak_Trigger_Edge_GetList m_peak_Trigger_Edge_GetList{};
    dyn_peak_Trigger_Edge_Set m_peak_Trigger_Edge_Set{};
    dyn_peak_Trigger_Edge_Get m_peak_Trigger_Edge_Get{};
    dyn_peak_Trigger_Delay_GetAccessStatus m_peak_Trigger_Delay_GetAccessStatus{};
    dyn_peak_Trigger_Delay_GetRange m_peak_Trigger_Delay_GetRange{};
    dyn_peak_Trigger_Delay_Set m_peak_Trigger_Delay_Set{};
    dyn_peak_Trigger_Delay_Get m_peak_Trigger_Delay_Get{};
    dyn_peak_Trigger_Divider_GetAccessStatus m_peak_Trigger_Divider_GetAccessStatus{};
    dyn_peak_Trigger_Divider_GetRange m_peak_Trigger_Divider_GetRange{};
    dyn_peak_Trigger_Divider_Set m_peak_Trigger_Divider_Set{};
    dyn_peak_Trigger_Divider_Get m_peak_Trigger_Divider_Get{};
    dyn_peak_Trigger_Burst_GetAccessStatus m_peak_Trigger_Burst_GetAccessStatus{};
    dyn_peak_Trigger_Burst_GetRange m_peak_Trigger_Burst_GetRange{};
    dyn_peak_Trigger_Burst_Set m_peak_Trigger_Burst_Set{};
    dyn_peak_Trigger_Burst_Get m_peak_Trigger_Burst_Get{};
    dyn_peak_Flash_GetAccessStatus m_peak_Flash_GetAccessStatus{};
    dyn_peak_Flash_Enable m_peak_Flash_Enable{};
    dyn_peak_Flash_IsEnabled m_peak_Flash_IsEnabled{};
    dyn_peak_Flash_Mode_GetAccessStatus m_peak_Flash_Mode_GetAccessStatus{};
    dyn_peak_Flash_Mode_Set m_peak_Flash_Mode_Set{};
    dyn_PEAK_API_STATUS_DEPRECATED m_PEAK_API_STATUS_DEPRECATED{};
    dyn_peak_Flash_Mode_Config_Get m_peak_Flash_Mode_Config_Get{};
    dyn_peak_Flash_StartDelay_GetAccessStatus m_peak_Flash_StartDelay_GetAccessStatus{};
    dyn_peak_Flash_StartDelay_GetRange m_peak_Flash_StartDelay_GetRange{};
    dyn_peak_Flash_StartDelay_Set m_peak_Flash_StartDelay_Set{};
    dyn_peak_Flash_StartDelay_Get m_peak_Flash_StartDelay_Get{};
    dyn_peak_Flash_EndDelay_GetAccessStatus m_peak_Flash_EndDelay_GetAccessStatus{};
    dyn_peak_Flash_EndDelay_GetRange m_peak_Flash_EndDelay_GetRange{};
    dyn_peak_Flash_EndDelay_Set m_peak_Flash_EndDelay_Set{};
    dyn_peak_Flash_EndDelay_Get m_peak_Flash_EndDelay_Get{};
    dyn_peak_Flash_Duration_GetAccessStatus m_peak_Flash_Duration_GetAccessStatus{};
    dyn_peak_Flash_Duration_GetRange m_peak_Flash_Duration_GetRange{};
    dyn_peak_Flash_Duration_Set m_peak_Flash_Duration_Set{};
    dyn_peak_Flash_Duration_Get m_peak_Flash_Duration_Get{};
    dyn_peak_Focus_GetAccessStatus m_peak_Focus_GetAccessStatus{};
    dyn_peak_Focus_GetRange m_peak_Focus_GetRange{};
    dyn_peak_Focus_Set m_peak_Focus_Set{};
    dyn_peak_Focus_Get m_peak_Focus_Get{};
    dyn_peak_PixelFormat_GetAccessStatus m_peak_PixelFormat_GetAccessStatus{};
    dyn_peak_PixelFormat_GetList m_peak_PixelFormat_GetList{};
    dyn_peak_PixelFormat_Set m_peak_PixelFormat_Set{};
    dyn_peak_PixelFormat_Get m_peak_PixelFormat_Get{};
    dyn_peak_Gain_GetAccessStatus m_peak_Gain_GetAccessStatus{};
    dyn_peak_Gain_GetChannelList m_peak_Gain_GetChannelList{};
    dyn_peak_Gain_GetRange m_peak_Gain_GetRange{};
    dyn_peak_Gain_Set m_peak_Gain_Set{};
    dyn_peak_Gain_Get m_peak_Gain_Get{};
    dyn_peak_Gamma_GetAccessStatus m_peak_Gamma_GetAccessStatus{};
    dyn_peak_Gamma_GetRange m_peak_Gamma_GetRange{};
    dyn_peak_Gamma_Set m_peak_Gamma_Set{};
    dyn_peak_Gamma_Get m_peak_Gamma_Get{};
    dyn_peak_ColorCorrection_GetAccessStatus m_peak_ColorCorrection_GetAccessStatus{};
    dyn_peak_ColorCorrection_Mode_GetList m_peak_ColorCorrection_Mode_GetList{};
    dyn_peak_ColorCorrection_Mode_Set m_peak_ColorCorrection_Mode_Set{};
    dyn_peak_ColorCorrection_Mode_Get m_peak_ColorCorrection_Mode_Get{};
    dyn_peak_ColorCorrection_Matrix_GetAccessStatus m_peak_ColorCorrection_Matrix_GetAccessStatus{};
    dyn_peak_ColorCorrection_Matrix_GetRange m_peak_ColorCorrection_Matrix_GetRange{};
    dyn_peak_ColorCorrection_Matrix_Set m_peak_ColorCorrection_Matrix_Set{};
    dyn_peak_ColorCorrection_Matrix_Get m_peak_ColorCorrection_Matrix_Get{};
    dyn_peak_ColorCorrection_Enable m_peak_ColorCorrection_Enable{};
    dyn_peak_ColorCorrection_IsEnabled m_peak_ColorCorrection_IsEnabled{};
    dyn_peak_AutoBrightness_GetAccessStatus m_peak_AutoBrightness_GetAccessStatus{};
    dyn_peak_AutoBrightness_Target_GetAccessStatus m_peak_AutoBrightness_Target_GetAccessStatus{};
    dyn_peak_AutoBrightness_Target_GetRange m_peak_AutoBrightness_Target_GetRange{};
    dyn_peak_AutoBrightness_Target_Set m_peak_AutoBrightness_Target_Set{};
    dyn_peak_AutoBrightness_Target_Get m_peak_AutoBrightness_Target_Get{};
    dyn_peak_AutoBrightness_TargetTolerance_GetAccessStatus m_peak_AutoBrightness_TargetTolerance_GetAccessStatus{};
    dyn_peak_AutoBrightness_TargetTolerance_GetRange m_peak_AutoBrightness_TargetTolerance_GetRange{};
    dyn_peak_AutoBrightness_TargetTolerance_Set m_peak_AutoBrightness_TargetTolerance_Set{};
    dyn_peak_AutoBrightness_TargetTolerance_Get m_peak_AutoBrightness_TargetTolerance_Get{};
    dyn_peak_AutoBrightness_TargetPercentile_GetAccessStatus m_peak_AutoBrightness_TargetPercentile_GetAccessStatus{};
    dyn_peak_AutoBrightness_TargetPercentile_GetRange m_peak_AutoBrightness_TargetPercentile_GetRange{};
    dyn_peak_AutoBrightness_TargetPercentile_Set m_peak_AutoBrightness_TargetPercentile_Set{};
    dyn_peak_AutoBrightness_TargetPercentile_Get m_peak_AutoBrightness_TargetPercentile_Get{};
    dyn_peak_AutoBrightness_ROI_GetAccessStatus m_peak_AutoBrightness_ROI_GetAccessStatus{};
    dyn_peak_AutoBrightness_ROI_Mode_Set m_peak_AutoBrightness_ROI_Mode_Set{};
    dyn_peak_AutoBrightness_ROI_Mode_Get m_peak_AutoBrightness_ROI_Mode_Get{};
    dyn_peak_AutoBrightness_ROI_Offset_GetRange m_peak_AutoBrightness_ROI_Offset_GetRange{};
    dyn_peak_AutoBrightness_ROI_Size_GetRange m_peak_AutoBrightness_ROI_Size_GetRange{};
    dyn_peak_AutoBrightness_ROI_Set m_peak_AutoBrightness_ROI_Set{};
    dyn_peak_AutoBrightness_ROI_Get m_peak_AutoBrightness_ROI_Get{};
    dyn_peak_AutoBrightness_Exposure_GetAccessStatus m_peak_AutoBrightness_Exposure_GetAccessStatus{};
    dyn_peak_AutoBrightness_Exposure_Mode_GetList m_peak_AutoBrightness_Exposure_Mode_GetList{};
    dyn_peak_AutoBrightness_Exposure_Mode_Set m_peak_AutoBrightness_Exposure_Mode_Set{};
    dyn_peak_AutoBrightness_Exposure_Mode_Get m_peak_AutoBrightness_Exposure_Mode_Get{};
    dyn_peak_AutoBrightness_Gain_GetAccessStatus m_peak_AutoBrightness_Gain_GetAccessStatus{};
    dyn_peak_AutoBrightness_Gain_Mode_GetList m_peak_AutoBrightness_Gain_Mode_GetList{};
    dyn_peak_AutoBrightness_Gain_Mode_Set m_peak_AutoBrightness_Gain_Mode_Set{};
    dyn_peak_AutoBrightness_Gain_Mode_Get m_peak_AutoBrightness_Gain_Mode_Get{};
    dyn_peak_AutoWhiteBalance_GetAccessStatus m_peak_AutoWhiteBalance_GetAccessStatus{};
    dyn_peak_AutoWhiteBalance_ROI_GetAccessStatus m_peak_AutoWhiteBalance_ROI_GetAccessStatus{};
    dyn_peak_AutoWhiteBalance_ROI_Mode_Set m_peak_AutoWhiteBalance_ROI_Mode_Set{};
    dyn_peak_AutoWhiteBalance_ROI_Mode_Get m_peak_AutoWhiteBalance_ROI_Mode_Get{};
    dyn_peak_AutoWhiteBalance_ROI_Offset_GetRange m_peak_AutoWhiteBalance_ROI_Offset_GetRange{};
    dyn_peak_AutoWhiteBalance_ROI_Size_GetRange m_peak_AutoWhiteBalance_ROI_Size_GetRange{};
    dyn_peak_AutoWhiteBalance_ROI_Set m_peak_AutoWhiteBalance_ROI_Set{};
    dyn_peak_AutoWhiteBalance_ROI_Get m_peak_AutoWhiteBalance_ROI_Get{};
    dyn_peak_AutoWhiteBalance_Mode_Set m_peak_AutoWhiteBalance_Mode_Set{};
    dyn_peak_AutoWhiteBalance_Mode_Get m_peak_AutoWhiteBalance_Mode_Get{};
    dyn_peak_AutoWhiteBalance_Mode_GetList m_peak_AutoWhiteBalance_Mode_GetList{};
    dyn_peak_ROI_GetAccessStatus m_peak_ROI_GetAccessStatus{};
    dyn_peak_ROI_Offset_GetRange m_peak_ROI_Offset_GetRange{};
    dyn_peak_ROI_Offset_GetAccessStatus m_peak_ROI_Offset_GetAccessStatus{};
    dyn_peak_ROI_Offset_Set m_peak_ROI_Offset_Set{};
    dyn_peak_ROI_Offset_Get m_peak_ROI_Offset_Get{};
    dyn_peak_ROI_Size_GetRange m_peak_ROI_Size_GetRange{};
    dyn_peak_ROI_Size_GetAccessStatus m_peak_ROI_Size_GetAccessStatus{};
    dyn_peak_ROI_Size_Set m_peak_ROI_Size_Set{};
    dyn_peak_ROI_Size_Get m_peak_ROI_Size_Get{};
    dyn_peak_ROI_Set m_peak_ROI_Set{};
    dyn_peak_ROI_Get m_peak_ROI_Get{};
    dyn_peak_Binning_GetAccessStatus m_peak_Binning_GetAccessStatus{};
    dyn_peak_Binning_FactorX_GetList m_peak_Binning_FactorX_GetList{};
    dyn_peak_Binning_FactorY_GetList m_peak_Binning_FactorY_GetList{};
    dyn_peak_Binning_Set m_peak_Binning_Set{};
    dyn_peak_Binning_Get m_peak_Binning_Get{};
    dyn_peak_BinningManual_GetAccessStatus m_peak_BinningManual_GetAccessStatus{};
    dyn_peak_BinningManual_FactorX_GetList m_peak_BinningManual_FactorX_GetList{};
    dyn_peak_BinningManual_FactorY_GetList m_peak_BinningManual_FactorY_GetList{};
    dyn_peak_BinningManual_Set m_peak_BinningManual_Set{};
    dyn_peak_BinningManual_Get m_peak_BinningManual_Get{};
    dyn_peak_Decimation_GetAccessStatus m_peak_Decimation_GetAccessStatus{};
    dyn_peak_Decimation_FactorX_GetList m_peak_Decimation_FactorX_GetList{};
    dyn_peak_Decimation_FactorY_GetList m_peak_Decimation_FactorY_GetList{};
    dyn_peak_Decimation_Set m_peak_Decimation_Set{};
    dyn_peak_Decimation_Get m_peak_Decimation_Get{};
    dyn_peak_DecimationManual_GetAccessStatus m_peak_DecimationManual_GetAccessStatus{};
    dyn_peak_DecimationManual_FactorX_GetList m_peak_DecimationManual_FactorX_GetList{};
    dyn_peak_DecimationManual_FactorY_GetList m_peak_DecimationManual_FactorY_GetList{};
    dyn_peak_DecimationManual_Set m_peak_DecimationManual_Set{};
    dyn_peak_DecimationManual_Get m_peak_DecimationManual_Get{};
    dyn_peak_Scaling_GetAccessStatus m_peak_Scaling_GetAccessStatus{};
    dyn_peak_Scaling_FactorX_GetRange m_peak_Scaling_FactorX_GetRange{};
    dyn_peak_Scaling_FactorY_GetRange m_peak_Scaling_FactorY_GetRange{};
    dyn_peak_Scaling_Set m_peak_Scaling_Set{};
    dyn_peak_Scaling_Get m_peak_Scaling_Get{};
    dyn_peak_Mirror_LeftRight_GetAccessStatus m_peak_Mirror_LeftRight_GetAccessStatus{};
    dyn_peak_Mirror_LeftRight_Enable m_peak_Mirror_LeftRight_Enable{};
    dyn_peak_Mirror_LeftRight_IsEnabled m_peak_Mirror_LeftRight_IsEnabled{};
    dyn_peak_Mirror_UpDown_GetAccessStatus m_peak_Mirror_UpDown_GetAccessStatus{};
    dyn_peak_Mirror_UpDown_Enable m_peak_Mirror_UpDown_Enable{};
    dyn_peak_Mirror_UpDown_IsEnabled m_peak_Mirror_UpDown_IsEnabled{};
    dyn_peak_CameraMemory_Area_GetAccessStatus m_peak_CameraMemory_Area_GetAccessStatus{};
    dyn_peak_CameraMemory_Area_GetList m_peak_CameraMemory_Area_GetList{};
    dyn_peak_CameraMemory_Area_Size_Get m_peak_CameraMemory_Area_Size_Get{};
    dyn_peak_CameraMemory_Area_Data_Clear m_peak_CameraMemory_Area_Data_Clear{};
    dyn_peak_CameraMemory_Area_Data_Write m_peak_CameraMemory_Area_Data_Write{};
    dyn_peak_CameraMemory_Area_Data_Read m_peak_CameraMemory_Area_Data_Read{};
    dyn_peak_GFA_EnableWriteAccess m_peak_GFA_EnableWriteAccess{};
    dyn_peak_GFA_IsWriteAccessEnabled m_peak_GFA_IsWriteAccessEnabled{};
    dyn_peak_GFA_Feature_GetAccessStatus m_peak_GFA_Feature_GetAccessStatus{};
    dyn_peak_GFA_Float_HasRange m_peak_GFA_Float_HasRange{};
    dyn_peak_GFA_Float_GetRange m_peak_GFA_Float_GetRange{};
    dyn_peak_GFA_Float_GetList m_peak_GFA_Float_GetList{};
    dyn_peak_GFA_Float_Set m_peak_GFA_Float_Set{};
    dyn_peak_GFA_Float_Get m_peak_GFA_Float_Get{};
    dyn_peak_GFA_Integer_HasRange m_peak_GFA_Integer_HasRange{};
    dyn_peak_GFA_Integer_GetRange m_peak_GFA_Integer_GetRange{};
    dyn_peak_GFA_Integer_GetList m_peak_GFA_Integer_GetList{};
    dyn_peak_GFA_Integer_Set m_peak_GFA_Integer_Set{};
    dyn_peak_GFA_Integer_Get m_peak_GFA_Integer_Get{};
    dyn_peak_GFA_Boolean_Set m_peak_GFA_Boolean_Set{};
    dyn_peak_GFA_Boolean_Get m_peak_GFA_Boolean_Get{};
    dyn_peak_GFA_String_Set m_peak_GFA_String_Set{};
    dyn_peak_GFA_String_Get m_peak_GFA_String_Get{};
    dyn_peak_GFA_Command_Execute m_peak_GFA_Command_Execute{};
    dyn_peak_GFA_Command_WaitForDone m_peak_GFA_Command_WaitForDone{};
    dyn_peak_GFA_Enumeration_GetList m_peak_GFA_Enumeration_GetList{};
    dyn_peak_GFA_EnumerationEntry_GetAccessStatus m_peak_GFA_EnumerationEntry_GetAccessStatus{};
    dyn_peak_GFA_EnumerationEntry_GetAccessStatusBySymbolicValue m_peak_GFA_EnumerationEntry_GetAccessStatusBySymbolicValue{};
    dyn_peak_GFA_EnumerationEntry_GetAccessStatusByIntegerValue m_peak_GFA_EnumerationEntry_GetAccessStatusByIntegerValue{};
    dyn_peak_GFA_Enumeration_Set m_peak_GFA_Enumeration_Set{};
    dyn_peak_GFA_Enumeration_SetBySymbolicValue m_peak_GFA_Enumeration_SetBySymbolicValue{};
    dyn_peak_GFA_Enumeration_SetByIntegerValue m_peak_GFA_Enumeration_SetByIntegerValue{};
    dyn_peak_GFA_Enumeration_Get m_peak_GFA_Enumeration_Get{};
    dyn_peak_GFA_Register_Set m_peak_GFA_Register_Set{};
    dyn_peak_GFA_Register_Get m_peak_GFA_Register_Get{};
    dyn_peak_GFA_Data_Write m_peak_GFA_Data_Write{};
    dyn_peak_GFA_Data_Read m_peak_GFA_Data_Read{};
    dyn_peak_IPL_PixelFormat_GetList m_peak_IPL_PixelFormat_GetList{};
    dyn_peak_IPL_PixelFormat_Set m_peak_IPL_PixelFormat_Set{};
    dyn_peak_IPL_PixelFormat_Get m_peak_IPL_PixelFormat_Get{};
    dyn_peak_IPL_Gain_GetRange m_peak_IPL_Gain_GetRange{};
    dyn_peak_IPL_Gain_Set m_peak_IPL_Gain_Set{};
    dyn_peak_IPL_Gain_Get m_peak_IPL_Gain_Get{};
    dyn_peak_IPL_Gamma_GetRange m_peak_IPL_Gamma_GetRange{};
    dyn_peak_IPL_Gamma_Set m_peak_IPL_Gamma_Set{};
    dyn_peak_IPL_Gamma_Get m_peak_IPL_Gamma_Get{};
    dyn_peak_IPL_ColorCorrection_Matrix_Set m_peak_IPL_ColorCorrection_Matrix_Set{};
    dyn_peak_IPL_ColorCorrection_Matrix_Get m_peak_IPL_ColorCorrection_Matrix_Get{};
    dyn_peak_IPL_ColorCorrection_Saturation_Get m_peak_IPL_ColorCorrection_Saturation_Get{};
    dyn_peak_IPL_ColorCorrection_Saturation_Set m_peak_IPL_ColorCorrection_Saturation_Set{};
    dyn_peak_IPL_ColorCorrection_Saturation_GetRange m_peak_IPL_ColorCorrection_Saturation_GetRange{};
    dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Get m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Get{};
    dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Set m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Set{};
    dyn_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Get m_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Get{};
    dyn_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Set m_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Set{};
    dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Get m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Get{};
    dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Set m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Set{};
    dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_GetRange m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_GetRange{};
    dyn_peak_IPL_ColorCorrection_ChromaticAdaption_Enable m_peak_IPL_ColorCorrection_ChromaticAdaption_Enable{};
    dyn_peak_IPL_ColorCorrection_ChromaticAdaption_IsEnabled m_peak_IPL_ColorCorrection_ChromaticAdaption_IsEnabled{};
    dyn_peak_IPL_ColorCorrection_Enable m_peak_IPL_ColorCorrection_Enable{};
    dyn_peak_IPL_ColorCorrection_IsEnabled m_peak_IPL_ColorCorrection_IsEnabled{};
    dyn_peak_IPL_AutoBrightness_Target_GetRange m_peak_IPL_AutoBrightness_Target_GetRange{};
    dyn_peak_IPL_AutoBrightness_Target_Set m_peak_IPL_AutoBrightness_Target_Set{};
    dyn_peak_IPL_AutoBrightness_Target_Get m_peak_IPL_AutoBrightness_Target_Get{};
    dyn_peak_IPL_AutoBrightness_TargetTolerance_GetRange m_peak_IPL_AutoBrightness_TargetTolerance_GetRange{};
    dyn_peak_IPL_AutoBrightness_TargetTolerance_Set m_peak_IPL_AutoBrightness_TargetTolerance_Set{};
    dyn_peak_IPL_AutoBrightness_TargetTolerance_Get m_peak_IPL_AutoBrightness_TargetTolerance_Get{};
    dyn_peak_IPL_AutoBrightness_TargetPercentile_GetRange m_peak_IPL_AutoBrightness_TargetPercentile_GetRange{};
    dyn_peak_IPL_AutoBrightness_TargetPercentile_Set m_peak_IPL_AutoBrightness_TargetPercentile_Set{};
    dyn_peak_IPL_AutoBrightness_TargetPercentile_Get m_peak_IPL_AutoBrightness_TargetPercentile_Get{};
    dyn_peak_IPL_AutoBrightness_ROI_Mode_Set m_peak_IPL_AutoBrightness_ROI_Mode_Set{};
    dyn_peak_IPL_AutoBrightness_ROI_Mode_Get m_peak_IPL_AutoBrightness_ROI_Mode_Get{};
    dyn_peak_IPL_AutoBrightness_ROI_Offset_GetRange m_peak_IPL_AutoBrightness_ROI_Offset_GetRange{};
    dyn_peak_IPL_AutoBrightness_ROI_Size_GetRange m_peak_IPL_AutoBrightness_ROI_Size_GetRange{};
    dyn_peak_IPL_AutoBrightness_ROI_Set m_peak_IPL_AutoBrightness_ROI_Set{};
    dyn_peak_IPL_AutoBrightness_ROI_Get m_peak_IPL_AutoBrightness_ROI_Get{};
    dyn_peak_IPL_AutoBrightness_Exposure_Mode_Set m_peak_IPL_AutoBrightness_Exposure_Mode_Set{};
    dyn_peak_IPL_AutoBrightness_Exposure_Mode_Get m_peak_IPL_AutoBrightness_Exposure_Mode_Get{};
    dyn_peak_IPL_AutoBrightness_ExposureLimit_Get m_peak_IPL_AutoBrightness_ExposureLimit_Get{};
    dyn_peak_IPL_AutoBrightness_ExposureLimit_Set m_peak_IPL_AutoBrightness_ExposureLimit_Set{};
    dyn_peak_IPL_AutoBrightness_Gain_Mode_Set m_peak_IPL_AutoBrightness_Gain_Mode_Set{};
    dyn_peak_IPL_AutoBrightness_Gain_Mode_Get m_peak_IPL_AutoBrightness_Gain_Mode_Get{};
    dyn_peak_IPL_AutoBrightness_GainAnalog_Mode_Set m_peak_IPL_AutoBrightness_GainAnalog_Mode_Set{};
    dyn_peak_IPL_AutoBrightness_GainAnalog_Mode_Get m_peak_IPL_AutoBrightness_GainAnalog_Mode_Get{};
    dyn_peak_IPL_AutoBrightness_GainDigital_Mode_Set m_peak_IPL_AutoBrightness_GainDigital_Mode_Set{};
    dyn_peak_IPL_AutoBrightness_GainDigital_Mode_Get m_peak_IPL_AutoBrightness_GainDigital_Mode_Get{};
    dyn_peak_IPL_AutoBrightness_GainCombined_Mode_Set m_peak_IPL_AutoBrightness_GainCombined_Mode_Set{};
    dyn_peak_IPL_AutoBrightness_GainCombined_Mode_Get m_peak_IPL_AutoBrightness_GainCombined_Mode_Get{};
    dyn_peak_IPL_AutoBrightness_GainHost_Mode_Set m_peak_IPL_AutoBrightness_GainHost_Mode_Set{};
    dyn_peak_IPL_AutoBrightness_GainHost_Mode_Get m_peak_IPL_AutoBrightness_GainHost_Mode_Get{};
    dyn_peak_IPL_AutoBrightness_Algorithm_Set m_peak_IPL_AutoBrightness_Algorithm_Set{};
    dyn_peak_IPL_AutoBrightness_Algorithm_Get m_peak_IPL_AutoBrightness_Algorithm_Get{};
    dyn_peak_IPL_AutoBrightness_AverageLast_Get m_peak_IPL_AutoBrightness_AverageLast_Get{};
    dyn_peak_IPL_AutoBrightness_GainLimit_Set m_peak_IPL_AutoBrightness_GainLimit_Set{};
    dyn_peak_IPL_AutoBrightness_GainLimit_Get m_peak_IPL_AutoBrightness_GainLimit_Get{};
    dyn_peak_IPL_AutoBrightness_GainLimit_GetRange m_peak_IPL_AutoBrightness_GainLimit_GetRange{};
    dyn_peak_IPL_AutoBrightness_GainAnalogLimit_Set m_peak_IPL_AutoBrightness_GainAnalogLimit_Set{};
    dyn_peak_IPL_AutoBrightness_GainAnalogLimit_Get m_peak_IPL_AutoBrightness_GainAnalogLimit_Get{};
    dyn_peak_IPL_AutoBrightness_GainAnalogLimit_GetRange m_peak_IPL_AutoBrightness_GainAnalogLimit_GetRange{};
    dyn_peak_IPL_AutoBrightness_GainDigitalLimit_Set m_peak_IPL_AutoBrightness_GainDigitalLimit_Set{};
    dyn_peak_IPL_AutoBrightness_GainDigitalLimit_Get m_peak_IPL_AutoBrightness_GainDigitalLimit_Get{};
    dyn_peak_IPL_AutoBrightness_GainDigitalLimit_GetRange m_peak_IPL_AutoBrightness_GainDigitalLimit_GetRange{};
    dyn_peak_IPL_AutoBrightness_GainCombinedLimit_Set m_peak_IPL_AutoBrightness_GainCombinedLimit_Set{};
    dyn_peak_IPL_AutoBrightness_GainCombinedLimit_Get m_peak_IPL_AutoBrightness_GainCombinedLimit_Get{};
    dyn_peak_IPL_AutoBrightness_GainCombinedLimit_GetRange m_peak_IPL_AutoBrightness_GainCombinedLimit_GetRange{};
    dyn_peak_IPL_AutoBrightness_GainHostLimit_Set m_peak_IPL_AutoBrightness_GainHostLimit_Set{};
    dyn_peak_IPL_AutoBrightness_GainHostLimit_Get m_peak_IPL_AutoBrightness_GainHostLimit_Get{};
    dyn_peak_IPL_AutoBrightness_GainHostLimit_GetRange m_peak_IPL_AutoBrightness_GainHostLimit_GetRange{};
    dyn_peak_IPL_AutoBrightness_SkipFrames_Set m_peak_IPL_AutoBrightness_SkipFrames_Set{};
    dyn_peak_IPL_AutoBrightness_SkipFrames_Get m_peak_IPL_AutoBrightness_SkipFrames_Get{};
    dyn_peak_IPL_AutoBrightness_SkipFrames_GetRange m_peak_IPL_AutoBrightness_SkipFrames_GetRange{};
    dyn_peak_IPL_AutoWhiteBalance_ROI_Mode_Set m_peak_IPL_AutoWhiteBalance_ROI_Mode_Set{};
    dyn_peak_IPL_AutoWhiteBalance_ROI_Mode_Get m_peak_IPL_AutoWhiteBalance_ROI_Mode_Get{};
    dyn_peak_IPL_AutoWhiteBalance_ROI_Offset_GetRange m_peak_IPL_AutoWhiteBalance_ROI_Offset_GetRange{};
    dyn_peak_IPL_AutoWhiteBalance_ROI_Size_GetRange m_peak_IPL_AutoWhiteBalance_ROI_Size_GetRange{};
    dyn_peak_IPL_AutoWhiteBalance_ROI_Set m_peak_IPL_AutoWhiteBalance_ROI_Set{};
    dyn_peak_IPL_AutoWhiteBalance_ROI_Get m_peak_IPL_AutoWhiteBalance_ROI_Get{};
    dyn_peak_IPL_AutoWhiteBalance_Mode_Set m_peak_IPL_AutoWhiteBalance_Mode_Set{};
    dyn_peak_IPL_AutoWhiteBalance_Mode_Get m_peak_IPL_AutoWhiteBalance_Mode_Get{};
    dyn_peak_IPL_AutoWhiteBalance_SkipFrames_Set m_peak_IPL_AutoWhiteBalance_SkipFrames_Set{};
    dyn_peak_IPL_AutoWhiteBalance_SkipFrames_Get m_peak_IPL_AutoWhiteBalance_SkipFrames_Get{};
    dyn_peak_IPL_AutoWhiteBalance_SkipFrames_GetRange m_peak_IPL_AutoWhiteBalance_SkipFrames_GetRange{};
    dyn_peak_IPL_AutoFocus_GetAccessStatus m_peak_IPL_AutoFocus_GetAccessStatus{};
    dyn_peak_IPL_AutoFocus_ROI_Set m_peak_IPL_AutoFocus_ROI_Set{};
    dyn_peak_IPL_AutoFocus_ROI_Get m_peak_IPL_AutoFocus_ROI_Get{};
    dyn_peak_IPL_AutoFocus_Mode_Set m_peak_IPL_AutoFocus_Mode_Set{};
    dyn_peak_IPL_AutoFocus_Mode_Get m_peak_IPL_AutoFocus_Mode_Get{};
    dyn_peak_IPL_AutoFocus_SearchAlgorithm_Set m_peak_IPL_AutoFocus_SearchAlgorithm_Set{};
    dyn_peak_IPL_AutoFocus_SearchAlgorithm_Get m_peak_IPL_AutoFocus_SearchAlgorithm_Get{};
    dyn_peak_IPL_AutoFocus_SharpnessAlgorithm_Set m_peak_IPL_AutoFocus_SharpnessAlgorithm_Set{};
    dyn_peak_IPL_AutoFocus_SharpnessAlgorithm_Get m_peak_IPL_AutoFocus_SharpnessAlgorithm_Get{};
    dyn_peak_IPL_AutoFocus_Range_Set m_peak_IPL_AutoFocus_Range_Set{};
    dyn_peak_IPL_AutoFocus_Range_Get m_peak_IPL_AutoFocus_Range_Get{};
    dyn_peak_IPL_AutoFocus_Hysteresis_Set m_peak_IPL_AutoFocus_Hysteresis_Set{};
    dyn_peak_IPL_AutoFocus_Hysteresis_Get m_peak_IPL_AutoFocus_Hysteresis_Get{};
    dyn_peak_IPL_AutoFocus_Hysteresis_GetRange m_peak_IPL_AutoFocus_Hysteresis_GetRange{};
    dyn_peak_IPL_HotpixelCorrection_Sensitivity_Set m_peak_IPL_HotpixelCorrection_Sensitivity_Set{};
    dyn_peak_IPL_HotpixelCorrection_Sensitivity_Get m_peak_IPL_HotpixelCorrection_Sensitivity_Get{};
    dyn_peak_IPL_HotpixelCorrection_GetList m_peak_IPL_HotpixelCorrection_GetList{};
    dyn_peak_IPL_HotpixelCorrection_SetList m_peak_IPL_HotpixelCorrection_SetList{};
    dyn_peak_IPL_HotpixelCorrection_ResetList m_peak_IPL_HotpixelCorrection_ResetList{};
    dyn_peak_IPL_HotpixelCorrection_Enable m_peak_IPL_HotpixelCorrection_Enable{};
    dyn_peak_IPL_HotpixelCorrection_IsEnabled m_peak_IPL_HotpixelCorrection_IsEnabled{};
    dyn_peak_IPL_Mirror_UpDown_Enable m_peak_IPL_Mirror_UpDown_Enable{};
    dyn_peak_IPL_Mirror_UpDown_IsEnabled m_peak_IPL_Mirror_UpDown_IsEnabled{};
    dyn_peak_IPL_Mirror_LeftRight_Enable m_peak_IPL_Mirror_LeftRight_Enable{};
    dyn_peak_IPL_Mirror_LeftRight_IsEnabled m_peak_IPL_Mirror_LeftRight_IsEnabled{};
    dyn_peak_IPL_ProcessFrame m_peak_IPL_ProcessFrame{};
    dyn_peak_IPL_ProcessFrameInplace m_peak_IPL_ProcessFrameInplace{};
    dyn_peak_IPL_ReadImage m_peak_IPL_ReadImage{};
    dyn_peak_IPL_EdgeEnhancement_Enable m_peak_IPL_EdgeEnhancement_Enable{};
    dyn_peak_IPL_EdgeEnhancement_IsEnabled m_peak_IPL_EdgeEnhancement_IsEnabled{};
    dyn_peak_IPL_EdgeEnhancement_Factor_Set m_peak_IPL_EdgeEnhancement_Factor_Set{};
    dyn_peak_IPL_EdgeEnhancement_Factor_Get m_peak_IPL_EdgeEnhancement_Factor_Get{};
    dyn_peak_IPL_EdgeEnhancement_Factor_GetDefault m_peak_IPL_EdgeEnhancement_Factor_GetDefault{};
    dyn_peak_IPL_EdgeEnhancement_Factor_GetRange m_peak_IPL_EdgeEnhancement_Factor_GetRange{};
    dyn_peak_IPL_Sharpness_Measure m_peak_IPL_Sharpness_Measure{};
    dyn_peak_IPL_Sharpness_GetList m_peak_IPL_Sharpness_GetList{};
    dyn_peak_IPL_Rotation_Angle_Set m_peak_IPL_Rotation_Angle_Set{};
    dyn_peak_IPL_Rotation_Angle_Get m_peak_IPL_Rotation_Angle_Get{};
    dyn_peak_IPL_Histogram_ProcessFrame m_peak_IPL_Histogram_ProcessFrame{};
    dyn_peak_IPL_Histogram_Release m_peak_IPL_Histogram_Release{};
    dyn_peak_IPL_Histogram_Channel_GetCount m_peak_IPL_Histogram_Channel_GetCount{};
    dyn_peak_IPL_Histogram_Channel_GetInfo m_peak_IPL_Histogram_Channel_GetInfo{};
    dyn_peak_IPL_Histogram_Channel_GetBinArray m_peak_IPL_Histogram_Channel_GetBinArray{};
    dyn_peak_VideoWriter_Open m_peak_VideoWriter_Open{};
    dyn_peak_VideoWriter_Close m_peak_VideoWriter_Close{};
    dyn_peak_VideoWriter_AddFrame m_peak_VideoWriter_AddFrame{};
    dyn_peak_VideoWriter_Container_GetEncoderList m_peak_VideoWriter_Container_GetEncoderList{};
    dyn_peak_VideoWriter_Encoder_GetPixelFormatList m_peak_VideoWriter_Encoder_GetPixelFormatList{};
    dyn_peak_VideoWriter_Encoder_GetContainerList m_peak_VideoWriter_Encoder_GetContainerList{};
    dyn_peak_VideoWriter_GetInfo m_peak_VideoWriter_GetInfo{};
    dyn_peak_VideoWriter_Container_Option_Set m_peak_VideoWriter_Container_Option_Set{};
    dyn_peak_VideoWriter_Container_Option_Get m_peak_VideoWriter_Container_Option_Get{};
    dyn_peak_VideoWriter_Encoder_Option_Set m_peak_VideoWriter_Encoder_Option_Set{};
    dyn_peak_VideoWriter_Encoder_Option_Get m_peak_VideoWriter_Encoder_Option_Get{};
    dyn_peak_VideoWriter_WaitUntilQueueEmpty m_peak_VideoWriter_WaitUntilQueueEmpty{};
    dyn_peak_Inference_CNN_Open m_peak_Inference_CNN_Open{};
    dyn_peak_Inference_CNN_Close m_peak_Inference_CNN_Close{};
    dyn_peak_Inference_CNN_ProcessFrame m_peak_Inference_CNN_ProcessFrame{};
    dyn_peak_Inference_CNN_Info_Get m_peak_Inference_CNN_Info_Get{};
    dyn_peak_Inference_Result_Get m_peak_Inference_Result_Get{};
    dyn_peak_Inference_Result_Release m_peak_Inference_Result_Release{};
    dyn_peak_Inference_Result_Classification_GetList m_peak_Inference_Result_Classification_GetList{};
    dyn_peak_Inference_Result_Detection_GetList m_peak_Inference_Result_Detection_GetList{};
    dyn_peak_Inference_Statistics_Get m_peak_Inference_Statistics_Get{};
    dyn_peak_Inference_Statistics_Reset m_peak_Inference_Statistics_Reset{};
    dyn_peak_Inference_ConfidenceThreshold_Get m_peak_Inference_ConfidenceThreshold_Get{};
    dyn_peak_Inference_ConfidenceThreshold_GetRange m_peak_Inference_ConfidenceThreshold_GetRange{};
    dyn_peak_Inference_ConfidenceThreshold_Set m_peak_Inference_ConfidenceThreshold_Set{};
    dyn_peak_MessageQueue_Create m_peak_MessageQueue_Create{};
    dyn_peak_MessageQueue_Destroy m_peak_MessageQueue_Destroy{};
    dyn_peak_MessageQueue_EnableMessage m_peak_MessageQueue_EnableMessage{};
    dyn_peak_MessageQueue_DisableMessage m_peak_MessageQueue_DisableMessage{};
    dyn_peak_MessageQueue_EnableMessageList m_peak_MessageQueue_EnableMessageList{};
    dyn_peak_MessageQueue_DisableMessageList m_peak_MessageQueue_DisableMessageList{};
    dyn_peak_MessageQueue_EnabledMessages_GetList m_peak_MessageQueue_EnabledMessages_GetList{};
    dyn_peak_MessageQueue_SetMode m_peak_MessageQueue_SetMode{};
    dyn_peak_MessageQueue_GetMode m_peak_MessageQueue_GetMode{};
    dyn_peak_MessageQueue_Start m_peak_MessageQueue_Start{};
    dyn_peak_MessageQueue_Stop m_peak_MessageQueue_Stop{};
    dyn_peak_MessageQueue_IsStarted m_peak_MessageQueue_IsStarted{};
    dyn_peak_MessageQueue_WaitForMessage m_peak_MessageQueue_WaitForMessage{};
    dyn_peak_MessageQueue_Flush m_peak_MessageQueue_Flush{};
    dyn_peak_MessageQueue_IsMessageSupported m_peak_MessageQueue_IsMessageSupported{};
    dyn_peak_MessageQueue_Statistics_Get m_peak_MessageQueue_Statistics_Get{};
    dyn_peak_MessageQueue_Statistics_Reset m_peak_MessageQueue_Statistics_Reset{};
    dyn_peak_MessageQueue_MaxQueueSize_Set m_peak_MessageQueue_MaxQueueSize_Set{};
    dyn_peak_MessageQueue_MaxQueueSize_Get m_peak_MessageQueue_MaxQueueSize_Get{};
    dyn_peak_Message_Release m_peak_Message_Release{};
    dyn_peak_Message_GetInfo m_peak_Message_GetInfo{};
    dyn_peak_Message_Type_Get m_peak_Message_Type_Get{};
    dyn_peak_Message_CameraHandle_Get m_peak_Message_CameraHandle_Get{};
    dyn_peak_Message_ID_Get m_peak_Message_ID_Get{};
    dyn_peak_Message_HostTimestamp_Get m_peak_Message_HostTimestamp_Get{};
    dyn_peak_Message_Data_Type_Get m_peak_Message_Data_Type_Get{};
    dyn_peak_Message_Data_RemoteDevice_Get m_peak_Message_Data_RemoteDevice_Get{};
    dyn_peak_Message_Data_RemoteDeviceError_Get m_peak_Message_Data_RemoteDeviceError_Get{};
    dyn_peak_Message_Data_RemoteDeviceDropped_Get m_peak_Message_Data_RemoteDeviceDropped_Get{};
    dyn_peak_Message_Data_RemoteDeviceFrame_Get m_peak_Message_Data_RemoteDeviceFrame_Get{};
    dyn_peak_Message_Data_RemoteDeviceTemperature_Get m_peak_Message_Data_RemoteDeviceTemperature_Get{};
    dyn_peak_Message_Data_AutoFocusData_Get m_peak_Message_Data_AutoFocusData_Get{};
    dyn_peak_Message_Data_DeviceFound_Get m_peak_Message_Data_DeviceFound_Get{};
    dyn_peak_Message_Data_DeviceLost_Get m_peak_Message_Data_DeviceLost_Get{};
    dyn_peak_Message_Data_DeviceReconnected_Get m_peak_Message_Data_DeviceReconnected_Get{};
    dyn_peak_Message_Data_DeviceDisconnected_Get m_peak_Message_Data_DeviceDisconnected_Get{};
    dyn_peak_Message_Data_FirmwareUpdate_Get m_peak_Message_Data_FirmwareUpdate_Get{};
    dyn_peak_I2C_Create m_peak_I2C_Create{};
    dyn_peak_I2C_Destroy m_peak_I2C_Destroy{};
    dyn_peak_I2C_GetAccessStatus m_peak_I2C_GetAccessStatus{};
    dyn_peak_I2C_Mode_GetList m_peak_I2C_Mode_GetList{};
    dyn_peak_I2C_Mode_Set m_peak_I2C_Mode_Set{};
    dyn_peak_I2C_Mode_Get m_peak_I2C_Mode_Get{};
    dyn_peak_I2C_DeviceAddress_GetRange m_peak_I2C_DeviceAddress_GetRange{};
    dyn_peak_I2C_DeviceAddress_Set m_peak_I2C_DeviceAddress_Set{};
    dyn_peak_I2C_DeviceAddress_Get m_peak_I2C_DeviceAddress_Get{};
    dyn_peak_I2C_RegisterAddress_Length_GetList m_peak_I2C_RegisterAddress_Length_GetList{};
    dyn_peak_I2C_RegisterAddress_Length_Set m_peak_I2C_RegisterAddress_Length_Set{};
    dyn_peak_I2C_RegisterAddress_Length_Get m_peak_I2C_RegisterAddress_Length_Get{};
    dyn_peak_I2C_RegisterAddress_Endianness_Set m_peak_I2C_RegisterAddress_Endianness_Set{};
    dyn_peak_I2C_RegisterAddress_Endianness_Get m_peak_I2C_RegisterAddress_Endianness_Get{};
    dyn_peak_I2C_RegisterAddress_Set m_peak_I2C_RegisterAddress_Set{};
    dyn_peak_I2C_RegisterAddress_Get m_peak_I2C_RegisterAddress_Get{};
    dyn_peak_I2C_AckPolling_Enable m_peak_I2C_AckPolling_Enable{};
    dyn_peak_I2C_AckPolling_IsEnabled m_peak_I2C_AckPolling_IsEnabled{};
    dyn_peak_I2C_AckPolling_Timeout_GetAccessStatus m_peak_I2C_AckPolling_Timeout_GetAccessStatus{};
    dyn_peak_I2C_AckPolling_Timeout_GetRange m_peak_I2C_AckPolling_Timeout_GetRange{};
    dyn_peak_I2C_AckPolling_Timeout_Set m_peak_I2C_AckPolling_Timeout_Set{};
    dyn_peak_I2C_AckPolling_Timeout_Get m_peak_I2C_AckPolling_Timeout_Get{};
    dyn_peak_I2C_Data_Write m_peak_I2C_Data_Write{};
    dyn_peak_I2C_Data_Read m_peak_I2C_Data_Read{};
    dyn_peak_I2C_Data_MaxSize_Get m_peak_I2C_Data_MaxSize_Get{};
    dyn_peak_I2C_OperationStatus_Get m_peak_I2C_OperationStatus_Get{};
    dyn_peak_IPL_ImageWriter_Create m_peak_IPL_ImageWriter_Create{};
    dyn_peak_IPL_ImageWriter_Destroy m_peak_IPL_ImageWriter_Destroy{};
    dyn_peak_IPL_ImageWriter_Save m_peak_IPL_ImageWriter_Save{};
    dyn_peak_IPL_ImageWriter_Format_Set m_peak_IPL_ImageWriter_Format_Set{};
    dyn_peak_IPL_ImageWriter_Format_Get m_peak_IPL_ImageWriter_Format_Get{};
    dyn_peak_IPL_ImageWriter_Compression_Set m_peak_IPL_ImageWriter_Compression_Set{};
    dyn_peak_IPL_ImageWriter_Compression_Get m_peak_IPL_ImageWriter_Compression_Get{};
    dyn_peak_IPL_Binning_FactorX_GetList m_peak_IPL_Binning_FactorX_GetList{};
    dyn_peak_IPL_Binning_FactorY_GetList m_peak_IPL_Binning_FactorY_GetList{};
    dyn_peak_IPL_Binning_Set m_peak_IPL_Binning_Set{};
    dyn_peak_IPL_Binning_Get m_peak_IPL_Binning_Get{};
    dyn_peak_IPL_Decimation_FactorX_GetList m_peak_IPL_Decimation_FactorX_GetList{};
    dyn_peak_IPL_Decimation_FactorY_GetList m_peak_IPL_Decimation_FactorY_GetList{};
    dyn_peak_IPL_Decimation_Set m_peak_IPL_Decimation_Set{};
    dyn_peak_IPL_Decimation_Get m_peak_IPL_Decimation_Get{};
    dyn_peak_FirmwareUpdate_CompatibleCameraList_Get m_peak_FirmwareUpdate_CompatibleCameraList_Get{};
    dyn_peak_FirmwareUpdate_Execute m_peak_FirmwareUpdate_Execute{};
    dyn_peak_TestPattern_GetAccessStatus m_peak_TestPattern_GetAccessStatus{};
    dyn_peak_TestPattern_Set m_peak_TestPattern_Set{};
    dyn_peak_TestPattern_Get m_peak_TestPattern_Get{};
    dyn_peak_TestPattern_GetList m_peak_TestPattern_GetList{};
    dyn_peak_LED_GetAccessStatus m_peak_LED_GetAccessStatus{};
    dyn_peak_LED_Target_GetList m_peak_LED_Target_GetList{};
    dyn_peak_LED_Mode_GetList m_peak_LED_Mode_GetList{};
    dyn_peak_LED_Set m_peak_LED_Set{};
    dyn_peak_LED_Get m_peak_LED_Get{};
    dyn_peak_BlackLevel_Auto_GetAccessStatus m_peak_BlackLevel_Auto_GetAccessStatus{};
    dyn_peak_BlackLevel_Auto_Enable m_peak_BlackLevel_Auto_Enable{};
    dyn_peak_BlackLevel_Auto_IsEnabled m_peak_BlackLevel_Auto_IsEnabled{};
    dyn_peak_BlackLevel_Offset_GetAccessStatus m_peak_BlackLevel_Offset_GetAccessStatus{};
    dyn_peak_BlackLevel_Offset_Set m_peak_BlackLevel_Offset_Set{};
    dyn_peak_BlackLevel_Offset_Get m_peak_BlackLevel_Offset_Get{};
    dyn_peak_BlackLevel_Offset_GetRange m_peak_BlackLevel_Offset_GetRange{};
    dyn_peak_Bandwidth_LinkSpeed_GetAccessStatus m_peak_Bandwidth_LinkSpeed_GetAccessStatus{};
    dyn_peak_Bandwidth_LinkSpeed_Get m_peak_Bandwidth_LinkSpeed_Get{};
    dyn_peak_Bandwidth_ThroughputLimit_GetAccessStatus m_peak_Bandwidth_ThroughputLimit_GetAccessStatus{};
    dyn_peak_Bandwidth_ThroughputLimit_GetRange m_peak_Bandwidth_ThroughputLimit_GetRange{};
    dyn_peak_Bandwidth_ThroughputLimit_Set m_peak_Bandwidth_ThroughputLimit_Set{};
    dyn_peak_Bandwidth_ThroughputLimit_Get m_peak_Bandwidth_ThroughputLimit_Get{};
    dyn_peak_Bandwidth_ThroughputFrameRateLimit_GetAccessStatus m_peak_Bandwidth_ThroughputFrameRateLimit_GetAccessStatus{};
    dyn_peak_Bandwidth_ThroughputFrameRateLimit_Get m_peak_Bandwidth_ThroughputFrameRateLimit_Get{};
    dyn_peak_Bandwidth_ThroughputCalculated_GetAccessStatus m_peak_Bandwidth_ThroughputCalculated_GetAccessStatus{};
    dyn_peak_Bandwidth_ThroughputCalculated_Get m_peak_Bandwidth_ThroughputCalculated_Get{};
    dyn_peak_IPO_GetAccessStatus m_peak_IPO_GetAccessStatus{};
    dyn_peak_IPO_IsEnabled m_peak_IPO_IsEnabled{};
    dyn_peak_IPO_Enable m_peak_IPO_Enable{};
    dyn_peak_IPL_DigitalBlack_GetRange m_peak_IPL_DigitalBlack_GetRange{};
    dyn_peak_IPL_DigitalBlack_Set m_peak_IPL_DigitalBlack_Set{};
    dyn_peak_IPL_DigitalBlack_Get m_peak_IPL_DigitalBlack_Get{};
    dyn_peak_IPL_LUT_Enable m_peak_IPL_LUT_Enable{};
    dyn_peak_IPL_LUT_IsEnabled m_peak_IPL_LUT_IsEnabled{};
    dyn_peak_IPL_LUT_Preset_Set m_peak_IPL_LUT_Preset_Set{};
    dyn_peak_IPL_LUT_Value_Set m_peak_IPL_LUT_Value_Set{};
    dyn_peak_IPL_LUT_Value_Get m_peak_IPL_LUT_Value_Get{};
    dyn_peak_IPL_LUT_ValueList_Set m_peak_IPL_LUT_ValueList_Set{};
    dyn_peak_IPL_LUT_ValueList_Get m_peak_IPL_LUT_ValueList_Get{};
    dyn_peak_Chunks_GetAccessStatus m_peak_Chunks_GetAccessStatus{};
    dyn_peak_Chunks_Enable m_peak_Chunks_Enable{};
    dyn_peak_Chunks_IsEnabled m_peak_Chunks_IsEnabled{};
    dyn_peak_Chunks_AutoUpdate_Enable m_peak_Chunks_AutoUpdate_Enable{};
    dyn_peak_Chunks_AutoUpdate_IsEnabled m_peak_Chunks_AutoUpdate_IsEnabled{};
    dyn_peak_Chunks_Update m_peak_Chunks_Update{};
    dyn_peak_Chunks_Type_GetAccessStatus m_peak_Chunks_Type_GetAccessStatus{};
    dyn_peak_Chunks_Type_Enable m_peak_Chunks_Type_Enable{};
    dyn_peak_Chunks_Type_IsEnabled m_peak_Chunks_Type_IsEnabled{};
    dyn_peak_Chunks_Type_Supported_GetList m_peak_Chunks_Type_Supported_GetList{};
    dyn_peak_Chunks_FrameInfo_Get m_peak_Chunks_FrameInfo_Get{};
    dyn_peak_Chunks_Exposure_Get m_peak_Chunks_Exposure_Get{};
    dyn_peak_Chunks_Gain_Get m_peak_Chunks_Gain_Get{};
    dyn_peak_Chunks_Sequencer_Get m_peak_Chunks_Sequencer_Get{};
    dyn_peak_Chunks_Timestamp_Get m_peak_Chunks_Timestamp_Get{};
    dyn_peak_Chunks_ExposureTrigger_Get m_peak_Chunks_ExposureTrigger_Get{};
    dyn_peak_Chunks_UsableROI_Get m_peak_Chunks_UsableROI_Get{};
    dyn_peak_Chunks_LineStatus_Get m_peak_Chunks_LineStatus_Get{};
    dyn_peak_Chunks_AutoFeature_Get m_peak_Chunks_AutoFeature_Get{};
    dyn_peak_Chunks_PTPStatus_Get m_peak_Chunks_PTPStatus_Get{};

};

inline void* import_function(void *module, const char* proc_name)
{
#ifdef __linux__
    return dlsym(module, proc_name);
#else
    return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(module), proc_name));
#endif
}
            
inline DynamicLoader::DynamicLoader()
{
#if defined _WIN32 || defined _WIN64
    size_t sz = 0;
    if (_wgetenv_s(&sz, NULL, 0, L"IDS_PEAK_COMFORT_SDK_PATH") == 0
        && sz > 0)
    {
        std::vector<wchar_t> env_ids_peak_comfort_c(sz);
        if (_wgetenv_s(&sz, env_ids_peak_comfort_c.data(), sz, L"IDS_PEAK_COMFORT_SDK_PATH") == 0)
        {
            if (_wgetenv_s(&sz, NULL, 0, L"PATH") == 0
                && sz > 0)
            {
                std::vector<wchar_t> env_path(sz);
                if (_wgetenv_s(&sz, env_path.data(), sz, L"PATH") == 0)
                {
                    std::wstring ids_peak_comfort_c_path(env_ids_peak_comfort_c.data());
#ifdef _WIN64
                    ids_peak_comfort_c_path.append(L"\\api\\lib\\x86_64");
#else
                    ids_peak_comfort_c_path.append(L"\\api\\lib\\x86_32");
#endif
                    AddDllDirectory(ids_peak_comfort_c_path.c_str());
                }
            }
        }
    }
    
    loadLib("ids_peak_comfort_c.dll");
#else
    loadLib("libids_peak_comfort_c.so");
#endif
}

inline DynamicLoader::~DynamicLoader()
{
    if(m_handle != nullptr)
    {
        unload();
    }
}

inline bool DynamicLoader::isLoaded()
{
    auto&& inst = instance();
    return inst.m_handle != nullptr;
}

inline void DynamicLoader::unload()
{
    setPointers(false);
    
    if (m_handle != nullptr)
    {
#ifdef __linux__
        dlclose(m_handle);
#else
        FreeLibrary(static_cast<HMODULE>(m_handle));
#endif
    }
    m_handle = nullptr;
}


inline bool DynamicLoader::loadLib(const char* file)
{
    bool ret = false;
    
    if (file)
    {
#ifdef __linux__
        m_handle = dlopen(file, RTLD_NOW);
#else
        m_handle = LoadLibraryExA(file, nullptr, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
#endif
        if (m_handle != nullptr)
        {
            try {
                setPointers(true);
                ret = true;
            } catch (const std::exception&) {
                unload();
                throw;
            }
        }
        else
        {
            throw LoadLibraryException(std::string("Lib load failed: ") + file);
        }
    }
    else
    {
        throw LoadLibraryException("Filename empty");
    }

    return ret;
}

inline bool DynamicLoader::setPointers(bool load)
{

    m_peak_Library_Init = (dyn_peak_Library_Init) (load ?  import_function(m_handle, "peak_Library_Init") : nullptr);
    if(m_peak_Library_Init == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Library_Init");
    }

    m_peak_Library_Exit = (dyn_peak_Library_Exit) (load ?  import_function(m_handle, "peak_Library_Exit") : nullptr);
    if(m_peak_Library_Exit == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Library_Exit");
    }

    m_peak_Library_GetVersion = (dyn_peak_Library_GetVersion) (load ?  import_function(m_handle, "peak_Library_GetVersion") : nullptr);
    if(m_peak_Library_GetVersion == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Library_GetVersion");
    }

    m_peak_Library_GetLastError = (dyn_peak_Library_GetLastError) (load ?  import_function(m_handle, "peak_Library_GetLastError") : nullptr);
    if(m_peak_Library_GetLastError == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Library_GetLastError");
    }

    m_peak_Library_InterfaceTechnology_IsSupported = (dyn_peak_Library_InterfaceTechnology_IsSupported) (load ?  import_function(m_handle, "peak_Library_InterfaceTechnology_IsSupported") : nullptr);
    if(m_peak_Library_InterfaceTechnology_IsSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Library_InterfaceTechnology_IsSupported");
    }

    m_peak_Reconnect_Enable = (dyn_peak_Reconnect_Enable) (load ?  import_function(m_handle, "peak_Reconnect_Enable") : nullptr);
    if(m_peak_Reconnect_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Reconnect_Enable");
    }

    m_peak_Reconnect_IsEnabled = (dyn_peak_Reconnect_IsEnabled) (load ?  import_function(m_handle, "peak_Reconnect_IsEnabled") : nullptr);
    if(m_peak_Reconnect_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Reconnect_IsEnabled");
    }

    m_peak_Reconnect_GetAccessStatus = (dyn_peak_Reconnect_GetAccessStatus) (load ?  import_function(m_handle, "peak_Reconnect_GetAccessStatus") : nullptr);
    if(m_peak_Reconnect_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Reconnect_GetAccessStatus");
    }

    m_peak_CameraList_Update = (dyn_peak_CameraList_Update) (load ?  import_function(m_handle, "peak_CameraList_Update") : nullptr);
    if(m_peak_CameraList_Update == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraList_Update");
    }

    m_peak_CameraList_Get = (dyn_peak_CameraList_Get) (load ?  import_function(m_handle, "peak_CameraList_Get") : nullptr);
    if(m_peak_CameraList_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraList_Get");
    }

    m_peak_Camera_ID_FromHandle = (dyn_peak_Camera_ID_FromHandle) (load ?  import_function(m_handle, "peak_Camera_ID_FromHandle") : nullptr);
    if(m_peak_Camera_ID_FromHandle == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Camera_ID_FromHandle");
    }

    m_peak_Camera_ID_FromSerialNumber = (dyn_peak_Camera_ID_FromSerialNumber) (load ?  import_function(m_handle, "peak_Camera_ID_FromSerialNumber") : nullptr);
    if(m_peak_Camera_ID_FromSerialNumber == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Camera_ID_FromSerialNumber");
    }

    m_peak_Camera_ID_FromUserDefinedName = (dyn_peak_Camera_ID_FromUserDefinedName) (load ?  import_function(m_handle, "peak_Camera_ID_FromUserDefinedName") : nullptr);
    if(m_peak_Camera_ID_FromUserDefinedName == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Camera_ID_FromUserDefinedName");
    }

    m_peak_Camera_ID_FromMAC = (dyn_peak_Camera_ID_FromMAC) (load ?  import_function(m_handle, "peak_Camera_ID_FromMAC") : nullptr);
    if(m_peak_Camera_ID_FromMAC == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Camera_ID_FromMAC");
    }

    m_peak_Camera_GetAccessStatus = (dyn_peak_Camera_GetAccessStatus) (load ?  import_function(m_handle, "peak_Camera_GetAccessStatus") : nullptr);
    if(m_peak_Camera_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Camera_GetAccessStatus");
    }

    m_peak_Camera_GetDescriptor = (dyn_peak_Camera_GetDescriptor) (load ?  import_function(m_handle, "peak_Camera_GetDescriptor") : nullptr);
    if(m_peak_Camera_GetDescriptor == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Camera_GetDescriptor");
    }

    m_peak_Camera_Open = (dyn_peak_Camera_Open) (load ?  import_function(m_handle, "peak_Camera_Open") : nullptr);
    if(m_peak_Camera_Open == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Camera_Open");
    }

    m_peak_Camera_OpenFirstAvailable = (dyn_peak_Camera_OpenFirstAvailable) (load ?  import_function(m_handle, "peak_Camera_OpenFirstAvailable") : nullptr);
    if(m_peak_Camera_OpenFirstAvailable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Camera_OpenFirstAvailable");
    }

    m_peak_Camera_Close = (dyn_peak_Camera_Close) (load ?  import_function(m_handle, "peak_Camera_Close") : nullptr);
    if(m_peak_Camera_Close == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Camera_Close");
    }

    m_peak_Camera_ResetToDefaultSettings = (dyn_peak_Camera_ResetToDefaultSettings) (load ?  import_function(m_handle, "peak_Camera_ResetToDefaultSettings") : nullptr);
    if(m_peak_Camera_ResetToDefaultSettings == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Camera_ResetToDefaultSettings");
    }

    m_peak_Camera_UserDefinedName_Set = (dyn_peak_Camera_UserDefinedName_Set) (load ?  import_function(m_handle, "peak_Camera_UserDefinedName_Set") : nullptr);
    if(m_peak_Camera_UserDefinedName_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Camera_UserDefinedName_Set");
    }

    m_peak_Camera_UserDefinedName_Get = (dyn_peak_Camera_UserDefinedName_Get) (load ?  import_function(m_handle, "peak_Camera_UserDefinedName_Get") : nullptr);
    if(m_peak_Camera_UserDefinedName_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Camera_UserDefinedName_Get");
    }

    m_peak_Camera_IsConnected = (dyn_peak_Camera_IsConnected) (load ?  import_function(m_handle, "peak_Camera_IsConnected") : nullptr);
    if(m_peak_Camera_IsConnected == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Camera_IsConnected");
    }

    m_peak_Camera_Reboot = (dyn_peak_Camera_Reboot) (load ?  import_function(m_handle, "peak_Camera_Reboot") : nullptr);
    if(m_peak_Camera_Reboot == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Camera_Reboot");
    }

    m_peak_EthernetConfig_GetAccessStatus = (dyn_peak_EthernetConfig_GetAccessStatus) (load ?  import_function(m_handle, "peak_EthernetConfig_GetAccessStatus") : nullptr);
    if(m_peak_EthernetConfig_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_EthernetConfig_GetAccessStatus");
    }

    m_peak_EthernetConfig_GetInfo = (dyn_peak_EthernetConfig_GetInfo) (load ?  import_function(m_handle, "peak_EthernetConfig_GetInfo") : nullptr);
    if(m_peak_EthernetConfig_GetInfo == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_EthernetConfig_GetInfo");
    }

    m_peak_EthernetConfig_DHCP_GetAccessStatus = (dyn_peak_EthernetConfig_DHCP_GetAccessStatus) (load ?  import_function(m_handle, "peak_EthernetConfig_DHCP_GetAccessStatus") : nullptr);
    if(m_peak_EthernetConfig_DHCP_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_EthernetConfig_DHCP_GetAccessStatus");
    }

    m_peak_EthernetConfig_DHCP_Enable = (dyn_peak_EthernetConfig_DHCP_Enable) (load ?  import_function(m_handle, "peak_EthernetConfig_DHCP_Enable") : nullptr);
    if(m_peak_EthernetConfig_DHCP_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_EthernetConfig_DHCP_Enable");
    }

    m_peak_EthernetConfig_DHCP_IsEnabled = (dyn_peak_EthernetConfig_DHCP_IsEnabled) (load ?  import_function(m_handle, "peak_EthernetConfig_DHCP_IsEnabled") : nullptr);
    if(m_peak_EthernetConfig_DHCP_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_EthernetConfig_DHCP_IsEnabled");
    }

    m_peak_EthernetConfig_PersistentIP_GetAccessStatus = (dyn_peak_EthernetConfig_PersistentIP_GetAccessStatus) (load ?  import_function(m_handle, "peak_EthernetConfig_PersistentIP_GetAccessStatus") : nullptr);
    if(m_peak_EthernetConfig_PersistentIP_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_EthernetConfig_PersistentIP_GetAccessStatus");
    }

    m_peak_EthernetConfig_PersistentIP_Set = (dyn_peak_EthernetConfig_PersistentIP_Set) (load ?  import_function(m_handle, "peak_EthernetConfig_PersistentIP_Set") : nullptr);
    if(m_peak_EthernetConfig_PersistentIP_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_EthernetConfig_PersistentIP_Set");
    }

    m_peak_EthernetConfig_PersistentIP_Get = (dyn_peak_EthernetConfig_PersistentIP_Get) (load ?  import_function(m_handle, "peak_EthernetConfig_PersistentIP_Get") : nullptr);
    if(m_peak_EthernetConfig_PersistentIP_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_EthernetConfig_PersistentIP_Get");
    }

    m_peak_Acquisition_Start = (dyn_peak_Acquisition_Start) (load ?  import_function(m_handle, "peak_Acquisition_Start") : nullptr);
    if(m_peak_Acquisition_Start == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_Start");
    }

    m_peak_Acquisition_Stop = (dyn_peak_Acquisition_Stop) (load ?  import_function(m_handle, "peak_Acquisition_Stop") : nullptr);
    if(m_peak_Acquisition_Stop == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_Stop");
    }

    m_peak_Acquisition_IsStarted = (dyn_peak_Acquisition_IsStarted) (load ?  import_function(m_handle, "peak_Acquisition_IsStarted") : nullptr);
    if(m_peak_Acquisition_IsStarted == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_IsStarted");
    }

    m_peak_Acquisition_WaitForFrame = (dyn_peak_Acquisition_WaitForFrame) (load ?  import_function(m_handle, "peak_Acquisition_WaitForFrame") : nullptr);
    if(m_peak_Acquisition_WaitForFrame == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_WaitForFrame");
    }

    m_peak_Acquisition_GetInfo = (dyn_peak_Acquisition_GetInfo) (load ?  import_function(m_handle, "peak_Acquisition_GetInfo") : nullptr);
    if(m_peak_Acquisition_GetInfo == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_GetInfo");
    }

    m_peak_Acquisition_Buffer_GetRequiredSize = (dyn_peak_Acquisition_Buffer_GetRequiredSize) (load ?  import_function(m_handle, "peak_Acquisition_Buffer_GetRequiredSize") : nullptr);
    if(m_peak_Acquisition_Buffer_GetRequiredSize == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_Buffer_GetRequiredSize");
    }

    m_peak_Acquisition_Buffer_GetRequiredCount = (dyn_peak_Acquisition_Buffer_GetRequiredCount) (load ?  import_function(m_handle, "peak_Acquisition_Buffer_GetRequiredCount") : nullptr);
    if(m_peak_Acquisition_Buffer_GetRequiredCount == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_Buffer_GetRequiredCount");
    }

    m_peak_Acquisition_Buffer_Announce = (dyn_peak_Acquisition_Buffer_Announce) (load ?  import_function(m_handle, "peak_Acquisition_Buffer_Announce") : nullptr);
    if(m_peak_Acquisition_Buffer_Announce == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_Buffer_Announce");
    }

    m_peak_Acquisition_Buffer_Revoke = (dyn_peak_Acquisition_Buffer_Revoke) (load ?  import_function(m_handle, "peak_Acquisition_Buffer_Revoke") : nullptr);
    if(m_peak_Acquisition_Buffer_Revoke == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_Buffer_Revoke");
    }

    m_peak_Acquisition_Buffer_RevokeAll = (dyn_peak_Acquisition_Buffer_RevokeAll) (load ?  import_function(m_handle, "peak_Acquisition_Buffer_RevokeAll") : nullptr);
    if(m_peak_Acquisition_Buffer_RevokeAll == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_Buffer_RevokeAll");
    }

    m_peak_Acquisition_BufferHandling_Mode_GetAccessStatus = (dyn_peak_Acquisition_BufferHandling_Mode_GetAccessStatus) (load ?  import_function(m_handle, "peak_Acquisition_BufferHandling_Mode_GetAccessStatus") : nullptr);
    if(m_peak_Acquisition_BufferHandling_Mode_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_BufferHandling_Mode_GetAccessStatus");
    }

    m_peak_Acquisition_BufferHandling_Mode_GetList = (dyn_peak_Acquisition_BufferHandling_Mode_GetList) (load ?  import_function(m_handle, "peak_Acquisition_BufferHandling_Mode_GetList") : nullptr);
    if(m_peak_Acquisition_BufferHandling_Mode_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_BufferHandling_Mode_GetList");
    }

    m_peak_Acquisition_BufferHandling_Mode_Set = (dyn_peak_Acquisition_BufferHandling_Mode_Set) (load ?  import_function(m_handle, "peak_Acquisition_BufferHandling_Mode_Set") : nullptr);
    if(m_peak_Acquisition_BufferHandling_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_BufferHandling_Mode_Set");
    }

    m_peak_Acquisition_BufferHandling_Mode_Get = (dyn_peak_Acquisition_BufferHandling_Mode_Get) (load ?  import_function(m_handle, "peak_Acquisition_BufferHandling_Mode_Get") : nullptr);
    if(m_peak_Acquisition_BufferHandling_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_BufferHandling_Mode_Get");
    }

    m_peak_Acquisition_LossHandling_Mode_GetAccessStatus = (dyn_peak_Acquisition_LossHandling_Mode_GetAccessStatus) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_Mode_GetAccessStatus") : nullptr);
    if(m_peak_Acquisition_LossHandling_Mode_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_Mode_GetAccessStatus");
    }

    m_peak_Acquisition_LossHandling_Mode_GetList = (dyn_peak_Acquisition_LossHandling_Mode_GetList) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_Mode_GetList") : nullptr);
    if(m_peak_Acquisition_LossHandling_Mode_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_Mode_GetList");
    }

    m_peak_Acquisition_LossHandling_Mode_Set = (dyn_peak_Acquisition_LossHandling_Mode_Set) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_Mode_Set") : nullptr);
    if(m_peak_Acquisition_LossHandling_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_Mode_Set");
    }

    m_peak_Acquisition_LossHandling_Mode_Get = (dyn_peak_Acquisition_LossHandling_Mode_Get) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_Mode_Get") : nullptr);
    if(m_peak_Acquisition_LossHandling_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_Mode_Get");
    }

    m_peak_Acquisition_LossHandling_Extent_GetAccessStatus = (dyn_peak_Acquisition_LossHandling_Extent_GetAccessStatus) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_Extent_GetAccessStatus") : nullptr);
    if(m_peak_Acquisition_LossHandling_Extent_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_Extent_GetAccessStatus");
    }

    m_peak_Acquisition_LossHandling_Extent_GetRange = (dyn_peak_Acquisition_LossHandling_Extent_GetRange) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_Extent_GetRange") : nullptr);
    if(m_peak_Acquisition_LossHandling_Extent_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_Extent_GetRange");
    }

    m_peak_Acquisition_LossHandling_Extent_Set = (dyn_peak_Acquisition_LossHandling_Extent_Set) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_Extent_Set") : nullptr);
    if(m_peak_Acquisition_LossHandling_Extent_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_Extent_Set");
    }

    m_peak_Acquisition_LossHandling_Extent_Get = (dyn_peak_Acquisition_LossHandling_Extent_Get) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_Extent_Get") : nullptr);
    if(m_peak_Acquisition_LossHandling_Extent_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_Extent_Get");
    }

    m_peak_Acquisition_LossHandling_FrameAbortTimeout_GetAccessStatus = (dyn_peak_Acquisition_LossHandling_FrameAbortTimeout_GetAccessStatus) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_FrameAbortTimeout_GetAccessStatus") : nullptr);
    if(m_peak_Acquisition_LossHandling_FrameAbortTimeout_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_FrameAbortTimeout_GetAccessStatus");
    }

    m_peak_Acquisition_LossHandling_FrameAbortTimeout_GetRange = (dyn_peak_Acquisition_LossHandling_FrameAbortTimeout_GetRange) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_FrameAbortTimeout_GetRange") : nullptr);
    if(m_peak_Acquisition_LossHandling_FrameAbortTimeout_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_FrameAbortTimeout_GetRange");
    }

    m_peak_Acquisition_LossHandling_FrameAbortTimeout_Set = (dyn_peak_Acquisition_LossHandling_FrameAbortTimeout_Set) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_FrameAbortTimeout_Set") : nullptr);
    if(m_peak_Acquisition_LossHandling_FrameAbortTimeout_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_FrameAbortTimeout_Set");
    }

    m_peak_Acquisition_LossHandling_FrameAbortTimeout_Get = (dyn_peak_Acquisition_LossHandling_FrameAbortTimeout_Get) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_FrameAbortTimeout_Get") : nullptr);
    if(m_peak_Acquisition_LossHandling_FrameAbortTimeout_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_FrameAbortTimeout_Get");
    }

    m_peak_Acquisition_LossHandling_ResendRequestTimeout_GetAccessStatus = (dyn_peak_Acquisition_LossHandling_ResendRequestTimeout_GetAccessStatus) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_ResendRequestTimeout_GetAccessStatus") : nullptr);
    if(m_peak_Acquisition_LossHandling_ResendRequestTimeout_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_ResendRequestTimeout_GetAccessStatus");
    }

    m_peak_Acquisition_LossHandling_ResendRequestTimeout_GetRange = (dyn_peak_Acquisition_LossHandling_ResendRequestTimeout_GetRange) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_ResendRequestTimeout_GetRange") : nullptr);
    if(m_peak_Acquisition_LossHandling_ResendRequestTimeout_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_ResendRequestTimeout_GetRange");
    }

    m_peak_Acquisition_LossHandling_ResendRequestTimeout_Set = (dyn_peak_Acquisition_LossHandling_ResendRequestTimeout_Set) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_ResendRequestTimeout_Set") : nullptr);
    if(m_peak_Acquisition_LossHandling_ResendRequestTimeout_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_ResendRequestTimeout_Set");
    }

    m_peak_Acquisition_LossHandling_ResendRequestTimeout_Get = (dyn_peak_Acquisition_LossHandling_ResendRequestTimeout_Get) (load ?  import_function(m_handle, "peak_Acquisition_LossHandling_ResendRequestTimeout_Get") : nullptr);
    if(m_peak_Acquisition_LossHandling_ResendRequestTimeout_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Acquisition_LossHandling_ResendRequestTimeout_Get");
    }

    m_peak_PixelFormat_GetInfo = (dyn_peak_PixelFormat_GetInfo) (load ?  import_function(m_handle, "peak_PixelFormat_GetInfo") : nullptr);
    if(m_peak_PixelFormat_GetInfo == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_PixelFormat_GetInfo");
    }

    m_peak_Frame_Release = (dyn_peak_Frame_Release) (load ?  import_function(m_handle, "peak_Frame_Release") : nullptr);
    if(m_peak_Frame_Release == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Frame_Release");
    }

    m_peak_Frame_GetInfo = (dyn_peak_Frame_GetInfo) (load ?  import_function(m_handle, "peak_Frame_GetInfo") : nullptr);
    if(m_peak_Frame_GetInfo == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Frame_GetInfo");
    }

    m_peak_Frame_Type_Get = (dyn_peak_Frame_Type_Get) (load ?  import_function(m_handle, "peak_Frame_Type_Get") : nullptr);
    if(m_peak_Frame_Type_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Frame_Type_Get");
    }

    m_peak_Frame_Buffer_Get = (dyn_peak_Frame_Buffer_Get) (load ?  import_function(m_handle, "peak_Frame_Buffer_Get") : nullptr);
    if(m_peak_Frame_Buffer_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Frame_Buffer_Get");
    }

    m_peak_Frame_ID_Get = (dyn_peak_Frame_ID_Get) (load ?  import_function(m_handle, "peak_Frame_ID_Get") : nullptr);
    if(m_peak_Frame_ID_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Frame_ID_Get");
    }

    m_peak_Frame_Timestamp_Get = (dyn_peak_Frame_Timestamp_Get) (load ?  import_function(m_handle, "peak_Frame_Timestamp_Get") : nullptr);
    if(m_peak_Frame_Timestamp_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Frame_Timestamp_Get");
    }

    m_peak_Frame_ROI_Get = (dyn_peak_Frame_ROI_Get) (load ?  import_function(m_handle, "peak_Frame_ROI_Get") : nullptr);
    if(m_peak_Frame_ROI_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Frame_ROI_Get");
    }

    m_peak_Frame_PixelFormat_Get = (dyn_peak_Frame_PixelFormat_Get) (load ?  import_function(m_handle, "peak_Frame_PixelFormat_Get") : nullptr);
    if(m_peak_Frame_PixelFormat_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Frame_PixelFormat_Get");
    }

    m_peak_Frame_IsComplete = (dyn_peak_Frame_IsComplete) (load ?  import_function(m_handle, "peak_Frame_IsComplete") : nullptr);
    if(m_peak_Frame_IsComplete == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Frame_IsComplete");
    }

    m_peak_Frame_BytesExpected_Get = (dyn_peak_Frame_BytesExpected_Get) (load ?  import_function(m_handle, "peak_Frame_BytesExpected_Get") : nullptr);
    if(m_peak_Frame_BytesExpected_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Frame_BytesExpected_Get");
    }

    m_peak_Frame_BytesWritten_Get = (dyn_peak_Frame_BytesWritten_Get) (load ?  import_function(m_handle, "peak_Frame_BytesWritten_Get") : nullptr);
    if(m_peak_Frame_BytesWritten_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Frame_BytesWritten_Get");
    }

    m_peak_Frame_ProcessingTime_Get = (dyn_peak_Frame_ProcessingTime_Get) (load ?  import_function(m_handle, "peak_Frame_ProcessingTime_Get") : nullptr);
    if(m_peak_Frame_ProcessingTime_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Frame_ProcessingTime_Get");
    }

    m_peak_Frame_Save = (dyn_peak_Frame_Save) (load ?  import_function(m_handle, "peak_Frame_Save") : nullptr);
    if(m_peak_Frame_Save == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Frame_Save");
    }

    m_peak_Frame_HasChunks = (dyn_peak_Frame_HasChunks) (load ?  import_function(m_handle, "peak_Frame_HasChunks") : nullptr);
    if(m_peak_Frame_HasChunks == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Frame_HasChunks");
    }

    m_peak_CameraSettings_ParameterSet_GetAccessStatus = (dyn_peak_CameraSettings_ParameterSet_GetAccessStatus) (load ?  import_function(m_handle, "peak_CameraSettings_ParameterSet_GetAccessStatus") : nullptr);
    if(m_peak_CameraSettings_ParameterSet_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraSettings_ParameterSet_GetAccessStatus");
    }

    m_peak_CameraSettings_ParameterSet_GetList = (dyn_peak_CameraSettings_ParameterSet_GetList) (load ?  import_function(m_handle, "peak_CameraSettings_ParameterSet_GetList") : nullptr);
    if(m_peak_CameraSettings_ParameterSet_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraSettings_ParameterSet_GetList");
    }

    m_peak_CameraSettings_ParameterSet_Store = (dyn_peak_CameraSettings_ParameterSet_Store) (load ?  import_function(m_handle, "peak_CameraSettings_ParameterSet_Store") : nullptr);
    if(m_peak_CameraSettings_ParameterSet_Store == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraSettings_ParameterSet_Store");
    }

    m_peak_CameraSettings_ParameterSet_Apply = (dyn_peak_CameraSettings_ParameterSet_Apply) (load ?  import_function(m_handle, "peak_CameraSettings_ParameterSet_Apply") : nullptr);
    if(m_peak_CameraSettings_ParameterSet_Apply == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraSettings_ParameterSet_Apply");
    }

    m_peak_CameraSettings_ParameterSet_Startup_GetAccessStatus = (dyn_peak_CameraSettings_ParameterSet_Startup_GetAccessStatus) (load ?  import_function(m_handle, "peak_CameraSettings_ParameterSet_Startup_GetAccessStatus") : nullptr);
    if(m_peak_CameraSettings_ParameterSet_Startup_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraSettings_ParameterSet_Startup_GetAccessStatus");
    }

    m_peak_CameraSettings_ParameterSet_Startup_Set = (dyn_peak_CameraSettings_ParameterSet_Startup_Set) (load ?  import_function(m_handle, "peak_CameraSettings_ParameterSet_Startup_Set") : nullptr);
    if(m_peak_CameraSettings_ParameterSet_Startup_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraSettings_ParameterSet_Startup_Set");
    }

    m_peak_CameraSettings_ParameterSet_Startup_Get = (dyn_peak_CameraSettings_ParameterSet_Startup_Get) (load ?  import_function(m_handle, "peak_CameraSettings_ParameterSet_Startup_Get") : nullptr);
    if(m_peak_CameraSettings_ParameterSet_Startup_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraSettings_ParameterSet_Startup_Get");
    }

    m_peak_CameraSettings_DiskFile_GetAccessStatus = (dyn_peak_CameraSettings_DiskFile_GetAccessStatus) (load ?  import_function(m_handle, "peak_CameraSettings_DiskFile_GetAccessStatus") : nullptr);
    if(m_peak_CameraSettings_DiskFile_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraSettings_DiskFile_GetAccessStatus");
    }

    m_peak_CameraSettings_DiskFile_Store = (dyn_peak_CameraSettings_DiskFile_Store) (load ?  import_function(m_handle, "peak_CameraSettings_DiskFile_Store") : nullptr);
    if(m_peak_CameraSettings_DiskFile_Store == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraSettings_DiskFile_Store");
    }

    m_peak_CameraSettings_DiskFile_Apply = (dyn_peak_CameraSettings_DiskFile_Apply) (load ?  import_function(m_handle, "peak_CameraSettings_DiskFile_Apply") : nullptr);
    if(m_peak_CameraSettings_DiskFile_Apply == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraSettings_DiskFile_Apply");
    }

    m_peak_FrameRate_GetAccessStatus = (dyn_peak_FrameRate_GetAccessStatus) (load ?  import_function(m_handle, "peak_FrameRate_GetAccessStatus") : nullptr);
    if(m_peak_FrameRate_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_FrameRate_GetAccessStatus");
    }

    m_peak_FrameRate_GetRange = (dyn_peak_FrameRate_GetRange) (load ?  import_function(m_handle, "peak_FrameRate_GetRange") : nullptr);
    if(m_peak_FrameRate_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_FrameRate_GetRange");
    }

    m_peak_FrameRate_Set = (dyn_peak_FrameRate_Set) (load ?  import_function(m_handle, "peak_FrameRate_Set") : nullptr);
    if(m_peak_FrameRate_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_FrameRate_Set");
    }

    m_peak_FrameRate_Get = (dyn_peak_FrameRate_Get) (load ?  import_function(m_handle, "peak_FrameRate_Get") : nullptr);
    if(m_peak_FrameRate_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_FrameRate_Get");
    }

    m_peak_ExposureTime_GetAccessStatus = (dyn_peak_ExposureTime_GetAccessStatus) (load ?  import_function(m_handle, "peak_ExposureTime_GetAccessStatus") : nullptr);
    if(m_peak_ExposureTime_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ExposureTime_GetAccessStatus");
    }

    m_peak_ExposureTime_GetRange = (dyn_peak_ExposureTime_GetRange) (load ?  import_function(m_handle, "peak_ExposureTime_GetRange") : nullptr);
    if(m_peak_ExposureTime_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ExposureTime_GetRange");
    }

    m_peak_ExposureTime_Set = (dyn_peak_ExposureTime_Set) (load ?  import_function(m_handle, "peak_ExposureTime_Set") : nullptr);
    if(m_peak_ExposureTime_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ExposureTime_Set");
    }

    m_peak_ExposureTime_Get = (dyn_peak_ExposureTime_Get) (load ?  import_function(m_handle, "peak_ExposureTime_Get") : nullptr);
    if(m_peak_ExposureTime_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ExposureTime_Get");
    }

    m_peak_ShutterMode_GetAccessStatus = (dyn_peak_ShutterMode_GetAccessStatus) (load ?  import_function(m_handle, "peak_ShutterMode_GetAccessStatus") : nullptr);
    if(m_peak_ShutterMode_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ShutterMode_GetAccessStatus");
    }

    m_peak_ShutterMode_GetList = (dyn_peak_ShutterMode_GetList) (load ?  import_function(m_handle, "peak_ShutterMode_GetList") : nullptr);
    if(m_peak_ShutterMode_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ShutterMode_GetList");
    }

    m_peak_ShutterMode_Set = (dyn_peak_ShutterMode_Set) (load ?  import_function(m_handle, "peak_ShutterMode_Set") : nullptr);
    if(m_peak_ShutterMode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ShutterMode_Set");
    }

    m_peak_ShutterMode_Get = (dyn_peak_ShutterMode_Get) (load ?  import_function(m_handle, "peak_ShutterMode_Get") : nullptr);
    if(m_peak_ShutterMode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ShutterMode_Get");
    }

    m_peak_PixelClock_GetAccessStatus = (dyn_peak_PixelClock_GetAccessStatus) (load ?  import_function(m_handle, "peak_PixelClock_GetAccessStatus") : nullptr);
    if(m_peak_PixelClock_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_PixelClock_GetAccessStatus");
    }

    m_peak_PixelClock_HasRange = (dyn_peak_PixelClock_HasRange) (load ?  import_function(m_handle, "peak_PixelClock_HasRange") : nullptr);
    if(m_peak_PixelClock_HasRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_PixelClock_HasRange");
    }

    m_peak_PixelClock_GetRange = (dyn_peak_PixelClock_GetRange) (load ?  import_function(m_handle, "peak_PixelClock_GetRange") : nullptr);
    if(m_peak_PixelClock_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_PixelClock_GetRange");
    }

    m_peak_PixelClock_GetList = (dyn_peak_PixelClock_GetList) (load ?  import_function(m_handle, "peak_PixelClock_GetList") : nullptr);
    if(m_peak_PixelClock_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_PixelClock_GetList");
    }

    m_peak_PixelClock_Set = (dyn_peak_PixelClock_Set) (load ?  import_function(m_handle, "peak_PixelClock_Set") : nullptr);
    if(m_peak_PixelClock_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_PixelClock_Set");
    }

    m_peak_PixelClock_Get = (dyn_peak_PixelClock_Get) (load ?  import_function(m_handle, "peak_PixelClock_Get") : nullptr);
    if(m_peak_PixelClock_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_PixelClock_Get");
    }

    m_peak_IOChannel_GetAccessStatus = (dyn_peak_IOChannel_GetAccessStatus) (load ?  import_function(m_handle, "peak_IOChannel_GetAccessStatus") : nullptr);
    if(m_peak_IOChannel_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_GetAccessStatus");
    }

    m_PEAK_API_STATUS_DEPRECATED = (dyn_PEAK_API_STATUS_DEPRECATED) (load ?  import_function(m_handle, "PEAK_API_STATUS_DEPRECATED") : nullptr);
    if(m_PEAK_API_STATUS_DEPRECATED == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_API_STATUS_DEPRECATED");
    }

    m_peak_IOChannel_GetListForDirection = (dyn_peak_IOChannel_GetListForDirection) (load ?  import_function(m_handle, "peak_IOChannel_GetListForDirection") : nullptr);
    if(m_peak_IOChannel_GetListForDirection == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_GetListForDirection");
    }

    m_peak_IOChannel_Direction_GetAccessStatus = (dyn_peak_IOChannel_Direction_GetAccessStatus) (load ?  import_function(m_handle, "peak_IOChannel_Direction_GetAccessStatus") : nullptr);
    if(m_peak_IOChannel_Direction_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_Direction_GetAccessStatus");
    }

    m_peak_IOChannel_Direction_Get = (dyn_peak_IOChannel_Direction_Get) (load ?  import_function(m_handle, "peak_IOChannel_Direction_Get") : nullptr);
    if(m_peak_IOChannel_Direction_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_Direction_Get");
    }

    m_peak_IOChannel_Type_GetAccessStatus = (dyn_peak_IOChannel_Type_GetAccessStatus) (load ?  import_function(m_handle, "peak_IOChannel_Type_GetAccessStatus") : nullptr);
    if(m_peak_IOChannel_Type_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_Type_GetAccessStatus");
    }

    m_peak_IOChannel_Type_Get = (dyn_peak_IOChannel_Type_Get) (load ?  import_function(m_handle, "peak_IOChannel_Type_Get") : nullptr);
    if(m_peak_IOChannel_Type_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_Type_Get");
    }

    m_peak_IOChannel_Level_GetAccessStatus = (dyn_peak_IOChannel_Level_GetAccessStatus) (load ?  import_function(m_handle, "peak_IOChannel_Level_GetAccessStatus") : nullptr);
    if(m_peak_IOChannel_Level_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_Level_GetAccessStatus");
    }

    m_peak_IOChannel_Level_IsHigh = (dyn_peak_IOChannel_Level_IsHigh) (load ?  import_function(m_handle, "peak_IOChannel_Level_IsHigh") : nullptr);
    if(m_peak_IOChannel_Level_IsHigh == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_Level_IsHigh");
    }

    m_peak_IOChannel_Level_SetHigh = (dyn_peak_IOChannel_Level_SetHigh) (load ?  import_function(m_handle, "peak_IOChannel_Level_SetHigh") : nullptr);
    if(m_peak_IOChannel_Level_SetHigh == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_Level_SetHigh");
    }

    m_peak_IOChannel_Inverter_GetAccessStatus = (dyn_peak_IOChannel_Inverter_GetAccessStatus) (load ?  import_function(m_handle, "peak_IOChannel_Inverter_GetAccessStatus") : nullptr);
    if(m_peak_IOChannel_Inverter_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_Inverter_GetAccessStatus");
    }

    m_peak_IOChannel_Inverter_Enable = (dyn_peak_IOChannel_Inverter_Enable) (load ?  import_function(m_handle, "peak_IOChannel_Inverter_Enable") : nullptr);
    if(m_peak_IOChannel_Inverter_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_Inverter_Enable");
    }

    m_peak_IOChannel_Inverter_IsEnabled = (dyn_peak_IOChannel_Inverter_IsEnabled) (load ?  import_function(m_handle, "peak_IOChannel_Inverter_IsEnabled") : nullptr);
    if(m_peak_IOChannel_Inverter_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_Inverter_IsEnabled");
    }

    m_peak_IOChannel_NoiseFilter_GetAccessStatus = (dyn_peak_IOChannel_NoiseFilter_GetAccessStatus) (load ?  import_function(m_handle, "peak_IOChannel_NoiseFilter_GetAccessStatus") : nullptr);
    if(m_peak_IOChannel_NoiseFilter_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_NoiseFilter_GetAccessStatus");
    }

    m_peak_IOChannel_NoiseFilter_Enable = (dyn_peak_IOChannel_NoiseFilter_Enable) (load ?  import_function(m_handle, "peak_IOChannel_NoiseFilter_Enable") : nullptr);
    if(m_peak_IOChannel_NoiseFilter_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_NoiseFilter_Enable");
    }

    m_peak_IOChannel_NoiseFilter_IsEnabled = (dyn_peak_IOChannel_NoiseFilter_IsEnabled) (load ?  import_function(m_handle, "peak_IOChannel_NoiseFilter_IsEnabled") : nullptr);
    if(m_peak_IOChannel_NoiseFilter_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_NoiseFilter_IsEnabled");
    }

    m_peak_IOChannel_NoiseFilter_Duration_GetRange = (dyn_peak_IOChannel_NoiseFilter_Duration_GetRange) (load ?  import_function(m_handle, "peak_IOChannel_NoiseFilter_Duration_GetRange") : nullptr);
    if(m_peak_IOChannel_NoiseFilter_Duration_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_NoiseFilter_Duration_GetRange");
    }

    m_peak_IOChannel_NoiseFilter_Duration_Set = (dyn_peak_IOChannel_NoiseFilter_Duration_Set) (load ?  import_function(m_handle, "peak_IOChannel_NoiseFilter_Duration_Set") : nullptr);
    if(m_peak_IOChannel_NoiseFilter_Duration_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_NoiseFilter_Duration_Set");
    }

    m_peak_IOChannel_NoiseFilter_Duration_Get = (dyn_peak_IOChannel_NoiseFilter_Duration_Get) (load ?  import_function(m_handle, "peak_IOChannel_NoiseFilter_Duration_Get") : nullptr);
    if(m_peak_IOChannel_NoiseFilter_Duration_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IOChannel_NoiseFilter_Duration_Get");
    }

    m_peak_Trigger_GetAccessStatus = (dyn_peak_Trigger_GetAccessStatus) (load ?  import_function(m_handle, "peak_Trigger_GetAccessStatus") : nullptr);
    if(m_peak_Trigger_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_GetAccessStatus");
    }

    m_peak_Trigger_Enable = (dyn_peak_Trigger_Enable) (load ?  import_function(m_handle, "peak_Trigger_Enable") : nullptr);
    if(m_peak_Trigger_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Enable");
    }

    m_peak_Trigger_IsEnabled = (dyn_peak_Trigger_IsEnabled) (load ?  import_function(m_handle, "peak_Trigger_IsEnabled") : nullptr);
    if(m_peak_Trigger_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_IsEnabled");
    }

    m_peak_Trigger_IsExecutable = (dyn_peak_Trigger_IsExecutable) (load ?  import_function(m_handle, "peak_Trigger_IsExecutable") : nullptr);
    if(m_peak_Trigger_IsExecutable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_IsExecutable");
    }

    m_peak_Trigger_Execute = (dyn_peak_Trigger_Execute) (load ?  import_function(m_handle, "peak_Trigger_Execute") : nullptr);
    if(m_peak_Trigger_Execute == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Execute");
    }

    m_peak_Trigger_Mode_GetAccessStatus = (dyn_peak_Trigger_Mode_GetAccessStatus) (load ?  import_function(m_handle, "peak_Trigger_Mode_GetAccessStatus") : nullptr);
    if(m_peak_Trigger_Mode_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Mode_GetAccessStatus");
    }

    m_peak_Trigger_Mode_Set = (dyn_peak_Trigger_Mode_Set) (load ?  import_function(m_handle, "peak_Trigger_Mode_Set") : nullptr);
    if(m_peak_Trigger_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Mode_Set");
    }

    m_PEAK_API_STATUS_DEPRECATED = (dyn_PEAK_API_STATUS_DEPRECATED) (load ?  import_function(m_handle, "PEAK_API_STATUS_DEPRECATED") : nullptr);
    if(m_PEAK_API_STATUS_DEPRECATED == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_API_STATUS_DEPRECATED");
    }

    m_peak_Trigger_Mode_Config_Get = (dyn_peak_Trigger_Mode_Config_Get) (load ?  import_function(m_handle, "peak_Trigger_Mode_Config_Get") : nullptr);
    if(m_peak_Trigger_Mode_Config_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Mode_Config_Get");
    }

    m_peak_Trigger_Edge_GetAccessStatus = (dyn_peak_Trigger_Edge_GetAccessStatus) (load ?  import_function(m_handle, "peak_Trigger_Edge_GetAccessStatus") : nullptr);
    if(m_peak_Trigger_Edge_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Edge_GetAccessStatus");
    }

    m_peak_Trigger_Edge_GetList = (dyn_peak_Trigger_Edge_GetList) (load ?  import_function(m_handle, "peak_Trigger_Edge_GetList") : nullptr);
    if(m_peak_Trigger_Edge_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Edge_GetList");
    }

    m_peak_Trigger_Edge_Set = (dyn_peak_Trigger_Edge_Set) (load ?  import_function(m_handle, "peak_Trigger_Edge_Set") : nullptr);
    if(m_peak_Trigger_Edge_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Edge_Set");
    }

    m_peak_Trigger_Edge_Get = (dyn_peak_Trigger_Edge_Get) (load ?  import_function(m_handle, "peak_Trigger_Edge_Get") : nullptr);
    if(m_peak_Trigger_Edge_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Edge_Get");
    }

    m_peak_Trigger_Delay_GetAccessStatus = (dyn_peak_Trigger_Delay_GetAccessStatus) (load ?  import_function(m_handle, "peak_Trigger_Delay_GetAccessStatus") : nullptr);
    if(m_peak_Trigger_Delay_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Delay_GetAccessStatus");
    }

    m_peak_Trigger_Delay_GetRange = (dyn_peak_Trigger_Delay_GetRange) (load ?  import_function(m_handle, "peak_Trigger_Delay_GetRange") : nullptr);
    if(m_peak_Trigger_Delay_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Delay_GetRange");
    }

    m_peak_Trigger_Delay_Set = (dyn_peak_Trigger_Delay_Set) (load ?  import_function(m_handle, "peak_Trigger_Delay_Set") : nullptr);
    if(m_peak_Trigger_Delay_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Delay_Set");
    }

    m_peak_Trigger_Delay_Get = (dyn_peak_Trigger_Delay_Get) (load ?  import_function(m_handle, "peak_Trigger_Delay_Get") : nullptr);
    if(m_peak_Trigger_Delay_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Delay_Get");
    }

    m_peak_Trigger_Divider_GetAccessStatus = (dyn_peak_Trigger_Divider_GetAccessStatus) (load ?  import_function(m_handle, "peak_Trigger_Divider_GetAccessStatus") : nullptr);
    if(m_peak_Trigger_Divider_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Divider_GetAccessStatus");
    }

    m_peak_Trigger_Divider_GetRange = (dyn_peak_Trigger_Divider_GetRange) (load ?  import_function(m_handle, "peak_Trigger_Divider_GetRange") : nullptr);
    if(m_peak_Trigger_Divider_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Divider_GetRange");
    }

    m_peak_Trigger_Divider_Set = (dyn_peak_Trigger_Divider_Set) (load ?  import_function(m_handle, "peak_Trigger_Divider_Set") : nullptr);
    if(m_peak_Trigger_Divider_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Divider_Set");
    }

    m_peak_Trigger_Divider_Get = (dyn_peak_Trigger_Divider_Get) (load ?  import_function(m_handle, "peak_Trigger_Divider_Get") : nullptr);
    if(m_peak_Trigger_Divider_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Divider_Get");
    }

    m_peak_Trigger_Burst_GetAccessStatus = (dyn_peak_Trigger_Burst_GetAccessStatus) (load ?  import_function(m_handle, "peak_Trigger_Burst_GetAccessStatus") : nullptr);
    if(m_peak_Trigger_Burst_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Burst_GetAccessStatus");
    }

    m_peak_Trigger_Burst_GetRange = (dyn_peak_Trigger_Burst_GetRange) (load ?  import_function(m_handle, "peak_Trigger_Burst_GetRange") : nullptr);
    if(m_peak_Trigger_Burst_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Burst_GetRange");
    }

    m_peak_Trigger_Burst_Set = (dyn_peak_Trigger_Burst_Set) (load ?  import_function(m_handle, "peak_Trigger_Burst_Set") : nullptr);
    if(m_peak_Trigger_Burst_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Burst_Set");
    }

    m_peak_Trigger_Burst_Get = (dyn_peak_Trigger_Burst_Get) (load ?  import_function(m_handle, "peak_Trigger_Burst_Get") : nullptr);
    if(m_peak_Trigger_Burst_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Trigger_Burst_Get");
    }

    m_peak_Flash_GetAccessStatus = (dyn_peak_Flash_GetAccessStatus) (load ?  import_function(m_handle, "peak_Flash_GetAccessStatus") : nullptr);
    if(m_peak_Flash_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_GetAccessStatus");
    }

    m_peak_Flash_Enable = (dyn_peak_Flash_Enable) (load ?  import_function(m_handle, "peak_Flash_Enable") : nullptr);
    if(m_peak_Flash_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_Enable");
    }

    m_peak_Flash_IsEnabled = (dyn_peak_Flash_IsEnabled) (load ?  import_function(m_handle, "peak_Flash_IsEnabled") : nullptr);
    if(m_peak_Flash_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_IsEnabled");
    }

    m_peak_Flash_Mode_GetAccessStatus = (dyn_peak_Flash_Mode_GetAccessStatus) (load ?  import_function(m_handle, "peak_Flash_Mode_GetAccessStatus") : nullptr);
    if(m_peak_Flash_Mode_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_Mode_GetAccessStatus");
    }

    m_peak_Flash_Mode_Set = (dyn_peak_Flash_Mode_Set) (load ?  import_function(m_handle, "peak_Flash_Mode_Set") : nullptr);
    if(m_peak_Flash_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_Mode_Set");
    }

    m_PEAK_API_STATUS_DEPRECATED = (dyn_PEAK_API_STATUS_DEPRECATED) (load ?  import_function(m_handle, "PEAK_API_STATUS_DEPRECATED") : nullptr);
    if(m_PEAK_API_STATUS_DEPRECATED == nullptr && load)
    {
        throw std::runtime_error("Failed to load PEAK_API_STATUS_DEPRECATED");
    }

    m_peak_Flash_Mode_Config_Get = (dyn_peak_Flash_Mode_Config_Get) (load ?  import_function(m_handle, "peak_Flash_Mode_Config_Get") : nullptr);
    if(m_peak_Flash_Mode_Config_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_Mode_Config_Get");
    }

    m_peak_Flash_StartDelay_GetAccessStatus = (dyn_peak_Flash_StartDelay_GetAccessStatus) (load ?  import_function(m_handle, "peak_Flash_StartDelay_GetAccessStatus") : nullptr);
    if(m_peak_Flash_StartDelay_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_StartDelay_GetAccessStatus");
    }

    m_peak_Flash_StartDelay_GetRange = (dyn_peak_Flash_StartDelay_GetRange) (load ?  import_function(m_handle, "peak_Flash_StartDelay_GetRange") : nullptr);
    if(m_peak_Flash_StartDelay_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_StartDelay_GetRange");
    }

    m_peak_Flash_StartDelay_Set = (dyn_peak_Flash_StartDelay_Set) (load ?  import_function(m_handle, "peak_Flash_StartDelay_Set") : nullptr);
    if(m_peak_Flash_StartDelay_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_StartDelay_Set");
    }

    m_peak_Flash_StartDelay_Get = (dyn_peak_Flash_StartDelay_Get) (load ?  import_function(m_handle, "peak_Flash_StartDelay_Get") : nullptr);
    if(m_peak_Flash_StartDelay_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_StartDelay_Get");
    }

    m_peak_Flash_EndDelay_GetAccessStatus = (dyn_peak_Flash_EndDelay_GetAccessStatus) (load ?  import_function(m_handle, "peak_Flash_EndDelay_GetAccessStatus") : nullptr);
    if(m_peak_Flash_EndDelay_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_EndDelay_GetAccessStatus");
    }

    m_peak_Flash_EndDelay_GetRange = (dyn_peak_Flash_EndDelay_GetRange) (load ?  import_function(m_handle, "peak_Flash_EndDelay_GetRange") : nullptr);
    if(m_peak_Flash_EndDelay_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_EndDelay_GetRange");
    }

    m_peak_Flash_EndDelay_Set = (dyn_peak_Flash_EndDelay_Set) (load ?  import_function(m_handle, "peak_Flash_EndDelay_Set") : nullptr);
    if(m_peak_Flash_EndDelay_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_EndDelay_Set");
    }

    m_peak_Flash_EndDelay_Get = (dyn_peak_Flash_EndDelay_Get) (load ?  import_function(m_handle, "peak_Flash_EndDelay_Get") : nullptr);
    if(m_peak_Flash_EndDelay_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_EndDelay_Get");
    }

    m_peak_Flash_Duration_GetAccessStatus = (dyn_peak_Flash_Duration_GetAccessStatus) (load ?  import_function(m_handle, "peak_Flash_Duration_GetAccessStatus") : nullptr);
    if(m_peak_Flash_Duration_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_Duration_GetAccessStatus");
    }

    m_peak_Flash_Duration_GetRange = (dyn_peak_Flash_Duration_GetRange) (load ?  import_function(m_handle, "peak_Flash_Duration_GetRange") : nullptr);
    if(m_peak_Flash_Duration_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_Duration_GetRange");
    }

    m_peak_Flash_Duration_Set = (dyn_peak_Flash_Duration_Set) (load ?  import_function(m_handle, "peak_Flash_Duration_Set") : nullptr);
    if(m_peak_Flash_Duration_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_Duration_Set");
    }

    m_peak_Flash_Duration_Get = (dyn_peak_Flash_Duration_Get) (load ?  import_function(m_handle, "peak_Flash_Duration_Get") : nullptr);
    if(m_peak_Flash_Duration_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Flash_Duration_Get");
    }

    m_peak_Focus_GetAccessStatus = (dyn_peak_Focus_GetAccessStatus) (load ?  import_function(m_handle, "peak_Focus_GetAccessStatus") : nullptr);
    if(m_peak_Focus_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Focus_GetAccessStatus");
    }

    m_peak_Focus_GetRange = (dyn_peak_Focus_GetRange) (load ?  import_function(m_handle, "peak_Focus_GetRange") : nullptr);
    if(m_peak_Focus_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Focus_GetRange");
    }

    m_peak_Focus_Set = (dyn_peak_Focus_Set) (load ?  import_function(m_handle, "peak_Focus_Set") : nullptr);
    if(m_peak_Focus_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Focus_Set");
    }

    m_peak_Focus_Get = (dyn_peak_Focus_Get) (load ?  import_function(m_handle, "peak_Focus_Get") : nullptr);
    if(m_peak_Focus_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Focus_Get");
    }

    m_peak_PixelFormat_GetAccessStatus = (dyn_peak_PixelFormat_GetAccessStatus) (load ?  import_function(m_handle, "peak_PixelFormat_GetAccessStatus") : nullptr);
    if(m_peak_PixelFormat_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_PixelFormat_GetAccessStatus");
    }

    m_peak_PixelFormat_GetList = (dyn_peak_PixelFormat_GetList) (load ?  import_function(m_handle, "peak_PixelFormat_GetList") : nullptr);
    if(m_peak_PixelFormat_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_PixelFormat_GetList");
    }

    m_peak_PixelFormat_Set = (dyn_peak_PixelFormat_Set) (load ?  import_function(m_handle, "peak_PixelFormat_Set") : nullptr);
    if(m_peak_PixelFormat_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_PixelFormat_Set");
    }

    m_peak_PixelFormat_Get = (dyn_peak_PixelFormat_Get) (load ?  import_function(m_handle, "peak_PixelFormat_Get") : nullptr);
    if(m_peak_PixelFormat_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_PixelFormat_Get");
    }

    m_peak_Gain_GetAccessStatus = (dyn_peak_Gain_GetAccessStatus) (load ?  import_function(m_handle, "peak_Gain_GetAccessStatus") : nullptr);
    if(m_peak_Gain_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Gain_GetAccessStatus");
    }

    m_peak_Gain_GetChannelList = (dyn_peak_Gain_GetChannelList) (load ?  import_function(m_handle, "peak_Gain_GetChannelList") : nullptr);
    if(m_peak_Gain_GetChannelList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Gain_GetChannelList");
    }

    m_peak_Gain_GetRange = (dyn_peak_Gain_GetRange) (load ?  import_function(m_handle, "peak_Gain_GetRange") : nullptr);
    if(m_peak_Gain_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Gain_GetRange");
    }

    m_peak_Gain_Set = (dyn_peak_Gain_Set) (load ?  import_function(m_handle, "peak_Gain_Set") : nullptr);
    if(m_peak_Gain_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Gain_Set");
    }

    m_peak_Gain_Get = (dyn_peak_Gain_Get) (load ?  import_function(m_handle, "peak_Gain_Get") : nullptr);
    if(m_peak_Gain_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Gain_Get");
    }

    m_peak_Gamma_GetAccessStatus = (dyn_peak_Gamma_GetAccessStatus) (load ?  import_function(m_handle, "peak_Gamma_GetAccessStatus") : nullptr);
    if(m_peak_Gamma_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Gamma_GetAccessStatus");
    }

    m_peak_Gamma_GetRange = (dyn_peak_Gamma_GetRange) (load ?  import_function(m_handle, "peak_Gamma_GetRange") : nullptr);
    if(m_peak_Gamma_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Gamma_GetRange");
    }

    m_peak_Gamma_Set = (dyn_peak_Gamma_Set) (load ?  import_function(m_handle, "peak_Gamma_Set") : nullptr);
    if(m_peak_Gamma_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Gamma_Set");
    }

    m_peak_Gamma_Get = (dyn_peak_Gamma_Get) (load ?  import_function(m_handle, "peak_Gamma_Get") : nullptr);
    if(m_peak_Gamma_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Gamma_Get");
    }

    m_peak_ColorCorrection_GetAccessStatus = (dyn_peak_ColorCorrection_GetAccessStatus) (load ?  import_function(m_handle, "peak_ColorCorrection_GetAccessStatus") : nullptr);
    if(m_peak_ColorCorrection_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ColorCorrection_GetAccessStatus");
    }

    m_peak_ColorCorrection_Mode_GetList = (dyn_peak_ColorCorrection_Mode_GetList) (load ?  import_function(m_handle, "peak_ColorCorrection_Mode_GetList") : nullptr);
    if(m_peak_ColorCorrection_Mode_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ColorCorrection_Mode_GetList");
    }

    m_peak_ColorCorrection_Mode_Set = (dyn_peak_ColorCorrection_Mode_Set) (load ?  import_function(m_handle, "peak_ColorCorrection_Mode_Set") : nullptr);
    if(m_peak_ColorCorrection_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ColorCorrection_Mode_Set");
    }

    m_peak_ColorCorrection_Mode_Get = (dyn_peak_ColorCorrection_Mode_Get) (load ?  import_function(m_handle, "peak_ColorCorrection_Mode_Get") : nullptr);
    if(m_peak_ColorCorrection_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ColorCorrection_Mode_Get");
    }

    m_peak_ColorCorrection_Matrix_GetAccessStatus = (dyn_peak_ColorCorrection_Matrix_GetAccessStatus) (load ?  import_function(m_handle, "peak_ColorCorrection_Matrix_GetAccessStatus") : nullptr);
    if(m_peak_ColorCorrection_Matrix_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ColorCorrection_Matrix_GetAccessStatus");
    }

    m_peak_ColorCorrection_Matrix_GetRange = (dyn_peak_ColorCorrection_Matrix_GetRange) (load ?  import_function(m_handle, "peak_ColorCorrection_Matrix_GetRange") : nullptr);
    if(m_peak_ColorCorrection_Matrix_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ColorCorrection_Matrix_GetRange");
    }

    m_peak_ColorCorrection_Matrix_Set = (dyn_peak_ColorCorrection_Matrix_Set) (load ?  import_function(m_handle, "peak_ColorCorrection_Matrix_Set") : nullptr);
    if(m_peak_ColorCorrection_Matrix_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ColorCorrection_Matrix_Set");
    }

    m_peak_ColorCorrection_Matrix_Get = (dyn_peak_ColorCorrection_Matrix_Get) (load ?  import_function(m_handle, "peak_ColorCorrection_Matrix_Get") : nullptr);
    if(m_peak_ColorCorrection_Matrix_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ColorCorrection_Matrix_Get");
    }

    m_peak_ColorCorrection_Enable = (dyn_peak_ColorCorrection_Enable) (load ?  import_function(m_handle, "peak_ColorCorrection_Enable") : nullptr);
    if(m_peak_ColorCorrection_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ColorCorrection_Enable");
    }

    m_peak_ColorCorrection_IsEnabled = (dyn_peak_ColorCorrection_IsEnabled) (load ?  import_function(m_handle, "peak_ColorCorrection_IsEnabled") : nullptr);
    if(m_peak_ColorCorrection_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ColorCorrection_IsEnabled");
    }

    m_peak_AutoBrightness_GetAccessStatus = (dyn_peak_AutoBrightness_GetAccessStatus) (load ?  import_function(m_handle, "peak_AutoBrightness_GetAccessStatus") : nullptr);
    if(m_peak_AutoBrightness_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_GetAccessStatus");
    }

    m_peak_AutoBrightness_Target_GetAccessStatus = (dyn_peak_AutoBrightness_Target_GetAccessStatus) (load ?  import_function(m_handle, "peak_AutoBrightness_Target_GetAccessStatus") : nullptr);
    if(m_peak_AutoBrightness_Target_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_Target_GetAccessStatus");
    }

    m_peak_AutoBrightness_Target_GetRange = (dyn_peak_AutoBrightness_Target_GetRange) (load ?  import_function(m_handle, "peak_AutoBrightness_Target_GetRange") : nullptr);
    if(m_peak_AutoBrightness_Target_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_Target_GetRange");
    }

    m_peak_AutoBrightness_Target_Set = (dyn_peak_AutoBrightness_Target_Set) (load ?  import_function(m_handle, "peak_AutoBrightness_Target_Set") : nullptr);
    if(m_peak_AutoBrightness_Target_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_Target_Set");
    }

    m_peak_AutoBrightness_Target_Get = (dyn_peak_AutoBrightness_Target_Get) (load ?  import_function(m_handle, "peak_AutoBrightness_Target_Get") : nullptr);
    if(m_peak_AutoBrightness_Target_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_Target_Get");
    }

    m_peak_AutoBrightness_TargetTolerance_GetAccessStatus = (dyn_peak_AutoBrightness_TargetTolerance_GetAccessStatus) (load ?  import_function(m_handle, "peak_AutoBrightness_TargetTolerance_GetAccessStatus") : nullptr);
    if(m_peak_AutoBrightness_TargetTolerance_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_TargetTolerance_GetAccessStatus");
    }

    m_peak_AutoBrightness_TargetTolerance_GetRange = (dyn_peak_AutoBrightness_TargetTolerance_GetRange) (load ?  import_function(m_handle, "peak_AutoBrightness_TargetTolerance_GetRange") : nullptr);
    if(m_peak_AutoBrightness_TargetTolerance_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_TargetTolerance_GetRange");
    }

    m_peak_AutoBrightness_TargetTolerance_Set = (dyn_peak_AutoBrightness_TargetTolerance_Set) (load ?  import_function(m_handle, "peak_AutoBrightness_TargetTolerance_Set") : nullptr);
    if(m_peak_AutoBrightness_TargetTolerance_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_TargetTolerance_Set");
    }

    m_peak_AutoBrightness_TargetTolerance_Get = (dyn_peak_AutoBrightness_TargetTolerance_Get) (load ?  import_function(m_handle, "peak_AutoBrightness_TargetTolerance_Get") : nullptr);
    if(m_peak_AutoBrightness_TargetTolerance_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_TargetTolerance_Get");
    }

    m_peak_AutoBrightness_TargetPercentile_GetAccessStatus = (dyn_peak_AutoBrightness_TargetPercentile_GetAccessStatus) (load ?  import_function(m_handle, "peak_AutoBrightness_TargetPercentile_GetAccessStatus") : nullptr);
    if(m_peak_AutoBrightness_TargetPercentile_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_TargetPercentile_GetAccessStatus");
    }

    m_peak_AutoBrightness_TargetPercentile_GetRange = (dyn_peak_AutoBrightness_TargetPercentile_GetRange) (load ?  import_function(m_handle, "peak_AutoBrightness_TargetPercentile_GetRange") : nullptr);
    if(m_peak_AutoBrightness_TargetPercentile_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_TargetPercentile_GetRange");
    }

    m_peak_AutoBrightness_TargetPercentile_Set = (dyn_peak_AutoBrightness_TargetPercentile_Set) (load ?  import_function(m_handle, "peak_AutoBrightness_TargetPercentile_Set") : nullptr);
    if(m_peak_AutoBrightness_TargetPercentile_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_TargetPercentile_Set");
    }

    m_peak_AutoBrightness_TargetPercentile_Get = (dyn_peak_AutoBrightness_TargetPercentile_Get) (load ?  import_function(m_handle, "peak_AutoBrightness_TargetPercentile_Get") : nullptr);
    if(m_peak_AutoBrightness_TargetPercentile_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_TargetPercentile_Get");
    }

    m_peak_AutoBrightness_ROI_GetAccessStatus = (dyn_peak_AutoBrightness_ROI_GetAccessStatus) (load ?  import_function(m_handle, "peak_AutoBrightness_ROI_GetAccessStatus") : nullptr);
    if(m_peak_AutoBrightness_ROI_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_ROI_GetAccessStatus");
    }

    m_peak_AutoBrightness_ROI_Mode_Set = (dyn_peak_AutoBrightness_ROI_Mode_Set) (load ?  import_function(m_handle, "peak_AutoBrightness_ROI_Mode_Set") : nullptr);
    if(m_peak_AutoBrightness_ROI_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_ROI_Mode_Set");
    }

    m_peak_AutoBrightness_ROI_Mode_Get = (dyn_peak_AutoBrightness_ROI_Mode_Get) (load ?  import_function(m_handle, "peak_AutoBrightness_ROI_Mode_Get") : nullptr);
    if(m_peak_AutoBrightness_ROI_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_ROI_Mode_Get");
    }

    m_peak_AutoBrightness_ROI_Offset_GetRange = (dyn_peak_AutoBrightness_ROI_Offset_GetRange) (load ?  import_function(m_handle, "peak_AutoBrightness_ROI_Offset_GetRange") : nullptr);
    if(m_peak_AutoBrightness_ROI_Offset_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_ROI_Offset_GetRange");
    }

    m_peak_AutoBrightness_ROI_Size_GetRange = (dyn_peak_AutoBrightness_ROI_Size_GetRange) (load ?  import_function(m_handle, "peak_AutoBrightness_ROI_Size_GetRange") : nullptr);
    if(m_peak_AutoBrightness_ROI_Size_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_ROI_Size_GetRange");
    }

    m_peak_AutoBrightness_ROI_Set = (dyn_peak_AutoBrightness_ROI_Set) (load ?  import_function(m_handle, "peak_AutoBrightness_ROI_Set") : nullptr);
    if(m_peak_AutoBrightness_ROI_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_ROI_Set");
    }

    m_peak_AutoBrightness_ROI_Get = (dyn_peak_AutoBrightness_ROI_Get) (load ?  import_function(m_handle, "peak_AutoBrightness_ROI_Get") : nullptr);
    if(m_peak_AutoBrightness_ROI_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_ROI_Get");
    }

    m_peak_AutoBrightness_Exposure_GetAccessStatus = (dyn_peak_AutoBrightness_Exposure_GetAccessStatus) (load ?  import_function(m_handle, "peak_AutoBrightness_Exposure_GetAccessStatus") : nullptr);
    if(m_peak_AutoBrightness_Exposure_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_Exposure_GetAccessStatus");
    }

    m_peak_AutoBrightness_Exposure_Mode_GetList = (dyn_peak_AutoBrightness_Exposure_Mode_GetList) (load ?  import_function(m_handle, "peak_AutoBrightness_Exposure_Mode_GetList") : nullptr);
    if(m_peak_AutoBrightness_Exposure_Mode_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_Exposure_Mode_GetList");
    }

    m_peak_AutoBrightness_Exposure_Mode_Set = (dyn_peak_AutoBrightness_Exposure_Mode_Set) (load ?  import_function(m_handle, "peak_AutoBrightness_Exposure_Mode_Set") : nullptr);
    if(m_peak_AutoBrightness_Exposure_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_Exposure_Mode_Set");
    }

    m_peak_AutoBrightness_Exposure_Mode_Get = (dyn_peak_AutoBrightness_Exposure_Mode_Get) (load ?  import_function(m_handle, "peak_AutoBrightness_Exposure_Mode_Get") : nullptr);
    if(m_peak_AutoBrightness_Exposure_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_Exposure_Mode_Get");
    }

    m_peak_AutoBrightness_Gain_GetAccessStatus = (dyn_peak_AutoBrightness_Gain_GetAccessStatus) (load ?  import_function(m_handle, "peak_AutoBrightness_Gain_GetAccessStatus") : nullptr);
    if(m_peak_AutoBrightness_Gain_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_Gain_GetAccessStatus");
    }

    m_peak_AutoBrightness_Gain_Mode_GetList = (dyn_peak_AutoBrightness_Gain_Mode_GetList) (load ?  import_function(m_handle, "peak_AutoBrightness_Gain_Mode_GetList") : nullptr);
    if(m_peak_AutoBrightness_Gain_Mode_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_Gain_Mode_GetList");
    }

    m_peak_AutoBrightness_Gain_Mode_Set = (dyn_peak_AutoBrightness_Gain_Mode_Set) (load ?  import_function(m_handle, "peak_AutoBrightness_Gain_Mode_Set") : nullptr);
    if(m_peak_AutoBrightness_Gain_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_Gain_Mode_Set");
    }

    m_peak_AutoBrightness_Gain_Mode_Get = (dyn_peak_AutoBrightness_Gain_Mode_Get) (load ?  import_function(m_handle, "peak_AutoBrightness_Gain_Mode_Get") : nullptr);
    if(m_peak_AutoBrightness_Gain_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoBrightness_Gain_Mode_Get");
    }

    m_peak_AutoWhiteBalance_GetAccessStatus = (dyn_peak_AutoWhiteBalance_GetAccessStatus) (load ?  import_function(m_handle, "peak_AutoWhiteBalance_GetAccessStatus") : nullptr);
    if(m_peak_AutoWhiteBalance_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoWhiteBalance_GetAccessStatus");
    }

    m_peak_AutoWhiteBalance_ROI_GetAccessStatus = (dyn_peak_AutoWhiteBalance_ROI_GetAccessStatus) (load ?  import_function(m_handle, "peak_AutoWhiteBalance_ROI_GetAccessStatus") : nullptr);
    if(m_peak_AutoWhiteBalance_ROI_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoWhiteBalance_ROI_GetAccessStatus");
    }

    m_peak_AutoWhiteBalance_ROI_Mode_Set = (dyn_peak_AutoWhiteBalance_ROI_Mode_Set) (load ?  import_function(m_handle, "peak_AutoWhiteBalance_ROI_Mode_Set") : nullptr);
    if(m_peak_AutoWhiteBalance_ROI_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoWhiteBalance_ROI_Mode_Set");
    }

    m_peak_AutoWhiteBalance_ROI_Mode_Get = (dyn_peak_AutoWhiteBalance_ROI_Mode_Get) (load ?  import_function(m_handle, "peak_AutoWhiteBalance_ROI_Mode_Get") : nullptr);
    if(m_peak_AutoWhiteBalance_ROI_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoWhiteBalance_ROI_Mode_Get");
    }

    m_peak_AutoWhiteBalance_ROI_Offset_GetRange = (dyn_peak_AutoWhiteBalance_ROI_Offset_GetRange) (load ?  import_function(m_handle, "peak_AutoWhiteBalance_ROI_Offset_GetRange") : nullptr);
    if(m_peak_AutoWhiteBalance_ROI_Offset_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoWhiteBalance_ROI_Offset_GetRange");
    }

    m_peak_AutoWhiteBalance_ROI_Size_GetRange = (dyn_peak_AutoWhiteBalance_ROI_Size_GetRange) (load ?  import_function(m_handle, "peak_AutoWhiteBalance_ROI_Size_GetRange") : nullptr);
    if(m_peak_AutoWhiteBalance_ROI_Size_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoWhiteBalance_ROI_Size_GetRange");
    }

    m_peak_AutoWhiteBalance_ROI_Set = (dyn_peak_AutoWhiteBalance_ROI_Set) (load ?  import_function(m_handle, "peak_AutoWhiteBalance_ROI_Set") : nullptr);
    if(m_peak_AutoWhiteBalance_ROI_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoWhiteBalance_ROI_Set");
    }

    m_peak_AutoWhiteBalance_ROI_Get = (dyn_peak_AutoWhiteBalance_ROI_Get) (load ?  import_function(m_handle, "peak_AutoWhiteBalance_ROI_Get") : nullptr);
    if(m_peak_AutoWhiteBalance_ROI_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoWhiteBalance_ROI_Get");
    }

    m_peak_AutoWhiteBalance_Mode_Set = (dyn_peak_AutoWhiteBalance_Mode_Set) (load ?  import_function(m_handle, "peak_AutoWhiteBalance_Mode_Set") : nullptr);
    if(m_peak_AutoWhiteBalance_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoWhiteBalance_Mode_Set");
    }

    m_peak_AutoWhiteBalance_Mode_Get = (dyn_peak_AutoWhiteBalance_Mode_Get) (load ?  import_function(m_handle, "peak_AutoWhiteBalance_Mode_Get") : nullptr);
    if(m_peak_AutoWhiteBalance_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoWhiteBalance_Mode_Get");
    }

    m_peak_AutoWhiteBalance_Mode_GetList = (dyn_peak_AutoWhiteBalance_Mode_GetList) (load ?  import_function(m_handle, "peak_AutoWhiteBalance_Mode_GetList") : nullptr);
    if(m_peak_AutoWhiteBalance_Mode_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_AutoWhiteBalance_Mode_GetList");
    }

    m_peak_ROI_GetAccessStatus = (dyn_peak_ROI_GetAccessStatus) (load ?  import_function(m_handle, "peak_ROI_GetAccessStatus") : nullptr);
    if(m_peak_ROI_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ROI_GetAccessStatus");
    }

    m_peak_ROI_Offset_GetRange = (dyn_peak_ROI_Offset_GetRange) (load ?  import_function(m_handle, "peak_ROI_Offset_GetRange") : nullptr);
    if(m_peak_ROI_Offset_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ROI_Offset_GetRange");
    }

    m_peak_ROI_Offset_GetAccessStatus = (dyn_peak_ROI_Offset_GetAccessStatus) (load ?  import_function(m_handle, "peak_ROI_Offset_GetAccessStatus") : nullptr);
    if(m_peak_ROI_Offset_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ROI_Offset_GetAccessStatus");
    }

    m_peak_ROI_Offset_Set = (dyn_peak_ROI_Offset_Set) (load ?  import_function(m_handle, "peak_ROI_Offset_Set") : nullptr);
    if(m_peak_ROI_Offset_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ROI_Offset_Set");
    }

    m_peak_ROI_Offset_Get = (dyn_peak_ROI_Offset_Get) (load ?  import_function(m_handle, "peak_ROI_Offset_Get") : nullptr);
    if(m_peak_ROI_Offset_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ROI_Offset_Get");
    }

    m_peak_ROI_Size_GetRange = (dyn_peak_ROI_Size_GetRange) (load ?  import_function(m_handle, "peak_ROI_Size_GetRange") : nullptr);
    if(m_peak_ROI_Size_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ROI_Size_GetRange");
    }

    m_peak_ROI_Size_GetAccessStatus = (dyn_peak_ROI_Size_GetAccessStatus) (load ?  import_function(m_handle, "peak_ROI_Size_GetAccessStatus") : nullptr);
    if(m_peak_ROI_Size_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ROI_Size_GetAccessStatus");
    }

    m_peak_ROI_Size_Set = (dyn_peak_ROI_Size_Set) (load ?  import_function(m_handle, "peak_ROI_Size_Set") : nullptr);
    if(m_peak_ROI_Size_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ROI_Size_Set");
    }

    m_peak_ROI_Size_Get = (dyn_peak_ROI_Size_Get) (load ?  import_function(m_handle, "peak_ROI_Size_Get") : nullptr);
    if(m_peak_ROI_Size_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ROI_Size_Get");
    }

    m_peak_ROI_Set = (dyn_peak_ROI_Set) (load ?  import_function(m_handle, "peak_ROI_Set") : nullptr);
    if(m_peak_ROI_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ROI_Set");
    }

    m_peak_ROI_Get = (dyn_peak_ROI_Get) (load ?  import_function(m_handle, "peak_ROI_Get") : nullptr);
    if(m_peak_ROI_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_ROI_Get");
    }

    m_peak_Binning_GetAccessStatus = (dyn_peak_Binning_GetAccessStatus) (load ?  import_function(m_handle, "peak_Binning_GetAccessStatus") : nullptr);
    if(m_peak_Binning_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Binning_GetAccessStatus");
    }

    m_peak_Binning_FactorX_GetList = (dyn_peak_Binning_FactorX_GetList) (load ?  import_function(m_handle, "peak_Binning_FactorX_GetList") : nullptr);
    if(m_peak_Binning_FactorX_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Binning_FactorX_GetList");
    }

    m_peak_Binning_FactorY_GetList = (dyn_peak_Binning_FactorY_GetList) (load ?  import_function(m_handle, "peak_Binning_FactorY_GetList") : nullptr);
    if(m_peak_Binning_FactorY_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Binning_FactorY_GetList");
    }

    m_peak_Binning_Set = (dyn_peak_Binning_Set) (load ?  import_function(m_handle, "peak_Binning_Set") : nullptr);
    if(m_peak_Binning_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Binning_Set");
    }

    m_peak_Binning_Get = (dyn_peak_Binning_Get) (load ?  import_function(m_handle, "peak_Binning_Get") : nullptr);
    if(m_peak_Binning_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Binning_Get");
    }

    m_peak_BinningManual_GetAccessStatus = (dyn_peak_BinningManual_GetAccessStatus) (load ?  import_function(m_handle, "peak_BinningManual_GetAccessStatus") : nullptr);
    if(m_peak_BinningManual_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_BinningManual_GetAccessStatus");
    }

    m_peak_BinningManual_FactorX_GetList = (dyn_peak_BinningManual_FactorX_GetList) (load ?  import_function(m_handle, "peak_BinningManual_FactorX_GetList") : nullptr);
    if(m_peak_BinningManual_FactorX_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_BinningManual_FactorX_GetList");
    }

    m_peak_BinningManual_FactorY_GetList = (dyn_peak_BinningManual_FactorY_GetList) (load ?  import_function(m_handle, "peak_BinningManual_FactorY_GetList") : nullptr);
    if(m_peak_BinningManual_FactorY_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_BinningManual_FactorY_GetList");
    }

    m_peak_BinningManual_Set = (dyn_peak_BinningManual_Set) (load ?  import_function(m_handle, "peak_BinningManual_Set") : nullptr);
    if(m_peak_BinningManual_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_BinningManual_Set");
    }

    m_peak_BinningManual_Get = (dyn_peak_BinningManual_Get) (load ?  import_function(m_handle, "peak_BinningManual_Get") : nullptr);
    if(m_peak_BinningManual_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_BinningManual_Get");
    }

    m_peak_Decimation_GetAccessStatus = (dyn_peak_Decimation_GetAccessStatus) (load ?  import_function(m_handle, "peak_Decimation_GetAccessStatus") : nullptr);
    if(m_peak_Decimation_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Decimation_GetAccessStatus");
    }

    m_peak_Decimation_FactorX_GetList = (dyn_peak_Decimation_FactorX_GetList) (load ?  import_function(m_handle, "peak_Decimation_FactorX_GetList") : nullptr);
    if(m_peak_Decimation_FactorX_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Decimation_FactorX_GetList");
    }

    m_peak_Decimation_FactorY_GetList = (dyn_peak_Decimation_FactorY_GetList) (load ?  import_function(m_handle, "peak_Decimation_FactorY_GetList") : nullptr);
    if(m_peak_Decimation_FactorY_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Decimation_FactorY_GetList");
    }

    m_peak_Decimation_Set = (dyn_peak_Decimation_Set) (load ?  import_function(m_handle, "peak_Decimation_Set") : nullptr);
    if(m_peak_Decimation_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Decimation_Set");
    }

    m_peak_Decimation_Get = (dyn_peak_Decimation_Get) (load ?  import_function(m_handle, "peak_Decimation_Get") : nullptr);
    if(m_peak_Decimation_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Decimation_Get");
    }

    m_peak_DecimationManual_GetAccessStatus = (dyn_peak_DecimationManual_GetAccessStatus) (load ?  import_function(m_handle, "peak_DecimationManual_GetAccessStatus") : nullptr);
    if(m_peak_DecimationManual_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_DecimationManual_GetAccessStatus");
    }

    m_peak_DecimationManual_FactorX_GetList = (dyn_peak_DecimationManual_FactorX_GetList) (load ?  import_function(m_handle, "peak_DecimationManual_FactorX_GetList") : nullptr);
    if(m_peak_DecimationManual_FactorX_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_DecimationManual_FactorX_GetList");
    }

    m_peak_DecimationManual_FactorY_GetList = (dyn_peak_DecimationManual_FactorY_GetList) (load ?  import_function(m_handle, "peak_DecimationManual_FactorY_GetList") : nullptr);
    if(m_peak_DecimationManual_FactorY_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_DecimationManual_FactorY_GetList");
    }

    m_peak_DecimationManual_Set = (dyn_peak_DecimationManual_Set) (load ?  import_function(m_handle, "peak_DecimationManual_Set") : nullptr);
    if(m_peak_DecimationManual_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_DecimationManual_Set");
    }

    m_peak_DecimationManual_Get = (dyn_peak_DecimationManual_Get) (load ?  import_function(m_handle, "peak_DecimationManual_Get") : nullptr);
    if(m_peak_DecimationManual_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_DecimationManual_Get");
    }

    m_peak_Scaling_GetAccessStatus = (dyn_peak_Scaling_GetAccessStatus) (load ?  import_function(m_handle, "peak_Scaling_GetAccessStatus") : nullptr);
    if(m_peak_Scaling_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Scaling_GetAccessStatus");
    }

    m_peak_Scaling_FactorX_GetRange = (dyn_peak_Scaling_FactorX_GetRange) (load ?  import_function(m_handle, "peak_Scaling_FactorX_GetRange") : nullptr);
    if(m_peak_Scaling_FactorX_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Scaling_FactorX_GetRange");
    }

    m_peak_Scaling_FactorY_GetRange = (dyn_peak_Scaling_FactorY_GetRange) (load ?  import_function(m_handle, "peak_Scaling_FactorY_GetRange") : nullptr);
    if(m_peak_Scaling_FactorY_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Scaling_FactorY_GetRange");
    }

    m_peak_Scaling_Set = (dyn_peak_Scaling_Set) (load ?  import_function(m_handle, "peak_Scaling_Set") : nullptr);
    if(m_peak_Scaling_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Scaling_Set");
    }

    m_peak_Scaling_Get = (dyn_peak_Scaling_Get) (load ?  import_function(m_handle, "peak_Scaling_Get") : nullptr);
    if(m_peak_Scaling_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Scaling_Get");
    }

    m_peak_Mirror_LeftRight_GetAccessStatus = (dyn_peak_Mirror_LeftRight_GetAccessStatus) (load ?  import_function(m_handle, "peak_Mirror_LeftRight_GetAccessStatus") : nullptr);
    if(m_peak_Mirror_LeftRight_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Mirror_LeftRight_GetAccessStatus");
    }

    m_peak_Mirror_LeftRight_Enable = (dyn_peak_Mirror_LeftRight_Enable) (load ?  import_function(m_handle, "peak_Mirror_LeftRight_Enable") : nullptr);
    if(m_peak_Mirror_LeftRight_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Mirror_LeftRight_Enable");
    }

    m_peak_Mirror_LeftRight_IsEnabled = (dyn_peak_Mirror_LeftRight_IsEnabled) (load ?  import_function(m_handle, "peak_Mirror_LeftRight_IsEnabled") : nullptr);
    if(m_peak_Mirror_LeftRight_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Mirror_LeftRight_IsEnabled");
    }

    m_peak_Mirror_UpDown_GetAccessStatus = (dyn_peak_Mirror_UpDown_GetAccessStatus) (load ?  import_function(m_handle, "peak_Mirror_UpDown_GetAccessStatus") : nullptr);
    if(m_peak_Mirror_UpDown_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Mirror_UpDown_GetAccessStatus");
    }

    m_peak_Mirror_UpDown_Enable = (dyn_peak_Mirror_UpDown_Enable) (load ?  import_function(m_handle, "peak_Mirror_UpDown_Enable") : nullptr);
    if(m_peak_Mirror_UpDown_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Mirror_UpDown_Enable");
    }

    m_peak_Mirror_UpDown_IsEnabled = (dyn_peak_Mirror_UpDown_IsEnabled) (load ?  import_function(m_handle, "peak_Mirror_UpDown_IsEnabled") : nullptr);
    if(m_peak_Mirror_UpDown_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Mirror_UpDown_IsEnabled");
    }

    m_peak_CameraMemory_Area_GetAccessStatus = (dyn_peak_CameraMemory_Area_GetAccessStatus) (load ?  import_function(m_handle, "peak_CameraMemory_Area_GetAccessStatus") : nullptr);
    if(m_peak_CameraMemory_Area_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraMemory_Area_GetAccessStatus");
    }

    m_peak_CameraMemory_Area_GetList = (dyn_peak_CameraMemory_Area_GetList) (load ?  import_function(m_handle, "peak_CameraMemory_Area_GetList") : nullptr);
    if(m_peak_CameraMemory_Area_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraMemory_Area_GetList");
    }

    m_peak_CameraMemory_Area_Size_Get = (dyn_peak_CameraMemory_Area_Size_Get) (load ?  import_function(m_handle, "peak_CameraMemory_Area_Size_Get") : nullptr);
    if(m_peak_CameraMemory_Area_Size_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraMemory_Area_Size_Get");
    }

    m_peak_CameraMemory_Area_Data_Clear = (dyn_peak_CameraMemory_Area_Data_Clear) (load ?  import_function(m_handle, "peak_CameraMemory_Area_Data_Clear") : nullptr);
    if(m_peak_CameraMemory_Area_Data_Clear == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraMemory_Area_Data_Clear");
    }

    m_peak_CameraMemory_Area_Data_Write = (dyn_peak_CameraMemory_Area_Data_Write) (load ?  import_function(m_handle, "peak_CameraMemory_Area_Data_Write") : nullptr);
    if(m_peak_CameraMemory_Area_Data_Write == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraMemory_Area_Data_Write");
    }

    m_peak_CameraMemory_Area_Data_Read = (dyn_peak_CameraMemory_Area_Data_Read) (load ?  import_function(m_handle, "peak_CameraMemory_Area_Data_Read") : nullptr);
    if(m_peak_CameraMemory_Area_Data_Read == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_CameraMemory_Area_Data_Read");
    }

    m_peak_GFA_EnableWriteAccess = (dyn_peak_GFA_EnableWriteAccess) (load ?  import_function(m_handle, "peak_GFA_EnableWriteAccess") : nullptr);
    if(m_peak_GFA_EnableWriteAccess == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_EnableWriteAccess");
    }

    m_peak_GFA_IsWriteAccessEnabled = (dyn_peak_GFA_IsWriteAccessEnabled) (load ?  import_function(m_handle, "peak_GFA_IsWriteAccessEnabled") : nullptr);
    if(m_peak_GFA_IsWriteAccessEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_IsWriteAccessEnabled");
    }

    m_peak_GFA_Feature_GetAccessStatus = (dyn_peak_GFA_Feature_GetAccessStatus) (load ?  import_function(m_handle, "peak_GFA_Feature_GetAccessStatus") : nullptr);
    if(m_peak_GFA_Feature_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Feature_GetAccessStatus");
    }

    m_peak_GFA_Float_HasRange = (dyn_peak_GFA_Float_HasRange) (load ?  import_function(m_handle, "peak_GFA_Float_HasRange") : nullptr);
    if(m_peak_GFA_Float_HasRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Float_HasRange");
    }

    m_peak_GFA_Float_GetRange = (dyn_peak_GFA_Float_GetRange) (load ?  import_function(m_handle, "peak_GFA_Float_GetRange") : nullptr);
    if(m_peak_GFA_Float_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Float_GetRange");
    }

    m_peak_GFA_Float_GetList = (dyn_peak_GFA_Float_GetList) (load ?  import_function(m_handle, "peak_GFA_Float_GetList") : nullptr);
    if(m_peak_GFA_Float_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Float_GetList");
    }

    m_peak_GFA_Float_Set = (dyn_peak_GFA_Float_Set) (load ?  import_function(m_handle, "peak_GFA_Float_Set") : nullptr);
    if(m_peak_GFA_Float_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Float_Set");
    }

    m_peak_GFA_Float_Get = (dyn_peak_GFA_Float_Get) (load ?  import_function(m_handle, "peak_GFA_Float_Get") : nullptr);
    if(m_peak_GFA_Float_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Float_Get");
    }

    m_peak_GFA_Integer_HasRange = (dyn_peak_GFA_Integer_HasRange) (load ?  import_function(m_handle, "peak_GFA_Integer_HasRange") : nullptr);
    if(m_peak_GFA_Integer_HasRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Integer_HasRange");
    }

    m_peak_GFA_Integer_GetRange = (dyn_peak_GFA_Integer_GetRange) (load ?  import_function(m_handle, "peak_GFA_Integer_GetRange") : nullptr);
    if(m_peak_GFA_Integer_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Integer_GetRange");
    }

    m_peak_GFA_Integer_GetList = (dyn_peak_GFA_Integer_GetList) (load ?  import_function(m_handle, "peak_GFA_Integer_GetList") : nullptr);
    if(m_peak_GFA_Integer_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Integer_GetList");
    }

    m_peak_GFA_Integer_Set = (dyn_peak_GFA_Integer_Set) (load ?  import_function(m_handle, "peak_GFA_Integer_Set") : nullptr);
    if(m_peak_GFA_Integer_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Integer_Set");
    }

    m_peak_GFA_Integer_Get = (dyn_peak_GFA_Integer_Get) (load ?  import_function(m_handle, "peak_GFA_Integer_Get") : nullptr);
    if(m_peak_GFA_Integer_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Integer_Get");
    }

    m_peak_GFA_Boolean_Set = (dyn_peak_GFA_Boolean_Set) (load ?  import_function(m_handle, "peak_GFA_Boolean_Set") : nullptr);
    if(m_peak_GFA_Boolean_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Boolean_Set");
    }

    m_peak_GFA_Boolean_Get = (dyn_peak_GFA_Boolean_Get) (load ?  import_function(m_handle, "peak_GFA_Boolean_Get") : nullptr);
    if(m_peak_GFA_Boolean_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Boolean_Get");
    }

    m_peak_GFA_String_Set = (dyn_peak_GFA_String_Set) (load ?  import_function(m_handle, "peak_GFA_String_Set") : nullptr);
    if(m_peak_GFA_String_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_String_Set");
    }

    m_peak_GFA_String_Get = (dyn_peak_GFA_String_Get) (load ?  import_function(m_handle, "peak_GFA_String_Get") : nullptr);
    if(m_peak_GFA_String_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_String_Get");
    }

    m_peak_GFA_Command_Execute = (dyn_peak_GFA_Command_Execute) (load ?  import_function(m_handle, "peak_GFA_Command_Execute") : nullptr);
    if(m_peak_GFA_Command_Execute == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Command_Execute");
    }

    m_peak_GFA_Command_WaitForDone = (dyn_peak_GFA_Command_WaitForDone) (load ?  import_function(m_handle, "peak_GFA_Command_WaitForDone") : nullptr);
    if(m_peak_GFA_Command_WaitForDone == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Command_WaitForDone");
    }

    m_peak_GFA_Enumeration_GetList = (dyn_peak_GFA_Enumeration_GetList) (load ?  import_function(m_handle, "peak_GFA_Enumeration_GetList") : nullptr);
    if(m_peak_GFA_Enumeration_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Enumeration_GetList");
    }

    m_peak_GFA_EnumerationEntry_GetAccessStatus = (dyn_peak_GFA_EnumerationEntry_GetAccessStatus) (load ?  import_function(m_handle, "peak_GFA_EnumerationEntry_GetAccessStatus") : nullptr);
    if(m_peak_GFA_EnumerationEntry_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_EnumerationEntry_GetAccessStatus");
    }

    m_peak_GFA_EnumerationEntry_GetAccessStatusBySymbolicValue = (dyn_peak_GFA_EnumerationEntry_GetAccessStatusBySymbolicValue) (load ?  import_function(m_handle, "peak_GFA_EnumerationEntry_GetAccessStatusBySymbolicValue") : nullptr);
    if(m_peak_GFA_EnumerationEntry_GetAccessStatusBySymbolicValue == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_EnumerationEntry_GetAccessStatusBySymbolicValue");
    }

    m_peak_GFA_EnumerationEntry_GetAccessStatusByIntegerValue = (dyn_peak_GFA_EnumerationEntry_GetAccessStatusByIntegerValue) (load ?  import_function(m_handle, "peak_GFA_EnumerationEntry_GetAccessStatusByIntegerValue") : nullptr);
    if(m_peak_GFA_EnumerationEntry_GetAccessStatusByIntegerValue == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_EnumerationEntry_GetAccessStatusByIntegerValue");
    }

    m_peak_GFA_Enumeration_Set = (dyn_peak_GFA_Enumeration_Set) (load ?  import_function(m_handle, "peak_GFA_Enumeration_Set") : nullptr);
    if(m_peak_GFA_Enumeration_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Enumeration_Set");
    }

    m_peak_GFA_Enumeration_SetBySymbolicValue = (dyn_peak_GFA_Enumeration_SetBySymbolicValue) (load ?  import_function(m_handle, "peak_GFA_Enumeration_SetBySymbolicValue") : nullptr);
    if(m_peak_GFA_Enumeration_SetBySymbolicValue == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Enumeration_SetBySymbolicValue");
    }

    m_peak_GFA_Enumeration_SetByIntegerValue = (dyn_peak_GFA_Enumeration_SetByIntegerValue) (load ?  import_function(m_handle, "peak_GFA_Enumeration_SetByIntegerValue") : nullptr);
    if(m_peak_GFA_Enumeration_SetByIntegerValue == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Enumeration_SetByIntegerValue");
    }

    m_peak_GFA_Enumeration_Get = (dyn_peak_GFA_Enumeration_Get) (load ?  import_function(m_handle, "peak_GFA_Enumeration_Get") : nullptr);
    if(m_peak_GFA_Enumeration_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Enumeration_Get");
    }

    m_peak_GFA_Register_Set = (dyn_peak_GFA_Register_Set) (load ?  import_function(m_handle, "peak_GFA_Register_Set") : nullptr);
    if(m_peak_GFA_Register_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Register_Set");
    }

    m_peak_GFA_Register_Get = (dyn_peak_GFA_Register_Get) (load ?  import_function(m_handle, "peak_GFA_Register_Get") : nullptr);
    if(m_peak_GFA_Register_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Register_Get");
    }

    m_peak_GFA_Data_Write = (dyn_peak_GFA_Data_Write) (load ?  import_function(m_handle, "peak_GFA_Data_Write") : nullptr);
    if(m_peak_GFA_Data_Write == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Data_Write");
    }

    m_peak_GFA_Data_Read = (dyn_peak_GFA_Data_Read) (load ?  import_function(m_handle, "peak_GFA_Data_Read") : nullptr);
    if(m_peak_GFA_Data_Read == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_GFA_Data_Read");
    }

    m_peak_IPL_PixelFormat_GetList = (dyn_peak_IPL_PixelFormat_GetList) (load ?  import_function(m_handle, "peak_IPL_PixelFormat_GetList") : nullptr);
    if(m_peak_IPL_PixelFormat_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_PixelFormat_GetList");
    }

    m_peak_IPL_PixelFormat_Set = (dyn_peak_IPL_PixelFormat_Set) (load ?  import_function(m_handle, "peak_IPL_PixelFormat_Set") : nullptr);
    if(m_peak_IPL_PixelFormat_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_PixelFormat_Set");
    }

    m_peak_IPL_PixelFormat_Get = (dyn_peak_IPL_PixelFormat_Get) (load ?  import_function(m_handle, "peak_IPL_PixelFormat_Get") : nullptr);
    if(m_peak_IPL_PixelFormat_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_PixelFormat_Get");
    }

    m_peak_IPL_Gain_GetRange = (dyn_peak_IPL_Gain_GetRange) (load ?  import_function(m_handle, "peak_IPL_Gain_GetRange") : nullptr);
    if(m_peak_IPL_Gain_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Gain_GetRange");
    }

    m_peak_IPL_Gain_Set = (dyn_peak_IPL_Gain_Set) (load ?  import_function(m_handle, "peak_IPL_Gain_Set") : nullptr);
    if(m_peak_IPL_Gain_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Gain_Set");
    }

    m_peak_IPL_Gain_Get = (dyn_peak_IPL_Gain_Get) (load ?  import_function(m_handle, "peak_IPL_Gain_Get") : nullptr);
    if(m_peak_IPL_Gain_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Gain_Get");
    }

    m_peak_IPL_Gamma_GetRange = (dyn_peak_IPL_Gamma_GetRange) (load ?  import_function(m_handle, "peak_IPL_Gamma_GetRange") : nullptr);
    if(m_peak_IPL_Gamma_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Gamma_GetRange");
    }

    m_peak_IPL_Gamma_Set = (dyn_peak_IPL_Gamma_Set) (load ?  import_function(m_handle, "peak_IPL_Gamma_Set") : nullptr);
    if(m_peak_IPL_Gamma_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Gamma_Set");
    }

    m_peak_IPL_Gamma_Get = (dyn_peak_IPL_Gamma_Get) (load ?  import_function(m_handle, "peak_IPL_Gamma_Get") : nullptr);
    if(m_peak_IPL_Gamma_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Gamma_Get");
    }

    m_peak_IPL_ColorCorrection_Matrix_Set = (dyn_peak_IPL_ColorCorrection_Matrix_Set) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_Matrix_Set") : nullptr);
    if(m_peak_IPL_ColorCorrection_Matrix_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_Matrix_Set");
    }

    m_peak_IPL_ColorCorrection_Matrix_Get = (dyn_peak_IPL_ColorCorrection_Matrix_Get) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_Matrix_Get") : nullptr);
    if(m_peak_IPL_ColorCorrection_Matrix_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_Matrix_Get");
    }

    m_peak_IPL_ColorCorrection_Saturation_Get = (dyn_peak_IPL_ColorCorrection_Saturation_Get) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_Saturation_Get") : nullptr);
    if(m_peak_IPL_ColorCorrection_Saturation_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_Saturation_Get");
    }

    m_peak_IPL_ColorCorrection_Saturation_Set = (dyn_peak_IPL_ColorCorrection_Saturation_Set) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_Saturation_Set") : nullptr);
    if(m_peak_IPL_ColorCorrection_Saturation_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_Saturation_Set");
    }

    m_peak_IPL_ColorCorrection_Saturation_GetRange = (dyn_peak_IPL_ColorCorrection_Saturation_GetRange) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_Saturation_GetRange") : nullptr);
    if(m_peak_IPL_ColorCorrection_Saturation_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_Saturation_GetRange");
    }

    m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Get = (dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Get) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Get") : nullptr);
    if(m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Get");
    }

    m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Set = (dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Set) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Set") : nullptr);
    if(m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Set");
    }

    m_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Get = (dyn_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Get) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Get") : nullptr);
    if(m_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Get");
    }

    m_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Set = (dyn_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Set) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Set") : nullptr);
    if(m_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Set");
    }

    m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Get = (dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Get) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Get") : nullptr);
    if(m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Get");
    }

    m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Set = (dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Set) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Set") : nullptr);
    if(m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Set");
    }

    m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_GetRange = (dyn_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_GetRange) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_GetRange") : nullptr);
    if(m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_GetRange");
    }

    m_peak_IPL_ColorCorrection_ChromaticAdaption_Enable = (dyn_peak_IPL_ColorCorrection_ChromaticAdaption_Enable) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_ChromaticAdaption_Enable") : nullptr);
    if(m_peak_IPL_ColorCorrection_ChromaticAdaption_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_ChromaticAdaption_Enable");
    }

    m_peak_IPL_ColorCorrection_ChromaticAdaption_IsEnabled = (dyn_peak_IPL_ColorCorrection_ChromaticAdaption_IsEnabled) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_ChromaticAdaption_IsEnabled") : nullptr);
    if(m_peak_IPL_ColorCorrection_ChromaticAdaption_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_ChromaticAdaption_IsEnabled");
    }

    m_peak_IPL_ColorCorrection_Enable = (dyn_peak_IPL_ColorCorrection_Enable) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_Enable") : nullptr);
    if(m_peak_IPL_ColorCorrection_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_Enable");
    }

    m_peak_IPL_ColorCorrection_IsEnabled = (dyn_peak_IPL_ColorCorrection_IsEnabled) (load ?  import_function(m_handle, "peak_IPL_ColorCorrection_IsEnabled") : nullptr);
    if(m_peak_IPL_ColorCorrection_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ColorCorrection_IsEnabled");
    }

    m_peak_IPL_AutoBrightness_Target_GetRange = (dyn_peak_IPL_AutoBrightness_Target_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_Target_GetRange") : nullptr);
    if(m_peak_IPL_AutoBrightness_Target_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_Target_GetRange");
    }

    m_peak_IPL_AutoBrightness_Target_Set = (dyn_peak_IPL_AutoBrightness_Target_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_Target_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_Target_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_Target_Set");
    }

    m_peak_IPL_AutoBrightness_Target_Get = (dyn_peak_IPL_AutoBrightness_Target_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_Target_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_Target_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_Target_Get");
    }

    m_peak_IPL_AutoBrightness_TargetTolerance_GetRange = (dyn_peak_IPL_AutoBrightness_TargetTolerance_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_TargetTolerance_GetRange") : nullptr);
    if(m_peak_IPL_AutoBrightness_TargetTolerance_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_TargetTolerance_GetRange");
    }

    m_peak_IPL_AutoBrightness_TargetTolerance_Set = (dyn_peak_IPL_AutoBrightness_TargetTolerance_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_TargetTolerance_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_TargetTolerance_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_TargetTolerance_Set");
    }

    m_peak_IPL_AutoBrightness_TargetTolerance_Get = (dyn_peak_IPL_AutoBrightness_TargetTolerance_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_TargetTolerance_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_TargetTolerance_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_TargetTolerance_Get");
    }

    m_peak_IPL_AutoBrightness_TargetPercentile_GetRange = (dyn_peak_IPL_AutoBrightness_TargetPercentile_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_TargetPercentile_GetRange") : nullptr);
    if(m_peak_IPL_AutoBrightness_TargetPercentile_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_TargetPercentile_GetRange");
    }

    m_peak_IPL_AutoBrightness_TargetPercentile_Set = (dyn_peak_IPL_AutoBrightness_TargetPercentile_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_TargetPercentile_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_TargetPercentile_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_TargetPercentile_Set");
    }

    m_peak_IPL_AutoBrightness_TargetPercentile_Get = (dyn_peak_IPL_AutoBrightness_TargetPercentile_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_TargetPercentile_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_TargetPercentile_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_TargetPercentile_Get");
    }

    m_peak_IPL_AutoBrightness_ROI_Mode_Set = (dyn_peak_IPL_AutoBrightness_ROI_Mode_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_ROI_Mode_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_ROI_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_ROI_Mode_Set");
    }

    m_peak_IPL_AutoBrightness_ROI_Mode_Get = (dyn_peak_IPL_AutoBrightness_ROI_Mode_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_ROI_Mode_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_ROI_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_ROI_Mode_Get");
    }

    m_peak_IPL_AutoBrightness_ROI_Offset_GetRange = (dyn_peak_IPL_AutoBrightness_ROI_Offset_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_ROI_Offset_GetRange") : nullptr);
    if(m_peak_IPL_AutoBrightness_ROI_Offset_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_ROI_Offset_GetRange");
    }

    m_peak_IPL_AutoBrightness_ROI_Size_GetRange = (dyn_peak_IPL_AutoBrightness_ROI_Size_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_ROI_Size_GetRange") : nullptr);
    if(m_peak_IPL_AutoBrightness_ROI_Size_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_ROI_Size_GetRange");
    }

    m_peak_IPL_AutoBrightness_ROI_Set = (dyn_peak_IPL_AutoBrightness_ROI_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_ROI_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_ROI_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_ROI_Set");
    }

    m_peak_IPL_AutoBrightness_ROI_Get = (dyn_peak_IPL_AutoBrightness_ROI_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_ROI_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_ROI_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_ROI_Get");
    }

    m_peak_IPL_AutoBrightness_Exposure_Mode_Set = (dyn_peak_IPL_AutoBrightness_Exposure_Mode_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_Exposure_Mode_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_Exposure_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_Exposure_Mode_Set");
    }

    m_peak_IPL_AutoBrightness_Exposure_Mode_Get = (dyn_peak_IPL_AutoBrightness_Exposure_Mode_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_Exposure_Mode_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_Exposure_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_Exposure_Mode_Get");
    }

    m_peak_IPL_AutoBrightness_ExposureLimit_Get = (dyn_peak_IPL_AutoBrightness_ExposureLimit_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_ExposureLimit_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_ExposureLimit_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_ExposureLimit_Get");
    }

    m_peak_IPL_AutoBrightness_ExposureLimit_Set = (dyn_peak_IPL_AutoBrightness_ExposureLimit_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_ExposureLimit_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_ExposureLimit_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_ExposureLimit_Set");
    }

    m_peak_IPL_AutoBrightness_Gain_Mode_Set = (dyn_peak_IPL_AutoBrightness_Gain_Mode_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_Gain_Mode_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_Gain_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_Gain_Mode_Set");
    }

    m_peak_IPL_AutoBrightness_Gain_Mode_Get = (dyn_peak_IPL_AutoBrightness_Gain_Mode_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_Gain_Mode_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_Gain_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_Gain_Mode_Get");
    }

    m_peak_IPL_AutoBrightness_GainAnalog_Mode_Set = (dyn_peak_IPL_AutoBrightness_GainAnalog_Mode_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainAnalog_Mode_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainAnalog_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainAnalog_Mode_Set");
    }

    m_peak_IPL_AutoBrightness_GainAnalog_Mode_Get = (dyn_peak_IPL_AutoBrightness_GainAnalog_Mode_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainAnalog_Mode_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainAnalog_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainAnalog_Mode_Get");
    }

    m_peak_IPL_AutoBrightness_GainDigital_Mode_Set = (dyn_peak_IPL_AutoBrightness_GainDigital_Mode_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainDigital_Mode_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainDigital_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainDigital_Mode_Set");
    }

    m_peak_IPL_AutoBrightness_GainDigital_Mode_Get = (dyn_peak_IPL_AutoBrightness_GainDigital_Mode_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainDigital_Mode_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainDigital_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainDigital_Mode_Get");
    }

    m_peak_IPL_AutoBrightness_GainCombined_Mode_Set = (dyn_peak_IPL_AutoBrightness_GainCombined_Mode_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainCombined_Mode_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainCombined_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainCombined_Mode_Set");
    }

    m_peak_IPL_AutoBrightness_GainCombined_Mode_Get = (dyn_peak_IPL_AutoBrightness_GainCombined_Mode_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainCombined_Mode_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainCombined_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainCombined_Mode_Get");
    }

    m_peak_IPL_AutoBrightness_GainHost_Mode_Set = (dyn_peak_IPL_AutoBrightness_GainHost_Mode_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainHost_Mode_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainHost_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainHost_Mode_Set");
    }

    m_peak_IPL_AutoBrightness_GainHost_Mode_Get = (dyn_peak_IPL_AutoBrightness_GainHost_Mode_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainHost_Mode_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainHost_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainHost_Mode_Get");
    }

    m_peak_IPL_AutoBrightness_Algorithm_Set = (dyn_peak_IPL_AutoBrightness_Algorithm_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_Algorithm_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_Algorithm_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_Algorithm_Set");
    }

    m_peak_IPL_AutoBrightness_Algorithm_Get = (dyn_peak_IPL_AutoBrightness_Algorithm_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_Algorithm_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_Algorithm_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_Algorithm_Get");
    }

    m_peak_IPL_AutoBrightness_AverageLast_Get = (dyn_peak_IPL_AutoBrightness_AverageLast_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_AverageLast_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_AverageLast_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_AverageLast_Get");
    }

    m_peak_IPL_AutoBrightness_GainLimit_Set = (dyn_peak_IPL_AutoBrightness_GainLimit_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainLimit_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainLimit_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainLimit_Set");
    }

    m_peak_IPL_AutoBrightness_GainLimit_Get = (dyn_peak_IPL_AutoBrightness_GainLimit_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainLimit_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainLimit_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainLimit_Get");
    }

    m_peak_IPL_AutoBrightness_GainLimit_GetRange = (dyn_peak_IPL_AutoBrightness_GainLimit_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainLimit_GetRange") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainLimit_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainLimit_GetRange");
    }

    m_peak_IPL_AutoBrightness_GainAnalogLimit_Set = (dyn_peak_IPL_AutoBrightness_GainAnalogLimit_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainAnalogLimit_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainAnalogLimit_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainAnalogLimit_Set");
    }

    m_peak_IPL_AutoBrightness_GainAnalogLimit_Get = (dyn_peak_IPL_AutoBrightness_GainAnalogLimit_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainAnalogLimit_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainAnalogLimit_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainAnalogLimit_Get");
    }

    m_peak_IPL_AutoBrightness_GainAnalogLimit_GetRange = (dyn_peak_IPL_AutoBrightness_GainAnalogLimit_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainAnalogLimit_GetRange") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainAnalogLimit_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainAnalogLimit_GetRange");
    }

    m_peak_IPL_AutoBrightness_GainDigitalLimit_Set = (dyn_peak_IPL_AutoBrightness_GainDigitalLimit_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainDigitalLimit_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainDigitalLimit_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainDigitalLimit_Set");
    }

    m_peak_IPL_AutoBrightness_GainDigitalLimit_Get = (dyn_peak_IPL_AutoBrightness_GainDigitalLimit_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainDigitalLimit_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainDigitalLimit_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainDigitalLimit_Get");
    }

    m_peak_IPL_AutoBrightness_GainDigitalLimit_GetRange = (dyn_peak_IPL_AutoBrightness_GainDigitalLimit_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainDigitalLimit_GetRange") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainDigitalLimit_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainDigitalLimit_GetRange");
    }

    m_peak_IPL_AutoBrightness_GainCombinedLimit_Set = (dyn_peak_IPL_AutoBrightness_GainCombinedLimit_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainCombinedLimit_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainCombinedLimit_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainCombinedLimit_Set");
    }

    m_peak_IPL_AutoBrightness_GainCombinedLimit_Get = (dyn_peak_IPL_AutoBrightness_GainCombinedLimit_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainCombinedLimit_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainCombinedLimit_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainCombinedLimit_Get");
    }

    m_peak_IPL_AutoBrightness_GainCombinedLimit_GetRange = (dyn_peak_IPL_AutoBrightness_GainCombinedLimit_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainCombinedLimit_GetRange") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainCombinedLimit_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainCombinedLimit_GetRange");
    }

    m_peak_IPL_AutoBrightness_GainHostLimit_Set = (dyn_peak_IPL_AutoBrightness_GainHostLimit_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainHostLimit_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainHostLimit_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainHostLimit_Set");
    }

    m_peak_IPL_AutoBrightness_GainHostLimit_Get = (dyn_peak_IPL_AutoBrightness_GainHostLimit_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainHostLimit_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainHostLimit_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainHostLimit_Get");
    }

    m_peak_IPL_AutoBrightness_GainHostLimit_GetRange = (dyn_peak_IPL_AutoBrightness_GainHostLimit_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_GainHostLimit_GetRange") : nullptr);
    if(m_peak_IPL_AutoBrightness_GainHostLimit_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_GainHostLimit_GetRange");
    }

    m_peak_IPL_AutoBrightness_SkipFrames_Set = (dyn_peak_IPL_AutoBrightness_SkipFrames_Set) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_SkipFrames_Set") : nullptr);
    if(m_peak_IPL_AutoBrightness_SkipFrames_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_SkipFrames_Set");
    }

    m_peak_IPL_AutoBrightness_SkipFrames_Get = (dyn_peak_IPL_AutoBrightness_SkipFrames_Get) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_SkipFrames_Get") : nullptr);
    if(m_peak_IPL_AutoBrightness_SkipFrames_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_SkipFrames_Get");
    }

    m_peak_IPL_AutoBrightness_SkipFrames_GetRange = (dyn_peak_IPL_AutoBrightness_SkipFrames_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoBrightness_SkipFrames_GetRange") : nullptr);
    if(m_peak_IPL_AutoBrightness_SkipFrames_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoBrightness_SkipFrames_GetRange");
    }

    m_peak_IPL_AutoWhiteBalance_ROI_Mode_Set = (dyn_peak_IPL_AutoWhiteBalance_ROI_Mode_Set) (load ?  import_function(m_handle, "peak_IPL_AutoWhiteBalance_ROI_Mode_Set") : nullptr);
    if(m_peak_IPL_AutoWhiteBalance_ROI_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoWhiteBalance_ROI_Mode_Set");
    }

    m_peak_IPL_AutoWhiteBalance_ROI_Mode_Get = (dyn_peak_IPL_AutoWhiteBalance_ROI_Mode_Get) (load ?  import_function(m_handle, "peak_IPL_AutoWhiteBalance_ROI_Mode_Get") : nullptr);
    if(m_peak_IPL_AutoWhiteBalance_ROI_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoWhiteBalance_ROI_Mode_Get");
    }

    m_peak_IPL_AutoWhiteBalance_ROI_Offset_GetRange = (dyn_peak_IPL_AutoWhiteBalance_ROI_Offset_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoWhiteBalance_ROI_Offset_GetRange") : nullptr);
    if(m_peak_IPL_AutoWhiteBalance_ROI_Offset_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoWhiteBalance_ROI_Offset_GetRange");
    }

    m_peak_IPL_AutoWhiteBalance_ROI_Size_GetRange = (dyn_peak_IPL_AutoWhiteBalance_ROI_Size_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoWhiteBalance_ROI_Size_GetRange") : nullptr);
    if(m_peak_IPL_AutoWhiteBalance_ROI_Size_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoWhiteBalance_ROI_Size_GetRange");
    }

    m_peak_IPL_AutoWhiteBalance_ROI_Set = (dyn_peak_IPL_AutoWhiteBalance_ROI_Set) (load ?  import_function(m_handle, "peak_IPL_AutoWhiteBalance_ROI_Set") : nullptr);
    if(m_peak_IPL_AutoWhiteBalance_ROI_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoWhiteBalance_ROI_Set");
    }

    m_peak_IPL_AutoWhiteBalance_ROI_Get = (dyn_peak_IPL_AutoWhiteBalance_ROI_Get) (load ?  import_function(m_handle, "peak_IPL_AutoWhiteBalance_ROI_Get") : nullptr);
    if(m_peak_IPL_AutoWhiteBalance_ROI_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoWhiteBalance_ROI_Get");
    }

    m_peak_IPL_AutoWhiteBalance_Mode_Set = (dyn_peak_IPL_AutoWhiteBalance_Mode_Set) (load ?  import_function(m_handle, "peak_IPL_AutoWhiteBalance_Mode_Set") : nullptr);
    if(m_peak_IPL_AutoWhiteBalance_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoWhiteBalance_Mode_Set");
    }

    m_peak_IPL_AutoWhiteBalance_Mode_Get = (dyn_peak_IPL_AutoWhiteBalance_Mode_Get) (load ?  import_function(m_handle, "peak_IPL_AutoWhiteBalance_Mode_Get") : nullptr);
    if(m_peak_IPL_AutoWhiteBalance_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoWhiteBalance_Mode_Get");
    }

    m_peak_IPL_AutoWhiteBalance_SkipFrames_Set = (dyn_peak_IPL_AutoWhiteBalance_SkipFrames_Set) (load ?  import_function(m_handle, "peak_IPL_AutoWhiteBalance_SkipFrames_Set") : nullptr);
    if(m_peak_IPL_AutoWhiteBalance_SkipFrames_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoWhiteBalance_SkipFrames_Set");
    }

    m_peak_IPL_AutoWhiteBalance_SkipFrames_Get = (dyn_peak_IPL_AutoWhiteBalance_SkipFrames_Get) (load ?  import_function(m_handle, "peak_IPL_AutoWhiteBalance_SkipFrames_Get") : nullptr);
    if(m_peak_IPL_AutoWhiteBalance_SkipFrames_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoWhiteBalance_SkipFrames_Get");
    }

    m_peak_IPL_AutoWhiteBalance_SkipFrames_GetRange = (dyn_peak_IPL_AutoWhiteBalance_SkipFrames_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoWhiteBalance_SkipFrames_GetRange") : nullptr);
    if(m_peak_IPL_AutoWhiteBalance_SkipFrames_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoWhiteBalance_SkipFrames_GetRange");
    }

    m_peak_IPL_AutoFocus_GetAccessStatus = (dyn_peak_IPL_AutoFocus_GetAccessStatus) (load ?  import_function(m_handle, "peak_IPL_AutoFocus_GetAccessStatus") : nullptr);
    if(m_peak_IPL_AutoFocus_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoFocus_GetAccessStatus");
    }

    m_peak_IPL_AutoFocus_ROI_Set = (dyn_peak_IPL_AutoFocus_ROI_Set) (load ?  import_function(m_handle, "peak_IPL_AutoFocus_ROI_Set") : nullptr);
    if(m_peak_IPL_AutoFocus_ROI_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoFocus_ROI_Set");
    }

    m_peak_IPL_AutoFocus_ROI_Get = (dyn_peak_IPL_AutoFocus_ROI_Get) (load ?  import_function(m_handle, "peak_IPL_AutoFocus_ROI_Get") : nullptr);
    if(m_peak_IPL_AutoFocus_ROI_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoFocus_ROI_Get");
    }

    m_peak_IPL_AutoFocus_Mode_Set = (dyn_peak_IPL_AutoFocus_Mode_Set) (load ?  import_function(m_handle, "peak_IPL_AutoFocus_Mode_Set") : nullptr);
    if(m_peak_IPL_AutoFocus_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoFocus_Mode_Set");
    }

    m_peak_IPL_AutoFocus_Mode_Get = (dyn_peak_IPL_AutoFocus_Mode_Get) (load ?  import_function(m_handle, "peak_IPL_AutoFocus_Mode_Get") : nullptr);
    if(m_peak_IPL_AutoFocus_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoFocus_Mode_Get");
    }

    m_peak_IPL_AutoFocus_SearchAlgorithm_Set = (dyn_peak_IPL_AutoFocus_SearchAlgorithm_Set) (load ?  import_function(m_handle, "peak_IPL_AutoFocus_SearchAlgorithm_Set") : nullptr);
    if(m_peak_IPL_AutoFocus_SearchAlgorithm_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoFocus_SearchAlgorithm_Set");
    }

    m_peak_IPL_AutoFocus_SearchAlgorithm_Get = (dyn_peak_IPL_AutoFocus_SearchAlgorithm_Get) (load ?  import_function(m_handle, "peak_IPL_AutoFocus_SearchAlgorithm_Get") : nullptr);
    if(m_peak_IPL_AutoFocus_SearchAlgorithm_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoFocus_SearchAlgorithm_Get");
    }

    m_peak_IPL_AutoFocus_SharpnessAlgorithm_Set = (dyn_peak_IPL_AutoFocus_SharpnessAlgorithm_Set) (load ?  import_function(m_handle, "peak_IPL_AutoFocus_SharpnessAlgorithm_Set") : nullptr);
    if(m_peak_IPL_AutoFocus_SharpnessAlgorithm_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoFocus_SharpnessAlgorithm_Set");
    }

    m_peak_IPL_AutoFocus_SharpnessAlgorithm_Get = (dyn_peak_IPL_AutoFocus_SharpnessAlgorithm_Get) (load ?  import_function(m_handle, "peak_IPL_AutoFocus_SharpnessAlgorithm_Get") : nullptr);
    if(m_peak_IPL_AutoFocus_SharpnessAlgorithm_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoFocus_SharpnessAlgorithm_Get");
    }

    m_peak_IPL_AutoFocus_Range_Set = (dyn_peak_IPL_AutoFocus_Range_Set) (load ?  import_function(m_handle, "peak_IPL_AutoFocus_Range_Set") : nullptr);
    if(m_peak_IPL_AutoFocus_Range_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoFocus_Range_Set");
    }

    m_peak_IPL_AutoFocus_Range_Get = (dyn_peak_IPL_AutoFocus_Range_Get) (load ?  import_function(m_handle, "peak_IPL_AutoFocus_Range_Get") : nullptr);
    if(m_peak_IPL_AutoFocus_Range_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoFocus_Range_Get");
    }

    m_peak_IPL_AutoFocus_Hysteresis_Set = (dyn_peak_IPL_AutoFocus_Hysteresis_Set) (load ?  import_function(m_handle, "peak_IPL_AutoFocus_Hysteresis_Set") : nullptr);
    if(m_peak_IPL_AutoFocus_Hysteresis_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoFocus_Hysteresis_Set");
    }

    m_peak_IPL_AutoFocus_Hysteresis_Get = (dyn_peak_IPL_AutoFocus_Hysteresis_Get) (load ?  import_function(m_handle, "peak_IPL_AutoFocus_Hysteresis_Get") : nullptr);
    if(m_peak_IPL_AutoFocus_Hysteresis_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoFocus_Hysteresis_Get");
    }

    m_peak_IPL_AutoFocus_Hysteresis_GetRange = (dyn_peak_IPL_AutoFocus_Hysteresis_GetRange) (load ?  import_function(m_handle, "peak_IPL_AutoFocus_Hysteresis_GetRange") : nullptr);
    if(m_peak_IPL_AutoFocus_Hysteresis_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_AutoFocus_Hysteresis_GetRange");
    }

    m_peak_IPL_HotpixelCorrection_Sensitivity_Set = (dyn_peak_IPL_HotpixelCorrection_Sensitivity_Set) (load ?  import_function(m_handle, "peak_IPL_HotpixelCorrection_Sensitivity_Set") : nullptr);
    if(m_peak_IPL_HotpixelCorrection_Sensitivity_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_HotpixelCorrection_Sensitivity_Set");
    }

    m_peak_IPL_HotpixelCorrection_Sensitivity_Get = (dyn_peak_IPL_HotpixelCorrection_Sensitivity_Get) (load ?  import_function(m_handle, "peak_IPL_HotpixelCorrection_Sensitivity_Get") : nullptr);
    if(m_peak_IPL_HotpixelCorrection_Sensitivity_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_HotpixelCorrection_Sensitivity_Get");
    }

    m_peak_IPL_HotpixelCorrection_GetList = (dyn_peak_IPL_HotpixelCorrection_GetList) (load ?  import_function(m_handle, "peak_IPL_HotpixelCorrection_GetList") : nullptr);
    if(m_peak_IPL_HotpixelCorrection_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_HotpixelCorrection_GetList");
    }

    m_peak_IPL_HotpixelCorrection_SetList = (dyn_peak_IPL_HotpixelCorrection_SetList) (load ?  import_function(m_handle, "peak_IPL_HotpixelCorrection_SetList") : nullptr);
    if(m_peak_IPL_HotpixelCorrection_SetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_HotpixelCorrection_SetList");
    }

    m_peak_IPL_HotpixelCorrection_ResetList = (dyn_peak_IPL_HotpixelCorrection_ResetList) (load ?  import_function(m_handle, "peak_IPL_HotpixelCorrection_ResetList") : nullptr);
    if(m_peak_IPL_HotpixelCorrection_ResetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_HotpixelCorrection_ResetList");
    }

    m_peak_IPL_HotpixelCorrection_Enable = (dyn_peak_IPL_HotpixelCorrection_Enable) (load ?  import_function(m_handle, "peak_IPL_HotpixelCorrection_Enable") : nullptr);
    if(m_peak_IPL_HotpixelCorrection_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_HotpixelCorrection_Enable");
    }

    m_peak_IPL_HotpixelCorrection_IsEnabled = (dyn_peak_IPL_HotpixelCorrection_IsEnabled) (load ?  import_function(m_handle, "peak_IPL_HotpixelCorrection_IsEnabled") : nullptr);
    if(m_peak_IPL_HotpixelCorrection_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_HotpixelCorrection_IsEnabled");
    }

    m_peak_IPL_Mirror_UpDown_Enable = (dyn_peak_IPL_Mirror_UpDown_Enable) (load ?  import_function(m_handle, "peak_IPL_Mirror_UpDown_Enable") : nullptr);
    if(m_peak_IPL_Mirror_UpDown_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Mirror_UpDown_Enable");
    }

    m_peak_IPL_Mirror_UpDown_IsEnabled = (dyn_peak_IPL_Mirror_UpDown_IsEnabled) (load ?  import_function(m_handle, "peak_IPL_Mirror_UpDown_IsEnabled") : nullptr);
    if(m_peak_IPL_Mirror_UpDown_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Mirror_UpDown_IsEnabled");
    }

    m_peak_IPL_Mirror_LeftRight_Enable = (dyn_peak_IPL_Mirror_LeftRight_Enable) (load ?  import_function(m_handle, "peak_IPL_Mirror_LeftRight_Enable") : nullptr);
    if(m_peak_IPL_Mirror_LeftRight_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Mirror_LeftRight_Enable");
    }

    m_peak_IPL_Mirror_LeftRight_IsEnabled = (dyn_peak_IPL_Mirror_LeftRight_IsEnabled) (load ?  import_function(m_handle, "peak_IPL_Mirror_LeftRight_IsEnabled") : nullptr);
    if(m_peak_IPL_Mirror_LeftRight_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Mirror_LeftRight_IsEnabled");
    }

    m_peak_IPL_ProcessFrame = (dyn_peak_IPL_ProcessFrame) (load ?  import_function(m_handle, "peak_IPL_ProcessFrame") : nullptr);
    if(m_peak_IPL_ProcessFrame == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ProcessFrame");
    }

    m_peak_IPL_ProcessFrameInplace = (dyn_peak_IPL_ProcessFrameInplace) (load ?  import_function(m_handle, "peak_IPL_ProcessFrameInplace") : nullptr);
    if(m_peak_IPL_ProcessFrameInplace == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ProcessFrameInplace");
    }

    m_peak_IPL_ReadImage = (dyn_peak_IPL_ReadImage) (load ?  import_function(m_handle, "peak_IPL_ReadImage") : nullptr);
    if(m_peak_IPL_ReadImage == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ReadImage");
    }

    m_peak_IPL_EdgeEnhancement_Enable = (dyn_peak_IPL_EdgeEnhancement_Enable) (load ?  import_function(m_handle, "peak_IPL_EdgeEnhancement_Enable") : nullptr);
    if(m_peak_IPL_EdgeEnhancement_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_EdgeEnhancement_Enable");
    }

    m_peak_IPL_EdgeEnhancement_IsEnabled = (dyn_peak_IPL_EdgeEnhancement_IsEnabled) (load ?  import_function(m_handle, "peak_IPL_EdgeEnhancement_IsEnabled") : nullptr);
    if(m_peak_IPL_EdgeEnhancement_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_EdgeEnhancement_IsEnabled");
    }

    m_peak_IPL_EdgeEnhancement_Factor_Set = (dyn_peak_IPL_EdgeEnhancement_Factor_Set) (load ?  import_function(m_handle, "peak_IPL_EdgeEnhancement_Factor_Set") : nullptr);
    if(m_peak_IPL_EdgeEnhancement_Factor_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_EdgeEnhancement_Factor_Set");
    }

    m_peak_IPL_EdgeEnhancement_Factor_Get = (dyn_peak_IPL_EdgeEnhancement_Factor_Get) (load ?  import_function(m_handle, "peak_IPL_EdgeEnhancement_Factor_Get") : nullptr);
    if(m_peak_IPL_EdgeEnhancement_Factor_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_EdgeEnhancement_Factor_Get");
    }

    m_peak_IPL_EdgeEnhancement_Factor_GetDefault = (dyn_peak_IPL_EdgeEnhancement_Factor_GetDefault) (load ?  import_function(m_handle, "peak_IPL_EdgeEnhancement_Factor_GetDefault") : nullptr);
    if(m_peak_IPL_EdgeEnhancement_Factor_GetDefault == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_EdgeEnhancement_Factor_GetDefault");
    }

    m_peak_IPL_EdgeEnhancement_Factor_GetRange = (dyn_peak_IPL_EdgeEnhancement_Factor_GetRange) (load ?  import_function(m_handle, "peak_IPL_EdgeEnhancement_Factor_GetRange") : nullptr);
    if(m_peak_IPL_EdgeEnhancement_Factor_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_EdgeEnhancement_Factor_GetRange");
    }

    m_peak_IPL_Sharpness_Measure = (dyn_peak_IPL_Sharpness_Measure) (load ?  import_function(m_handle, "peak_IPL_Sharpness_Measure") : nullptr);
    if(m_peak_IPL_Sharpness_Measure == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Sharpness_Measure");
    }

    m_peak_IPL_Sharpness_GetList = (dyn_peak_IPL_Sharpness_GetList) (load ?  import_function(m_handle, "peak_IPL_Sharpness_GetList") : nullptr);
    if(m_peak_IPL_Sharpness_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Sharpness_GetList");
    }

    m_peak_IPL_Rotation_Angle_Set = (dyn_peak_IPL_Rotation_Angle_Set) (load ?  import_function(m_handle, "peak_IPL_Rotation_Angle_Set") : nullptr);
    if(m_peak_IPL_Rotation_Angle_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Rotation_Angle_Set");
    }

    m_peak_IPL_Rotation_Angle_Get = (dyn_peak_IPL_Rotation_Angle_Get) (load ?  import_function(m_handle, "peak_IPL_Rotation_Angle_Get") : nullptr);
    if(m_peak_IPL_Rotation_Angle_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Rotation_Angle_Get");
    }

    m_peak_IPL_Histogram_ProcessFrame = (dyn_peak_IPL_Histogram_ProcessFrame) (load ?  import_function(m_handle, "peak_IPL_Histogram_ProcessFrame") : nullptr);
    if(m_peak_IPL_Histogram_ProcessFrame == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Histogram_ProcessFrame");
    }

    m_peak_IPL_Histogram_Release = (dyn_peak_IPL_Histogram_Release) (load ?  import_function(m_handle, "peak_IPL_Histogram_Release") : nullptr);
    if(m_peak_IPL_Histogram_Release == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Histogram_Release");
    }

    m_peak_IPL_Histogram_Channel_GetCount = (dyn_peak_IPL_Histogram_Channel_GetCount) (load ?  import_function(m_handle, "peak_IPL_Histogram_Channel_GetCount") : nullptr);
    if(m_peak_IPL_Histogram_Channel_GetCount == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Histogram_Channel_GetCount");
    }

    m_peak_IPL_Histogram_Channel_GetInfo = (dyn_peak_IPL_Histogram_Channel_GetInfo) (load ?  import_function(m_handle, "peak_IPL_Histogram_Channel_GetInfo") : nullptr);
    if(m_peak_IPL_Histogram_Channel_GetInfo == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Histogram_Channel_GetInfo");
    }

    m_peak_IPL_Histogram_Channel_GetBinArray = (dyn_peak_IPL_Histogram_Channel_GetBinArray) (load ?  import_function(m_handle, "peak_IPL_Histogram_Channel_GetBinArray") : nullptr);
    if(m_peak_IPL_Histogram_Channel_GetBinArray == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Histogram_Channel_GetBinArray");
    }

    m_peak_VideoWriter_Open = (dyn_peak_VideoWriter_Open) (load ?  import_function(m_handle, "peak_VideoWriter_Open") : nullptr);
    if(m_peak_VideoWriter_Open == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_VideoWriter_Open");
    }

    m_peak_VideoWriter_Close = (dyn_peak_VideoWriter_Close) (load ?  import_function(m_handle, "peak_VideoWriter_Close") : nullptr);
    if(m_peak_VideoWriter_Close == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_VideoWriter_Close");
    }

    m_peak_VideoWriter_AddFrame = (dyn_peak_VideoWriter_AddFrame) (load ?  import_function(m_handle, "peak_VideoWriter_AddFrame") : nullptr);
    if(m_peak_VideoWriter_AddFrame == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_VideoWriter_AddFrame");
    }

    m_peak_VideoWriter_Container_GetEncoderList = (dyn_peak_VideoWriter_Container_GetEncoderList) (load ?  import_function(m_handle, "peak_VideoWriter_Container_GetEncoderList") : nullptr);
    if(m_peak_VideoWriter_Container_GetEncoderList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_VideoWriter_Container_GetEncoderList");
    }

    m_peak_VideoWriter_Encoder_GetPixelFormatList = (dyn_peak_VideoWriter_Encoder_GetPixelFormatList) (load ?  import_function(m_handle, "peak_VideoWriter_Encoder_GetPixelFormatList") : nullptr);
    if(m_peak_VideoWriter_Encoder_GetPixelFormatList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_VideoWriter_Encoder_GetPixelFormatList");
    }

    m_peak_VideoWriter_Encoder_GetContainerList = (dyn_peak_VideoWriter_Encoder_GetContainerList) (load ?  import_function(m_handle, "peak_VideoWriter_Encoder_GetContainerList") : nullptr);
    if(m_peak_VideoWriter_Encoder_GetContainerList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_VideoWriter_Encoder_GetContainerList");
    }

    m_peak_VideoWriter_GetInfo = (dyn_peak_VideoWriter_GetInfo) (load ?  import_function(m_handle, "peak_VideoWriter_GetInfo") : nullptr);
    if(m_peak_VideoWriter_GetInfo == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_VideoWriter_GetInfo");
    }

    m_peak_VideoWriter_Container_Option_Set = (dyn_peak_VideoWriter_Container_Option_Set) (load ?  import_function(m_handle, "peak_VideoWriter_Container_Option_Set") : nullptr);
    if(m_peak_VideoWriter_Container_Option_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_VideoWriter_Container_Option_Set");
    }

    m_peak_VideoWriter_Container_Option_Get = (dyn_peak_VideoWriter_Container_Option_Get) (load ?  import_function(m_handle, "peak_VideoWriter_Container_Option_Get") : nullptr);
    if(m_peak_VideoWriter_Container_Option_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_VideoWriter_Container_Option_Get");
    }

    m_peak_VideoWriter_Encoder_Option_Set = (dyn_peak_VideoWriter_Encoder_Option_Set) (load ?  import_function(m_handle, "peak_VideoWriter_Encoder_Option_Set") : nullptr);
    if(m_peak_VideoWriter_Encoder_Option_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_VideoWriter_Encoder_Option_Set");
    }

    m_peak_VideoWriter_Encoder_Option_Get = (dyn_peak_VideoWriter_Encoder_Option_Get) (load ?  import_function(m_handle, "peak_VideoWriter_Encoder_Option_Get") : nullptr);
    if(m_peak_VideoWriter_Encoder_Option_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_VideoWriter_Encoder_Option_Get");
    }

    m_peak_VideoWriter_WaitUntilQueueEmpty = (dyn_peak_VideoWriter_WaitUntilQueueEmpty) (load ?  import_function(m_handle, "peak_VideoWriter_WaitUntilQueueEmpty") : nullptr);
    if(m_peak_VideoWriter_WaitUntilQueueEmpty == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_VideoWriter_WaitUntilQueueEmpty");
    }

    m_peak_Inference_CNN_Open = (dyn_peak_Inference_CNN_Open) (load ?  import_function(m_handle, "peak_Inference_CNN_Open") : nullptr);
    if(m_peak_Inference_CNN_Open == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Inference_CNN_Open");
    }

    m_peak_Inference_CNN_Close = (dyn_peak_Inference_CNN_Close) (load ?  import_function(m_handle, "peak_Inference_CNN_Close") : nullptr);
    if(m_peak_Inference_CNN_Close == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Inference_CNN_Close");
    }

    m_peak_Inference_CNN_ProcessFrame = (dyn_peak_Inference_CNN_ProcessFrame) (load ?  import_function(m_handle, "peak_Inference_CNN_ProcessFrame") : nullptr);
    if(m_peak_Inference_CNN_ProcessFrame == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Inference_CNN_ProcessFrame");
    }

    m_peak_Inference_CNN_Info_Get = (dyn_peak_Inference_CNN_Info_Get) (load ?  import_function(m_handle, "peak_Inference_CNN_Info_Get") : nullptr);
    if(m_peak_Inference_CNN_Info_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Inference_CNN_Info_Get");
    }

    m_peak_Inference_Result_Get = (dyn_peak_Inference_Result_Get) (load ?  import_function(m_handle, "peak_Inference_Result_Get") : nullptr);
    if(m_peak_Inference_Result_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Inference_Result_Get");
    }

    m_peak_Inference_Result_Release = (dyn_peak_Inference_Result_Release) (load ?  import_function(m_handle, "peak_Inference_Result_Release") : nullptr);
    if(m_peak_Inference_Result_Release == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Inference_Result_Release");
    }

    m_peak_Inference_Result_Classification_GetList = (dyn_peak_Inference_Result_Classification_GetList) (load ?  import_function(m_handle, "peak_Inference_Result_Classification_GetList") : nullptr);
    if(m_peak_Inference_Result_Classification_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Inference_Result_Classification_GetList");
    }

    m_peak_Inference_Result_Detection_GetList = (dyn_peak_Inference_Result_Detection_GetList) (load ?  import_function(m_handle, "peak_Inference_Result_Detection_GetList") : nullptr);
    if(m_peak_Inference_Result_Detection_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Inference_Result_Detection_GetList");
    }

    m_peak_Inference_Statistics_Get = (dyn_peak_Inference_Statistics_Get) (load ?  import_function(m_handle, "peak_Inference_Statistics_Get") : nullptr);
    if(m_peak_Inference_Statistics_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Inference_Statistics_Get");
    }

    m_peak_Inference_Statistics_Reset = (dyn_peak_Inference_Statistics_Reset) (load ?  import_function(m_handle, "peak_Inference_Statistics_Reset") : nullptr);
    if(m_peak_Inference_Statistics_Reset == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Inference_Statistics_Reset");
    }

    m_peak_Inference_ConfidenceThreshold_Get = (dyn_peak_Inference_ConfidenceThreshold_Get) (load ?  import_function(m_handle, "peak_Inference_ConfidenceThreshold_Get") : nullptr);
    if(m_peak_Inference_ConfidenceThreshold_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Inference_ConfidenceThreshold_Get");
    }

    m_peak_Inference_ConfidenceThreshold_GetRange = (dyn_peak_Inference_ConfidenceThreshold_GetRange) (load ?  import_function(m_handle, "peak_Inference_ConfidenceThreshold_GetRange") : nullptr);
    if(m_peak_Inference_ConfidenceThreshold_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Inference_ConfidenceThreshold_GetRange");
    }

    m_peak_Inference_ConfidenceThreshold_Set = (dyn_peak_Inference_ConfidenceThreshold_Set) (load ?  import_function(m_handle, "peak_Inference_ConfidenceThreshold_Set") : nullptr);
    if(m_peak_Inference_ConfidenceThreshold_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Inference_ConfidenceThreshold_Set");
    }

    m_peak_MessageQueue_Create = (dyn_peak_MessageQueue_Create) (load ?  import_function(m_handle, "peak_MessageQueue_Create") : nullptr);
    if(m_peak_MessageQueue_Create == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_Create");
    }

    m_peak_MessageQueue_Destroy = (dyn_peak_MessageQueue_Destroy) (load ?  import_function(m_handle, "peak_MessageQueue_Destroy") : nullptr);
    if(m_peak_MessageQueue_Destroy == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_Destroy");
    }

    m_peak_MessageQueue_EnableMessage = (dyn_peak_MessageQueue_EnableMessage) (load ?  import_function(m_handle, "peak_MessageQueue_EnableMessage") : nullptr);
    if(m_peak_MessageQueue_EnableMessage == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_EnableMessage");
    }

    m_peak_MessageQueue_DisableMessage = (dyn_peak_MessageQueue_DisableMessage) (load ?  import_function(m_handle, "peak_MessageQueue_DisableMessage") : nullptr);
    if(m_peak_MessageQueue_DisableMessage == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_DisableMessage");
    }

    m_peak_MessageQueue_EnableMessageList = (dyn_peak_MessageQueue_EnableMessageList) (load ?  import_function(m_handle, "peak_MessageQueue_EnableMessageList") : nullptr);
    if(m_peak_MessageQueue_EnableMessageList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_EnableMessageList");
    }

    m_peak_MessageQueue_DisableMessageList = (dyn_peak_MessageQueue_DisableMessageList) (load ?  import_function(m_handle, "peak_MessageQueue_DisableMessageList") : nullptr);
    if(m_peak_MessageQueue_DisableMessageList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_DisableMessageList");
    }

    m_peak_MessageQueue_EnabledMessages_GetList = (dyn_peak_MessageQueue_EnabledMessages_GetList) (load ?  import_function(m_handle, "peak_MessageQueue_EnabledMessages_GetList") : nullptr);
    if(m_peak_MessageQueue_EnabledMessages_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_EnabledMessages_GetList");
    }

    m_peak_MessageQueue_SetMode = (dyn_peak_MessageQueue_SetMode) (load ?  import_function(m_handle, "peak_MessageQueue_SetMode") : nullptr);
    if(m_peak_MessageQueue_SetMode == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_SetMode");
    }

    m_peak_MessageQueue_GetMode = (dyn_peak_MessageQueue_GetMode) (load ?  import_function(m_handle, "peak_MessageQueue_GetMode") : nullptr);
    if(m_peak_MessageQueue_GetMode == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_GetMode");
    }

    m_peak_MessageQueue_Start = (dyn_peak_MessageQueue_Start) (load ?  import_function(m_handle, "peak_MessageQueue_Start") : nullptr);
    if(m_peak_MessageQueue_Start == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_Start");
    }

    m_peak_MessageQueue_Stop = (dyn_peak_MessageQueue_Stop) (load ?  import_function(m_handle, "peak_MessageQueue_Stop") : nullptr);
    if(m_peak_MessageQueue_Stop == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_Stop");
    }

    m_peak_MessageQueue_IsStarted = (dyn_peak_MessageQueue_IsStarted) (load ?  import_function(m_handle, "peak_MessageQueue_IsStarted") : nullptr);
    if(m_peak_MessageQueue_IsStarted == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_IsStarted");
    }

    m_peak_MessageQueue_WaitForMessage = (dyn_peak_MessageQueue_WaitForMessage) (load ?  import_function(m_handle, "peak_MessageQueue_WaitForMessage") : nullptr);
    if(m_peak_MessageQueue_WaitForMessage == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_WaitForMessage");
    }

    m_peak_MessageQueue_Flush = (dyn_peak_MessageQueue_Flush) (load ?  import_function(m_handle, "peak_MessageQueue_Flush") : nullptr);
    if(m_peak_MessageQueue_Flush == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_Flush");
    }

    m_peak_MessageQueue_IsMessageSupported = (dyn_peak_MessageQueue_IsMessageSupported) (load ?  import_function(m_handle, "peak_MessageQueue_IsMessageSupported") : nullptr);
    if(m_peak_MessageQueue_IsMessageSupported == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_IsMessageSupported");
    }

    m_peak_MessageQueue_Statistics_Get = (dyn_peak_MessageQueue_Statistics_Get) (load ?  import_function(m_handle, "peak_MessageQueue_Statistics_Get") : nullptr);
    if(m_peak_MessageQueue_Statistics_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_Statistics_Get");
    }

    m_peak_MessageQueue_Statistics_Reset = (dyn_peak_MessageQueue_Statistics_Reset) (load ?  import_function(m_handle, "peak_MessageQueue_Statistics_Reset") : nullptr);
    if(m_peak_MessageQueue_Statistics_Reset == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_Statistics_Reset");
    }

    m_peak_MessageQueue_MaxQueueSize_Set = (dyn_peak_MessageQueue_MaxQueueSize_Set) (load ?  import_function(m_handle, "peak_MessageQueue_MaxQueueSize_Set") : nullptr);
    if(m_peak_MessageQueue_MaxQueueSize_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_MaxQueueSize_Set");
    }

    m_peak_MessageQueue_MaxQueueSize_Get = (dyn_peak_MessageQueue_MaxQueueSize_Get) (load ?  import_function(m_handle, "peak_MessageQueue_MaxQueueSize_Get") : nullptr);
    if(m_peak_MessageQueue_MaxQueueSize_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_MessageQueue_MaxQueueSize_Get");
    }

    m_peak_Message_Release = (dyn_peak_Message_Release) (load ?  import_function(m_handle, "peak_Message_Release") : nullptr);
    if(m_peak_Message_Release == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_Release");
    }

    m_peak_Message_GetInfo = (dyn_peak_Message_GetInfo) (load ?  import_function(m_handle, "peak_Message_GetInfo") : nullptr);
    if(m_peak_Message_GetInfo == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_GetInfo");
    }

    m_peak_Message_Type_Get = (dyn_peak_Message_Type_Get) (load ?  import_function(m_handle, "peak_Message_Type_Get") : nullptr);
    if(m_peak_Message_Type_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_Type_Get");
    }

    m_peak_Message_CameraHandle_Get = (dyn_peak_Message_CameraHandle_Get) (load ?  import_function(m_handle, "peak_Message_CameraHandle_Get") : nullptr);
    if(m_peak_Message_CameraHandle_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_CameraHandle_Get");
    }

    m_peak_Message_ID_Get = (dyn_peak_Message_ID_Get) (load ?  import_function(m_handle, "peak_Message_ID_Get") : nullptr);
    if(m_peak_Message_ID_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_ID_Get");
    }

    m_peak_Message_HostTimestamp_Get = (dyn_peak_Message_HostTimestamp_Get) (load ?  import_function(m_handle, "peak_Message_HostTimestamp_Get") : nullptr);
    if(m_peak_Message_HostTimestamp_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_HostTimestamp_Get");
    }

    m_peak_Message_Data_Type_Get = (dyn_peak_Message_Data_Type_Get) (load ?  import_function(m_handle, "peak_Message_Data_Type_Get") : nullptr);
    if(m_peak_Message_Data_Type_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_Data_Type_Get");
    }

    m_peak_Message_Data_RemoteDevice_Get = (dyn_peak_Message_Data_RemoteDevice_Get) (load ?  import_function(m_handle, "peak_Message_Data_RemoteDevice_Get") : nullptr);
    if(m_peak_Message_Data_RemoteDevice_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_Data_RemoteDevice_Get");
    }

    m_peak_Message_Data_RemoteDeviceError_Get = (dyn_peak_Message_Data_RemoteDeviceError_Get) (load ?  import_function(m_handle, "peak_Message_Data_RemoteDeviceError_Get") : nullptr);
    if(m_peak_Message_Data_RemoteDeviceError_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_Data_RemoteDeviceError_Get");
    }

    m_peak_Message_Data_RemoteDeviceDropped_Get = (dyn_peak_Message_Data_RemoteDeviceDropped_Get) (load ?  import_function(m_handle, "peak_Message_Data_RemoteDeviceDropped_Get") : nullptr);
    if(m_peak_Message_Data_RemoteDeviceDropped_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_Data_RemoteDeviceDropped_Get");
    }

    m_peak_Message_Data_RemoteDeviceFrame_Get = (dyn_peak_Message_Data_RemoteDeviceFrame_Get) (load ?  import_function(m_handle, "peak_Message_Data_RemoteDeviceFrame_Get") : nullptr);
    if(m_peak_Message_Data_RemoteDeviceFrame_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_Data_RemoteDeviceFrame_Get");
    }

    m_peak_Message_Data_RemoteDeviceTemperature_Get = (dyn_peak_Message_Data_RemoteDeviceTemperature_Get) (load ?  import_function(m_handle, "peak_Message_Data_RemoteDeviceTemperature_Get") : nullptr);
    if(m_peak_Message_Data_RemoteDeviceTemperature_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_Data_RemoteDeviceTemperature_Get");
    }

    m_peak_Message_Data_AutoFocusData_Get = (dyn_peak_Message_Data_AutoFocusData_Get) (load ?  import_function(m_handle, "peak_Message_Data_AutoFocusData_Get") : nullptr);
    if(m_peak_Message_Data_AutoFocusData_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_Data_AutoFocusData_Get");
    }

    m_peak_Message_Data_DeviceFound_Get = (dyn_peak_Message_Data_DeviceFound_Get) (load ?  import_function(m_handle, "peak_Message_Data_DeviceFound_Get") : nullptr);
    if(m_peak_Message_Data_DeviceFound_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_Data_DeviceFound_Get");
    }

    m_peak_Message_Data_DeviceLost_Get = (dyn_peak_Message_Data_DeviceLost_Get) (load ?  import_function(m_handle, "peak_Message_Data_DeviceLost_Get") : nullptr);
    if(m_peak_Message_Data_DeviceLost_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_Data_DeviceLost_Get");
    }

    m_peak_Message_Data_DeviceReconnected_Get = (dyn_peak_Message_Data_DeviceReconnected_Get) (load ?  import_function(m_handle, "peak_Message_Data_DeviceReconnected_Get") : nullptr);
    if(m_peak_Message_Data_DeviceReconnected_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_Data_DeviceReconnected_Get");
    }

    m_peak_Message_Data_DeviceDisconnected_Get = (dyn_peak_Message_Data_DeviceDisconnected_Get) (load ?  import_function(m_handle, "peak_Message_Data_DeviceDisconnected_Get") : nullptr);
    if(m_peak_Message_Data_DeviceDisconnected_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_Data_DeviceDisconnected_Get");
    }

    m_peak_Message_Data_FirmwareUpdate_Get = (dyn_peak_Message_Data_FirmwareUpdate_Get) (load ?  import_function(m_handle, "peak_Message_Data_FirmwareUpdate_Get") : nullptr);
    if(m_peak_Message_Data_FirmwareUpdate_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Message_Data_FirmwareUpdate_Get");
    }

    m_peak_I2C_Create = (dyn_peak_I2C_Create) (load ?  import_function(m_handle, "peak_I2C_Create") : nullptr);
    if(m_peak_I2C_Create == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_Create");
    }

    m_peak_I2C_Destroy = (dyn_peak_I2C_Destroy) (load ?  import_function(m_handle, "peak_I2C_Destroy") : nullptr);
    if(m_peak_I2C_Destroy == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_Destroy");
    }

    m_peak_I2C_GetAccessStatus = (dyn_peak_I2C_GetAccessStatus) (load ?  import_function(m_handle, "peak_I2C_GetAccessStatus") : nullptr);
    if(m_peak_I2C_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_GetAccessStatus");
    }

    m_peak_I2C_Mode_GetList = (dyn_peak_I2C_Mode_GetList) (load ?  import_function(m_handle, "peak_I2C_Mode_GetList") : nullptr);
    if(m_peak_I2C_Mode_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_Mode_GetList");
    }

    m_peak_I2C_Mode_Set = (dyn_peak_I2C_Mode_Set) (load ?  import_function(m_handle, "peak_I2C_Mode_Set") : nullptr);
    if(m_peak_I2C_Mode_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_Mode_Set");
    }

    m_peak_I2C_Mode_Get = (dyn_peak_I2C_Mode_Get) (load ?  import_function(m_handle, "peak_I2C_Mode_Get") : nullptr);
    if(m_peak_I2C_Mode_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_Mode_Get");
    }

    m_peak_I2C_DeviceAddress_GetRange = (dyn_peak_I2C_DeviceAddress_GetRange) (load ?  import_function(m_handle, "peak_I2C_DeviceAddress_GetRange") : nullptr);
    if(m_peak_I2C_DeviceAddress_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_DeviceAddress_GetRange");
    }

    m_peak_I2C_DeviceAddress_Set = (dyn_peak_I2C_DeviceAddress_Set) (load ?  import_function(m_handle, "peak_I2C_DeviceAddress_Set") : nullptr);
    if(m_peak_I2C_DeviceAddress_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_DeviceAddress_Set");
    }

    m_peak_I2C_DeviceAddress_Get = (dyn_peak_I2C_DeviceAddress_Get) (load ?  import_function(m_handle, "peak_I2C_DeviceAddress_Get") : nullptr);
    if(m_peak_I2C_DeviceAddress_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_DeviceAddress_Get");
    }

    m_peak_I2C_RegisterAddress_Length_GetList = (dyn_peak_I2C_RegisterAddress_Length_GetList) (load ?  import_function(m_handle, "peak_I2C_RegisterAddress_Length_GetList") : nullptr);
    if(m_peak_I2C_RegisterAddress_Length_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_RegisterAddress_Length_GetList");
    }

    m_peak_I2C_RegisterAddress_Length_Set = (dyn_peak_I2C_RegisterAddress_Length_Set) (load ?  import_function(m_handle, "peak_I2C_RegisterAddress_Length_Set") : nullptr);
    if(m_peak_I2C_RegisterAddress_Length_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_RegisterAddress_Length_Set");
    }

    m_peak_I2C_RegisterAddress_Length_Get = (dyn_peak_I2C_RegisterAddress_Length_Get) (load ?  import_function(m_handle, "peak_I2C_RegisterAddress_Length_Get") : nullptr);
    if(m_peak_I2C_RegisterAddress_Length_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_RegisterAddress_Length_Get");
    }

    m_peak_I2C_RegisterAddress_Endianness_Set = (dyn_peak_I2C_RegisterAddress_Endianness_Set) (load ?  import_function(m_handle, "peak_I2C_RegisterAddress_Endianness_Set") : nullptr);
    if(m_peak_I2C_RegisterAddress_Endianness_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_RegisterAddress_Endianness_Set");
    }

    m_peak_I2C_RegisterAddress_Endianness_Get = (dyn_peak_I2C_RegisterAddress_Endianness_Get) (load ?  import_function(m_handle, "peak_I2C_RegisterAddress_Endianness_Get") : nullptr);
    if(m_peak_I2C_RegisterAddress_Endianness_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_RegisterAddress_Endianness_Get");
    }

    m_peak_I2C_RegisterAddress_Set = (dyn_peak_I2C_RegisterAddress_Set) (load ?  import_function(m_handle, "peak_I2C_RegisterAddress_Set") : nullptr);
    if(m_peak_I2C_RegisterAddress_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_RegisterAddress_Set");
    }

    m_peak_I2C_RegisterAddress_Get = (dyn_peak_I2C_RegisterAddress_Get) (load ?  import_function(m_handle, "peak_I2C_RegisterAddress_Get") : nullptr);
    if(m_peak_I2C_RegisterAddress_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_RegisterAddress_Get");
    }

    m_peak_I2C_AckPolling_Enable = (dyn_peak_I2C_AckPolling_Enable) (load ?  import_function(m_handle, "peak_I2C_AckPolling_Enable") : nullptr);
    if(m_peak_I2C_AckPolling_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_AckPolling_Enable");
    }

    m_peak_I2C_AckPolling_IsEnabled = (dyn_peak_I2C_AckPolling_IsEnabled) (load ?  import_function(m_handle, "peak_I2C_AckPolling_IsEnabled") : nullptr);
    if(m_peak_I2C_AckPolling_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_AckPolling_IsEnabled");
    }

    m_peak_I2C_AckPolling_Timeout_GetAccessStatus = (dyn_peak_I2C_AckPolling_Timeout_GetAccessStatus) (load ?  import_function(m_handle, "peak_I2C_AckPolling_Timeout_GetAccessStatus") : nullptr);
    if(m_peak_I2C_AckPolling_Timeout_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_AckPolling_Timeout_GetAccessStatus");
    }

    m_peak_I2C_AckPolling_Timeout_GetRange = (dyn_peak_I2C_AckPolling_Timeout_GetRange) (load ?  import_function(m_handle, "peak_I2C_AckPolling_Timeout_GetRange") : nullptr);
    if(m_peak_I2C_AckPolling_Timeout_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_AckPolling_Timeout_GetRange");
    }

    m_peak_I2C_AckPolling_Timeout_Set = (dyn_peak_I2C_AckPolling_Timeout_Set) (load ?  import_function(m_handle, "peak_I2C_AckPolling_Timeout_Set") : nullptr);
    if(m_peak_I2C_AckPolling_Timeout_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_AckPolling_Timeout_Set");
    }

    m_peak_I2C_AckPolling_Timeout_Get = (dyn_peak_I2C_AckPolling_Timeout_Get) (load ?  import_function(m_handle, "peak_I2C_AckPolling_Timeout_Get") : nullptr);
    if(m_peak_I2C_AckPolling_Timeout_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_AckPolling_Timeout_Get");
    }

    m_peak_I2C_Data_Write = (dyn_peak_I2C_Data_Write) (load ?  import_function(m_handle, "peak_I2C_Data_Write") : nullptr);
    if(m_peak_I2C_Data_Write == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_Data_Write");
    }

    m_peak_I2C_Data_Read = (dyn_peak_I2C_Data_Read) (load ?  import_function(m_handle, "peak_I2C_Data_Read") : nullptr);
    if(m_peak_I2C_Data_Read == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_Data_Read");
    }

    m_peak_I2C_Data_MaxSize_Get = (dyn_peak_I2C_Data_MaxSize_Get) (load ?  import_function(m_handle, "peak_I2C_Data_MaxSize_Get") : nullptr);
    if(m_peak_I2C_Data_MaxSize_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_Data_MaxSize_Get");
    }

    m_peak_I2C_OperationStatus_Get = (dyn_peak_I2C_OperationStatus_Get) (load ?  import_function(m_handle, "peak_I2C_OperationStatus_Get") : nullptr);
    if(m_peak_I2C_OperationStatus_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_I2C_OperationStatus_Get");
    }

    m_peak_IPL_ImageWriter_Create = (dyn_peak_IPL_ImageWriter_Create) (load ?  import_function(m_handle, "peak_IPL_ImageWriter_Create") : nullptr);
    if(m_peak_IPL_ImageWriter_Create == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ImageWriter_Create");
    }

    m_peak_IPL_ImageWriter_Destroy = (dyn_peak_IPL_ImageWriter_Destroy) (load ?  import_function(m_handle, "peak_IPL_ImageWriter_Destroy") : nullptr);
    if(m_peak_IPL_ImageWriter_Destroy == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ImageWriter_Destroy");
    }

    m_peak_IPL_ImageWriter_Save = (dyn_peak_IPL_ImageWriter_Save) (load ?  import_function(m_handle, "peak_IPL_ImageWriter_Save") : nullptr);
    if(m_peak_IPL_ImageWriter_Save == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ImageWriter_Save");
    }

    m_peak_IPL_ImageWriter_Format_Set = (dyn_peak_IPL_ImageWriter_Format_Set) (load ?  import_function(m_handle, "peak_IPL_ImageWriter_Format_Set") : nullptr);
    if(m_peak_IPL_ImageWriter_Format_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ImageWriter_Format_Set");
    }

    m_peak_IPL_ImageWriter_Format_Get = (dyn_peak_IPL_ImageWriter_Format_Get) (load ?  import_function(m_handle, "peak_IPL_ImageWriter_Format_Get") : nullptr);
    if(m_peak_IPL_ImageWriter_Format_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ImageWriter_Format_Get");
    }

    m_peak_IPL_ImageWriter_Compression_Set = (dyn_peak_IPL_ImageWriter_Compression_Set) (load ?  import_function(m_handle, "peak_IPL_ImageWriter_Compression_Set") : nullptr);
    if(m_peak_IPL_ImageWriter_Compression_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ImageWriter_Compression_Set");
    }

    m_peak_IPL_ImageWriter_Compression_Get = (dyn_peak_IPL_ImageWriter_Compression_Get) (load ?  import_function(m_handle, "peak_IPL_ImageWriter_Compression_Get") : nullptr);
    if(m_peak_IPL_ImageWriter_Compression_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_ImageWriter_Compression_Get");
    }

    m_peak_IPL_Binning_FactorX_GetList = (dyn_peak_IPL_Binning_FactorX_GetList) (load ?  import_function(m_handle, "peak_IPL_Binning_FactorX_GetList") : nullptr);
    if(m_peak_IPL_Binning_FactorX_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Binning_FactorX_GetList");
    }

    m_peak_IPL_Binning_FactorY_GetList = (dyn_peak_IPL_Binning_FactorY_GetList) (load ?  import_function(m_handle, "peak_IPL_Binning_FactorY_GetList") : nullptr);
    if(m_peak_IPL_Binning_FactorY_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Binning_FactorY_GetList");
    }

    m_peak_IPL_Binning_Set = (dyn_peak_IPL_Binning_Set) (load ?  import_function(m_handle, "peak_IPL_Binning_Set") : nullptr);
    if(m_peak_IPL_Binning_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Binning_Set");
    }

    m_peak_IPL_Binning_Get = (dyn_peak_IPL_Binning_Get) (load ?  import_function(m_handle, "peak_IPL_Binning_Get") : nullptr);
    if(m_peak_IPL_Binning_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Binning_Get");
    }

    m_peak_IPL_Decimation_FactorX_GetList = (dyn_peak_IPL_Decimation_FactorX_GetList) (load ?  import_function(m_handle, "peak_IPL_Decimation_FactorX_GetList") : nullptr);
    if(m_peak_IPL_Decimation_FactorX_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Decimation_FactorX_GetList");
    }

    m_peak_IPL_Decimation_FactorY_GetList = (dyn_peak_IPL_Decimation_FactorY_GetList) (load ?  import_function(m_handle, "peak_IPL_Decimation_FactorY_GetList") : nullptr);
    if(m_peak_IPL_Decimation_FactorY_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Decimation_FactorY_GetList");
    }

    m_peak_IPL_Decimation_Set = (dyn_peak_IPL_Decimation_Set) (load ?  import_function(m_handle, "peak_IPL_Decimation_Set") : nullptr);
    if(m_peak_IPL_Decimation_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Decimation_Set");
    }

    m_peak_IPL_Decimation_Get = (dyn_peak_IPL_Decimation_Get) (load ?  import_function(m_handle, "peak_IPL_Decimation_Get") : nullptr);
    if(m_peak_IPL_Decimation_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_Decimation_Get");
    }

    m_peak_FirmwareUpdate_CompatibleCameraList_Get = (dyn_peak_FirmwareUpdate_CompatibleCameraList_Get) (load ?  import_function(m_handle, "peak_FirmwareUpdate_CompatibleCameraList_Get") : nullptr);
    if(m_peak_FirmwareUpdate_CompatibleCameraList_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_FirmwareUpdate_CompatibleCameraList_Get");
    }

    m_peak_FirmwareUpdate_Execute = (dyn_peak_FirmwareUpdate_Execute) (load ?  import_function(m_handle, "peak_FirmwareUpdate_Execute") : nullptr);
    if(m_peak_FirmwareUpdate_Execute == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_FirmwareUpdate_Execute");
    }

    m_peak_TestPattern_GetAccessStatus = (dyn_peak_TestPattern_GetAccessStatus) (load ?  import_function(m_handle, "peak_TestPattern_GetAccessStatus") : nullptr);
    if(m_peak_TestPattern_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_TestPattern_GetAccessStatus");
    }

    m_peak_TestPattern_Set = (dyn_peak_TestPattern_Set) (load ?  import_function(m_handle, "peak_TestPattern_Set") : nullptr);
    if(m_peak_TestPattern_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_TestPattern_Set");
    }

    m_peak_TestPattern_Get = (dyn_peak_TestPattern_Get) (load ?  import_function(m_handle, "peak_TestPattern_Get") : nullptr);
    if(m_peak_TestPattern_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_TestPattern_Get");
    }

    m_peak_TestPattern_GetList = (dyn_peak_TestPattern_GetList) (load ?  import_function(m_handle, "peak_TestPattern_GetList") : nullptr);
    if(m_peak_TestPattern_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_TestPattern_GetList");
    }

    m_peak_LED_GetAccessStatus = (dyn_peak_LED_GetAccessStatus) (load ?  import_function(m_handle, "peak_LED_GetAccessStatus") : nullptr);
    if(m_peak_LED_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_LED_GetAccessStatus");
    }

    m_peak_LED_Target_GetList = (dyn_peak_LED_Target_GetList) (load ?  import_function(m_handle, "peak_LED_Target_GetList") : nullptr);
    if(m_peak_LED_Target_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_LED_Target_GetList");
    }

    m_peak_LED_Mode_GetList = (dyn_peak_LED_Mode_GetList) (load ?  import_function(m_handle, "peak_LED_Mode_GetList") : nullptr);
    if(m_peak_LED_Mode_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_LED_Mode_GetList");
    }

    m_peak_LED_Set = (dyn_peak_LED_Set) (load ?  import_function(m_handle, "peak_LED_Set") : nullptr);
    if(m_peak_LED_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_LED_Set");
    }

    m_peak_LED_Get = (dyn_peak_LED_Get) (load ?  import_function(m_handle, "peak_LED_Get") : nullptr);
    if(m_peak_LED_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_LED_Get");
    }

    m_peak_BlackLevel_Auto_GetAccessStatus = (dyn_peak_BlackLevel_Auto_GetAccessStatus) (load ?  import_function(m_handle, "peak_BlackLevel_Auto_GetAccessStatus") : nullptr);
    if(m_peak_BlackLevel_Auto_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_BlackLevel_Auto_GetAccessStatus");
    }

    m_peak_BlackLevel_Auto_Enable = (dyn_peak_BlackLevel_Auto_Enable) (load ?  import_function(m_handle, "peak_BlackLevel_Auto_Enable") : nullptr);
    if(m_peak_BlackLevel_Auto_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_BlackLevel_Auto_Enable");
    }

    m_peak_BlackLevel_Auto_IsEnabled = (dyn_peak_BlackLevel_Auto_IsEnabled) (load ?  import_function(m_handle, "peak_BlackLevel_Auto_IsEnabled") : nullptr);
    if(m_peak_BlackLevel_Auto_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_BlackLevel_Auto_IsEnabled");
    }

    m_peak_BlackLevel_Offset_GetAccessStatus = (dyn_peak_BlackLevel_Offset_GetAccessStatus) (load ?  import_function(m_handle, "peak_BlackLevel_Offset_GetAccessStatus") : nullptr);
    if(m_peak_BlackLevel_Offset_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_BlackLevel_Offset_GetAccessStatus");
    }

    m_peak_BlackLevel_Offset_Set = (dyn_peak_BlackLevel_Offset_Set) (load ?  import_function(m_handle, "peak_BlackLevel_Offset_Set") : nullptr);
    if(m_peak_BlackLevel_Offset_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_BlackLevel_Offset_Set");
    }

    m_peak_BlackLevel_Offset_Get = (dyn_peak_BlackLevel_Offset_Get) (load ?  import_function(m_handle, "peak_BlackLevel_Offset_Get") : nullptr);
    if(m_peak_BlackLevel_Offset_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_BlackLevel_Offset_Get");
    }

    m_peak_BlackLevel_Offset_GetRange = (dyn_peak_BlackLevel_Offset_GetRange) (load ?  import_function(m_handle, "peak_BlackLevel_Offset_GetRange") : nullptr);
    if(m_peak_BlackLevel_Offset_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_BlackLevel_Offset_GetRange");
    }

    m_peak_Bandwidth_LinkSpeed_GetAccessStatus = (dyn_peak_Bandwidth_LinkSpeed_GetAccessStatus) (load ?  import_function(m_handle, "peak_Bandwidth_LinkSpeed_GetAccessStatus") : nullptr);
    if(m_peak_Bandwidth_LinkSpeed_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Bandwidth_LinkSpeed_GetAccessStatus");
    }

    m_peak_Bandwidth_LinkSpeed_Get = (dyn_peak_Bandwidth_LinkSpeed_Get) (load ?  import_function(m_handle, "peak_Bandwidth_LinkSpeed_Get") : nullptr);
    if(m_peak_Bandwidth_LinkSpeed_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Bandwidth_LinkSpeed_Get");
    }

    m_peak_Bandwidth_ThroughputLimit_GetAccessStatus = (dyn_peak_Bandwidth_ThroughputLimit_GetAccessStatus) (load ?  import_function(m_handle, "peak_Bandwidth_ThroughputLimit_GetAccessStatus") : nullptr);
    if(m_peak_Bandwidth_ThroughputLimit_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Bandwidth_ThroughputLimit_GetAccessStatus");
    }

    m_peak_Bandwidth_ThroughputLimit_GetRange = (dyn_peak_Bandwidth_ThroughputLimit_GetRange) (load ?  import_function(m_handle, "peak_Bandwidth_ThroughputLimit_GetRange") : nullptr);
    if(m_peak_Bandwidth_ThroughputLimit_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Bandwidth_ThroughputLimit_GetRange");
    }

    m_peak_Bandwidth_ThroughputLimit_Set = (dyn_peak_Bandwidth_ThroughputLimit_Set) (load ?  import_function(m_handle, "peak_Bandwidth_ThroughputLimit_Set") : nullptr);
    if(m_peak_Bandwidth_ThroughputLimit_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Bandwidth_ThroughputLimit_Set");
    }

    m_peak_Bandwidth_ThroughputLimit_Get = (dyn_peak_Bandwidth_ThroughputLimit_Get) (load ?  import_function(m_handle, "peak_Bandwidth_ThroughputLimit_Get") : nullptr);
    if(m_peak_Bandwidth_ThroughputLimit_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Bandwidth_ThroughputLimit_Get");
    }

    m_peak_Bandwidth_ThroughputFrameRateLimit_GetAccessStatus = (dyn_peak_Bandwidth_ThroughputFrameRateLimit_GetAccessStatus) (load ?  import_function(m_handle, "peak_Bandwidth_ThroughputFrameRateLimit_GetAccessStatus") : nullptr);
    if(m_peak_Bandwidth_ThroughputFrameRateLimit_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Bandwidth_ThroughputFrameRateLimit_GetAccessStatus");
    }

    m_peak_Bandwidth_ThroughputFrameRateLimit_Get = (dyn_peak_Bandwidth_ThroughputFrameRateLimit_Get) (load ?  import_function(m_handle, "peak_Bandwidth_ThroughputFrameRateLimit_Get") : nullptr);
    if(m_peak_Bandwidth_ThroughputFrameRateLimit_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Bandwidth_ThroughputFrameRateLimit_Get");
    }

    m_peak_Bandwidth_ThroughputCalculated_GetAccessStatus = (dyn_peak_Bandwidth_ThroughputCalculated_GetAccessStatus) (load ?  import_function(m_handle, "peak_Bandwidth_ThroughputCalculated_GetAccessStatus") : nullptr);
    if(m_peak_Bandwidth_ThroughputCalculated_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Bandwidth_ThroughputCalculated_GetAccessStatus");
    }

    m_peak_Bandwidth_ThroughputCalculated_Get = (dyn_peak_Bandwidth_ThroughputCalculated_Get) (load ?  import_function(m_handle, "peak_Bandwidth_ThroughputCalculated_Get") : nullptr);
    if(m_peak_Bandwidth_ThroughputCalculated_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Bandwidth_ThroughputCalculated_Get");
    }

    m_peak_IPO_GetAccessStatus = (dyn_peak_IPO_GetAccessStatus) (load ?  import_function(m_handle, "peak_IPO_GetAccessStatus") : nullptr);
    if(m_peak_IPO_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPO_GetAccessStatus");
    }

    m_peak_IPO_IsEnabled = (dyn_peak_IPO_IsEnabled) (load ?  import_function(m_handle, "peak_IPO_IsEnabled") : nullptr);
    if(m_peak_IPO_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPO_IsEnabled");
    }

    m_peak_IPO_Enable = (dyn_peak_IPO_Enable) (load ?  import_function(m_handle, "peak_IPO_Enable") : nullptr);
    if(m_peak_IPO_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPO_Enable");
    }

    m_peak_IPL_DigitalBlack_GetRange = (dyn_peak_IPL_DigitalBlack_GetRange) (load ?  import_function(m_handle, "peak_IPL_DigitalBlack_GetRange") : nullptr);
    if(m_peak_IPL_DigitalBlack_GetRange == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_DigitalBlack_GetRange");
    }

    m_peak_IPL_DigitalBlack_Set = (dyn_peak_IPL_DigitalBlack_Set) (load ?  import_function(m_handle, "peak_IPL_DigitalBlack_Set") : nullptr);
    if(m_peak_IPL_DigitalBlack_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_DigitalBlack_Set");
    }

    m_peak_IPL_DigitalBlack_Get = (dyn_peak_IPL_DigitalBlack_Get) (load ?  import_function(m_handle, "peak_IPL_DigitalBlack_Get") : nullptr);
    if(m_peak_IPL_DigitalBlack_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_DigitalBlack_Get");
    }

    m_peak_IPL_LUT_Enable = (dyn_peak_IPL_LUT_Enable) (load ?  import_function(m_handle, "peak_IPL_LUT_Enable") : nullptr);
    if(m_peak_IPL_LUT_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_LUT_Enable");
    }

    m_peak_IPL_LUT_IsEnabled = (dyn_peak_IPL_LUT_IsEnabled) (load ?  import_function(m_handle, "peak_IPL_LUT_IsEnabled") : nullptr);
    if(m_peak_IPL_LUT_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_LUT_IsEnabled");
    }

    m_peak_IPL_LUT_Preset_Set = (dyn_peak_IPL_LUT_Preset_Set) (load ?  import_function(m_handle, "peak_IPL_LUT_Preset_Set") : nullptr);
    if(m_peak_IPL_LUT_Preset_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_LUT_Preset_Set");
    }

    m_peak_IPL_LUT_Value_Set = (dyn_peak_IPL_LUT_Value_Set) (load ?  import_function(m_handle, "peak_IPL_LUT_Value_Set") : nullptr);
    if(m_peak_IPL_LUT_Value_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_LUT_Value_Set");
    }

    m_peak_IPL_LUT_Value_Get = (dyn_peak_IPL_LUT_Value_Get) (load ?  import_function(m_handle, "peak_IPL_LUT_Value_Get") : nullptr);
    if(m_peak_IPL_LUT_Value_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_LUT_Value_Get");
    }

    m_peak_IPL_LUT_ValueList_Set = (dyn_peak_IPL_LUT_ValueList_Set) (load ?  import_function(m_handle, "peak_IPL_LUT_ValueList_Set") : nullptr);
    if(m_peak_IPL_LUT_ValueList_Set == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_LUT_ValueList_Set");
    }

    m_peak_IPL_LUT_ValueList_Get = (dyn_peak_IPL_LUT_ValueList_Get) (load ?  import_function(m_handle, "peak_IPL_LUT_ValueList_Get") : nullptr);
    if(m_peak_IPL_LUT_ValueList_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_IPL_LUT_ValueList_Get");
    }

    m_peak_Chunks_GetAccessStatus = (dyn_peak_Chunks_GetAccessStatus) (load ?  import_function(m_handle, "peak_Chunks_GetAccessStatus") : nullptr);
    if(m_peak_Chunks_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_GetAccessStatus");
    }

    m_peak_Chunks_Enable = (dyn_peak_Chunks_Enable) (load ?  import_function(m_handle, "peak_Chunks_Enable") : nullptr);
    if(m_peak_Chunks_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_Enable");
    }

    m_peak_Chunks_IsEnabled = (dyn_peak_Chunks_IsEnabled) (load ?  import_function(m_handle, "peak_Chunks_IsEnabled") : nullptr);
    if(m_peak_Chunks_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_IsEnabled");
    }

    m_peak_Chunks_AutoUpdate_Enable = (dyn_peak_Chunks_AutoUpdate_Enable) (load ?  import_function(m_handle, "peak_Chunks_AutoUpdate_Enable") : nullptr);
    if(m_peak_Chunks_AutoUpdate_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_AutoUpdate_Enable");
    }

    m_peak_Chunks_AutoUpdate_IsEnabled = (dyn_peak_Chunks_AutoUpdate_IsEnabled) (load ?  import_function(m_handle, "peak_Chunks_AutoUpdate_IsEnabled") : nullptr);
    if(m_peak_Chunks_AutoUpdate_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_AutoUpdate_IsEnabled");
    }

    m_peak_Chunks_Update = (dyn_peak_Chunks_Update) (load ?  import_function(m_handle, "peak_Chunks_Update") : nullptr);
    if(m_peak_Chunks_Update == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_Update");
    }

    m_peak_Chunks_Type_GetAccessStatus = (dyn_peak_Chunks_Type_GetAccessStatus) (load ?  import_function(m_handle, "peak_Chunks_Type_GetAccessStatus") : nullptr);
    if(m_peak_Chunks_Type_GetAccessStatus == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_Type_GetAccessStatus");
    }

    m_peak_Chunks_Type_Enable = (dyn_peak_Chunks_Type_Enable) (load ?  import_function(m_handle, "peak_Chunks_Type_Enable") : nullptr);
    if(m_peak_Chunks_Type_Enable == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_Type_Enable");
    }

    m_peak_Chunks_Type_IsEnabled = (dyn_peak_Chunks_Type_IsEnabled) (load ?  import_function(m_handle, "peak_Chunks_Type_IsEnabled") : nullptr);
    if(m_peak_Chunks_Type_IsEnabled == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_Type_IsEnabled");
    }

    m_peak_Chunks_Type_Supported_GetList = (dyn_peak_Chunks_Type_Supported_GetList) (load ?  import_function(m_handle, "peak_Chunks_Type_Supported_GetList") : nullptr);
    if(m_peak_Chunks_Type_Supported_GetList == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_Type_Supported_GetList");
    }

    m_peak_Chunks_FrameInfo_Get = (dyn_peak_Chunks_FrameInfo_Get) (load ?  import_function(m_handle, "peak_Chunks_FrameInfo_Get") : nullptr);
    if(m_peak_Chunks_FrameInfo_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_FrameInfo_Get");
    }

    m_peak_Chunks_Exposure_Get = (dyn_peak_Chunks_Exposure_Get) (load ?  import_function(m_handle, "peak_Chunks_Exposure_Get") : nullptr);
    if(m_peak_Chunks_Exposure_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_Exposure_Get");
    }

    m_peak_Chunks_Gain_Get = (dyn_peak_Chunks_Gain_Get) (load ?  import_function(m_handle, "peak_Chunks_Gain_Get") : nullptr);
    if(m_peak_Chunks_Gain_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_Gain_Get");
    }

    m_peak_Chunks_Sequencer_Get = (dyn_peak_Chunks_Sequencer_Get) (load ?  import_function(m_handle, "peak_Chunks_Sequencer_Get") : nullptr);
    if(m_peak_Chunks_Sequencer_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_Sequencer_Get");
    }

    m_peak_Chunks_Timestamp_Get = (dyn_peak_Chunks_Timestamp_Get) (load ?  import_function(m_handle, "peak_Chunks_Timestamp_Get") : nullptr);
    if(m_peak_Chunks_Timestamp_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_Timestamp_Get");
    }

    m_peak_Chunks_ExposureTrigger_Get = (dyn_peak_Chunks_ExposureTrigger_Get) (load ?  import_function(m_handle, "peak_Chunks_ExposureTrigger_Get") : nullptr);
    if(m_peak_Chunks_ExposureTrigger_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_ExposureTrigger_Get");
    }

    m_peak_Chunks_UsableROI_Get = (dyn_peak_Chunks_UsableROI_Get) (load ?  import_function(m_handle, "peak_Chunks_UsableROI_Get") : nullptr);
    if(m_peak_Chunks_UsableROI_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_UsableROI_Get");
    }

    m_peak_Chunks_LineStatus_Get = (dyn_peak_Chunks_LineStatus_Get) (load ?  import_function(m_handle, "peak_Chunks_LineStatus_Get") : nullptr);
    if(m_peak_Chunks_LineStatus_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_LineStatus_Get");
    }

    m_peak_Chunks_AutoFeature_Get = (dyn_peak_Chunks_AutoFeature_Get) (load ?  import_function(m_handle, "peak_Chunks_AutoFeature_Get") : nullptr);
    if(m_peak_Chunks_AutoFeature_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_AutoFeature_Get");
    }

    m_peak_Chunks_PTPStatus_Get = (dyn_peak_Chunks_PTPStatus_Get) (load ?  import_function(m_handle, "peak_Chunks_PTPStatus_Get") : nullptr);
    if(m_peak_Chunks_PTPStatus_Get == nullptr && load)
    {
        throw std::runtime_error("Failed to load peak_Chunks_PTPStatus_Get");
    }


            return true;
}

inline PEAK_API_STATUS DynamicLoader::peak_Library_Init()
{
    auto& inst = instance();
    if(inst.m_peak_Library_Init)
    {
        return inst.m_peak_Library_Init();
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Library_Exit()
{
    auto& inst = instance();
    if(inst.m_peak_Library_Exit)
    {
        return inst.m_peak_Library_Exit();
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Library_GetVersion(uint32_t * majorVersion, uint32_t * minorVersion, uint32_t * subminorVersion, uint32_t * patchVersion)
{
    auto& inst = instance();
    if(inst.m_peak_Library_GetVersion)
    {
        return inst.m_peak_Library_GetVersion(majorVersion, minorVersion, subminorVersion, patchVersion);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Library_GetLastError(peak_status * lastErrorCode, char * lastErrorMessage, size_t * lastErrorMessageSize)
{
    auto& inst = instance();
    if(inst.m_peak_Library_GetLastError)
    {
        return inst.m_peak_Library_GetLastError(lastErrorCode, lastErrorMessage, lastErrorMessageSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_Library_InterfaceTechnology_IsSupported(peak_interface_technology interfaceTech)
{
    auto& inst = instance();
    if(inst.m_peak_Library_InterfaceTechnology_IsSupported)
    {
        return inst.m_peak_Library_InterfaceTechnology_IsSupported(interfaceTech);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Reconnect_Enable(peak_interface_technology interfaceTech, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_Reconnect_Enable)
    {
        return inst.m_peak_Reconnect_Enable(interfaceTech, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_Reconnect_IsEnabled(peak_interface_technology interfaceTech)
{
    auto& inst = instance();
    if(inst.m_peak_Reconnect_IsEnabled)
    {
        return inst.m_peak_Reconnect_IsEnabled(interfaceTech);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Reconnect_GetAccessStatus(peak_interface_technology interfaceTech)
{
    auto& inst = instance();
    if(inst.m_peak_Reconnect_GetAccessStatus)
    {
        return inst.m_peak_Reconnect_GetAccessStatus(interfaceTech);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_CameraList_Update(size_t * cameraCount)
{
    auto& inst = instance();
    if(inst.m_peak_CameraList_Update)
    {
        return inst.m_peak_CameraList_Update(cameraCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_CameraList_Get(peak_camera_descriptor * cameraList, size_t * cameraCount)
{
    auto& inst = instance();
    if(inst.m_peak_CameraList_Get)
    {
        return inst.m_peak_CameraList_Get(cameraList, cameraCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_CAMERA_ID DynamicLoader::peak_Camera_ID_FromHandle(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Camera_ID_FromHandle)
    {
        return inst.m_peak_Camera_ID_FromHandle(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_CAMERA_ID DynamicLoader::peak_Camera_ID_FromSerialNumber(const char * serialNumber)
{
    auto& inst = instance();
    if(inst.m_peak_Camera_ID_FromSerialNumber)
    {
        return inst.m_peak_Camera_ID_FromSerialNumber(serialNumber);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_CAMERA_ID DynamicLoader::peak_Camera_ID_FromUserDefinedName(const char * userDefinedName)
{
    auto& inst = instance();
    if(inst.m_peak_Camera_ID_FromUserDefinedName)
    {
        return inst.m_peak_Camera_ID_FromUserDefinedName(userDefinedName);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_CAMERA_ID DynamicLoader::peak_Camera_ID_FromMAC(peak_mac_address macAddress)
{
    auto& inst = instance();
    if(inst.m_peak_Camera_ID_FromMAC)
    {
        return inst.m_peak_Camera_ID_FromMAC(macAddress);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Camera_GetAccessStatus(peak_camera_id cameraID)
{
    auto& inst = instance();
    if(inst.m_peak_Camera_GetAccessStatus)
    {
        return inst.m_peak_Camera_GetAccessStatus(cameraID);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Camera_GetDescriptor(peak_camera_id cameraID, peak_camera_descriptor * cameraDescriptor)
{
    auto& inst = instance();
    if(inst.m_peak_Camera_GetDescriptor)
    {
        return inst.m_peak_Camera_GetDescriptor(cameraID, cameraDescriptor);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Camera_Open(peak_camera_id cameraID, peak_camera_handle * hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Camera_Open)
    {
        return inst.m_peak_Camera_Open(cameraID, hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Camera_OpenFirstAvailable(peak_camera_handle * hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Camera_OpenFirstAvailable)
    {
        return inst.m_peak_Camera_OpenFirstAvailable(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Camera_Close(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Camera_Close)
    {
        return inst.m_peak_Camera_Close(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Camera_ResetToDefaultSettings(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Camera_ResetToDefaultSettings)
    {
        return inst.m_peak_Camera_ResetToDefaultSettings(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Camera_UserDefinedName_Set(peak_camera_handle hCam, const char * userDefinedName)
{
    auto& inst = instance();
    if(inst.m_peak_Camera_UserDefinedName_Set)
    {
        return inst.m_peak_Camera_UserDefinedName_Set(hCam, userDefinedName);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Camera_UserDefinedName_Get(peak_camera_handle hCam, char * userDefinedName, size_t * userDefinedNameSize)
{
    auto& inst = instance();
    if(inst.m_peak_Camera_UserDefinedName_Get)
    {
        return inst.m_peak_Camera_UserDefinedName_Get(hCam, userDefinedName, userDefinedNameSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_Camera_IsConnected(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Camera_IsConnected)
    {
        return inst.m_peak_Camera_IsConnected(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Camera_Reboot(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Camera_Reboot)
    {
        return inst.m_peak_Camera_Reboot(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_EthernetConfig_GetAccessStatus(peak_camera_id cameraID)
{
    auto& inst = instance();
    if(inst.m_peak_EthernetConfig_GetAccessStatus)
    {
        return inst.m_peak_EthernetConfig_GetAccessStatus(cameraID);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_EthernetConfig_GetInfo(peak_camera_id cameraID, peak_ethernet_info * ethernetInfo)
{
    auto& inst = instance();
    if(inst.m_peak_EthernetConfig_GetInfo)
    {
        return inst.m_peak_EthernetConfig_GetInfo(cameraID, ethernetInfo);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_EthernetConfig_DHCP_GetAccessStatus(peak_camera_id cameraID)
{
    auto& inst = instance();
    if(inst.m_peak_EthernetConfig_DHCP_GetAccessStatus)
    {
        return inst.m_peak_EthernetConfig_DHCP_GetAccessStatus(cameraID);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_EthernetConfig_DHCP_Enable(peak_camera_id cameraID, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_EthernetConfig_DHCP_Enable)
    {
        return inst.m_peak_EthernetConfig_DHCP_Enable(cameraID, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_EthernetConfig_DHCP_IsEnabled(peak_camera_id cameraID)
{
    auto& inst = instance();
    if(inst.m_peak_EthernetConfig_DHCP_IsEnabled)
    {
        return inst.m_peak_EthernetConfig_DHCP_IsEnabled(cameraID);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_EthernetConfig_PersistentIP_GetAccessStatus(peak_camera_id cameraID)
{
    auto& inst = instance();
    if(inst.m_peak_EthernetConfig_PersistentIP_GetAccessStatus)
    {
        return inst.m_peak_EthernetConfig_PersistentIP_GetAccessStatus(cameraID);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_EthernetConfig_PersistentIP_Set(peak_camera_id cameraID, peak_ip_config persistentIP)
{
    auto& inst = instance();
    if(inst.m_peak_EthernetConfig_PersistentIP_Set)
    {
        return inst.m_peak_EthernetConfig_PersistentIP_Set(cameraID, persistentIP);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_EthernetConfig_PersistentIP_Get(peak_camera_id cameraID, peak_ip_config * persistentIP)
{
    auto& inst = instance();
    if(inst.m_peak_EthernetConfig_PersistentIP_Get)
    {
        return inst.m_peak_EthernetConfig_PersistentIP_Get(cameraID, persistentIP);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_Start(peak_camera_handle hCam, uint32_t numberOfFrames)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_Start)
    {
        return inst.m_peak_Acquisition_Start(hCam, numberOfFrames);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_Stop(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_Stop)
    {
        return inst.m_peak_Acquisition_Stop(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_Acquisition_IsStarted(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_IsStarted)
    {
        return inst.m_peak_Acquisition_IsStarted(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_WaitForFrame(peak_camera_handle hCam, uint32_t timeout_ms, peak_frame_handle * hFrame)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_WaitForFrame)
    {
        return inst.m_peak_Acquisition_WaitForFrame(hCam, timeout_ms, hFrame);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_GetInfo(peak_camera_handle hCam, peak_acquisition_info * acquisitionInfo)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_GetInfo)
    {
        return inst.m_peak_Acquisition_GetInfo(hCam, acquisitionInfo);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_Buffer_GetRequiredSize(peak_camera_handle hCam, size_t * requiredBufferSize)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_Buffer_GetRequiredSize)
    {
        return inst.m_peak_Acquisition_Buffer_GetRequiredSize(hCam, requiredBufferSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_Buffer_GetRequiredCount(peak_camera_handle hCam, size_t * requiredBufferCount)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_Buffer_GetRequiredCount)
    {
        return inst.m_peak_Acquisition_Buffer_GetRequiredCount(hCam, requiredBufferCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_Buffer_Announce(peak_camera_handle hCam, uint8_t * memoryAddress, size_t memorySize, void * userContext)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_Buffer_Announce)
    {
        return inst.m_peak_Acquisition_Buffer_Announce(hCam, memoryAddress, memorySize, userContext);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_Buffer_Revoke(peak_camera_handle hCam, uint8_t * memoryAddress)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_Buffer_Revoke)
    {
        return inst.m_peak_Acquisition_Buffer_Revoke(hCam, memoryAddress);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_Buffer_RevokeAll(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_Buffer_RevokeAll)
    {
        return inst.m_peak_Acquisition_Buffer_RevokeAll(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Acquisition_BufferHandling_Mode_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_BufferHandling_Mode_GetAccessStatus)
    {
        return inst.m_peak_Acquisition_BufferHandling_Mode_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_BufferHandling_Mode_GetList(peak_camera_handle hCam, peak_buffer_handling_mode * bufferHandlingModeList, size_t * bufferHandlingModesCount)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_BufferHandling_Mode_GetList)
    {
        return inst.m_peak_Acquisition_BufferHandling_Mode_GetList(hCam, bufferHandlingModeList, bufferHandlingModesCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_BufferHandling_Mode_Set(peak_camera_handle hCam, peak_buffer_handling_mode mode)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_BufferHandling_Mode_Set)
    {
        return inst.m_peak_Acquisition_BufferHandling_Mode_Set(hCam, mode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_BufferHandling_Mode_Get(peak_camera_handle hCam, peak_buffer_handling_mode * mode)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_BufferHandling_Mode_Get)
    {
        return inst.m_peak_Acquisition_BufferHandling_Mode_Get(hCam, mode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Acquisition_LossHandling_Mode_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_Mode_GetAccessStatus)
    {
        return inst.m_peak_Acquisition_LossHandling_Mode_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_LossHandling_Mode_GetList(peak_camera_handle hCam, peak_loss_handling_mode * lossHandlingModeList, size_t * lossHandlingModesCount)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_Mode_GetList)
    {
        return inst.m_peak_Acquisition_LossHandling_Mode_GetList(hCam, lossHandlingModeList, lossHandlingModesCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_LossHandling_Mode_Set(peak_camera_handle hCam, peak_loss_handling_mode mode)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_Mode_Set)
    {
        return inst.m_peak_Acquisition_LossHandling_Mode_Set(hCam, mode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_LossHandling_Mode_Get(peak_camera_handle hCam, peak_loss_handling_mode * mode)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_Mode_Get)
    {
        return inst.m_peak_Acquisition_LossHandling_Mode_Get(hCam, mode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Acquisition_LossHandling_Extent_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_Extent_GetAccessStatus)
    {
        return inst.m_peak_Acquisition_LossHandling_Extent_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_LossHandling_Extent_GetRange(peak_camera_handle hCam, int64_t * minExtent, int64_t * maxExtent, int64_t * incExtent)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_Extent_GetRange)
    {
        return inst.m_peak_Acquisition_LossHandling_Extent_GetRange(hCam, minExtent, maxExtent, incExtent);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_LossHandling_Extent_Set(peak_camera_handle hCam, int64_t extent)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_Extent_Set)
    {
        return inst.m_peak_Acquisition_LossHandling_Extent_Set(hCam, extent);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_LossHandling_Extent_Get(peak_camera_handle hCam, int64_t * extent)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_Extent_Get)
    {
        return inst.m_peak_Acquisition_LossHandling_Extent_Get(hCam, extent);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Acquisition_LossHandling_FrameAbortTimeout_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_FrameAbortTimeout_GetAccessStatus)
    {
        return inst.m_peak_Acquisition_LossHandling_FrameAbortTimeout_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_LossHandling_FrameAbortTimeout_GetRange(peak_camera_handle hCam, int64_t * minTimeout, int64_t * maxTimeout, int64_t * incTimeout)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_FrameAbortTimeout_GetRange)
    {
        return inst.m_peak_Acquisition_LossHandling_FrameAbortTimeout_GetRange(hCam, minTimeout, maxTimeout, incTimeout);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_LossHandling_FrameAbortTimeout_Set(peak_camera_handle hCam, int64_t timeout)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_FrameAbortTimeout_Set)
    {
        return inst.m_peak_Acquisition_LossHandling_FrameAbortTimeout_Set(hCam, timeout);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_LossHandling_FrameAbortTimeout_Get(peak_camera_handle hCam, int64_t * timeout)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_FrameAbortTimeout_Get)
    {
        return inst.m_peak_Acquisition_LossHandling_FrameAbortTimeout_Get(hCam, timeout);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Acquisition_LossHandling_ResendRequestTimeout_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_ResendRequestTimeout_GetAccessStatus)
    {
        return inst.m_peak_Acquisition_LossHandling_ResendRequestTimeout_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_LossHandling_ResendRequestTimeout_GetRange(peak_camera_handle hCam, int64_t * minTimeout, int64_t * maxTimeout, int64_t * incTimeout)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_ResendRequestTimeout_GetRange)
    {
        return inst.m_peak_Acquisition_LossHandling_ResendRequestTimeout_GetRange(hCam, minTimeout, maxTimeout, incTimeout);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_LossHandling_ResendRequestTimeout_Set(peak_camera_handle hCam, int64_t timeout)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_ResendRequestTimeout_Set)
    {
        return inst.m_peak_Acquisition_LossHandling_ResendRequestTimeout_Set(hCam, timeout);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Acquisition_LossHandling_ResendRequestTimeout_Get(peak_camera_handle hCam, int64_t * timeout)
{
    auto& inst = instance();
    if(inst.m_peak_Acquisition_LossHandling_ResendRequestTimeout_Get)
    {
        return inst.m_peak_Acquisition_LossHandling_ResendRequestTimeout_Get(hCam, timeout);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_PixelFormat_GetInfo(peak_pixel_format pixelFormat, peak_pixel_format_info * pixelFormatInfo)
{
    auto& inst = instance();
    if(inst.m_peak_PixelFormat_GetInfo)
    {
        return inst.m_peak_PixelFormat_GetInfo(pixelFormat, pixelFormatInfo);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Frame_Release(peak_camera_handle hCam, peak_frame_handle hFrame)
{
    auto& inst = instance();
    if(inst.m_peak_Frame_Release)
    {
        return inst.m_peak_Frame_Release(hCam, hFrame);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Frame_GetInfo(peak_frame_handle hFrame, peak_frame_info * frameInfo)
{
    auto& inst = instance();
    if(inst.m_peak_Frame_GetInfo)
    {
        return inst.m_peak_Frame_GetInfo(hFrame, frameInfo);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Frame_Type_Get(peak_frame_handle hFrame, peak_frame_type * frameType)
{
    auto& inst = instance();
    if(inst.m_peak_Frame_Type_Get)
    {
        return inst.m_peak_Frame_Type_Get(hFrame, frameType);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Frame_Buffer_Get(peak_frame_handle hFrame, peak_buffer * buffer)
{
    auto& inst = instance();
    if(inst.m_peak_Frame_Buffer_Get)
    {
        return inst.m_peak_Frame_Buffer_Get(hFrame, buffer);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Frame_ID_Get(peak_frame_handle hFrame, uint64_t * frameID)
{
    auto& inst = instance();
    if(inst.m_peak_Frame_ID_Get)
    {
        return inst.m_peak_Frame_ID_Get(hFrame, frameID);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Frame_Timestamp_Get(peak_frame_handle hFrame, uint64_t * timestamp_ns)
{
    auto& inst = instance();
    if(inst.m_peak_Frame_Timestamp_Get)
    {
        return inst.m_peak_Frame_Timestamp_Get(hFrame, timestamp_ns);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Frame_ROI_Get(peak_frame_handle hFrame, peak_roi * roi)
{
    auto& inst = instance();
    if(inst.m_peak_Frame_ROI_Get)
    {
        return inst.m_peak_Frame_ROI_Get(hFrame, roi);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Frame_PixelFormat_Get(peak_frame_handle hFrame, peak_pixel_format * pixelFormat)
{
    auto& inst = instance();
    if(inst.m_peak_Frame_PixelFormat_Get)
    {
        return inst.m_peak_Frame_PixelFormat_Get(hFrame, pixelFormat);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_Frame_IsComplete(peak_frame_handle hFrame)
{
    auto& inst = instance();
    if(inst.m_peak_Frame_IsComplete)
    {
        return inst.m_peak_Frame_IsComplete(hFrame);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Frame_BytesExpected_Get(peak_frame_handle hFrame, size_t * bytesExpected)
{
    auto& inst = instance();
    if(inst.m_peak_Frame_BytesExpected_Get)
    {
        return inst.m_peak_Frame_BytesExpected_Get(hFrame, bytesExpected);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Frame_BytesWritten_Get(peak_frame_handle hFrame, size_t * bytesWritten)
{
    auto& inst = instance();
    if(inst.m_peak_Frame_BytesWritten_Get)
    {
        return inst.m_peak_Frame_BytesWritten_Get(hFrame, bytesWritten);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Frame_ProcessingTime_Get(peak_frame_handle hFrame, uint32_t * processingTime_ms)
{
    auto& inst = instance();
    if(inst.m_peak_Frame_ProcessingTime_Get)
    {
        return inst.m_peak_Frame_ProcessingTime_Get(hFrame, processingTime_ms);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Frame_Save(peak_frame_handle hFrame, const char * fileName)
{
    auto& inst = instance();
    if(inst.m_peak_Frame_Save)
    {
        return inst.m_peak_Frame_Save(hFrame, fileName);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_Frame_HasChunks(peak_frame_handle hFrame)
{
    auto& inst = instance();
    if(inst.m_peak_Frame_HasChunks)
    {
        return inst.m_peak_Frame_HasChunks(hFrame);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_CameraSettings_ParameterSet_GetAccessStatus(peak_camera_handle hCam, peak_parameter_set parameterSet)
{
    auto& inst = instance();
    if(inst.m_peak_CameraSettings_ParameterSet_GetAccessStatus)
    {
        return inst.m_peak_CameraSettings_ParameterSet_GetAccessStatus(hCam, parameterSet);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_CameraSettings_ParameterSet_GetList(peak_camera_handle hCam, peak_parameter_set * parameterSetList, size_t * parameterSetCount)
{
    auto& inst = instance();
    if(inst.m_peak_CameraSettings_ParameterSet_GetList)
    {
        return inst.m_peak_CameraSettings_ParameterSet_GetList(hCam, parameterSetList, parameterSetCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_CameraSettings_ParameterSet_Store(peak_camera_handle hCam, peak_parameter_set parameterSet)
{
    auto& inst = instance();
    if(inst.m_peak_CameraSettings_ParameterSet_Store)
    {
        return inst.m_peak_CameraSettings_ParameterSet_Store(hCam, parameterSet);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_CameraSettings_ParameterSet_Apply(peak_camera_handle hCam, peak_parameter_set parameterSet)
{
    auto& inst = instance();
    if(inst.m_peak_CameraSettings_ParameterSet_Apply)
    {
        return inst.m_peak_CameraSettings_ParameterSet_Apply(hCam, parameterSet);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_CameraSettings_ParameterSet_Startup_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_CameraSettings_ParameterSet_Startup_GetAccessStatus)
    {
        return inst.m_peak_CameraSettings_ParameterSet_Startup_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_CameraSettings_ParameterSet_Startup_Set(peak_camera_handle hCam, peak_parameter_set parameterSet)
{
    auto& inst = instance();
    if(inst.m_peak_CameraSettings_ParameterSet_Startup_Set)
    {
        return inst.m_peak_CameraSettings_ParameterSet_Startup_Set(hCam, parameterSet);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_CameraSettings_ParameterSet_Startup_Get(peak_camera_handle hCam, peak_parameter_set * parameterSet)
{
    auto& inst = instance();
    if(inst.m_peak_CameraSettings_ParameterSet_Startup_Get)
    {
        return inst.m_peak_CameraSettings_ParameterSet_Startup_Get(hCam, parameterSet);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_CameraSettings_DiskFile_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_CameraSettings_DiskFile_GetAccessStatus)
    {
        return inst.m_peak_CameraSettings_DiskFile_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_CameraSettings_DiskFile_Store(peak_camera_handle hCam, const char * file)
{
    auto& inst = instance();
    if(inst.m_peak_CameraSettings_DiskFile_Store)
    {
        return inst.m_peak_CameraSettings_DiskFile_Store(hCam, file);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_CameraSettings_DiskFile_Apply(peak_camera_handle hCam, const char * file)
{
    auto& inst = instance();
    if(inst.m_peak_CameraSettings_DiskFile_Apply)
    {
        return inst.m_peak_CameraSettings_DiskFile_Apply(hCam, file);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_FrameRate_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_FrameRate_GetAccessStatus)
    {
        return inst.m_peak_FrameRate_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_FrameRate_GetRange(peak_camera_handle hCam, double * minFrameRate_fps, double * maxFrameRate_fps, double * incFrameRate_fps)
{
    auto& inst = instance();
    if(inst.m_peak_FrameRate_GetRange)
    {
        return inst.m_peak_FrameRate_GetRange(hCam, minFrameRate_fps, maxFrameRate_fps, incFrameRate_fps);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_FrameRate_Set(peak_camera_handle hCam, double frameRate_fps)
{
    auto& inst = instance();
    if(inst.m_peak_FrameRate_Set)
    {
        return inst.m_peak_FrameRate_Set(hCam, frameRate_fps);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_FrameRate_Get(peak_camera_handle hCam, double * frameRate_fps)
{
    auto& inst = instance();
    if(inst.m_peak_FrameRate_Get)
    {
        return inst.m_peak_FrameRate_Get(hCam, frameRate_fps);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_ExposureTime_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_ExposureTime_GetAccessStatus)
    {
        return inst.m_peak_ExposureTime_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ExposureTime_GetRange(peak_camera_handle hCam, double * minExposureTime_us, double * maxExposureTime_us, double * incExposureTime_us)
{
    auto& inst = instance();
    if(inst.m_peak_ExposureTime_GetRange)
    {
        return inst.m_peak_ExposureTime_GetRange(hCam, minExposureTime_us, maxExposureTime_us, incExposureTime_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ExposureTime_Set(peak_camera_handle hCam, double exposureTime_us)
{
    auto& inst = instance();
    if(inst.m_peak_ExposureTime_Set)
    {
        return inst.m_peak_ExposureTime_Set(hCam, exposureTime_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ExposureTime_Get(peak_camera_handle hCam, double * exposureTime_us)
{
    auto& inst = instance();
    if(inst.m_peak_ExposureTime_Get)
    {
        return inst.m_peak_ExposureTime_Get(hCam, exposureTime_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_ShutterMode_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_ShutterMode_GetAccessStatus)
    {
        return inst.m_peak_ShutterMode_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ShutterMode_GetList(peak_camera_handle hCam, peak_shutter_mode * shutterModeList, size_t * shutterModeCount)
{
    auto& inst = instance();
    if(inst.m_peak_ShutterMode_GetList)
    {
        return inst.m_peak_ShutterMode_GetList(hCam, shutterModeList, shutterModeCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ShutterMode_Set(peak_camera_handle hCam, peak_shutter_mode shutterMode)
{
    auto& inst = instance();
    if(inst.m_peak_ShutterMode_Set)
    {
        return inst.m_peak_ShutterMode_Set(hCam, shutterMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ShutterMode_Get(peak_camera_handle hCam, peak_shutter_mode * shutterMode)
{
    auto& inst = instance();
    if(inst.m_peak_ShutterMode_Get)
    {
        return inst.m_peak_ShutterMode_Get(hCam, shutterMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_PixelClock_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_PixelClock_GetAccessStatus)
    {
        return inst.m_peak_PixelClock_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_PixelClock_HasRange(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_PixelClock_HasRange)
    {
        return inst.m_peak_PixelClock_HasRange(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_PixelClock_GetRange(peak_camera_handle hCam, double * minPixelClock_MHz, double * maxPixelClock_MHz, double * incPixelClock_MHz)
{
    auto& inst = instance();
    if(inst.m_peak_PixelClock_GetRange)
    {
        return inst.m_peak_PixelClock_GetRange(hCam, minPixelClock_MHz, maxPixelClock_MHz, incPixelClock_MHz);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_PixelClock_GetList(peak_camera_handle hCam, double * pixelClockList, size_t * pixelClockCount)
{
    auto& inst = instance();
    if(inst.m_peak_PixelClock_GetList)
    {
        return inst.m_peak_PixelClock_GetList(hCam, pixelClockList, pixelClockCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_PixelClock_Set(peak_camera_handle hCam, double pixelClock_MHz)
{
    auto& inst = instance();
    if(inst.m_peak_PixelClock_Set)
    {
        return inst.m_peak_PixelClock_Set(hCam, pixelClock_MHz);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_PixelClock_Get(peak_camera_handle hCam, double * pixelClock_MHz)
{
    auto& inst = instance();
    if(inst.m_peak_PixelClock_Get)
    {
        return inst.m_peak_PixelClock_Get(hCam, pixelClock_MHz);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_IOChannel_GetAccessStatus(peak_camera_handle hCam, peak_io_channel ioChannel)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_GetAccessStatus)
    {
        return inst.m_peak_IOChannel_GetAccessStatus(hCam, ioChannel);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline  DynamicLoader::PEAK_API_STATUS_DEPRECATED("Use peak_IOChannel_GetListForDirection instead") peak_IOChannel_GetList(peak_camera_handlehCam,peak_io_channel*ioChannelList,size_t*ioChannelCount)
{
    auto& inst = instance();
    if(inst.m_PEAK_API_STATUS_DEPRECATED)
    {
        return inst.m_PEAK_API_STATUS_DEPRECATED(peak_IOChannel_GetList(peak_camera_handlehCam,peak_io_channel*ioChannelList,size_t*ioChannelCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IOChannel_GetListForDirection(peak_camera_handle hCam, peak_io_direction direction, peak_io_channel * ioChannelList, size_t * ioChannelCount)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_GetListForDirection)
    {
        return inst.m_peak_IOChannel_GetListForDirection(hCam, direction, ioChannelList, ioChannelCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_IOChannel_Direction_GetAccessStatus(peak_camera_handle hCam, peak_io_channel ioChannel)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_Direction_GetAccessStatus)
    {
        return inst.m_peak_IOChannel_Direction_GetAccessStatus(hCam, ioChannel);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IOChannel_Direction_Get(peak_camera_handle hCam, peak_io_channel ioChannel, peak_io_direction * direction)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_Direction_Get)
    {
        return inst.m_peak_IOChannel_Direction_Get(hCam, ioChannel, direction);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_IOChannel_Type_GetAccessStatus(peak_camera_handle hCam, peak_io_channel ioChannel)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_Type_GetAccessStatus)
    {
        return inst.m_peak_IOChannel_Type_GetAccessStatus(hCam, ioChannel);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IOChannel_Type_Get(peak_camera_handle hCam, peak_io_channel ioChannel, peak_io_type * type)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_Type_Get)
    {
        return inst.m_peak_IOChannel_Type_Get(hCam, ioChannel, type);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_IOChannel_Level_GetAccessStatus(peak_camera_handle hCam, peak_io_channel ioChannel)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_Level_GetAccessStatus)
    {
        return inst.m_peak_IOChannel_Level_GetAccessStatus(hCam, ioChannel);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_IOChannel_Level_IsHigh(peak_camera_handle hCam, peak_io_channel ioChannel)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_Level_IsHigh)
    {
        return inst.m_peak_IOChannel_Level_IsHigh(hCam, ioChannel);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IOChannel_Level_SetHigh(peak_camera_handle hCam, peak_io_channel ioChannel, peak_bool high)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_Level_SetHigh)
    {
        return inst.m_peak_IOChannel_Level_SetHigh(hCam, ioChannel, high);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_IOChannel_Inverter_GetAccessStatus(peak_camera_handle hCam, peak_io_channel ioChannel)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_Inverter_GetAccessStatus)
    {
        return inst.m_peak_IOChannel_Inverter_GetAccessStatus(hCam, ioChannel);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IOChannel_Inverter_Enable(peak_camera_handle hCam, peak_io_channel ioChannel, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_Inverter_Enable)
    {
        return inst.m_peak_IOChannel_Inverter_Enable(hCam, ioChannel, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_IOChannel_Inverter_IsEnabled(peak_camera_handle hCam, peak_io_channel ioChannel)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_Inverter_IsEnabled)
    {
        return inst.m_peak_IOChannel_Inverter_IsEnabled(hCam, ioChannel);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_IOChannel_NoiseFilter_GetAccessStatus(peak_camera_handle hCam, peak_io_channel ioChannel)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_NoiseFilter_GetAccessStatus)
    {
        return inst.m_peak_IOChannel_NoiseFilter_GetAccessStatus(hCam, ioChannel);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IOChannel_NoiseFilter_Enable(peak_camera_handle hCam, peak_io_channel ioChannel, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_NoiseFilter_Enable)
    {
        return inst.m_peak_IOChannel_NoiseFilter_Enable(hCam, ioChannel, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_IOChannel_NoiseFilter_IsEnabled(peak_camera_handle hCam, peak_io_channel ioChannel)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_NoiseFilter_IsEnabled)
    {
        return inst.m_peak_IOChannel_NoiseFilter_IsEnabled(hCam, ioChannel);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IOChannel_NoiseFilter_Duration_GetRange(peak_camera_handle hCam, peak_io_channel ioChannel, double * minNoiseFilterDuration_us, double * maxNoiseFilterDuration_us, double * incNoiseFilterDuration_us)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_NoiseFilter_Duration_GetRange)
    {
        return inst.m_peak_IOChannel_NoiseFilter_Duration_GetRange(hCam, ioChannel, minNoiseFilterDuration_us, maxNoiseFilterDuration_us, incNoiseFilterDuration_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IOChannel_NoiseFilter_Duration_Set(peak_camera_handle hCam, peak_io_channel ioChannel, double noiseFilterDuration_us)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_NoiseFilter_Duration_Set)
    {
        return inst.m_peak_IOChannel_NoiseFilter_Duration_Set(hCam, ioChannel, noiseFilterDuration_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IOChannel_NoiseFilter_Duration_Get(peak_camera_handle hCam, peak_io_channel ioChannel, double * noiseFilterDuration_us)
{
    auto& inst = instance();
    if(inst.m_peak_IOChannel_NoiseFilter_Duration_Get)
    {
        return inst.m_peak_IOChannel_NoiseFilter_Duration_Get(hCam, ioChannel, noiseFilterDuration_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Trigger_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_GetAccessStatus)
    {
        return inst.m_peak_Trigger_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Enable(peak_camera_handle hCam, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Enable)
    {
        return inst.m_peak_Trigger_Enable(hCam, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_Trigger_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_IsEnabled)
    {
        return inst.m_peak_Trigger_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_Trigger_IsExecutable(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_IsExecutable)
    {
        return inst.m_peak_Trigger_IsExecutable(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Execute(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Execute)
    {
        return inst.m_peak_Trigger_Execute(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Trigger_Mode_GetAccessStatus(peak_camera_handle hCam, peak_trigger_mode triggerMode)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Mode_GetAccessStatus)
    {
        return inst.m_peak_Trigger_Mode_GetAccessStatus(hCam, triggerMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Mode_Set(peak_camera_handle hCam, peak_trigger_mode triggerMode)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Mode_Set)
    {
        return inst.m_peak_Trigger_Mode_Set(hCam, triggerMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline  DynamicLoader::PEAK_API_STATUS_DEPRECATED("Use peak_Trigger_Mode_Config_Get instead") peak_Trigger_Mode_Get(peak_camera_handlehCam,peak_trigger_mode*triggerMode)
{
    auto& inst = instance();
    if(inst.m_PEAK_API_STATUS_DEPRECATED)
    {
        return inst.m_PEAK_API_STATUS_DEPRECATED(peak_Trigger_Mode_Get(peak_camera_handlehCam,peak_trigger_mode*triggerMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Mode_Config_Get(peak_camera_handle hCam, peak_trigger_mode * triggerMode)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Mode_Config_Get)
    {
        return inst.m_peak_Trigger_Mode_Config_Get(hCam, triggerMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Trigger_Edge_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Edge_GetAccessStatus)
    {
        return inst.m_peak_Trigger_Edge_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Edge_GetList(peak_camera_handle hCam, peak_trigger_edge * triggerEdgeList, size_t * triggerEdgeCount)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Edge_GetList)
    {
        return inst.m_peak_Trigger_Edge_GetList(hCam, triggerEdgeList, triggerEdgeCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Edge_Set(peak_camera_handle hCam, peak_trigger_edge triggerEdge)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Edge_Set)
    {
        return inst.m_peak_Trigger_Edge_Set(hCam, triggerEdge);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Edge_Get(peak_camera_handle hCam, peak_trigger_edge * triggerEdge)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Edge_Get)
    {
        return inst.m_peak_Trigger_Edge_Get(hCam, triggerEdge);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Trigger_Delay_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Delay_GetAccessStatus)
    {
        return inst.m_peak_Trigger_Delay_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Delay_GetRange(peak_camera_handle hCam, double * minTriggerDelay_us, double * maxTriggerDelay_us, double * incTriggerDelay_us)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Delay_GetRange)
    {
        return inst.m_peak_Trigger_Delay_GetRange(hCam, minTriggerDelay_us, maxTriggerDelay_us, incTriggerDelay_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Delay_Set(peak_camera_handle hCam, double triggerDelay_us)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Delay_Set)
    {
        return inst.m_peak_Trigger_Delay_Set(hCam, triggerDelay_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Delay_Get(peak_camera_handle hCam, double * triggerDelay_us)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Delay_Get)
    {
        return inst.m_peak_Trigger_Delay_Get(hCam, triggerDelay_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Trigger_Divider_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Divider_GetAccessStatus)
    {
        return inst.m_peak_Trigger_Divider_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Divider_GetRange(peak_camera_handle hCam, uint32_t * minTriggerDivider, uint32_t * maxTriggerDivider, uint32_t * incTriggerDivider)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Divider_GetRange)
    {
        return inst.m_peak_Trigger_Divider_GetRange(hCam, minTriggerDivider, maxTriggerDivider, incTriggerDivider);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Divider_Set(peak_camera_handle hCam, uint32_t triggerDivider)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Divider_Set)
    {
        return inst.m_peak_Trigger_Divider_Set(hCam, triggerDivider);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Divider_Get(peak_camera_handle hCam, uint32_t * triggerDivider)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Divider_Get)
    {
        return inst.m_peak_Trigger_Divider_Get(hCam, triggerDivider);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Trigger_Burst_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Burst_GetAccessStatus)
    {
        return inst.m_peak_Trigger_Burst_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Burst_GetRange(peak_camera_handle hCam, uint32_t * minTriggerBurst, uint32_t * maxTriggerBurst, uint32_t * incTriggerBurst)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Burst_GetRange)
    {
        return inst.m_peak_Trigger_Burst_GetRange(hCam, minTriggerBurst, maxTriggerBurst, incTriggerBurst);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Burst_Set(peak_camera_handle hCam, uint32_t triggerBurst)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Burst_Set)
    {
        return inst.m_peak_Trigger_Burst_Set(hCam, triggerBurst);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Trigger_Burst_Get(peak_camera_handle hCam, uint32_t * triggerBurst)
{
    auto& inst = instance();
    if(inst.m_peak_Trigger_Burst_Get)
    {
        return inst.m_peak_Trigger_Burst_Get(hCam, triggerBurst);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Flash_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_GetAccessStatus)
    {
        return inst.m_peak_Flash_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Flash_Enable(peak_camera_handle hCam, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_Enable)
    {
        return inst.m_peak_Flash_Enable(hCam, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_Flash_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_IsEnabled)
    {
        return inst.m_peak_Flash_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Flash_Mode_GetAccessStatus(peak_camera_handle hCam, peak_flash_mode flashMode)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_Mode_GetAccessStatus)
    {
        return inst.m_peak_Flash_Mode_GetAccessStatus(hCam, flashMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Flash_Mode_Set(peak_camera_handle hCam, peak_flash_mode flashMode)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_Mode_Set)
    {
        return inst.m_peak_Flash_Mode_Set(hCam, flashMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline  DynamicLoader::PEAK_API_STATUS_DEPRECATED("Use peak_Flash_Mode_Config_Get instead") peak_Flash_Mode_Get(peak_camera_handlehCam,peak_flash_mode*flashMode)
{
    auto& inst = instance();
    if(inst.m_PEAK_API_STATUS_DEPRECATED)
    {
        return inst.m_PEAK_API_STATUS_DEPRECATED(peak_Flash_Mode_Get(peak_camera_handlehCam,peak_flash_mode*flashMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Flash_Mode_Config_Get(peak_camera_handle hCam, peak_flash_mode * flashMode)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_Mode_Config_Get)
    {
        return inst.m_peak_Flash_Mode_Config_Get(hCam, flashMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Flash_StartDelay_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_StartDelay_GetAccessStatus)
    {
        return inst.m_peak_Flash_StartDelay_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Flash_StartDelay_GetRange(peak_camera_handle hCam, double * minFlashStartDelay_us, double * maxFlashStartDelay_us, double * incFlashStartDelay_us)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_StartDelay_GetRange)
    {
        return inst.m_peak_Flash_StartDelay_GetRange(hCam, minFlashStartDelay_us, maxFlashStartDelay_us, incFlashStartDelay_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Flash_StartDelay_Set(peak_camera_handle hCam, double flashStartDelay_us)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_StartDelay_Set)
    {
        return inst.m_peak_Flash_StartDelay_Set(hCam, flashStartDelay_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Flash_StartDelay_Get(peak_camera_handle hCam, double * flashStartDelay_us)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_StartDelay_Get)
    {
        return inst.m_peak_Flash_StartDelay_Get(hCam, flashStartDelay_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Flash_EndDelay_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_EndDelay_GetAccessStatus)
    {
        return inst.m_peak_Flash_EndDelay_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Flash_EndDelay_GetRange(peak_camera_handle hCam, double * minFlashEndDelay_us, double * maxFlashEndDelay_us, double * incFlashEndDelay_us)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_EndDelay_GetRange)
    {
        return inst.m_peak_Flash_EndDelay_GetRange(hCam, minFlashEndDelay_us, maxFlashEndDelay_us, incFlashEndDelay_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Flash_EndDelay_Set(peak_camera_handle hCam, double flashEndDelay_us)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_EndDelay_Set)
    {
        return inst.m_peak_Flash_EndDelay_Set(hCam, flashEndDelay_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Flash_EndDelay_Get(peak_camera_handle hCam, double * flashEndDelay_us)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_EndDelay_Get)
    {
        return inst.m_peak_Flash_EndDelay_Get(hCam, flashEndDelay_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Flash_Duration_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_Duration_GetAccessStatus)
    {
        return inst.m_peak_Flash_Duration_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Flash_Duration_GetRange(peak_camera_handle hCam, double * minFlashDuration_us, double * maxFlashDuration_us, double * incFlashDuration_us)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_Duration_GetRange)
    {
        return inst.m_peak_Flash_Duration_GetRange(hCam, minFlashDuration_us, maxFlashDuration_us, incFlashDuration_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Flash_Duration_Set(peak_camera_handle hCam, double flashDuration_us)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_Duration_Set)
    {
        return inst.m_peak_Flash_Duration_Set(hCam, flashDuration_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Flash_Duration_Get(peak_camera_handle hCam, double * flashDuration_us)
{
    auto& inst = instance();
    if(inst.m_peak_Flash_Duration_Get)
    {
        return inst.m_peak_Flash_Duration_Get(hCam, flashDuration_us);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Focus_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Focus_GetAccessStatus)
    {
        return inst.m_peak_Focus_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Focus_GetRange(peak_camera_handle hCam, uint32_t * minFocus, uint32_t * maxFocus, uint32_t * incFocus)
{
    auto& inst = instance();
    if(inst.m_peak_Focus_GetRange)
    {
        return inst.m_peak_Focus_GetRange(hCam, minFocus, maxFocus, incFocus);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Focus_Set(peak_camera_handle hCam, uint32_t focus)
{
    auto& inst = instance();
    if(inst.m_peak_Focus_Set)
    {
        return inst.m_peak_Focus_Set(hCam, focus);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Focus_Get(peak_camera_handle hCam, uint32_t * focus)
{
    auto& inst = instance();
    if(inst.m_peak_Focus_Get)
    {
        return inst.m_peak_Focus_Get(hCam, focus);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_PixelFormat_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_PixelFormat_GetAccessStatus)
    {
        return inst.m_peak_PixelFormat_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_PixelFormat_GetList(peak_camera_handle hCam, peak_pixel_format * pixelFormatList, size_t * pixelFormatCount)
{
    auto& inst = instance();
    if(inst.m_peak_PixelFormat_GetList)
    {
        return inst.m_peak_PixelFormat_GetList(hCam, pixelFormatList, pixelFormatCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_PixelFormat_Set(peak_camera_handle hCam, peak_pixel_format pixelFormat)
{
    auto& inst = instance();
    if(inst.m_peak_PixelFormat_Set)
    {
        return inst.m_peak_PixelFormat_Set(hCam, pixelFormat);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_PixelFormat_Get(peak_camera_handle hCam, peak_pixel_format * pixelFormat)
{
    auto& inst = instance();
    if(inst.m_peak_PixelFormat_Get)
    {
        return inst.m_peak_PixelFormat_Get(hCam, pixelFormat);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Gain_GetAccessStatus(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel)
{
    auto& inst = instance();
    if(inst.m_peak_Gain_GetAccessStatus)
    {
        return inst.m_peak_Gain_GetAccessStatus(hCam, gainType, gainChannel);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Gain_GetChannelList(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel * gainChannelList, size_t * gainChannelCount)
{
    auto& inst = instance();
    if(inst.m_peak_Gain_GetChannelList)
    {
        return inst.m_peak_Gain_GetChannelList(hCam, gainType, gainChannelList, gainChannelCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Gain_GetRange(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double * minGain, double * maxGain, double * incGain)
{
    auto& inst = instance();
    if(inst.m_peak_Gain_GetRange)
    {
        return inst.m_peak_Gain_GetRange(hCam, gainType, gainChannel, minGain, maxGain, incGain);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Gain_Set(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double gain)
{
    auto& inst = instance();
    if(inst.m_peak_Gain_Set)
    {
        return inst.m_peak_Gain_Set(hCam, gainType, gainChannel, gain);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Gain_Get(peak_camera_handle hCam, peak_gain_type gainType, peak_gain_channel gainChannel, double * gain)
{
    auto& inst = instance();
    if(inst.m_peak_Gain_Get)
    {
        return inst.m_peak_Gain_Get(hCam, gainType, gainChannel, gain);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Gamma_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Gamma_GetAccessStatus)
    {
        return inst.m_peak_Gamma_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Gamma_GetRange(peak_camera_handle hCam, double * minGamma, double * maxGamma, double * incGamma)
{
    auto& inst = instance();
    if(inst.m_peak_Gamma_GetRange)
    {
        return inst.m_peak_Gamma_GetRange(hCam, minGamma, maxGamma, incGamma);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Gamma_Set(peak_camera_handle hCam, double gamma)
{
    auto& inst = instance();
    if(inst.m_peak_Gamma_Set)
    {
        return inst.m_peak_Gamma_Set(hCam, gamma);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Gamma_Get(peak_camera_handle hCam, double * gamma)
{
    auto& inst = instance();
    if(inst.m_peak_Gamma_Get)
    {
        return inst.m_peak_Gamma_Get(hCam, gamma);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_ColorCorrection_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_ColorCorrection_GetAccessStatus)
    {
        return inst.m_peak_ColorCorrection_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ColorCorrection_Mode_GetList(peak_camera_handle hCam, peak_color_correction_mode * colorCorrectionModeList, size_t * colorCorrectionModeCount)
{
    auto& inst = instance();
    if(inst.m_peak_ColorCorrection_Mode_GetList)
    {
        return inst.m_peak_ColorCorrection_Mode_GetList(hCam, colorCorrectionModeList, colorCorrectionModeCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ColorCorrection_Mode_Set(peak_camera_handle hCam, peak_color_correction_mode colorCorrectionMode)
{
    auto& inst = instance();
    if(inst.m_peak_ColorCorrection_Mode_Set)
    {
        return inst.m_peak_ColorCorrection_Mode_Set(hCam, colorCorrectionMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ColorCorrection_Mode_Get(peak_camera_handle hCam, peak_color_correction_mode * colorCorrectionMode)
{
    auto& inst = instance();
    if(inst.m_peak_ColorCorrection_Mode_Get)
    {
        return inst.m_peak_ColorCorrection_Mode_Get(hCam, colorCorrectionMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_ColorCorrection_Matrix_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_ColorCorrection_Matrix_GetAccessStatus)
    {
        return inst.m_peak_ColorCorrection_Matrix_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ColorCorrection_Matrix_GetRange(peak_camera_handle hCam, double * minMatrixElementValue, double * maxMatrixElementValue, double * incMatrixElementValue)
{
    auto& inst = instance();
    if(inst.m_peak_ColorCorrection_Matrix_GetRange)
    {
        return inst.m_peak_ColorCorrection_Matrix_GetRange(hCam, minMatrixElementValue, maxMatrixElementValue, incMatrixElementValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ColorCorrection_Matrix_Set(peak_camera_handle hCam, peak_matrix colorCorrectionMatrix)
{
    auto& inst = instance();
    if(inst.m_peak_ColorCorrection_Matrix_Set)
    {
        return inst.m_peak_ColorCorrection_Matrix_Set(hCam, colorCorrectionMatrix);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ColorCorrection_Matrix_Get(peak_camera_handle hCam, peak_matrix * colorCorrectionMatrix)
{
    auto& inst = instance();
    if(inst.m_peak_ColorCorrection_Matrix_Get)
    {
        return inst.m_peak_ColorCorrection_Matrix_Get(hCam, colorCorrectionMatrix);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ColorCorrection_Enable(peak_camera_handle hCam, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_ColorCorrection_Enable)
    {
        return inst.m_peak_ColorCorrection_Enable(hCam, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_ColorCorrection_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_ColorCorrection_IsEnabled)
    {
        return inst.m_peak_ColorCorrection_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_AutoBrightness_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_GetAccessStatus)
    {
        return inst.m_peak_AutoBrightness_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_AutoBrightness_Target_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_Target_GetAccessStatus)
    {
        return inst.m_peak_AutoBrightness_Target_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_Target_GetRange(peak_camera_handle hCam, uint32_t * minAutoBrightnessTarget, uint32_t * maxAutoBrightnessTarget, uint32_t * incAutoBrightnessTarget)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_Target_GetRange)
    {
        return inst.m_peak_AutoBrightness_Target_GetRange(hCam, minAutoBrightnessTarget, maxAutoBrightnessTarget, incAutoBrightnessTarget);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_Target_Set(peak_camera_handle hCam, uint32_t autoBrightnessTarget)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_Target_Set)
    {
        return inst.m_peak_AutoBrightness_Target_Set(hCam, autoBrightnessTarget);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_Target_Get(peak_camera_handle hCam, uint32_t * autoBrightnessTarget)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_Target_Get)
    {
        return inst.m_peak_AutoBrightness_Target_Get(hCam, autoBrightnessTarget);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_AutoBrightness_TargetTolerance_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_TargetTolerance_GetAccessStatus)
    {
        return inst.m_peak_AutoBrightness_TargetTolerance_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_TargetTolerance_GetRange(peak_camera_handle hCam, uint32_t * minAutoBrightnessTargetTolerance, uint32_t * maxAutoBrightnessTargetTolerance, uint32_t * incAutoBrightnessTargetTolerance)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_TargetTolerance_GetRange)
    {
        return inst.m_peak_AutoBrightness_TargetTolerance_GetRange(hCam, minAutoBrightnessTargetTolerance, maxAutoBrightnessTargetTolerance, incAutoBrightnessTargetTolerance);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_TargetTolerance_Set(peak_camera_handle hCam, uint32_t autoBrightnessTargetTolerance)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_TargetTolerance_Set)
    {
        return inst.m_peak_AutoBrightness_TargetTolerance_Set(hCam, autoBrightnessTargetTolerance);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_TargetTolerance_Get(peak_camera_handle hCam, uint32_t * autoBrightnessTargetTolerance)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_TargetTolerance_Get)
    {
        return inst.m_peak_AutoBrightness_TargetTolerance_Get(hCam, autoBrightnessTargetTolerance);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_AutoBrightness_TargetPercentile_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_TargetPercentile_GetAccessStatus)
    {
        return inst.m_peak_AutoBrightness_TargetPercentile_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_TargetPercentile_GetRange(peak_camera_handle hCam, double * minAutoBrightnessTargetPercentile, double * maxAutoBrightnessTargetPercentile, double * incAutoBrightnessTargetPercentile)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_TargetPercentile_GetRange)
    {
        return inst.m_peak_AutoBrightness_TargetPercentile_GetRange(hCam, minAutoBrightnessTargetPercentile, maxAutoBrightnessTargetPercentile, incAutoBrightnessTargetPercentile);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_TargetPercentile_Set(peak_camera_handle hCam, double autoBrightnessTargetPercentile)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_TargetPercentile_Set)
    {
        return inst.m_peak_AutoBrightness_TargetPercentile_Set(hCam, autoBrightnessTargetPercentile);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_TargetPercentile_Get(peak_camera_handle hCam, double * autoBrightnessTargetPercentile)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_TargetPercentile_Get)
    {
        return inst.m_peak_AutoBrightness_TargetPercentile_Get(hCam, autoBrightnessTargetPercentile);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_AutoBrightness_ROI_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_ROI_GetAccessStatus)
    {
        return inst.m_peak_AutoBrightness_ROI_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_ROI_Mode_Set(peak_camera_handle hCam, peak_auto_feature_roi_mode autoBrightnessROIMode)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_ROI_Mode_Set)
    {
        return inst.m_peak_AutoBrightness_ROI_Mode_Set(hCam, autoBrightnessROIMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_ROI_Mode_Get(peak_camera_handle hCam, peak_auto_feature_roi_mode * autoBrightnessROIMode)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_ROI_Mode_Get)
    {
        return inst.m_peak_AutoBrightness_ROI_Mode_Get(hCam, autoBrightnessROIMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_ROI_Offset_GetRange(peak_camera_handle hCam, peak_position * minAutoBrightnessROIOffset, peak_position * maxAutoBrightnessROIOffset, peak_position * incAutoBrightnessROIOffset)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_ROI_Offset_GetRange)
    {
        return inst.m_peak_AutoBrightness_ROI_Offset_GetRange(hCam, minAutoBrightnessROIOffset, maxAutoBrightnessROIOffset, incAutoBrightnessROIOffset);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_ROI_Size_GetRange(peak_camera_handle hCam, peak_size * minAutoBrightnessROISize, peak_size * maxAutoBrightnessROISize, peak_size * incAutoBrightnessROISize)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_ROI_Size_GetRange)
    {
        return inst.m_peak_AutoBrightness_ROI_Size_GetRange(hCam, minAutoBrightnessROISize, maxAutoBrightnessROISize, incAutoBrightnessROISize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_ROI_Set(peak_camera_handle hCam, peak_roi autoBrightnessROI)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_ROI_Set)
    {
        return inst.m_peak_AutoBrightness_ROI_Set(hCam, autoBrightnessROI);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_ROI_Get(peak_camera_handle hCam, peak_roi * autoBrightnessROI)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_ROI_Get)
    {
        return inst.m_peak_AutoBrightness_ROI_Get(hCam, autoBrightnessROI);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_AutoBrightness_Exposure_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_Exposure_GetAccessStatus)
    {
        return inst.m_peak_AutoBrightness_Exposure_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_Exposure_Mode_GetList(peak_camera_handle hCam, peak_auto_feature_mode * modeList, size_t * modeListSize)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_Exposure_Mode_GetList)
    {
        return inst.m_peak_AutoBrightness_Exposure_Mode_GetList(hCam, modeList, modeListSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_Exposure_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoExposureMode)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_Exposure_Mode_Set)
    {
        return inst.m_peak_AutoBrightness_Exposure_Mode_Set(hCam, autoExposureMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_Exposure_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoExposureMode)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_Exposure_Mode_Get)
    {
        return inst.m_peak_AutoBrightness_Exposure_Mode_Get(hCam, autoExposureMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_AutoBrightness_Gain_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_Gain_GetAccessStatus)
    {
        return inst.m_peak_AutoBrightness_Gain_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_Gain_Mode_GetList(peak_camera_handle hCam, peak_auto_feature_mode * modeList, size_t * modeListSize)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_Gain_Mode_GetList)
    {
        return inst.m_peak_AutoBrightness_Gain_Mode_GetList(hCam, modeList, modeListSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_Gain_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_Gain_Mode_Set)
    {
        return inst.m_peak_AutoBrightness_Gain_Mode_Set(hCam, autoGainMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoBrightness_Gain_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode)
{
    auto& inst = instance();
    if(inst.m_peak_AutoBrightness_Gain_Mode_Get)
    {
        return inst.m_peak_AutoBrightness_Gain_Mode_Get(hCam, autoGainMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_AutoWhiteBalance_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_AutoWhiteBalance_GetAccessStatus)
    {
        return inst.m_peak_AutoWhiteBalance_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_AutoWhiteBalance_ROI_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_AutoWhiteBalance_ROI_GetAccessStatus)
    {
        return inst.m_peak_AutoWhiteBalance_ROI_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoWhiteBalance_ROI_Mode_Set(peak_camera_handle hCam, peak_auto_feature_roi_mode autoWhiteBalanceROIMode)
{
    auto& inst = instance();
    if(inst.m_peak_AutoWhiteBalance_ROI_Mode_Set)
    {
        return inst.m_peak_AutoWhiteBalance_ROI_Mode_Set(hCam, autoWhiteBalanceROIMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoWhiteBalance_ROI_Mode_Get(peak_camera_handle hCam, peak_auto_feature_roi_mode * autoWhiteBalanceROIMode)
{
    auto& inst = instance();
    if(inst.m_peak_AutoWhiteBalance_ROI_Mode_Get)
    {
        return inst.m_peak_AutoWhiteBalance_ROI_Mode_Get(hCam, autoWhiteBalanceROIMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoWhiteBalance_ROI_Offset_GetRange(peak_camera_handle hCam, peak_position * minAutoWhiteBalanceROIOffset, peak_position * maxAutoWhiteBalanceROIOffset, peak_position * incAutoWhiteBalanceROIOffset)
{
    auto& inst = instance();
    if(inst.m_peak_AutoWhiteBalance_ROI_Offset_GetRange)
    {
        return inst.m_peak_AutoWhiteBalance_ROI_Offset_GetRange(hCam, minAutoWhiteBalanceROIOffset, maxAutoWhiteBalanceROIOffset, incAutoWhiteBalanceROIOffset);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoWhiteBalance_ROI_Size_GetRange(peak_camera_handle hCam, peak_size * minAutoWhiteBalanceROISize, peak_size * maxAutoWhiteBalanceROISize, peak_size * incAutoWhiteBalanceROISize)
{
    auto& inst = instance();
    if(inst.m_peak_AutoWhiteBalance_ROI_Size_GetRange)
    {
        return inst.m_peak_AutoWhiteBalance_ROI_Size_GetRange(hCam, minAutoWhiteBalanceROISize, maxAutoWhiteBalanceROISize, incAutoWhiteBalanceROISize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoWhiteBalance_ROI_Set(peak_camera_handle hCam, peak_roi autoWhiteBalanceROI)
{
    auto& inst = instance();
    if(inst.m_peak_AutoWhiteBalance_ROI_Set)
    {
        return inst.m_peak_AutoWhiteBalance_ROI_Set(hCam, autoWhiteBalanceROI);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoWhiteBalance_ROI_Get(peak_camera_handle hCam, peak_roi * autoWhiteBalanceROI)
{
    auto& inst = instance();
    if(inst.m_peak_AutoWhiteBalance_ROI_Get)
    {
        return inst.m_peak_AutoWhiteBalance_ROI_Get(hCam, autoWhiteBalanceROI);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoWhiteBalance_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoWhiteBalanceMode)
{
    auto& inst = instance();
    if(inst.m_peak_AutoWhiteBalance_Mode_Set)
    {
        return inst.m_peak_AutoWhiteBalance_Mode_Set(hCam, autoWhiteBalanceMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoWhiteBalance_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoWhiteBalanceMode)
{
    auto& inst = instance();
    if(inst.m_peak_AutoWhiteBalance_Mode_Get)
    {
        return inst.m_peak_AutoWhiteBalance_Mode_Get(hCam, autoWhiteBalanceMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_AutoWhiteBalance_Mode_GetList(peak_camera_handle hCam, peak_auto_feature_mode * modeList, size_t * modeListSize)
{
    auto& inst = instance();
    if(inst.m_peak_AutoWhiteBalance_Mode_GetList)
    {
        return inst.m_peak_AutoWhiteBalance_Mode_GetList(hCam, modeList, modeListSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_ROI_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_ROI_GetAccessStatus)
    {
        return inst.m_peak_ROI_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ROI_Offset_GetRange(peak_camera_handle hCam, peak_position * minROIOffset, peak_position * maxROIOffset, peak_position * incROIOffset)
{
    auto& inst = instance();
    if(inst.m_peak_ROI_Offset_GetRange)
    {
        return inst.m_peak_ROI_Offset_GetRange(hCam, minROIOffset, maxROIOffset, incROIOffset);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_ROI_Offset_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_ROI_Offset_GetAccessStatus)
    {
        return inst.m_peak_ROI_Offset_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ROI_Offset_Set(peak_camera_handle hCam, peak_position position)
{
    auto& inst = instance();
    if(inst.m_peak_ROI_Offset_Set)
    {
        return inst.m_peak_ROI_Offset_Set(hCam, position);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ROI_Offset_Get(peak_camera_handle hCam, peak_position * position)
{
    auto& inst = instance();
    if(inst.m_peak_ROI_Offset_Get)
    {
        return inst.m_peak_ROI_Offset_Get(hCam, position);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ROI_Size_GetRange(peak_camera_handle hCam, peak_size * minROISize, peak_size * maxROISize, peak_size * incROISize)
{
    auto& inst = instance();
    if(inst.m_peak_ROI_Size_GetRange)
    {
        return inst.m_peak_ROI_Size_GetRange(hCam, minROISize, maxROISize, incROISize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_ROI_Size_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_ROI_Size_GetAccessStatus)
    {
        return inst.m_peak_ROI_Size_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ROI_Size_Set(peak_camera_handle hCam, peak_size size)
{
    auto& inst = instance();
    if(inst.m_peak_ROI_Size_Set)
    {
        return inst.m_peak_ROI_Size_Set(hCam, size);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ROI_Size_Get(peak_camera_handle hCam, peak_size * size)
{
    auto& inst = instance();
    if(inst.m_peak_ROI_Size_Get)
    {
        return inst.m_peak_ROI_Size_Get(hCam, size);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ROI_Set(peak_camera_handle hCam, peak_roi roi)
{
    auto& inst = instance();
    if(inst.m_peak_ROI_Set)
    {
        return inst.m_peak_ROI_Set(hCam, roi);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_ROI_Get(peak_camera_handle hCam, peak_roi * roi)
{
    auto& inst = instance();
    if(inst.m_peak_ROI_Get)
    {
        return inst.m_peak_ROI_Get(hCam, roi);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Binning_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Binning_GetAccessStatus)
    {
        return inst.m_peak_Binning_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Binning_FactorX_GetList(peak_camera_handle hCam, uint32_t * binningFactorXList, size_t * binningFactorXCount)
{
    auto& inst = instance();
    if(inst.m_peak_Binning_FactorX_GetList)
    {
        return inst.m_peak_Binning_FactorX_GetList(hCam, binningFactorXList, binningFactorXCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Binning_FactorY_GetList(peak_camera_handle hCam, uint32_t * binningFactorYList, size_t * binningFactorYCount)
{
    auto& inst = instance();
    if(inst.m_peak_Binning_FactorY_GetList)
    {
        return inst.m_peak_Binning_FactorY_GetList(hCam, binningFactorYList, binningFactorYCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Binning_Set(peak_camera_handle hCam, uint32_t binningFactorX, uint32_t binningFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_Binning_Set)
    {
        return inst.m_peak_Binning_Set(hCam, binningFactorX, binningFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Binning_Get(peak_camera_handle hCam, uint32_t * binningFactorX, uint32_t * binningFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_Binning_Get)
    {
        return inst.m_peak_Binning_Get(hCam, binningFactorX, binningFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_BinningManual_GetAccessStatus(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine)
{
    auto& inst = instance();
    if(inst.m_peak_BinningManual_GetAccessStatus)
    {
        return inst.m_peak_BinningManual_GetAccessStatus(hCam, subsamplingEngine);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_BinningManual_FactorX_GetList(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * binningFactorXList, size_t * binningFactorXCount)
{
    auto& inst = instance();
    if(inst.m_peak_BinningManual_FactorX_GetList)
    {
        return inst.m_peak_BinningManual_FactorX_GetList(hCam, subsamplingEngine, binningFactorXList, binningFactorXCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_BinningManual_FactorY_GetList(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * binningFactorYList, size_t * binningFactorYCount)
{
    auto& inst = instance();
    if(inst.m_peak_BinningManual_FactorY_GetList)
    {
        return inst.m_peak_BinningManual_FactorY_GetList(hCam, subsamplingEngine, binningFactorYList, binningFactorYCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_BinningManual_Set(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t binningFactorX, uint32_t binningFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_BinningManual_Set)
    {
        return inst.m_peak_BinningManual_Set(hCam, subsamplingEngine, binningFactorX, binningFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_BinningManual_Get(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * binningFactorX, uint32_t * binningFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_BinningManual_Get)
    {
        return inst.m_peak_BinningManual_Get(hCam, subsamplingEngine, binningFactorX, binningFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Decimation_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Decimation_GetAccessStatus)
    {
        return inst.m_peak_Decimation_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Decimation_FactorX_GetList(peak_camera_handle hCam, uint32_t * decimationFactorXList, size_t * decimationFactorXCount)
{
    auto& inst = instance();
    if(inst.m_peak_Decimation_FactorX_GetList)
    {
        return inst.m_peak_Decimation_FactorX_GetList(hCam, decimationFactorXList, decimationFactorXCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Decimation_FactorY_GetList(peak_camera_handle hCam, uint32_t * decimationFactorYList, size_t * decimationFactorYCount)
{
    auto& inst = instance();
    if(inst.m_peak_Decimation_FactorY_GetList)
    {
        return inst.m_peak_Decimation_FactorY_GetList(hCam, decimationFactorYList, decimationFactorYCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Decimation_Set(peak_camera_handle hCam, uint32_t decimationFactorX, uint32_t decimationFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_Decimation_Set)
    {
        return inst.m_peak_Decimation_Set(hCam, decimationFactorX, decimationFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Decimation_Get(peak_camera_handle hCam, uint32_t * decimationFactorX, uint32_t * decimationFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_Decimation_Get)
    {
        return inst.m_peak_Decimation_Get(hCam, decimationFactorX, decimationFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_DecimationManual_GetAccessStatus(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine)
{
    auto& inst = instance();
    if(inst.m_peak_DecimationManual_GetAccessStatus)
    {
        return inst.m_peak_DecimationManual_GetAccessStatus(hCam, subsamplingEngine);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_DecimationManual_FactorX_GetList(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * decimationFactorXList, size_t * decimationFactorXCount)
{
    auto& inst = instance();
    if(inst.m_peak_DecimationManual_FactorX_GetList)
    {
        return inst.m_peak_DecimationManual_FactorX_GetList(hCam, subsamplingEngine, decimationFactorXList, decimationFactorXCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_DecimationManual_FactorY_GetList(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * decimationFactorYList, size_t * decimationFactorYCount)
{
    auto& inst = instance();
    if(inst.m_peak_DecimationManual_FactorY_GetList)
    {
        return inst.m_peak_DecimationManual_FactorY_GetList(hCam, subsamplingEngine, decimationFactorYList, decimationFactorYCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_DecimationManual_Set(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t decimationFactorX, uint32_t decimationFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_DecimationManual_Set)
    {
        return inst.m_peak_DecimationManual_Set(hCam, subsamplingEngine, decimationFactorX, decimationFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_DecimationManual_Get(peak_camera_handle hCam, peak_subsampling_engine subsamplingEngine, uint32_t * decimationFactorX, uint32_t * decimationFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_DecimationManual_Get)
    {
        return inst.m_peak_DecimationManual_Get(hCam, subsamplingEngine, decimationFactorX, decimationFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Scaling_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Scaling_GetAccessStatus)
    {
        return inst.m_peak_Scaling_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Scaling_FactorX_GetRange(peak_camera_handle hCam, double * minScalingFactorX, double * maxScalingFactorX, double * incScalingFactorX)
{
    auto& inst = instance();
    if(inst.m_peak_Scaling_FactorX_GetRange)
    {
        return inst.m_peak_Scaling_FactorX_GetRange(hCam, minScalingFactorX, maxScalingFactorX, incScalingFactorX);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Scaling_FactorY_GetRange(peak_camera_handle hCam, double * minScalingFactorY, double * maxScalingFactorY, double * incScalingFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_Scaling_FactorY_GetRange)
    {
        return inst.m_peak_Scaling_FactorY_GetRange(hCam, minScalingFactorY, maxScalingFactorY, incScalingFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Scaling_Set(peak_camera_handle hCam, double scalingFactorX, double scalingFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_Scaling_Set)
    {
        return inst.m_peak_Scaling_Set(hCam, scalingFactorX, scalingFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Scaling_Get(peak_camera_handle hCam, double * scalingFactorX, double * scalingFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_Scaling_Get)
    {
        return inst.m_peak_Scaling_Get(hCam, scalingFactorX, scalingFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Mirror_LeftRight_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Mirror_LeftRight_GetAccessStatus)
    {
        return inst.m_peak_Mirror_LeftRight_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Mirror_LeftRight_Enable(peak_camera_handle hCam, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_Mirror_LeftRight_Enable)
    {
        return inst.m_peak_Mirror_LeftRight_Enable(hCam, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_Mirror_LeftRight_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Mirror_LeftRight_IsEnabled)
    {
        return inst.m_peak_Mirror_LeftRight_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Mirror_UpDown_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Mirror_UpDown_GetAccessStatus)
    {
        return inst.m_peak_Mirror_UpDown_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Mirror_UpDown_Enable(peak_camera_handle hCam, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_Mirror_UpDown_Enable)
    {
        return inst.m_peak_Mirror_UpDown_Enable(hCam, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_Mirror_UpDown_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Mirror_UpDown_IsEnabled)
    {
        return inst.m_peak_Mirror_UpDown_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_CameraMemory_Area_GetAccessStatus(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea)
{
    auto& inst = instance();
    if(inst.m_peak_CameraMemory_Area_GetAccessStatus)
    {
        return inst.m_peak_CameraMemory_Area_GetAccessStatus(hCam, cameraMemoryArea);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_CameraMemory_Area_GetList(peak_camera_handle hCam, peak_camera_memory_area * cameraMemoryAreaList, size_t * cameraMemoryAreaCount)
{
    auto& inst = instance();
    if(inst.m_peak_CameraMemory_Area_GetList)
    {
        return inst.m_peak_CameraMemory_Area_GetList(hCam, cameraMemoryAreaList, cameraMemoryAreaCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_CameraMemory_Area_Size_Get(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea, size_t * cameraMemoryAreaSize)
{
    auto& inst = instance();
    if(inst.m_peak_CameraMemory_Area_Size_Get)
    {
        return inst.m_peak_CameraMemory_Area_Size_Get(hCam, cameraMemoryArea, cameraMemoryAreaSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_CameraMemory_Area_Data_Clear(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea)
{
    auto& inst = instance();
    if(inst.m_peak_CameraMemory_Area_Data_Clear)
    {
        return inst.m_peak_CameraMemory_Area_Data_Clear(hCam, cameraMemoryArea);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_CameraMemory_Area_Data_Write(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea, const uint8_t * data, size_t dataSize)
{
    auto& inst = instance();
    if(inst.m_peak_CameraMemory_Area_Data_Write)
    {
        return inst.m_peak_CameraMemory_Area_Data_Write(hCam, cameraMemoryArea, data, dataSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_CameraMemory_Area_Data_Read(peak_camera_handle hCam, peak_camera_memory_area cameraMemoryArea, uint8_t * data, size_t dataSize)
{
    auto& inst = instance();
    if(inst.m_peak_CameraMemory_Area_Data_Read)
    {
        return inst.m_peak_CameraMemory_Area_Data_Read(hCam, cameraMemoryArea, data, dataSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_EnableWriteAccess(peak_camera_handle hCam, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_EnableWriteAccess)
    {
        return inst.m_peak_GFA_EnableWriteAccess(hCam, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_GFA_IsWriteAccessEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_IsWriteAccessEnabled)
    {
        return inst.m_peak_GFA_IsWriteAccessEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_GFA_Feature_GetAccessStatus(peak_camera_handle hCam, peak_gfa_module module, const char * featureName)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Feature_GetAccessStatus)
    {
        return inst.m_peak_GFA_Feature_GetAccessStatus(hCam, module, featureName);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_GFA_Float_HasRange(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Float_HasRange)
    {
        return inst.m_peak_GFA_Float_HasRange(hCam, module, floatFeatureName);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Float_GetRange(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName, double * minFloatValue, double * maxFloatValue, double * incFloatValue)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Float_GetRange)
    {
        return inst.m_peak_GFA_Float_GetRange(hCam, module, floatFeatureName, minFloatValue, maxFloatValue, incFloatValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Float_GetList(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName, double * floatList, size_t * floatCount)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Float_GetList)
    {
        return inst.m_peak_GFA_Float_GetList(hCam, module, floatFeatureName, floatList, floatCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Float_Set(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName, double floatValue)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Float_Set)
    {
        return inst.m_peak_GFA_Float_Set(hCam, module, floatFeatureName, floatValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Float_Get(peak_camera_handle hCam, peak_gfa_module module, const char * floatFeatureName, double * floatValue)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Float_Get)
    {
        return inst.m_peak_GFA_Float_Get(hCam, module, floatFeatureName, floatValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_GFA_Integer_HasRange(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Integer_HasRange)
    {
        return inst.m_peak_GFA_Integer_HasRange(hCam, module, integerFeatureName);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Integer_GetRange(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName, int64_t * minIntegerValue, int64_t * maxIntegerValue, int64_t * incIntegerValue)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Integer_GetRange)
    {
        return inst.m_peak_GFA_Integer_GetRange(hCam, module, integerFeatureName, minIntegerValue, maxIntegerValue, incIntegerValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Integer_GetList(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName, int64_t * integerList, size_t * integerCount)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Integer_GetList)
    {
        return inst.m_peak_GFA_Integer_GetList(hCam, module, integerFeatureName, integerList, integerCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Integer_Set(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName, int64_t integerValue)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Integer_Set)
    {
        return inst.m_peak_GFA_Integer_Set(hCam, module, integerFeatureName, integerValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Integer_Get(peak_camera_handle hCam, peak_gfa_module module, const char * integerFeatureName, int64_t * integerValue)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Integer_Get)
    {
        return inst.m_peak_GFA_Integer_Get(hCam, module, integerFeatureName, integerValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Boolean_Set(peak_camera_handle hCam, peak_gfa_module module, const char * booleanFeatureName, peak_bool booleanValue)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Boolean_Set)
    {
        return inst.m_peak_GFA_Boolean_Set(hCam, module, booleanFeatureName, booleanValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Boolean_Get(peak_camera_handle hCam, peak_gfa_module module, const char * booleanFeatureName, peak_bool * booleanValue)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Boolean_Get)
    {
        return inst.m_peak_GFA_Boolean_Get(hCam, module, booleanFeatureName, booleanValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_String_Set(peak_camera_handle hCam, peak_gfa_module module, const char * stringFeatureName, const char * stringValue)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_String_Set)
    {
        return inst.m_peak_GFA_String_Set(hCam, module, stringFeatureName, stringValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_String_Get(peak_camera_handle hCam, peak_gfa_module module, const char * stringFeatureName, char * stringValue, size_t * stringValueSize)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_String_Get)
    {
        return inst.m_peak_GFA_String_Get(hCam, module, stringFeatureName, stringValue, stringValueSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Command_Execute(peak_camera_handle hCam, peak_gfa_module module, const char * commandFeatureName)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Command_Execute)
    {
        return inst.m_peak_GFA_Command_Execute(hCam, module, commandFeatureName);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Command_WaitForDone(peak_camera_handle hCam, peak_gfa_module module, const char * commandFeatureName, uint32_t timeout_ms)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Command_WaitForDone)
    {
        return inst.m_peak_GFA_Command_WaitForDone(hCam, module, commandFeatureName, timeout_ms);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Enumeration_GetList(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, peak_gfa_enumeration_entry * enumerationEntryList, size_t * enumerationEntryCount)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Enumeration_GetList)
    {
        return inst.m_peak_GFA_Enumeration_GetList(hCam, module, enumerationFeatureName, enumerationEntryList, enumerationEntryCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_GFA_EnumerationEntry_GetAccessStatus(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, const peak_gfa_enumeration_entry * enumerationEntry)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_EnumerationEntry_GetAccessStatus)
    {
        return inst.m_peak_GFA_EnumerationEntry_GetAccessStatus(hCam, module, enumerationFeatureName, enumerationEntry);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_GFA_EnumerationEntry_GetAccessStatusBySymbolicValue(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, const char * enumerationEntrySymbolicValue)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_EnumerationEntry_GetAccessStatusBySymbolicValue)
    {
        return inst.m_peak_GFA_EnumerationEntry_GetAccessStatusBySymbolicValue(hCam, module, enumerationFeatureName, enumerationEntrySymbolicValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_GFA_EnumerationEntry_GetAccessStatusByIntegerValue(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, int64_t enumerationEntryIntegerValue)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_EnumerationEntry_GetAccessStatusByIntegerValue)
    {
        return inst.m_peak_GFA_EnumerationEntry_GetAccessStatusByIntegerValue(hCam, module, enumerationFeatureName, enumerationEntryIntegerValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Enumeration_Set(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, const peak_gfa_enumeration_entry * enumerationEntry)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Enumeration_Set)
    {
        return inst.m_peak_GFA_Enumeration_Set(hCam, module, enumerationFeatureName, enumerationEntry);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Enumeration_SetBySymbolicValue(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, const char * enumerationEntrySymbolicValue)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Enumeration_SetBySymbolicValue)
    {
        return inst.m_peak_GFA_Enumeration_SetBySymbolicValue(hCam, module, enumerationFeatureName, enumerationEntrySymbolicValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Enumeration_SetByIntegerValue(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, int64_t enumerationEntryIntegerValue)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Enumeration_SetByIntegerValue)
    {
        return inst.m_peak_GFA_Enumeration_SetByIntegerValue(hCam, module, enumerationFeatureName, enumerationEntryIntegerValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Enumeration_Get(peak_camera_handle hCam, peak_gfa_module module, const char * enumerationFeatureName, peak_gfa_enumeration_entry * enumerationEntry)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Enumeration_Get)
    {
        return inst.m_peak_GFA_Enumeration_Get(hCam, module, enumerationFeatureName, enumerationEntry);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Register_Set(peak_camera_handle hCam, peak_gfa_module module, const char * registerFeatureName, const uint8_t * registerValue, size_t registerValueSize)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Register_Set)
    {
        return inst.m_peak_GFA_Register_Set(hCam, module, registerFeatureName, registerValue, registerValueSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Register_Get(peak_camera_handle hCam, peak_gfa_module module, const char * registerFeatureName, uint8_t * registerValue, size_t * registerValueSize)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Register_Get)
    {
        return inst.m_peak_GFA_Register_Get(hCam, module, registerFeatureName, registerValue, registerValueSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Data_Write(peak_camera_handle hCam, peak_gfa_module module, uint64_t address, const uint8_t * data, size_t dataSize)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Data_Write)
    {
        return inst.m_peak_GFA_Data_Write(hCam, module, address, data, dataSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_GFA_Data_Read(peak_camera_handle hCam, peak_gfa_module module, uint64_t address, uint8_t * data, size_t dataSize)
{
    auto& inst = instance();
    if(inst.m_peak_GFA_Data_Read)
    {
        return inst.m_peak_GFA_Data_Read(hCam, module, address, data, dataSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_PixelFormat_GetList(peak_camera_handle hCam, peak_pixel_format inputPixelFormat, peak_pixel_format * outputPixelFormatList, size_t * outputPixelFormatCount)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_PixelFormat_GetList)
    {
        return inst.m_peak_IPL_PixelFormat_GetList(hCam, inputPixelFormat, outputPixelFormatList, outputPixelFormatCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_PixelFormat_Set(peak_camera_handle hCam, peak_pixel_format pixelFormat)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_PixelFormat_Set)
    {
        return inst.m_peak_IPL_PixelFormat_Set(hCam, pixelFormat);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_PixelFormat_Get(peak_camera_handle hCam, peak_pixel_format * pixelFormat)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_PixelFormat_Get)
    {
        return inst.m_peak_IPL_PixelFormat_Get(hCam, pixelFormat);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Gain_GetRange(peak_camera_handle hCam, peak_gain_channel gainChannel, double * minGain, double * maxGain, double * incGain)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Gain_GetRange)
    {
        return inst.m_peak_IPL_Gain_GetRange(hCam, gainChannel, minGain, maxGain, incGain);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Gain_Set(peak_camera_handle hCam, peak_gain_channel gainChannel, double gain)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Gain_Set)
    {
        return inst.m_peak_IPL_Gain_Set(hCam, gainChannel, gain);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Gain_Get(peak_camera_handle hCam, peak_gain_channel gainChannel, double * gain)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Gain_Get)
    {
        return inst.m_peak_IPL_Gain_Get(hCam, gainChannel, gain);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Gamma_GetRange(peak_camera_handle hCam, double * minGamma, double * maxGamma, double * incGamma)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Gamma_GetRange)
    {
        return inst.m_peak_IPL_Gamma_GetRange(hCam, minGamma, maxGamma, incGamma);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Gamma_Set(peak_camera_handle hCam, double gamma)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Gamma_Set)
    {
        return inst.m_peak_IPL_Gamma_Set(hCam, gamma);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Gamma_Get(peak_camera_handle hCam, double * gamma)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Gamma_Get)
    {
        return inst.m_peak_IPL_Gamma_Get(hCam, gamma);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ColorCorrection_Matrix_Set(peak_camera_handle hCam, peak_matrix colorCorrectionMatrix)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_Matrix_Set)
    {
        return inst.m_peak_IPL_ColorCorrection_Matrix_Set(hCam, colorCorrectionMatrix);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ColorCorrection_Matrix_Get(peak_camera_handle hCam, peak_matrix * colorCorrectionMatrix)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_Matrix_Get)
    {
        return inst.m_peak_IPL_ColorCorrection_Matrix_Get(hCam, colorCorrectionMatrix);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ColorCorrection_Saturation_Get(peak_camera_handle hCam, double * saturation)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_Saturation_Get)
    {
        return inst.m_peak_IPL_ColorCorrection_Saturation_Get(hCam, saturation);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ColorCorrection_Saturation_Set(peak_camera_handle hCam, double saturation)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_Saturation_Set)
    {
        return inst.m_peak_IPL_ColorCorrection_Saturation_Set(hCam, saturation);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ColorCorrection_Saturation_GetRange(peak_camera_handle hCam, double * minSaturation, double * maxSaturation, double * incSaturation)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_Saturation_GetRange)
    {
        return inst.m_peak_IPL_ColorCorrection_Saturation_GetRange(hCam, minSaturation, maxSaturation, incSaturation);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Get(peak_camera_handle hCam, peak_chromatic_adaption_color_space * colorSpace)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Get)
    {
        return inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Get(hCam, colorSpace);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Set(peak_camera_handle hCam, peak_chromatic_adaption_color_space colorSpace)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Set)
    {
        return inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorSpace_Set(hCam, colorSpace);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Get(peak_camera_handle hCam, peak_chromatic_adaption_algorithm * algorithm)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Get)
    {
        return inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Get(hCam, algorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Set(peak_camera_handle hCam, peak_chromatic_adaption_algorithm algorithm)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Set)
    {
        return inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_Algorithm_Set(hCam, algorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Get(peak_camera_handle hCam, uint32_t * colorTemperature)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Get)
    {
        return inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Get(hCam, colorTemperature);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Set(peak_camera_handle hCam, uint32_t colorTemperature)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Set)
    {
        return inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_Set(hCam, colorTemperature);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_GetRange(peak_camera_handle hCam, uint32_t * minColorTemperature, uint32_t * maxColorTemperature, uint32_t * incColorTemperature)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_GetRange)
    {
        return inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_ColorTemperature_GetRange(hCam, minColorTemperature, maxColorTemperature, incColorTemperature);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ColorCorrection_ChromaticAdaption_Enable(peak_camera_handle hCam, peak_bool enable)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_Enable)
    {
        return inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_Enable(hCam, enable);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_IPL_ColorCorrection_ChromaticAdaption_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_IsEnabled)
    {
        return inst.m_peak_IPL_ColorCorrection_ChromaticAdaption_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ColorCorrection_Enable(peak_camera_handle hCam, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_Enable)
    {
        return inst.m_peak_IPL_ColorCorrection_Enable(hCam, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_IPL_ColorCorrection_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ColorCorrection_IsEnabled)
    {
        return inst.m_peak_IPL_ColorCorrection_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_Target_GetRange(peak_camera_handle hCam, uint32_t * minAutoBrightnessTarget, uint32_t * maxAutoBrightnessTarget, uint32_t * incAutoBrightnessTarget)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_Target_GetRange)
    {
        return inst.m_peak_IPL_AutoBrightness_Target_GetRange(hCam, minAutoBrightnessTarget, maxAutoBrightnessTarget, incAutoBrightnessTarget);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_Target_Set(peak_camera_handle hCam, uint32_t autoBrightnessTarget)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_Target_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_Target_Set(hCam, autoBrightnessTarget);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_Target_Get(peak_camera_handle hCam, uint32_t * autoBrightnessTarget)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_Target_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_Target_Get(hCam, autoBrightnessTarget);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_TargetTolerance_GetRange(peak_camera_handle hCam, uint32_t * minAutoBrightnessTargetTolerance, uint32_t * maxAutoBrightnessTargetTolerance, uint32_t * incAutoBrightnessTargetTolerance)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_TargetTolerance_GetRange)
    {
        return inst.m_peak_IPL_AutoBrightness_TargetTolerance_GetRange(hCam, minAutoBrightnessTargetTolerance, maxAutoBrightnessTargetTolerance, incAutoBrightnessTargetTolerance);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_TargetTolerance_Set(peak_camera_handle hCam, uint32_t autoBrightnessTargetTolerance)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_TargetTolerance_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_TargetTolerance_Set(hCam, autoBrightnessTargetTolerance);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_TargetTolerance_Get(peak_camera_handle hCam, uint32_t * autoBrightnessTargetTolerance)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_TargetTolerance_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_TargetTolerance_Get(hCam, autoBrightnessTargetTolerance);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_TargetPercentile_GetRange(peak_camera_handle hCam, double * minAutoBrightnessTargetPercentile, double * maxAutoBrightnessTargetPercentile, double * incAutoBrightnessTargetPercentile)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_TargetPercentile_GetRange)
    {
        return inst.m_peak_IPL_AutoBrightness_TargetPercentile_GetRange(hCam, minAutoBrightnessTargetPercentile, maxAutoBrightnessTargetPercentile, incAutoBrightnessTargetPercentile);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_TargetPercentile_Set(peak_camera_handle hCam, double autoBrightnessTargetPercentile)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_TargetPercentile_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_TargetPercentile_Set(hCam, autoBrightnessTargetPercentile);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_TargetPercentile_Get(peak_camera_handle hCam, double * autoBrightnessTargetPercentile)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_TargetPercentile_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_TargetPercentile_Get(hCam, autoBrightnessTargetPercentile);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_ROI_Mode_Set(peak_camera_handle hCam, peak_auto_feature_roi_mode autoBrightnessROIMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_ROI_Mode_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_ROI_Mode_Set(hCam, autoBrightnessROIMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_ROI_Mode_Get(peak_camera_handle hCam, peak_auto_feature_roi_mode * autoBrightnessROIMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_ROI_Mode_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_ROI_Mode_Get(hCam, autoBrightnessROIMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_ROI_Offset_GetRange(peak_camera_handle hCam, peak_position * minAutoBrightnessROIOffset, peak_position * maxAutoBrightnessROIOffset, peak_position * incAutoBrightnessROIOffset)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_ROI_Offset_GetRange)
    {
        return inst.m_peak_IPL_AutoBrightness_ROI_Offset_GetRange(hCam, minAutoBrightnessROIOffset, maxAutoBrightnessROIOffset, incAutoBrightnessROIOffset);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_ROI_Size_GetRange(peak_camera_handle hCam, peak_size * minAutoBrightnessROISize, peak_size * maxAutoBrightnessROISize, peak_size * incAutoBrightnessROISize)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_ROI_Size_GetRange)
    {
        return inst.m_peak_IPL_AutoBrightness_ROI_Size_GetRange(hCam, minAutoBrightnessROISize, maxAutoBrightnessROISize, incAutoBrightnessROISize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_ROI_Set(peak_camera_handle hCam, peak_roi autoBrightnessROI)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_ROI_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_ROI_Set(hCam, autoBrightnessROI);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_ROI_Get(peak_camera_handle hCam, peak_roi * autoBrightnessROI)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_ROI_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_ROI_Get(hCam, autoBrightnessROI);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_Exposure_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoExposureMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_Exposure_Mode_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_Exposure_Mode_Set(hCam, autoExposureMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_Exposure_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoExposureMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_Exposure_Mode_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_Exposure_Mode_Get(hCam, autoExposureMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_ExposureLimit_Get(peak_camera_handle hCam, peak_double_limit * exposureLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_ExposureLimit_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_ExposureLimit_Get(hCam, exposureLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_ExposureLimit_Set(peak_camera_handle hCam, peak_double_limit exposureLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_ExposureLimit_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_ExposureLimit_Set(hCam, exposureLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_Gain_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_Gain_Mode_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_Gain_Mode_Set(hCam, autoGainMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_Gain_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_Gain_Mode_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_Gain_Mode_Get(hCam, autoGainMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainAnalog_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainAnalog_Mode_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_GainAnalog_Mode_Set(hCam, autoGainMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainAnalog_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainAnalog_Mode_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_GainAnalog_Mode_Get(hCam, autoGainMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainDigital_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainDigital_Mode_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_GainDigital_Mode_Set(hCam, autoGainMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainDigital_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainDigital_Mode_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_GainDigital_Mode_Get(hCam, autoGainMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainCombined_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainCombined_Mode_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_GainCombined_Mode_Set(hCam, autoGainMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainCombined_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainCombined_Mode_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_GainCombined_Mode_Get(hCam, autoGainMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainHost_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoGainMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainHost_Mode_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_GainHost_Mode_Set(hCam, autoGainMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainHost_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoGainMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainHost_Mode_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_GainHost_Mode_Get(hCam, autoGainMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_Algorithm_Set(peak_camera_handle hCam, peak_auto_feature_brightness_algorithm algorithm)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_Algorithm_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_Algorithm_Set(hCam, algorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_Algorithm_Get(peak_camera_handle hCam, peak_auto_feature_brightness_algorithm * algorithm)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_Algorithm_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_Algorithm_Get(hCam, algorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_AverageLast_Get(peak_camera_handle hCam, uint32_t * lastAverage)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_AverageLast_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_AverageLast_Get(hCam, lastAverage);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainLimit_Set(peak_camera_handle hCam, peak_double_limit gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainLimit_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_GainLimit_Set(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainLimit_Get(peak_camera_handle hCam, peak_double_limit * gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainLimit_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_GainLimit_Get(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainLimit_GetRange(peak_camera_handle hCam, peak_double_limit * gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainLimit_GetRange)
    {
        return inst.m_peak_IPL_AutoBrightness_GainLimit_GetRange(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainAnalogLimit_Set(peak_camera_handle hCam, peak_double_limit gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainAnalogLimit_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_GainAnalogLimit_Set(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainAnalogLimit_Get(peak_camera_handle hCam, peak_double_limit * gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainAnalogLimit_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_GainAnalogLimit_Get(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainAnalogLimit_GetRange(peak_camera_handle hCam, peak_double_limit * gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainAnalogLimit_GetRange)
    {
        return inst.m_peak_IPL_AutoBrightness_GainAnalogLimit_GetRange(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainDigitalLimit_Set(peak_camera_handle hCam, peak_double_limit gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainDigitalLimit_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_GainDigitalLimit_Set(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainDigitalLimit_Get(peak_camera_handle hCam, peak_double_limit * gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainDigitalLimit_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_GainDigitalLimit_Get(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainDigitalLimit_GetRange(peak_camera_handle hCam, peak_double_limit * gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainDigitalLimit_GetRange)
    {
        return inst.m_peak_IPL_AutoBrightness_GainDigitalLimit_GetRange(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainCombinedLimit_Set(peak_camera_handle hCam, peak_double_limit gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainCombinedLimit_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_GainCombinedLimit_Set(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainCombinedLimit_Get(peak_camera_handle hCam, peak_double_limit * gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainCombinedLimit_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_GainCombinedLimit_Get(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainCombinedLimit_GetRange(peak_camera_handle hCam, peak_double_limit * gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainCombinedLimit_GetRange)
    {
        return inst.m_peak_IPL_AutoBrightness_GainCombinedLimit_GetRange(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainHostLimit_Set(peak_camera_handle hCam, peak_double_limit gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainHostLimit_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_GainHostLimit_Set(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainHostLimit_Get(peak_camera_handle hCam, peak_double_limit * gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainHostLimit_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_GainHostLimit_Get(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_GainHostLimit_GetRange(peak_camera_handle hCam, peak_double_limit * gainLimit)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_GainHostLimit_GetRange)
    {
        return inst.m_peak_IPL_AutoBrightness_GainHostLimit_GetRange(hCam, gainLimit);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_SkipFrames_Set(peak_camera_handle hCam, uint32_t skipFrames)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_SkipFrames_Set)
    {
        return inst.m_peak_IPL_AutoBrightness_SkipFrames_Set(hCam, skipFrames);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_SkipFrames_Get(peak_camera_handle hCam, uint32_t * skipFrames)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_SkipFrames_Get)
    {
        return inst.m_peak_IPL_AutoBrightness_SkipFrames_Get(hCam, skipFrames);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoBrightness_SkipFrames_GetRange(peak_camera_handle hCam, uint32_t * skipFramesMin, uint32_t * skipFramesMax, uint32_t * skipFramesInc)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoBrightness_SkipFrames_GetRange)
    {
        return inst.m_peak_IPL_AutoBrightness_SkipFrames_GetRange(hCam, skipFramesMin, skipFramesMax, skipFramesInc);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoWhiteBalance_ROI_Mode_Set(peak_camera_handle hCam, peak_auto_feature_roi_mode autoWhiteBalanceROIMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoWhiteBalance_ROI_Mode_Set)
    {
        return inst.m_peak_IPL_AutoWhiteBalance_ROI_Mode_Set(hCam, autoWhiteBalanceROIMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoWhiteBalance_ROI_Mode_Get(peak_camera_handle hCam, peak_auto_feature_roi_mode * autoWhiteBalanceROIMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoWhiteBalance_ROI_Mode_Get)
    {
        return inst.m_peak_IPL_AutoWhiteBalance_ROI_Mode_Get(hCam, autoWhiteBalanceROIMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoWhiteBalance_ROI_Offset_GetRange(peak_camera_handle hCam, peak_position * minAutoWhiteBalanceROIOffset, peak_position * maxAutoWhiteBalanceROIOffset, peak_position * incAutoWhiteBalanceROIOffset)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoWhiteBalance_ROI_Offset_GetRange)
    {
        return inst.m_peak_IPL_AutoWhiteBalance_ROI_Offset_GetRange(hCam, minAutoWhiteBalanceROIOffset, maxAutoWhiteBalanceROIOffset, incAutoWhiteBalanceROIOffset);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoWhiteBalance_ROI_Size_GetRange(peak_camera_handle hCam, peak_size * minAutoWhiteBalanceROISize, peak_size * maxAutoWhiteBalanceROISize, peak_size * incAutoWhiteBalanceROISize)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoWhiteBalance_ROI_Size_GetRange)
    {
        return inst.m_peak_IPL_AutoWhiteBalance_ROI_Size_GetRange(hCam, minAutoWhiteBalanceROISize, maxAutoWhiteBalanceROISize, incAutoWhiteBalanceROISize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoWhiteBalance_ROI_Set(peak_camera_handle hCam, peak_roi autoWhiteBalanceROI)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoWhiteBalance_ROI_Set)
    {
        return inst.m_peak_IPL_AutoWhiteBalance_ROI_Set(hCam, autoWhiteBalanceROI);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoWhiteBalance_ROI_Get(peak_camera_handle hCam, peak_roi * autoWhiteBalanceROI)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoWhiteBalance_ROI_Get)
    {
        return inst.m_peak_IPL_AutoWhiteBalance_ROI_Get(hCam, autoWhiteBalanceROI);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoWhiteBalance_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoWhiteBalanceMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoWhiteBalance_Mode_Set)
    {
        return inst.m_peak_IPL_AutoWhiteBalance_Mode_Set(hCam, autoWhiteBalanceMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoWhiteBalance_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoWhiteBalanceMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoWhiteBalance_Mode_Get)
    {
        return inst.m_peak_IPL_AutoWhiteBalance_Mode_Get(hCam, autoWhiteBalanceMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoWhiteBalance_SkipFrames_Set(peak_camera_handle hCam, uint32_t skipFrames)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoWhiteBalance_SkipFrames_Set)
    {
        return inst.m_peak_IPL_AutoWhiteBalance_SkipFrames_Set(hCam, skipFrames);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoWhiteBalance_SkipFrames_Get(peak_camera_handle hCam, uint32_t * skipFrames)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoWhiteBalance_SkipFrames_Get)
    {
        return inst.m_peak_IPL_AutoWhiteBalance_SkipFrames_Get(hCam, skipFrames);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoWhiteBalance_SkipFrames_GetRange(peak_camera_handle hCam, uint32_t * skipFramesMin, uint32_t * skipFramesMax, uint32_t * skipFramesInc)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoWhiteBalance_SkipFrames_GetRange)
    {
        return inst.m_peak_IPL_AutoWhiteBalance_SkipFrames_GetRange(hCam, skipFramesMin, skipFramesMax, skipFramesInc);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_IPL_AutoFocus_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoFocus_GetAccessStatus)
    {
        return inst.m_peak_IPL_AutoFocus_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoFocus_ROI_Set(peak_camera_handle hCam, const peak_focus_roi * autoFocusROIList, size_t autoFocusROICount)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoFocus_ROI_Set)
    {
        return inst.m_peak_IPL_AutoFocus_ROI_Set(hCam, autoFocusROIList, autoFocusROICount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoFocus_ROI_Get(peak_camera_handle hCam, peak_focus_roi * autoFocusROIList, size_t * autoFocusROICount)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoFocus_ROI_Get)
    {
        return inst.m_peak_IPL_AutoFocus_ROI_Get(hCam, autoFocusROIList, autoFocusROICount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoFocus_Mode_Set(peak_camera_handle hCam, peak_auto_feature_mode autoFocusMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoFocus_Mode_Set)
    {
        return inst.m_peak_IPL_AutoFocus_Mode_Set(hCam, autoFocusMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoFocus_Mode_Get(peak_camera_handle hCam, peak_auto_feature_mode * autoFocusMode)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoFocus_Mode_Get)
    {
        return inst.m_peak_IPL_AutoFocus_Mode_Get(hCam, autoFocusMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoFocus_SearchAlgorithm_Set(peak_camera_handle hCam, peak_auto_focus_search_algorithm searchAlgorithm)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoFocus_SearchAlgorithm_Set)
    {
        return inst.m_peak_IPL_AutoFocus_SearchAlgorithm_Set(hCam, searchAlgorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoFocus_SearchAlgorithm_Get(peak_camera_handle hCam, peak_auto_focus_search_algorithm * searchAlgorithm)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoFocus_SearchAlgorithm_Get)
    {
        return inst.m_peak_IPL_AutoFocus_SearchAlgorithm_Get(hCam, searchAlgorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoFocus_SharpnessAlgorithm_Set(peak_camera_handle hCam, peak_sharpness_algorithm sharpnessAlgorithm)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoFocus_SharpnessAlgorithm_Set)
    {
        return inst.m_peak_IPL_AutoFocus_SharpnessAlgorithm_Set(hCam, sharpnessAlgorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoFocus_SharpnessAlgorithm_Get(peak_camera_handle hCam, peak_sharpness_algorithm * sharpnessAlgorithm)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoFocus_SharpnessAlgorithm_Get)
    {
        return inst.m_peak_IPL_AutoFocus_SharpnessAlgorithm_Get(hCam, sharpnessAlgorithm);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoFocus_Range_Set(peak_camera_handle hCam, uint32_t rangeBegin, uint32_t rangeEnd)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoFocus_Range_Set)
    {
        return inst.m_peak_IPL_AutoFocus_Range_Set(hCam, rangeBegin, rangeEnd);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoFocus_Range_Get(peak_camera_handle hCam, uint32_t * rangeBegin, uint32_t * rangeEnd)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoFocus_Range_Get)
    {
        return inst.m_peak_IPL_AutoFocus_Range_Get(hCam, rangeBegin, rangeEnd);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoFocus_Hysteresis_Set(peak_camera_handle hCam, uint8_t hysteresis)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoFocus_Hysteresis_Set)
    {
        return inst.m_peak_IPL_AutoFocus_Hysteresis_Set(hCam, hysteresis);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoFocus_Hysteresis_Get(peak_camera_handle hCam, uint8_t * hysteresis)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoFocus_Hysteresis_Get)
    {
        return inst.m_peak_IPL_AutoFocus_Hysteresis_Get(hCam, hysteresis);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_AutoFocus_Hysteresis_GetRange(peak_camera_handle hCam, uint8_t * minHysteresis, uint8_t * maxHysteresis, uint8_t * incHysteresis)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_AutoFocus_Hysteresis_GetRange)
    {
        return inst.m_peak_IPL_AutoFocus_Hysteresis_GetRange(hCam, minHysteresis, maxHysteresis, incHysteresis);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_HotpixelCorrection_Sensitivity_Set(peak_camera_handle hCam, peak_hotpixel_correction_sensitivity hotpixelCorrectionSensitivity)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_HotpixelCorrection_Sensitivity_Set)
    {
        return inst.m_peak_IPL_HotpixelCorrection_Sensitivity_Set(hCam, hotpixelCorrectionSensitivity);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_HotpixelCorrection_Sensitivity_Get(peak_camera_handle hCam, peak_hotpixel_correction_sensitivity * hotpixelCorrectionSensitivity)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_HotpixelCorrection_Sensitivity_Get)
    {
        return inst.m_peak_IPL_HotpixelCorrection_Sensitivity_Get(hCam, hotpixelCorrectionSensitivity);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_HotpixelCorrection_GetList(peak_camera_handle hCam, peak_position * hotpixelList, size_t * hotpixelCount)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_HotpixelCorrection_GetList)
    {
        return inst.m_peak_IPL_HotpixelCorrection_GetList(hCam, hotpixelList, hotpixelCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_HotpixelCorrection_SetList(peak_camera_handle hCam, const peak_position * hotpixelList, size_t hotpixelCount)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_HotpixelCorrection_SetList)
    {
        return inst.m_peak_IPL_HotpixelCorrection_SetList(hCam, hotpixelList, hotpixelCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_HotpixelCorrection_ResetList(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_HotpixelCorrection_ResetList)
    {
        return inst.m_peak_IPL_HotpixelCorrection_ResetList(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_HotpixelCorrection_Enable(peak_camera_handle hCam, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_HotpixelCorrection_Enable)
    {
        return inst.m_peak_IPL_HotpixelCorrection_Enable(hCam, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_IPL_HotpixelCorrection_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_HotpixelCorrection_IsEnabled)
    {
        return inst.m_peak_IPL_HotpixelCorrection_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Mirror_UpDown_Enable(peak_camera_handle hCam, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Mirror_UpDown_Enable)
    {
        return inst.m_peak_IPL_Mirror_UpDown_Enable(hCam, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_IPL_Mirror_UpDown_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Mirror_UpDown_IsEnabled)
    {
        return inst.m_peak_IPL_Mirror_UpDown_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Mirror_LeftRight_Enable(peak_camera_handle hCam, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Mirror_LeftRight_Enable)
    {
        return inst.m_peak_IPL_Mirror_LeftRight_Enable(hCam, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_IPL_Mirror_LeftRight_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Mirror_LeftRight_IsEnabled)
    {
        return inst.m_peak_IPL_Mirror_LeftRight_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ProcessFrame(peak_camera_handle hCam, peak_frame_handle hFrame, peak_frame_handle * hResultFrame)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ProcessFrame)
    {
        return inst.m_peak_IPL_ProcessFrame(hCam, hFrame, hResultFrame);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ProcessFrameInplace(peak_camera_handle hCam, peak_frame_handle hFrame)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ProcessFrameInplace)
    {
        return inst.m_peak_IPL_ProcessFrameInplace(hCam, hFrame);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ReadImage(peak_camera_handle hCam, const char * path, peak_frame_handle * hFrame)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ReadImage)
    {
        return inst.m_peak_IPL_ReadImage(hCam, path, hFrame);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_EdgeEnhancement_Enable(peak_camera_handle hCam, peak_bool enable)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_EdgeEnhancement_Enable)
    {
        return inst.m_peak_IPL_EdgeEnhancement_Enable(hCam, enable);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_IPL_EdgeEnhancement_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_EdgeEnhancement_IsEnabled)
    {
        return inst.m_peak_IPL_EdgeEnhancement_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_EdgeEnhancement_Factor_Set(peak_camera_handle hCam, uint32_t factor)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_EdgeEnhancement_Factor_Set)
    {
        return inst.m_peak_IPL_EdgeEnhancement_Factor_Set(hCam, factor);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_EdgeEnhancement_Factor_Get(peak_camera_handle hCam, uint32_t * factor)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_EdgeEnhancement_Factor_Get)
    {
        return inst.m_peak_IPL_EdgeEnhancement_Factor_Get(hCam, factor);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_EdgeEnhancement_Factor_GetDefault(peak_camera_handle hCam, uint32_t * defaultFactor)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_EdgeEnhancement_Factor_GetDefault)
    {
        return inst.m_peak_IPL_EdgeEnhancement_Factor_GetDefault(hCam, defaultFactor);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_EdgeEnhancement_Factor_GetRange(peak_camera_handle hCam, uint32_t * minFactor, uint32_t * maxFactor, uint32_t * incFactor)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_EdgeEnhancement_Factor_GetRange)
    {
        return inst.m_peak_IPL_EdgeEnhancement_Factor_GetRange(hCam, minFactor, maxFactor, incFactor);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Sharpness_Measure(peak_frame_handle hFrame, peak_roi roi, peak_sharpness_algorithm sharpnessAlgorithm, double * calculatedValue)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Sharpness_Measure)
    {
        return inst.m_peak_IPL_Sharpness_Measure(hFrame, roi, sharpnessAlgorithm, calculatedValue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Sharpness_GetList(peak_sharpness_algorithm sharpnessAlgorithm, peak_pixel_format * pixelFormatList, size_t * pixelFormatSize)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Sharpness_GetList)
    {
        return inst.m_peak_IPL_Sharpness_GetList(sharpnessAlgorithm, pixelFormatList, pixelFormatSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Rotation_Angle_Set(peak_camera_handle hCam, int32_t rotationAngle)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Rotation_Angle_Set)
    {
        return inst.m_peak_IPL_Rotation_Angle_Set(hCam, rotationAngle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Rotation_Angle_Get(peak_camera_handle hCam, int32_t * rotationAngle)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Rotation_Angle_Get)
    {
        return inst.m_peak_IPL_Rotation_Angle_Get(hCam, rotationAngle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Histogram_ProcessFrame(peak_frame_handle hFrame, peak_histogram_handle * hHistogram)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Histogram_ProcessFrame)
    {
        return inst.m_peak_IPL_Histogram_ProcessFrame(hFrame, hHistogram);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Histogram_Release(peak_histogram_handle hHistogram)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Histogram_Release)
    {
        return inst.m_peak_IPL_Histogram_Release(hHistogram);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Histogram_Channel_GetCount(peak_histogram_handle hHistogram, size_t * numChannels)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Histogram_Channel_GetCount)
    {
        return inst.m_peak_IPL_Histogram_Channel_GetCount(hHistogram, numChannels);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Histogram_Channel_GetInfo(peak_histogram_handle hHistogram, size_t channel, peak_histogram_channel_info * channelInfo)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Histogram_Channel_GetInfo)
    {
        return inst.m_peak_IPL_Histogram_Channel_GetInfo(hHistogram, channel, channelInfo);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Histogram_Channel_GetBinArray(peak_histogram_handle hHistogram, size_t channel, uint64_t * binArray, size_t * binArraySize)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Histogram_Channel_GetBinArray)
    {
        return inst.m_peak_IPL_Histogram_Channel_GetBinArray(hHistogram, channel, binArray, binArraySize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_VideoWriter_Open(peak_video_handle * hVideo, const char * fileName, peak_video_container container, peak_video_encoder encoder)
{
    auto& inst = instance();
    if(inst.m_peak_VideoWriter_Open)
    {
        return inst.m_peak_VideoWriter_Open(hVideo, fileName, container, encoder);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_VideoWriter_Close(peak_video_handle hVideo)
{
    auto& inst = instance();
    if(inst.m_peak_VideoWriter_Close)
    {
        return inst.m_peak_VideoWriter_Close(hVideo);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_VideoWriter_AddFrame(peak_video_handle hVideo, peak_frame_handle hFrame)
{
    auto& inst = instance();
    if(inst.m_peak_VideoWriter_AddFrame)
    {
        return inst.m_peak_VideoWriter_AddFrame(hVideo, hFrame);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_VideoWriter_Container_GetEncoderList(peak_video_container container, peak_video_encoder * encoderList, size_t * encoderCount)
{
    auto& inst = instance();
    if(inst.m_peak_VideoWriter_Container_GetEncoderList)
    {
        return inst.m_peak_VideoWriter_Container_GetEncoderList(container, encoderList, encoderCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_VideoWriter_Encoder_GetPixelFormatList(peak_video_encoder encoder, peak_pixel_format * pixelFormatList, size_t * pixelFormatCount)
{
    auto& inst = instance();
    if(inst.m_peak_VideoWriter_Encoder_GetPixelFormatList)
    {
        return inst.m_peak_VideoWriter_Encoder_GetPixelFormatList(encoder, pixelFormatList, pixelFormatCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_VideoWriter_Encoder_GetContainerList(peak_video_encoder encoder, peak_video_container * containerList, size_t * containerCount)
{
    auto& inst = instance();
    if(inst.m_peak_VideoWriter_Encoder_GetContainerList)
    {
        return inst.m_peak_VideoWriter_Encoder_GetContainerList(encoder, containerList, containerCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_VideoWriter_GetInfo(peak_video_handle hVideo, peak_video_info * videoInfo)
{
    auto& inst = instance();
    if(inst.m_peak_VideoWriter_GetInfo)
    {
        return inst.m_peak_VideoWriter_GetInfo(hVideo, videoInfo);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_VideoWriter_Container_Option_Set(peak_video_handle hVideo, peak_video_container_option containerOption, const void * value, size_t count)
{
    auto& inst = instance();
    if(inst.m_peak_VideoWriter_Container_Option_Set)
    {
        return inst.m_peak_VideoWriter_Container_Option_Set(hVideo, containerOption, value, count);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_VideoWriter_Container_Option_Get(peak_video_handle hVideo, peak_video_container_option containerOption, void * value, size_t count, size_t * outCount)
{
    auto& inst = instance();
    if(inst.m_peak_VideoWriter_Container_Option_Get)
    {
        return inst.m_peak_VideoWriter_Container_Option_Get(hVideo, containerOption, value, count, outCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_VideoWriter_Encoder_Option_Set(peak_video_handle hVideo, peak_video_encoder_option encoderOption, const void * value, size_t count)
{
    auto& inst = instance();
    if(inst.m_peak_VideoWriter_Encoder_Option_Set)
    {
        return inst.m_peak_VideoWriter_Encoder_Option_Set(hVideo, encoderOption, value, count);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_VideoWriter_Encoder_Option_Get(peak_video_handle hVideo, peak_video_encoder_option encoderOption, void * value, size_t count, size_t * outCount)
{
    auto& inst = instance();
    if(inst.m_peak_VideoWriter_Encoder_Option_Get)
    {
        return inst.m_peak_VideoWriter_Encoder_Option_Get(hVideo, encoderOption, value, count, outCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_VideoWriter_WaitUntilQueueEmpty(peak_video_handle hVideo, int32_t timeout_ms)
{
    auto& inst = instance();
    if(inst.m_peak_VideoWriter_WaitUntilQueueEmpty)
    {
        return inst.m_peak_VideoWriter_WaitUntilQueueEmpty(hVideo, timeout_ms);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Inference_CNN_Open(const char * path, peak_inference_handle * hInference)
{
    auto& inst = instance();
    if(inst.m_peak_Inference_CNN_Open)
    {
        return inst.m_peak_Inference_CNN_Open(path, hInference);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Inference_CNN_Close(peak_inference_handle hInference)
{
    auto& inst = instance();
    if(inst.m_peak_Inference_CNN_Close)
    {
        return inst.m_peak_Inference_CNN_Close(hInference);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Inference_CNN_ProcessFrame(peak_inference_handle hInference, peak_frame_handle hFrame, peak_inference_result_handle * hInferenceHandle)
{
    auto& inst = instance();
    if(inst.m_peak_Inference_CNN_ProcessFrame)
    {
        return inst.m_peak_Inference_CNN_ProcessFrame(hInference, hFrame, hInferenceHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Inference_CNN_Info_Get(peak_inference_handle hInference, peak_inference_info * info)
{
    auto& inst = instance();
    if(inst.m_peak_Inference_CNN_Info_Get)
    {
        return inst.m_peak_Inference_CNN_Info_Get(hInference, info);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Inference_Result_Get(peak_inference_result_handle hInferenceHandle, peak_inference_result_data * result)
{
    auto& inst = instance();
    if(inst.m_peak_Inference_Result_Get)
    {
        return inst.m_peak_Inference_Result_Get(hInferenceHandle, result);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Inference_Result_Release(peak_inference_result_handle hInferenceHandle)
{
    auto& inst = instance();
    if(inst.m_peak_Inference_Result_Release)
    {
        return inst.m_peak_Inference_Result_Release(hInferenceHandle);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Inference_Result_Classification_GetList(peak_inference_result_handle hInferenceHandle, peak_inference_result_classification * resultList, size_t * resultCount)
{
    auto& inst = instance();
    if(inst.m_peak_Inference_Result_Classification_GetList)
    {
        return inst.m_peak_Inference_Result_Classification_GetList(hInferenceHandle, resultList, resultCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Inference_Result_Detection_GetList(peak_inference_result_handle hInferenceHandle, peak_inference_result_detection * resultList, size_t * resultCount)
{
    auto& inst = instance();
    if(inst.m_peak_Inference_Result_Detection_GetList)
    {
        return inst.m_peak_Inference_Result_Detection_GetList(hInferenceHandle, resultList, resultCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Inference_Statistics_Get(peak_inference_handle hInference, peak_inference_statistics * statistics)
{
    auto& inst = instance();
    if(inst.m_peak_Inference_Statistics_Get)
    {
        return inst.m_peak_Inference_Statistics_Get(hInference, statistics);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Inference_Statistics_Reset(peak_inference_handle hInference)
{
    auto& inst = instance();
    if(inst.m_peak_Inference_Statistics_Reset)
    {
        return inst.m_peak_Inference_Statistics_Reset(hInference);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Inference_ConfidenceThreshold_Get(peak_inference_handle hInference, uint32_t * threshold)
{
    auto& inst = instance();
    if(inst.m_peak_Inference_ConfidenceThreshold_Get)
    {
        return inst.m_peak_Inference_ConfidenceThreshold_Get(hInference, threshold);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Inference_ConfidenceThreshold_GetRange(peak_inference_handle hInference, uint32_t * minThreshold, uint32_t * maxThreshold, uint32_t * incThreshold)
{
    auto& inst = instance();
    if(inst.m_peak_Inference_ConfidenceThreshold_GetRange)
    {
        return inst.m_peak_Inference_ConfidenceThreshold_GetRange(hInference, minThreshold, maxThreshold, incThreshold);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Inference_ConfidenceThreshold_Set(peak_inference_handle hInference, uint32_t threshold)
{
    auto& inst = instance();
    if(inst.m_peak_Inference_ConfidenceThreshold_Set)
    {
        return inst.m_peak_Inference_ConfidenceThreshold_Set(hInference, threshold);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_Create(peak_message_queue_handle * hMessageQueue)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_Create)
    {
        return inst.m_peak_MessageQueue_Create(hMessageQueue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_Destroy(peak_message_queue_handle hMessageQueue)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_Destroy)
    {
        return inst.m_peak_MessageQueue_Destroy(hMessageQueue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_EnableMessage(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type messageType)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_EnableMessage)
    {
        return inst.m_peak_MessageQueue_EnableMessage(hMessageQueue, hCam, messageType);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_DisableMessage(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type messageType)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_DisableMessage)
    {
        return inst.m_peak_MessageQueue_DisableMessage(hMessageQueue, hCam, messageType);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_EnableMessageList(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, const peak_message_type * messageTypesArray, size_t messageTypesArraySize)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_EnableMessageList)
    {
        return inst.m_peak_MessageQueue_EnableMessageList(hMessageQueue, hCam, messageTypesArray, messageTypesArraySize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_DisableMessageList(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, const peak_message_type * messageTypesArray, size_t messageTypesArraySize)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_DisableMessageList)
    {
        return inst.m_peak_MessageQueue_DisableMessageList(hMessageQueue, hCam, messageTypesArray, messageTypesArraySize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_EnabledMessages_GetList(peak_message_queue_handle hMessageQueue, peak_message_type * messageTypesArray, size_t * messageTypesArraySize)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_EnabledMessages_GetList)
    {
        return inst.m_peak_MessageQueue_EnabledMessages_GetList(hMessageQueue, messageTypesArray, messageTypesArraySize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_SetMode(peak_message_queue_handle hMessageQueue, peak_message_queue_mode queueMode)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_SetMode)
    {
        return inst.m_peak_MessageQueue_SetMode(hMessageQueue, queueMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_GetMode(peak_message_queue_handle hMessageQueue, peak_message_queue_mode * queueMode)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_GetMode)
    {
        return inst.m_peak_MessageQueue_GetMode(hMessageQueue, queueMode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_Start(peak_message_queue_handle hMessageQueue)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_Start)
    {
        return inst.m_peak_MessageQueue_Start(hMessageQueue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_Stop(peak_message_queue_handle hMessageQueue)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_Stop)
    {
        return inst.m_peak_MessageQueue_Stop(hMessageQueue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_MessageQueue_IsStarted(peak_message_queue_handle hMessageQueue)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_IsStarted)
    {
        return inst.m_peak_MessageQueue_IsStarted(hMessageQueue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_WaitForMessage(peak_message_queue_handle hMessageQueue, uint32_t timeout_ms, peak_message_handle * hMessage)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_WaitForMessage)
    {
        return inst.m_peak_MessageQueue_WaitForMessage(hMessageQueue, timeout_ms, hMessage);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_Flush(peak_message_queue_handle hMessageQueue)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_Flush)
    {
        return inst.m_peak_MessageQueue_Flush(hMessageQueue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_MessageQueue_IsMessageSupported(peak_message_queue_handle hMessageQueue, peak_camera_handle hCam, peak_message_type messageType)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_IsMessageSupported)
    {
        return inst.m_peak_MessageQueue_IsMessageSupported(hMessageQueue, hCam, messageType);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_Statistics_Get(peak_message_queue_handle hMessageQueue, peak_message_queue_statistics_info * messageQueueInfo)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_Statistics_Get)
    {
        return inst.m_peak_MessageQueue_Statistics_Get(hMessageQueue, messageQueueInfo);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_Statistics_Reset(peak_message_queue_handle hMessageQueue)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_Statistics_Reset)
    {
        return inst.m_peak_MessageQueue_Statistics_Reset(hMessageQueue);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_MaxQueueSize_Set(peak_message_queue_handle hMessageQueue, size_t messageQueueMaxSize)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_MaxQueueSize_Set)
    {
        return inst.m_peak_MessageQueue_MaxQueueSize_Set(hMessageQueue, messageQueueMaxSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_MessageQueue_MaxQueueSize_Get(peak_message_queue_handle hMessageQueue, size_t * messageQueueMaxSize)
{
    auto& inst = instance();
    if(inst.m_peak_MessageQueue_MaxQueueSize_Get)
    {
        return inst.m_peak_MessageQueue_MaxQueueSize_Get(hMessageQueue, messageQueueMaxSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_Release(peak_message_handle hMessage)
{
    auto& inst = instance();
    if(inst.m_peak_Message_Release)
    {
        return inst.m_peak_Message_Release(hMessage);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_GetInfo(peak_message_handle hMessage, peak_message_info * messageInfo)
{
    auto& inst = instance();
    if(inst.m_peak_Message_GetInfo)
    {
        return inst.m_peak_Message_GetInfo(hMessage, messageInfo);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_Type_Get(peak_message_handle hMessage, peak_message_type * messageType)
{
    auto& inst = instance();
    if(inst.m_peak_Message_Type_Get)
    {
        return inst.m_peak_Message_Type_Get(hMessage, messageType);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_CameraHandle_Get(peak_message_handle hMessage, peak_camera_handle * hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Message_CameraHandle_Get)
    {
        return inst.m_peak_Message_CameraHandle_Get(hMessage, hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_ID_Get(peak_message_handle hMessage, uint64_t * messageID)
{
    auto& inst = instance();
    if(inst.m_peak_Message_ID_Get)
    {
        return inst.m_peak_Message_ID_Get(hMessage, messageID);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_HostTimestamp_Get(peak_message_handle hMessage, uint64_t * hostTimestamp_ns)
{
    auto& inst = instance();
    if(inst.m_peak_Message_HostTimestamp_Get)
    {
        return inst.m_peak_Message_HostTimestamp_Get(hMessage, hostTimestamp_ns);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_Data_Type_Get(peak_message_handle hMessage, peak_message_data_type * messageType)
{
    auto& inst = instance();
    if(inst.m_peak_Message_Data_Type_Get)
    {
        return inst.m_peak_Message_Data_Type_Get(hMessage, messageType);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_Data_RemoteDevice_Get(peak_message_handle hMessage, peak_message_data_remote_device * message)
{
    auto& inst = instance();
    if(inst.m_peak_Message_Data_RemoteDevice_Get)
    {
        return inst.m_peak_Message_Data_RemoteDevice_Get(hMessage, message);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_Data_RemoteDeviceError_Get(peak_message_handle hMessage, peak_message_data_remote_device_error * message)
{
    auto& inst = instance();
    if(inst.m_peak_Message_Data_RemoteDeviceError_Get)
    {
        return inst.m_peak_Message_Data_RemoteDeviceError_Get(hMessage, message);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_Data_RemoteDeviceDropped_Get(peak_message_handle hMessage, peak_message_data_remote_device_dropped * message)
{
    auto& inst = instance();
    if(inst.m_peak_Message_Data_RemoteDeviceDropped_Get)
    {
        return inst.m_peak_Message_Data_RemoteDeviceDropped_Get(hMessage, message);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_Data_RemoteDeviceFrame_Get(peak_message_handle hMessage, peak_message_data_remote_device_frame * message)
{
    auto& inst = instance();
    if(inst.m_peak_Message_Data_RemoteDeviceFrame_Get)
    {
        return inst.m_peak_Message_Data_RemoteDeviceFrame_Get(hMessage, message);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_Data_RemoteDeviceTemperature_Get(peak_message_handle hMessage, peak_message_data_remote_device_temperature * message)
{
    auto& inst = instance();
    if(inst.m_peak_Message_Data_RemoteDeviceTemperature_Get)
    {
        return inst.m_peak_Message_Data_RemoteDeviceTemperature_Get(hMessage, message);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_Data_AutoFocusData_Get(peak_message_handle hMessage, peak_message_data_autofocus * message)
{
    auto& inst = instance();
    if(inst.m_peak_Message_Data_AutoFocusData_Get)
    {
        return inst.m_peak_Message_Data_AutoFocusData_Get(hMessage, message);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_Data_DeviceFound_Get(peak_message_handle hMessage, peak_message_data_device_found * message)
{
    auto& inst = instance();
    if(inst.m_peak_Message_Data_DeviceFound_Get)
    {
        return inst.m_peak_Message_Data_DeviceFound_Get(hMessage, message);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_Data_DeviceLost_Get(peak_message_handle hMessage, peak_message_data_device_lost * message)
{
    auto& inst = instance();
    if(inst.m_peak_Message_Data_DeviceLost_Get)
    {
        return inst.m_peak_Message_Data_DeviceLost_Get(hMessage, message);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_Data_DeviceReconnected_Get(peak_message_handle hMessage, peak_message_data_device_reconnected * message)
{
    auto& inst = instance();
    if(inst.m_peak_Message_Data_DeviceReconnected_Get)
    {
        return inst.m_peak_Message_Data_DeviceReconnected_Get(hMessage, message);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_Data_DeviceDisconnected_Get(peak_message_handle hMessage, peak_message_data_device_disconnected * message)
{
    auto& inst = instance();
    if(inst.m_peak_Message_Data_DeviceDisconnected_Get)
    {
        return inst.m_peak_Message_Data_DeviceDisconnected_Get(hMessage, message);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Message_Data_FirmwareUpdate_Get(peak_message_handle hMessage, peak_message_data_firmware_update * message)
{
    auto& inst = instance();
    if(inst.m_peak_Message_Data_FirmwareUpdate_Get)
    {
        return inst.m_peak_Message_Data_FirmwareUpdate_Get(hMessage, message);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_Create(peak_camera_handle hCam, peak_i2c_handle * hI2C)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_Create)
    {
        return inst.m_peak_I2C_Create(hCam, hI2C);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_Destroy(peak_i2c_handle hI2C)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_Destroy)
    {
        return inst.m_peak_I2C_Destroy(hI2C);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_I2C_GetAccessStatus(peak_i2c_handle hI2C)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_GetAccessStatus)
    {
        return inst.m_peak_I2C_GetAccessStatus(hI2C);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_Mode_GetList(peak_i2c_handle hI2C, peak_i2c_mode * modeList, size_t * modeListSize)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_Mode_GetList)
    {
        return inst.m_peak_I2C_Mode_GetList(hI2C, modeList, modeListSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_Mode_Set(peak_i2c_handle hI2C, peak_i2c_mode mode)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_Mode_Set)
    {
        return inst.m_peak_I2C_Mode_Set(hI2C, mode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_Mode_Get(peak_i2c_handle hI2C, peak_i2c_mode * mode)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_Mode_Get)
    {
        return inst.m_peak_I2C_Mode_Get(hI2C, mode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_DeviceAddress_GetRange(peak_i2c_handle hI2C, uint32_t * minAddress, uint32_t * maxAddress)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_DeviceAddress_GetRange)
    {
        return inst.m_peak_I2C_DeviceAddress_GetRange(hI2C, minAddress, maxAddress);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_DeviceAddress_Set(peak_i2c_handle hI2C, uint32_t address)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_DeviceAddress_Set)
    {
        return inst.m_peak_I2C_DeviceAddress_Set(hI2C, address);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_DeviceAddress_Get(peak_i2c_handle hI2C, uint32_t * address)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_DeviceAddress_Get)
    {
        return inst.m_peak_I2C_DeviceAddress_Get(hI2C, address);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_RegisterAddress_Length_GetList(peak_i2c_handle hI2C, peak_i2c_register_address_length * i2cLengthList, size_t * i2cLengthCount)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_RegisterAddress_Length_GetList)
    {
        return inst.m_peak_I2C_RegisterAddress_Length_GetList(hI2C, i2cLengthList, i2cLengthCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_RegisterAddress_Length_Set(peak_i2c_handle hI2C, peak_i2c_register_address_length length)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_RegisterAddress_Length_Set)
    {
        return inst.m_peak_I2C_RegisterAddress_Length_Set(hI2C, length);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_RegisterAddress_Length_Get(peak_i2c_handle hI2C, peak_i2c_register_address_length * length)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_RegisterAddress_Length_Get)
    {
        return inst.m_peak_I2C_RegisterAddress_Length_Get(hI2C, length);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_RegisterAddress_Endianness_Set(peak_i2c_handle hI2C, peak_endianness endianness)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_RegisterAddress_Endianness_Set)
    {
        return inst.m_peak_I2C_RegisterAddress_Endianness_Set(hI2C, endianness);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_RegisterAddress_Endianness_Get(peak_i2c_handle hI2C, peak_endianness * endianness)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_RegisterAddress_Endianness_Get)
    {
        return inst.m_peak_I2C_RegisterAddress_Endianness_Get(hI2C, endianness);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_RegisterAddress_Set(peak_i2c_handle hI2C, uint32_t address)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_RegisterAddress_Set)
    {
        return inst.m_peak_I2C_RegisterAddress_Set(hI2C, address);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_RegisterAddress_Get(peak_i2c_handle hI2C, uint32_t * address)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_RegisterAddress_Get)
    {
        return inst.m_peak_I2C_RegisterAddress_Get(hI2C, address);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_AckPolling_Enable(peak_i2c_handle hI2C, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_AckPolling_Enable)
    {
        return inst.m_peak_I2C_AckPolling_Enable(hI2C, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_I2C_AckPolling_IsEnabled(peak_i2c_handle hI2C)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_AckPolling_IsEnabled)
    {
        return inst.m_peak_I2C_AckPolling_IsEnabled(hI2C);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_I2C_AckPolling_Timeout_GetAccessStatus(peak_i2c_handle hI2C)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_AckPolling_Timeout_GetAccessStatus)
    {
        return inst.m_peak_I2C_AckPolling_Timeout_GetAccessStatus(hI2C);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_AckPolling_Timeout_GetRange(peak_i2c_handle hI2C, uint32_t * minTimeout_ms, uint32_t * maxTimeout_ms, uint32_t * incTimeout_ms)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_AckPolling_Timeout_GetRange)
    {
        return inst.m_peak_I2C_AckPolling_Timeout_GetRange(hI2C, minTimeout_ms, maxTimeout_ms, incTimeout_ms);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_AckPolling_Timeout_Set(peak_i2c_handle hI2C, uint32_t timeout_ms)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_AckPolling_Timeout_Set)
    {
        return inst.m_peak_I2C_AckPolling_Timeout_Set(hI2C, timeout_ms);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_AckPolling_Timeout_Get(peak_i2c_handle hI2C, uint32_t * timeout_ms)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_AckPolling_Timeout_Get)
    {
        return inst.m_peak_I2C_AckPolling_Timeout_Get(hI2C, timeout_ms);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_Data_Write(peak_i2c_handle hI2C, const uint8_t * data, size_t dataSize)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_Data_Write)
    {
        return inst.m_peak_I2C_Data_Write(hI2C, data, dataSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_Data_Read(peak_i2c_handle hI2C, size_t maxDataSize, uint8_t * data, size_t * dataSize)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_Data_Read)
    {
        return inst.m_peak_I2C_Data_Read(hI2C, maxDataSize, data, dataSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_Data_MaxSize_Get(peak_i2c_handle hI2C, size_t * maxDataSize)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_Data_MaxSize_Get)
    {
        return inst.m_peak_I2C_Data_MaxSize_Get(hI2C, maxDataSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_I2C_OperationStatus_Get(peak_i2c_handle hI2C, peak_i2c_operation_status * operationStatus)
{
    auto& inst = instance();
    if(inst.m_peak_I2C_OperationStatus_Get)
    {
        return inst.m_peak_I2C_OperationStatus_Get(hI2C, operationStatus);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ImageWriter_Create(peak_imagewriter_handle * hImageWriter)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ImageWriter_Create)
    {
        return inst.m_peak_IPL_ImageWriter_Create(hImageWriter);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ImageWriter_Destroy(peak_imagewriter_handle hImageWriter)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ImageWriter_Destroy)
    {
        return inst.m_peak_IPL_ImageWriter_Destroy(hImageWriter);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ImageWriter_Save(peak_imagewriter_handle hImageWriter, peak_frame_handle hFrame, const char * fileName)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ImageWriter_Save)
    {
        return inst.m_peak_IPL_ImageWriter_Save(hImageWriter, hFrame, fileName);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ImageWriter_Format_Set(peak_imagewriter_handle hImageWriter, peak_imagefile_format imageFormat)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ImageWriter_Format_Set)
    {
        return inst.m_peak_IPL_ImageWriter_Format_Set(hImageWriter, imageFormat);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ImageWriter_Format_Get(peak_imagewriter_handle hImageWriter, peak_imagefile_format * imageFormat)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ImageWriter_Format_Get)
    {
        return inst.m_peak_IPL_ImageWriter_Format_Get(hImageWriter, imageFormat);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ImageWriter_Compression_Set(peak_imagewriter_handle hImageWriter, uint32_t compression)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ImageWriter_Compression_Set)
    {
        return inst.m_peak_IPL_ImageWriter_Compression_Set(hImageWriter, compression);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_ImageWriter_Compression_Get(peak_imagewriter_handle hImageWriter, uint32_t * compression)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_ImageWriter_Compression_Get)
    {
        return inst.m_peak_IPL_ImageWriter_Compression_Get(hImageWriter, compression);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Binning_FactorX_GetList(peak_camera_handle hCam, uint32_t * binningFactorXList, size_t * binningFactorXCount)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Binning_FactorX_GetList)
    {
        return inst.m_peak_IPL_Binning_FactorX_GetList(hCam, binningFactorXList, binningFactorXCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Binning_FactorY_GetList(peak_camera_handle hCam, uint32_t * binningFactorYList, size_t * binningFactorYCount)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Binning_FactorY_GetList)
    {
        return inst.m_peak_IPL_Binning_FactorY_GetList(hCam, binningFactorYList, binningFactorYCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Binning_Set(peak_camera_handle hCam, uint32_t binningFactorX, uint32_t binningFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Binning_Set)
    {
        return inst.m_peak_IPL_Binning_Set(hCam, binningFactorX, binningFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Binning_Get(peak_camera_handle hCam, uint32_t * binningFactorX, uint32_t * binningFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Binning_Get)
    {
        return inst.m_peak_IPL_Binning_Get(hCam, binningFactorX, binningFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Decimation_FactorX_GetList(peak_camera_handle hCam, uint32_t * decimationFactorXList, size_t * decimationFactorXCount)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Decimation_FactorX_GetList)
    {
        return inst.m_peak_IPL_Decimation_FactorX_GetList(hCam, decimationFactorXList, decimationFactorXCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Decimation_FactorY_GetList(peak_camera_handle hCam, uint32_t * decimationFactorYList, size_t * decimationFactorYCount)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Decimation_FactorY_GetList)
    {
        return inst.m_peak_IPL_Decimation_FactorY_GetList(hCam, decimationFactorYList, decimationFactorYCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Decimation_Set(peak_camera_handle hCam, uint32_t decimationFactorX, uint32_t decimationFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Decimation_Set)
    {
        return inst.m_peak_IPL_Decimation_Set(hCam, decimationFactorX, decimationFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_Decimation_Get(peak_camera_handle hCam, uint32_t * decimationFactorX, uint32_t * decimationFactorY)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_Decimation_Get)
    {
        return inst.m_peak_IPL_Decimation_Get(hCam, decimationFactorX, decimationFactorY);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_FirmwareUpdate_CompatibleCameraList_Get(const char * gufFileName, peak_camera_descriptor * cameraList, size_t * cameraCount)
{
    auto& inst = instance();
    if(inst.m_peak_FirmwareUpdate_CompatibleCameraList_Get)
    {
        return inst.m_peak_FirmwareUpdate_CompatibleCameraList_Get(gufFileName, cameraList, cameraCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_FirmwareUpdate_Execute(peak_camera_id cameraID, const char * gufFileName)
{
    auto& inst = instance();
    if(inst.m_peak_FirmwareUpdate_Execute)
    {
        return inst.m_peak_FirmwareUpdate_Execute(cameraID, gufFileName);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_TestPattern_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_TestPattern_GetAccessStatus)
    {
        return inst.m_peak_TestPattern_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_TestPattern_Set(peak_camera_handle hCam, peak_test_pattern pattern)
{
    auto& inst = instance();
    if(inst.m_peak_TestPattern_Set)
    {
        return inst.m_peak_TestPattern_Set(hCam, pattern);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_TestPattern_Get(peak_camera_handle hCam, peak_test_pattern * pattern)
{
    auto& inst = instance();
    if(inst.m_peak_TestPattern_Get)
    {
        return inst.m_peak_TestPattern_Get(hCam, pattern);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_TestPattern_GetList(peak_camera_handle hCam, peak_test_pattern * testPatternList, size_t * testPatternCount)
{
    auto& inst = instance();
    if(inst.m_peak_TestPattern_GetList)
    {
        return inst.m_peak_TestPattern_GetList(hCam, testPatternList, testPatternCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_LED_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_LED_GetAccessStatus)
    {
        return inst.m_peak_LED_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_LED_Target_GetList(peak_camera_handle hCam, peak_led_target * targetList, size_t * targetCount)
{
    auto& inst = instance();
    if(inst.m_peak_LED_Target_GetList)
    {
        return inst.m_peak_LED_Target_GetList(hCam, targetList, targetCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_LED_Mode_GetList(peak_camera_handle hCam, peak_led_target target, peak_led_mode * modeList, size_t * ledModeCount)
{
    auto& inst = instance();
    if(inst.m_peak_LED_Mode_GetList)
    {
        return inst.m_peak_LED_Mode_GetList(hCam, target, modeList, ledModeCount);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_LED_Set(peak_camera_handle hCam, peak_led_target target, peak_led_mode mode)
{
    auto& inst = instance();
    if(inst.m_peak_LED_Set)
    {
        return inst.m_peak_LED_Set(hCam, target, mode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_LED_Get(peak_camera_handle hCam, peak_led_target target, peak_led_mode * mode)
{
    auto& inst = instance();
    if(inst.m_peak_LED_Get)
    {
        return inst.m_peak_LED_Get(hCam, target, mode);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_BlackLevel_Auto_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_BlackLevel_Auto_GetAccessStatus)
    {
        return inst.m_peak_BlackLevel_Auto_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_BlackLevel_Auto_Enable(peak_camera_handle hCam, peak_bool enable)
{
    auto& inst = instance();
    if(inst.m_peak_BlackLevel_Auto_Enable)
    {
        return inst.m_peak_BlackLevel_Auto_Enable(hCam, enable);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_BlackLevel_Auto_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_BlackLevel_Auto_IsEnabled)
    {
        return inst.m_peak_BlackLevel_Auto_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_BlackLevel_Offset_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_BlackLevel_Offset_GetAccessStatus)
    {
        return inst.m_peak_BlackLevel_Offset_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_BlackLevel_Offset_Set(peak_camera_handle hCam, double offset)
{
    auto& inst = instance();
    if(inst.m_peak_BlackLevel_Offset_Set)
    {
        return inst.m_peak_BlackLevel_Offset_Set(hCam, offset);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_BlackLevel_Offset_Get(peak_camera_handle hCam, double * offset)
{
    auto& inst = instance();
    if(inst.m_peak_BlackLevel_Offset_Get)
    {
        return inst.m_peak_BlackLevel_Offset_Get(hCam, offset);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_BlackLevel_Offset_GetRange(peak_camera_handle hCam, double * min, double * max, double * inc)
{
    auto& inst = instance();
    if(inst.m_peak_BlackLevel_Offset_GetRange)
    {
        return inst.m_peak_BlackLevel_Offset_GetRange(hCam, min, max, inc);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Bandwidth_LinkSpeed_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Bandwidth_LinkSpeed_GetAccessStatus)
    {
        return inst.m_peak_Bandwidth_LinkSpeed_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Bandwidth_LinkSpeed_Get(peak_camera_handle hCam, int64_t * linkSpeed_Bps)
{
    auto& inst = instance();
    if(inst.m_peak_Bandwidth_LinkSpeed_Get)
    {
        return inst.m_peak_Bandwidth_LinkSpeed_Get(hCam, linkSpeed_Bps);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Bandwidth_ThroughputLimit_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Bandwidth_ThroughputLimit_GetAccessStatus)
    {
        return inst.m_peak_Bandwidth_ThroughputLimit_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Bandwidth_ThroughputLimit_GetRange(peak_camera_handle hCam, int64_t * minThroughputLimit_Bps, int64_t * maxThroughputLimit_Bps, int64_t * incThroughputLimit_Bps)
{
    auto& inst = instance();
    if(inst.m_peak_Bandwidth_ThroughputLimit_GetRange)
    {
        return inst.m_peak_Bandwidth_ThroughputLimit_GetRange(hCam, minThroughputLimit_Bps, maxThroughputLimit_Bps, incThroughputLimit_Bps);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Bandwidth_ThroughputLimit_Set(peak_camera_handle hCam, int64_t throughputLimit_Bps)
{
    auto& inst = instance();
    if(inst.m_peak_Bandwidth_ThroughputLimit_Set)
    {
        return inst.m_peak_Bandwidth_ThroughputLimit_Set(hCam, throughputLimit_Bps);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Bandwidth_ThroughputLimit_Get(peak_camera_handle hCam, int64_t * throughputLimit_Bps)
{
    auto& inst = instance();
    if(inst.m_peak_Bandwidth_ThroughputLimit_Get)
    {
        return inst.m_peak_Bandwidth_ThroughputLimit_Get(hCam, throughputLimit_Bps);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Bandwidth_ThroughputFrameRateLimit_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Bandwidth_ThroughputFrameRateLimit_GetAccessStatus)
    {
        return inst.m_peak_Bandwidth_ThroughputFrameRateLimit_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Bandwidth_ThroughputFrameRateLimit_Get(peak_camera_handle hCam, double * throughputFrameRateLimit_fps)
{
    auto& inst = instance();
    if(inst.m_peak_Bandwidth_ThroughputFrameRateLimit_Get)
    {
        return inst.m_peak_Bandwidth_ThroughputFrameRateLimit_Get(hCam, throughputFrameRateLimit_fps);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Bandwidth_ThroughputCalculated_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Bandwidth_ThroughputCalculated_GetAccessStatus)
    {
        return inst.m_peak_Bandwidth_ThroughputCalculated_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Bandwidth_ThroughputCalculated_Get(peak_camera_handle hCam, int64_t * throughputCalculated_Bps)
{
    auto& inst = instance();
    if(inst.m_peak_Bandwidth_ThroughputCalculated_Get)
    {
        return inst.m_peak_Bandwidth_ThroughputCalculated_Get(hCam, throughputCalculated_Bps);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_IPO_GetAccessStatus(peak_interface_technology interfaceTech)
{
    auto& inst = instance();
    if(inst.m_peak_IPO_GetAccessStatus)
    {
        return inst.m_peak_IPO_GetAccessStatus(interfaceTech);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_IPO_IsEnabled(peak_interface_technology interfaceTech)
{
    auto& inst = instance();
    if(inst.m_peak_IPO_IsEnabled)
    {
        return inst.m_peak_IPO_IsEnabled(interfaceTech);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPO_Enable(peak_interface_technology interfaceTech, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_IPO_Enable)
    {
        return inst.m_peak_IPO_Enable(interfaceTech, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_DigitalBlack_GetRange(peak_camera_handle hCam, double * minDigitalBlack, double * maxDigitalBlack)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_DigitalBlack_GetRange)
    {
        return inst.m_peak_IPL_DigitalBlack_GetRange(hCam, minDigitalBlack, maxDigitalBlack);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_DigitalBlack_Set(peak_camera_handle hCam, double digitalBlack)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_DigitalBlack_Set)
    {
        return inst.m_peak_IPL_DigitalBlack_Set(hCam, digitalBlack);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_DigitalBlack_Get(peak_camera_handle hCam, double * digitalBlack)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_DigitalBlack_Get)
    {
        return inst.m_peak_IPL_DigitalBlack_Get(hCam, digitalBlack);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_LUT_Enable(peak_camera_handle hCam, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_LUT_Enable)
    {
        return inst.m_peak_IPL_LUT_Enable(hCam, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_IPL_LUT_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_LUT_IsEnabled)
    {
        return inst.m_peak_IPL_LUT_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_LUT_Preset_Set(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_preset preset)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_LUT_Preset_Set)
    {
        return inst.m_peak_IPL_LUT_Preset_Set(hCam, selector, preset);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_LUT_Value_Set(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_channel channel, uint32_t index, uint32_t value)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_LUT_Value_Set)
    {
        return inst.m_peak_IPL_LUT_Value_Set(hCam, selector, channel, index, value);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_LUT_Value_Get(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_channel channel, uint32_t index, uint32_t * value)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_LUT_Value_Get)
    {
        return inst.m_peak_IPL_LUT_Value_Get(hCam, selector, channel, index, value);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_LUT_ValueList_Set(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_channel channel, uint32_t * values, size_t size)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_LUT_ValueList_Set)
    {
        return inst.m_peak_IPL_LUT_ValueList_Set(hCam, selector, channel, values, size);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_IPL_LUT_ValueList_Get(peak_camera_handle hCam, peak_lut_selector selector, peak_lut_channel channel, uint32_t * values, size_t * size)
{
    auto& inst = instance();
    if(inst.m_peak_IPL_LUT_ValueList_Get)
    {
        return inst.m_peak_IPL_LUT_ValueList_Get(hCam, selector, channel, values, size);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Chunks_GetAccessStatus(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_GetAccessStatus)
    {
        return inst.m_peak_Chunks_GetAccessStatus(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_Enable(peak_camera_handle hCam, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_Enable)
    {
        return inst.m_peak_Chunks_Enable(hCam, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_Chunks_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_IsEnabled)
    {
        return inst.m_peak_Chunks_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_AutoUpdate_Enable(peak_camera_handle hCam, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_AutoUpdate_Enable)
    {
        return inst.m_peak_Chunks_AutoUpdate_Enable(hCam, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_Chunks_AutoUpdate_IsEnabled(peak_camera_handle hCam)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_AutoUpdate_IsEnabled)
    {
        return inst.m_peak_Chunks_AutoUpdate_IsEnabled(hCam);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_Update(peak_camera_handle hCam, peak_frame_handle hFrame)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_Update)
    {
        return inst.m_peak_Chunks_Update(hCam, hFrame);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_ACCESS_STATUS DynamicLoader::peak_Chunks_Type_GetAccessStatus(peak_camera_handle hCam, peak_chunks_type type)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_Type_GetAccessStatus)
    {
        return inst.m_peak_Chunks_Type_GetAccessStatus(hCam, type);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_Type_Enable(peak_camera_handle hCam, peak_chunks_type type, peak_bool enabled)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_Type_Enable)
    {
        return inst.m_peak_Chunks_Type_Enable(hCam, type, enabled);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_BOOL DynamicLoader::peak_Chunks_Type_IsEnabled(peak_camera_handle hCam, peak_chunks_type type)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_Type_IsEnabled)
    {
        return inst.m_peak_Chunks_Type_IsEnabled(hCam, type);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_Type_Supported_GetList(peak_camera_handle hCam, peak_chunks_type * chunksTypesSupported, size_t * chunksTypesSize)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_Type_Supported_GetList)
    {
        return inst.m_peak_Chunks_Type_Supported_GetList(hCam, chunksTypesSupported, chunksTypesSize);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_FrameInfo_Get(peak_camera_handle hCam, peak_chunks_frame_info * data)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_FrameInfo_Get)
    {
        return inst.m_peak_Chunks_FrameInfo_Get(hCam, data);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_Exposure_Get(peak_camera_handle hCam, peak_chunks_exposure * data)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_Exposure_Get)
    {
        return inst.m_peak_Chunks_Exposure_Get(hCam, data);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_Gain_Get(peak_camera_handle hCam, peak_chunks_gain * data)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_Gain_Get)
    {
        return inst.m_peak_Chunks_Gain_Get(hCam, data);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_Sequencer_Get(peak_camera_handle hCam, peak_chunks_sequencer * data)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_Sequencer_Get)
    {
        return inst.m_peak_Chunks_Sequencer_Get(hCam, data);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_Timestamp_Get(peak_camera_handle hCam, peak_chunks_timestamp * data)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_Timestamp_Get)
    {
        return inst.m_peak_Chunks_Timestamp_Get(hCam, data);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_ExposureTrigger_Get(peak_camera_handle hCam, peak_chunks_exposure_trigger * data)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_ExposureTrigger_Get)
    {
        return inst.m_peak_Chunks_ExposureTrigger_Get(hCam, data);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_UsableROI_Get(peak_camera_handle hCam, peak_chunks_usable_roi * data)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_UsableROI_Get)
    {
        return inst.m_peak_Chunks_UsableROI_Get(hCam, data);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_LineStatus_Get(peak_camera_handle hCam, peak_chunks_line_status * data)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_LineStatus_Get)
    {
        return inst.m_peak_Chunks_LineStatus_Get(hCam, data);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_AutoFeature_Get(peak_camera_handle hCam, peak_chunks_autofeature * data)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_AutoFeature_Get)
    {
        return inst.m_peak_Chunks_AutoFeature_Get(hCam, data);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

inline PEAK_API_STATUS DynamicLoader::peak_Chunks_PTPStatus_Get(peak_camera_handle hCam, peak_chunks_ptp_status * data)
{
    auto& inst = instance();
    if(inst.m_peak_Chunks_PTPStatus_Get)
    {
        return inst.m_peak_Chunks_PTPStatus_Get(hCam, data);
    }
    else
    {
        throw std::runtime_error("Library not loaded!");
    }
}

} /* namespace dynamic */
} /* namespace peak_comfortC */
