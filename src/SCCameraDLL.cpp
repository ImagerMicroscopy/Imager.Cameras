#define COMPILING_SCCameraDLL_H
#include "SCCameraDLL.h"

#include "CameraManager.h"
#include "BaseCameraClass.h"
#include <memory>
#include <string>
#include <algorithm>

#include "CameraPropertiesEncoding.h"
#include "ImageProcessingUtils.h"

std::string gLastError = std::string();

CameraManager* gCameraManager = nullptr;
void StopCameraManager();
bool StartCameraManager() {
    try {
        if (gCameraManager == nullptr) {
            gCameraManager = new CameraManager();
            gCameraManager->discoverCameras();
        }
    }
    catch (const std::exception& e) {
        gLastError = e.what();
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
    catch (const std::exception& e) {
        gLastError = e.what();
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
        camPtr->setCameraProperties({ CameraProperty::decodeFromJSONObject(nlohmann::json::parse(encodedOption)) });
    }
    catch (const std::exception& e) {
        gLastError = e.what();
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
    catch (const std::exception& e) {
        gLastError = e.what();
        return GENERIC_ERROR;
    }
    return 0;
}

int IsConfiguredForHardwareTriggering(char* cameraName, int* isConfiguredForHardwareTriggering) {
    if (!gHaveInit)
        return NO_INIT;

    try {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        *isConfiguredForHardwareTriggering = camPtr->isConfiguredForHardwareTriggering();
    }
    catch (const std::exception& e) {
        gLastError = e.what();
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
    catch (const std::exception& e) {
        gLastError = e.what();
        return GENERIC_ERROR;
    }
    return 0;
}

std::vector<std::shared_ptr<std::uint16_t>> gImagesInFlight;
std::mutex gImagesInFlightMutex;

int AcquireSingleImage(char* cameraName, uint16_t** imagePtr, int* nRows, int* nCols) {
    if (!gHaveInit)
        return NO_INIT;

    try {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);

        std::shared_ptr<std::uint16_t> imageData;
        std::tie(imageData, *nRows, *nCols) = camPtr->acquireSingleImage();
        *imagePtr = imageData.get();
        {
            std::lock_guard<std::mutex> lock(gImagesInFlightMutex);
            gImagesInFlight.push_back(imageData);
        }
    }
    catch (const std::exception& e) {
        gLastError = e.what();
        return GENERIC_ERROR;
    }
    return 0;
}

int StartAsyncAcquisition(char* cameraName) {
    return StartBoundedAsyncAcquisition(cameraName, std::numeric_limits<std::uint64_t>::max());
}

int StartBoundedAsyncAcquisition(char* cameraName, std::uint64_t nImagesToAcquire) {
    if (!gHaveInit)
        return NO_INIT;

    try {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        camPtr->startAsyncAcquisition(BaseCameraClass::AcqFillAndStop, nImagesToAcquire);
    }
    catch (const std::exception& e) {
        gLastError = e.what();
        return GENERIC_ERROR;
    }
    return 0;
}

int GetOldestImageAsyncAcquired(char* cameraName, uint32_t timeoutMillis, uint16_t** imagePtr, int* nRows, int* nCols, double* timeStamp) {
    if (!gHaveInit)
        return NO_INIT;

    try {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        std::shared_ptr<std::uint16_t> imageData;
        std::optional<std::tuple<std::shared_ptr<std::uint16_t>, int, int, double>> result = camPtr->getOldestImageAsyncAcquiredWithTimeout(timeoutMillis);
        if (result.has_value()) {	// got image
            std::tie(imageData, *nRows, *nCols, *timeStamp) = result.value();
            *imagePtr = imageData.get();
            {
                std::lock_guard<std::mutex> lock(gImagesInFlightMutex);
                gImagesInFlight.push_back(imageData);
            }
        } else {					// timeout
            *imagePtr = nullptr;
            *nRows = -1;
            *nCols = -1;
            *timeStamp = -1;
        }
        
    }
    catch (const std::exception& e) {
        gLastError = e.what();
        return GENERIC_ERROR;
    }
    return 0;
}

void ReleaseImageData(uint16_t* imagePtr) {
    if (!gHaveInit)
        return;

    try {
        std::shared_ptr<std::uint16_t> imageData;
        {
            std::lock_guard<std::mutex> lock(gImagesInFlightMutex);
            auto it = std::find_if(gImagesInFlight.begin(), gImagesInFlight.end(), [=](const std::shared_ptr<std::uint16_t>& ptr) -> bool {
                return (ptr.get() == imagePtr);
            });
            if (it == gImagesInFlight.end()) {
                throw std::runtime_error("trying to release unknown image pointer");
            }
            gImagesInFlight.erase(it);
        }
    }
    catch (const std::exception& e) {
        gLastError = e.what();
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
    catch (const std::exception& e) {
        gLastError = e.what();
        return GENERIC_ERROR;
    }
    return 0;
}

LIBSPEC void GetLastSCCamError(char* msgBuf, size_t bufSize) {
    if (bufSize > 1) {
        size_t nBytesToCopy = std::min(gLastError.length(), bufSize - 1);
        memcpy(msgBuf, gLastError.c_str(), nBytesToCopy);
        msgBuf[nBytesToCopy] = 0;
    }
}
