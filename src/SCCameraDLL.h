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

	LIBSPEC int GetSensorDimensions(char *cameraName, int* rows, int* cols);
	LIBSPEC int SetEMGain(char *cameraName, double emGain);
	LIBSPEC int ReadEMGain(char* cameraName, double* emGain);
	LIBSPEC int SetExposureTime(char* cameraName, double exposureTime);
	LIBSPEC int ReadExposureTime(char* cameraName, double* exposureTime);

	LIBSPEC int AcquireImages(char* cameraName, int nImages, uint16_t* buffer, uint64_t bufferSize);

	LIBSPEC int StartAsyncAcquisition(char* cameraName, uint16_t* buffer, uint64_t bufferSize);
	LIBSPEC int GetIndexOfLastImageAsyncAcquired(char* cameraName, int* indexOfLastImageAcquired);
	LIBSPEC int StopAsyncAcquisition(char* cameraName, uint16_t buffer);
	LIBSPEC int AbortAsyncAcquisition(char* cameraName);
#ifdef __cplusplus
}
#endif

#endif
