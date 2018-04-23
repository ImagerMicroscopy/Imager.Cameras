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
		camPtr->acquireImages(nImages, nImagesToAverage, buffer);
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int StartAsyncAcquisition(char* cameraName, unsigned int nImagesToAverage, uint16_t* buffer, uint64_t bufferSizeinBytes) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		std::pair<int, int> size = camPtr->getActualImageSize();
		uint64_t imageSize = (uint64_t)size.first * (uint64_t)size.second * sizeof(uint16_t);
		if ((bufferSizeinBytes == 0) || ((bufferSizeinBytes % imageSize) != 0))
			return BUFFER_SIZE_MUST_BE_MULTIPLE_OF_IMAGE_SIZE;
		int nImagesInBuffer = bufferSizeinBytes / imageSize;
		if (nImagesInBuffer < 2)
			return TOO_FEW_IMAGES_IN_BUFFER;
		camPtr->startAsyncAcquisition(BaseCameraClass::AcqFreeRunMode, nImagesToAverage, buffer, nImagesInBuffer);
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
        camPtr->abortAsyncAquisitionIfRunning();
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

void RotateImageCW(uint16_t* image, int nRows, int nCols, uint16_t* rotatedImage, int* newNRows, int* newNCols) {
    RotateCW(image, nRows,  nCols, rotatedImage);
    *newNRows = nCols;
    *newNCols = nRows;
}

void RotateImageCCW(uint16_t *image, int nRows, int nCols, uint16_t* rotatedImage, int* newNRows, int* newNCols) {
    RotateCCW(image, nRows, nCols, rotatedImage);
    *newNRows = nCols;
    *newNCols = nRows;
}

void FlipImageHorizontal(uint16_t *image, int nRows, int nCols, uint16_t* flippedImage, int* newNRows, int* newNCols) {
    FlipHorizontal(image, nRows, nCols, flippedImage);
    *newNRows = nRows;
    *newNCols = nCols;
}

void FlipImageVertical(uint16_t *image, int nRows, int nCols, uint16_t* flippedImage, int* newNRows, int* newNCols) {
    FlipVertical(image, nRows, nCols, flippedImage);
    *newNRows = nRows;
    *newNCols = nCols;
}
