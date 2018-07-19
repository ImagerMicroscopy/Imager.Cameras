#define COMPILING_SCCameraDLL_H
#include "SCCameraDLL.h"

#include "CameraManager.h"
#include "BaseCameraClass.h"
#include <memory>
#include <string>
#include <algorithm>

#include "CameraPropertiesEncoding.h"
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

LIBSPEC int GetCameraOptions(char * cameraName, char ** encodedOptionsPtr) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		*encodedOptionsPtr = nullptr;
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		std::vector<CameraProperty> properties = camPtr->getCameraProperties();
		std::vector<nlohmann::json> encodedProps;
		for (const auto& p : properties) {
			encodedProps.push_back(p.encodeAsJSONObject());
		}
		nlohmann::json object;
		object["properties"] = encodedProps;
		std::string serialized = object.dump();
		*encodedOptionsPtr = new char[serialized.size() + 1];
		memcpy(*encodedOptionsPtr, serialized.data(), serialized.size() + 1);
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

LIBSPEC void ReleaseOptionsData(char* data) {
	delete[] data;
}

LIBSPEC int SetCameraOption(char * cameraName, char * encodedOption) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		camPtr->setCameraProperty(CameraProperty::decodeFromJSONObject(nlohmann::json::parse(encodedOption)));
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int GetFrameRate(char* cameraName, double* frameRate) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		*frameRate = camPtr->getFrameRate();
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
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

std::vector<std::shared_ptr<std::uint16_t>> gImagesInFlight;

int AcquireImages(char* cameraName, int nImages, uint16_t**bufferPtr, int* nRows, int* nCols) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		std::tie(*nRows, *nCols) = camPtr->getActualImageSize();
		size_t nPixels = (size_t)*nRows * (size_t)*nCols * (size_t)nImages;
		std::shared_ptr<std::uint16_t> buffer(new std::uint16_t[nPixels], [](std::uint16_t* ptr) {delete[] ptr; });
		gImagesInFlight.push_back(buffer);
		*bufferPtr = buffer.get();
		camPtr->acquireImages(nImages, *bufferPtr);
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

int StartAsyncAcquisition(char* cameraName) {
	if (!gHaveInit)
		return NO_INIT;

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		std::string identifier(cameraName);
		camPtr = gCameraManager->getCamera(identifier);
		camPtr->startAsyncAcquisition(BaseCameraClass::AcqFreeRunMode, -1);
	}
	catch (...) {
		return GENERIC_ERROR;
	}
	return 0;
}

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
        gImagesInFlight.push_back(imageData);
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
