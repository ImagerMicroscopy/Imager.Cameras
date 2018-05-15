#define COMPILING_SCCameraDLL_H
#include "SCCameraDLL.h"

#include "CameraManager.h"
#include "BaseCameraClass.h"
#include <memory>
#include <string>
#include <algorithm>

#include "ImageProcessingUtils.h"

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

int SetImageOrientation(char* cameraName, int* orientationOps, int nOps) {
    try {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);

        std::vector<std::shared_ptr<ImageProcessingDescriptor>> ops;
        for (int i = 0; i < nOps; i += 1) {
            switch (orientationOps[i]) {
                case kRotateCW:
                    ops.push_back(std::shared_ptr<ImageProcessingDescriptor>(new IPDRotateCW));
                    break;
                case kRotateCCW:
                    ops.push_back(std::shared_ptr<ImageProcessingDescriptor>(new IPDRotateCCW));
                    break;
                case kFlipHorizontal:
                    ops.push_back(std::shared_ptr<ImageProcessingDescriptor>(new IPDFlipHorizontal));
                    break;
                case kFlipVertical:
                    ops.push_back(std::shared_ptr<ImageProcessingDescriptor>(new IPDFlipVertical));
                    break;
                default:
                    return -1;
                    break;
            }
        }

        camPtr->setImageOrientationOps(ops);
    }
    catch (...) {
        return GENERIC_ERROR;
    }
    return 0;
}

int GetImageDimensions(char *cameraName, int* rows, int* cols) {
    if (!gHaveInit)
        return NO_INIT;

    try {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        std::pair<int, int> size = camPtr->getActualImageSize();
        *rows = size.first;
        *cols = size.second;
    }
    catch (...) {
        return GENERIC_ERROR;
    }
    return 0;
}

int GetAllowedCropSizes(char* cameraName, int* nRowsPtr, int* nColsPtr, int nEntriesInBuffers, int* nCropSizesReturned) {
    if (!gHaveInit)
        return NO_INIT;

    try {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        std::vector<std::pair<int, int>> cropSizes = camPtr->getSupportedCropSizes();
        *nCropSizesReturned = 0;
        for (int i = 0; i < std::min((size_t)nEntriesInBuffers, cropSizes.size()); i += 1) {
            nRowsPtr[i] = cropSizes.at(i).first;
            nColsPtr[i] = cropSizes.at(i).second;
            *nCropSizesReturned += 1;
        }
    }
    catch (...) {
        return GENERIC_ERROR;
    }
    return 0;
}

int SetCropSize(char* cameraName, int nRows, int nCols) {
    if (!gHaveInit)
        return NO_INIT;

    try {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        camPtr->setImageCrop(std::pair<int, int>(nRows, nCols));
    }
    catch (...) {
        return GENERIC_ERROR;
    }
    return 0;
}

int GetAllowedBinningFactors(char* cameraName, int* binningFactors, int nEntriesInBuffer, int* nBinningFactorsReturned) {
    if (!gHaveInit)
        return NO_INIT;

    try {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        std::vector<int> binFactors = camPtr->getSupportedBinningFactors();
        *nBinningFactorsReturned = 0;
        for (int i = 0; i < std::min((size_t)nEntriesInBuffer, binFactors.size()); i += 1) {
            binningFactors[i] = binFactors.at(i);
            *nBinningFactorsReturned += 1;
        }
    }
    catch (...) {
        return GENERIC_ERROR;
    }
    return 0;
}

int SetBinningFactor(char* cameraName, int binningFactor) {
    if (!gHaveInit)
        return NO_INIT;

    try {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        camPtr->setBinningFactor(binningFactor);
    }
    catch (...) {
        return GENERIC_ERROR;
    }
    return 0;
}

int GetBinningFactor(char* cameraName, int* binningFactor) {
    if (!gHaveInit)
        return NO_INIT;

    try {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        *binningFactor = camPtr->getBinningFactor();
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

int SetIntegrationTime(char* cameraName, double exposureTime) {
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

int ReadIntegrationTime(char* cameraName, double* exposureTime) {
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

int ReadIntegrationTimeRange(char* cameraName, double* minExposureTime, double* maxExposureTime) {
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

int SetTemperatureSetpoint(char* cameraName, double temperature) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		camPtr->setTemperature(temperature);
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int ReadCurrentTemperature(char* cameraName, double* temperature) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		*temperature = camPtr->getTemperature();
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int ReadTemperatureSetpoint(char* cameraName, double* temperature) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		*temperature = camPtr->getTemperatureSetpoint();
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int AcquireImages(char* cameraName, int nImages, unsigned int nImagesToAverage, uint16_t* buffer, uint64_t bufferSizeinBytes) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		std::pair<int, int> size = camPtr->getActualImageSize();
		uint64_t requiredBufferSize = (uint64_t)size.first * (uint64_t)size.second * sizeof(uint16_t);
		if (bufferSizeinBytes < requiredBufferSize)
			return GENERIC_ERROR;
		camPtr->acquireImages(nImages, nImagesToAverage, nImages, buffer);
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int StartAsyncAcquisition(char* cameraName, unsigned int nImagesToAverage) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		camPtr->startAsyncAcquisition(BaseCameraClass::AcqFreeRunMode, nImagesToAverage, -1);
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

std::vector<std::shared_ptr<std::uint16_t>> gImagesInFlight;

int GetOldestImageAsyncAcquired(char* cameraName, uint16_t** imagePtr, int* nRows, int* nCols, double* timeStamp) {
    if (!gHaveInit)
        return NO_INIT;

    try {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        std::shared_ptr<std::uint16_t> imageData;
        std::tie(imageData, *nRows, *nCols, *timeStamp) = camPtr->getOldestImageAsyncAcquired();
        *imagePtr = imageData.get();
        if (imageData.get() != nullptr) {
            gImagesInFlight.push_back(imageData);
        }
    }
    catch (...) {
        return GENERIC_ERROR;
    }
    return 0;
}

void ReleaseImageData(uint16_t* imagePtr) {
    if (!gHaveInit)
        return;

    try {
        std::shared_ptr<std::uint16_t> imageData;
        auto it = std::find_if(gImagesInFlight.begin(), gImagesInFlight.end(), [=](const std::shared_ptr<std::uint16_t>& ptr) -> bool {
            return (ptr.get() == imagePtr);
        });
        if (it == gImagesInFlight.end()) {
            throw std::runtime_error("trying to release unknown image pointer");
        }
        gImagesInFlight.erase(it);
    }
    catch (...) {
    }
}

int AbortAsyncAcquisition(char* cameraName) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
        camPtr->abortAsyncAquisitionIfRunning();
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}
