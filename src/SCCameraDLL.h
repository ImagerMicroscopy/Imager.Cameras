#ifndef SCCameraDLL_H
#define SCCameraDLL_H

// http://www.flounder.com/ultimateheaderfile.htm

#if _MSC_VER > 1000
	#pragma once
#endif

#include <cstdint>

#define MAX_CAMERAS 10
#define MAX_CAMERA_NAME_LENGTH 128

#define GENERIC_ERROR -1
#define NO_INIT -2
#define BUFFER_SIZE_MUST_BE_MULTIPLE_OF_IMAGE_SIZE -3
#define TOO_FEW_IMAGES_IN_BUFFER -4
#define BUFFER_SIZE_TOO_SMALL -5

#ifdef COMPILING_SCCameraDLL_H
	#define LIBSPEC __declspec(dllexport)
#else
	#define LIBSPEC __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	LIBSPEC int InitCameraDLL();
	LIBSPEC void ShutdownCameraDLL();

	LIBSPEC int ListConnectedCameraNames(char** namesPtr);

	LIBSPEC int GetCameraOptions(char* cameraName, char** encodedOptionsPtr);
	LIBSPEC void ReleaseOptionsData(char* data);
	LIBSPEC int SetCameraOption(char* cameraName, char* encodedOption);
	LIBSPEC int GetFrameRate(char* cameraName, double* frameRate);
	LIBSPEC int IsConfiguredForHardwareTriggering(char* cameraName, int* isConfiguredForHardwareTriggering);

    LIBSPEC int SetImageOrientation(char* cameraName, int* orientationOps, int nOps);
    
	LIBSPEC int AcquireSingleImage(char* cameraName, uint16_t** imagePtr, int* nRows, int* nCols);

	LIBSPEC int StartAsyncAcquisition(char* cameraName);
	LIBSPEC int StartBoundedAsyncAcquisition(char* cameraName, uint64_t nImagesToAcquire);
    LIBSPEC int GetOldestImageAsyncAcquired(char* cameraName, uint32_t timeoutMillis, uint16_t** imagePtr, int* nRows, int* nCols, double* timeStamp);
    LIBSPEC void ReleaseImageData(uint16_t* imagePtr);
	LIBSPEC int AbortAsyncAcquisition(char* cameraName);

	LIBSPEC void GetLastSCCamError(char* msgBuf, size_t bufSize);
#ifdef __cplusplus
}
#endif

#endif
