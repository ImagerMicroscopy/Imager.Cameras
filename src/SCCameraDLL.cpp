#define COMPILING_SCCameraDLL_H
#include "SCCameraDLL.h"

#include "CameraManager.h"
#include "BaseCameraClass.h"
#include <memory>
#include <string>
#include <algorithm>

CameraManager* gCameraManager = nullptr;
void StopCameraManager();
bool StartCameraManager() {
	try {
		if (gCameraManager == nullptr) {
			gCameraManager = new CameraManager();
			gCameraManager->discoverCameras();
		}
	}
	catch (std::runtime_error e) {
		StopCameraManager();
	}

	return (gCameraManager != nullptr);
}

void StopCameraManager() {
	if (gCameraManager != nullptr) {
		delete gCameraManager;
		gCameraManager = nullptr;
	}
}

bool CameraManagerIsAvailable() {
	return StartCameraManager();
}

bool gHaveInit = false;

int InitCameraDLL() {
	StartCameraManager();
	if (CameraManagerIsAvailable()) {
		gHaveInit = true;
		return 0;
	} else {
		return GENERIC_ERROR;
	}
}

void ShutdownCameraDLL() {
	StopCameraManager();
	gHaveInit = false;
}

int ListConnectedCameraNames(char** namesPtr) {
	if (!gHaveInit)
		return NO_INIT;
	
	std::vector<std::string> cameraIdentifiers = gCameraManager->getCameraIdentifiers();
	for (size_t i = 0; i < std::min(cameraIdentifiers.size(), (size_t)MAX_CAMERAS); ++i) {
		strncpy(namesPtr[i], cameraIdentifiers[i].c_str(), MAX_CAMERA_NAME_LENGTH);
		namesPtr[MAX_CAMERA_NAME_LENGTH] = 0;
	}
	return cameraIdentifiers.size();
}

int GetSensorDimensions(char *cameraName, int* rows, int* cols) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		std::pair<int, int> size = camPtr->getSensorSize();
		*rows = size.first;
		*cols = size.second;
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int SetEMGain(char *cameraName, double emGain) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		camPtr->setEMGain(emGain);
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int ReadEMGain(char* cameraName, double* emGain) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		*emGain = camPtr->getEMGain();
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int ReadEMGainRange(char* cameraName, double* minGain, double* maxGain) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		camPtr->getAllowableEMGains(minGain, maxGain);
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int SetExposureTime(char* cameraName, double exposureTime) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		camPtr->setExposureTime(exposureTime);
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int ReadExposureTime(char* cameraName, double* exposureTime) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		*exposureTime = camPtr->getExposureTime();
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int ReadExposureTimeRange(char* cameraName, double* minExposureTime, double* maxExposureTime) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		camPtr->getAllowableExposureTimes(minExposureTime, maxExposureTime);
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int AcquireImages(char* cameraName, int nImages, uint16_t* buffer, uint64_t bufferSizeinBytes) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		std::pair<int, int> size = camPtr->getSensorSize();
		uint64_t requiredBufferSize = (uint64_t)size.first * (uint64_t)size.second * sizeof(uint16_t);
		if (bufferSizeinBytes < requiredBufferSize)
			return GENERIC_ERROR;
		camPtr->acquireImages(nImages, buffer);
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int StartAsyncAcquisition(char* cameraName, uint16_t* buffer, uint64_t bufferSizeinBytes) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		std::pair<int, int> size = camPtr->getSensorSize();
		uint64_t imageSize = (uint64_t)size.first * (uint64_t)size.second * sizeof(uint16_t);
		if ((bufferSizeinBytes == 0) || ((bufferSizeinBytes % imageSize) != 0))
			return BUFFER_SIZE_MUST_BE_MULTIPLE_OF_IMAGE_SIZE;
		int nImagesInBuffer = bufferSizeinBytes / imageSize;
		if (nImagesInBuffer < 2)
			return TOO_FEW_IMAGES_IN_BUFFER;
		camPtr->startAsyncAcquisition(true, buffer, nImagesInBuffer);
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int GetIndexOfLastImageAsyncAcquired(char* cameraName, int* indexOfLastImageAcquired) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		*indexOfLastImageAcquired = camPtr->getIndexOfLastImageAsyncAcquired();
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int AbortAsyncAcquisition(char* cameraName) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		camPtr->abortAsyncAquisition();
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}
