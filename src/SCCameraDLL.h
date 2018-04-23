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

    LIBSPEC int SetImageOrientation(char* cameraName, int* orientationOps, int nOps);
	LIBSPEC int GetImageDimensions(char *cameraName, int* rows, int* cols);
    LIBSPEC int GetAllowedCropSizes(char* cameraName, int* nRowsPtr, int* nColsPtr, int nEntriesInBuffers, int* nCropSizesReturned);
    LIBSPEC int SetCropSize(char* cameraName, int nRows, int nCols);
    LIBSPEC int GetAllowedBinningFactors(char* cameraName, int* binningFactors, int nEntriesInBuffer, int* nBinningFactorsReturned);
    LIBSPEC int SetBinningFactor(char* cameraName, int binningFactor);
    LIBSPEC int GetBinningFactor(char* cameraName, int* binningFactor);

	LIBSPEC int SetEMGain(char *cameraName, double emGain);
	LIBSPEC int ReadEMGain(char* cameraName, double* emGain);
	LIBSPEC int ReadEMGainRange(char* cameraName, double* minGain, double* maxGain);
	LIBSPEC int SetIntegrationTime(char* cameraName, double exposureTime);
	LIBSPEC int ReadIntegrationTime(char* cameraName, double* exposureTime);
	LIBSPEC int ReadIntegrationTimeRange(char* cameraName, double* minExposureTime, double* maxExposureTime);
	LIBSPEC int SetTemperatureSetpoint(char* cameraName, double temperature);
	LIBSPEC int ReadCurrentTemperature(char* cameraName, double* temperature);
	LIBSPEC int ReadTemperatureSetpoint(char* cameraName, double* temperature);

	LIBSPEC int AcquireImages(char* cameraName, int nImages, unsigned int nImagesToAverage, uint16_t* buffer, uint64_t bufferSizeinBytes);

	LIBSPEC int StartAsyncAcquisition(char* cameraName, unsigned int nImagesToAverage, uint16_t* buffer, uint64_t bufferSizeinBytes);
	LIBSPEC int GetIndexOfLastImageAsyncAcquired(char* cameraName, int* indexOfLastImageAcquired);
	LIBSPEC int AbortAsyncAcquisition(char* cameraName);

    LIBSPEC void RotateImageCW(uint16_t *image, int nRows, int nCols, uint16_t* rotatedImage, int* newNRows, int* newNCols);
    LIBSPEC void RotateImageCCW(uint16_t *image, int nRows, int nCols, uint16_t* rotatedImage, int* newNRows, int* newNCols);
    LIBSPEC void FlipImageHorizontal(uint16_t *image, int nRows, int nCols, uint16_t* flippedImage, int* newNRows, int* newNCols);
    LIBSPEC void FlipImageVertical(uint16_t *image, int nRows, int nCols, uint16_t* flippedImage, int* newNRows, int* newNCols);

#ifdef __cplusplus
}
#endif

#endif
