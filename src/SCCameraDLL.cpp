#define COMPILING_SCCameraDLL_H
#include "SCCameraDLL.h"
#include "CameraManager.h"
#include "BaseCameraClass.h"
#include <memory>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include <mutex>
#include <optional>
#include <tuple>
#include <stdexcept>
#include <cstdint>
#include <climits>
#include <iostream>

#include "CameraPropertiesEncoding.h"
#include "ImageProcessingUtils.h"

void (*gPrinterFunc)(const char*) = nullptr;
std::string gLastError = std::string();
CameraManager* gCameraManager = nullptr;

// Define a function to handle exceptions
int HandleExceptions(const std::function<void()>& func) {
    try {
        func(); // Execute the provided function
        return 0; // Return success
    } catch (const std::exception& e) {
        gLastError = e.what();
        return GENERIC_ERROR; // Return error code
    }
}

void StopCameraManager() {
    if (gCameraManager != nullptr) {
        delete gCameraManager;
        gCameraManager = nullptr;
    }
}

bool StartCameraManager() {
    return HandleExceptions([&]() {
        if (gCameraManager == nullptr) {
            gCameraManager = new CameraManager();
            gCameraManager->discoverCameras();
        }
    }) == 0;
}

bool CameraManagerIsAvailable() {
    return StartCameraManager();
}

void PrintOutput(const std::string& output) {
    if (gPrinterFunc != nullptr) {
        gPrinterFunc(output.c_str());
    }
}

bool gHaveInit = false;

int InitCameraDLL(void(*printer)(const char*)) {
    if (printer == nullptr) {
        return -1;
    }
    gPrinterFunc = printer;

    StartCameraManager();
    if (CameraManagerIsAvailable()) {
        gHaveInit = true;
        return 0;
    } else {
        PrintOutput("the camera manager is unavailable");
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
        namesPtr[i][MAX_CAMERA_NAME_LENGTH - 1] = 0; // Ensure null-termination
    }
    return cameraIdentifiers.size();
}

int GetCameraOptions(char* cameraName, char** encodedOptionsPtr) {
    if (!gHaveInit) {
        PrintOutput("didn't init SCCamera");
        return NO_INIT;
    }

    return HandleExceptions([&]() {
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
    });
}

void ReleaseOptionsData(char* data) {
    delete[] data;
}

int SetCameraOption(char* cameraName, char* encodedOption) {
    if (!gHaveInit) {
        PrintOutput("didn't init SCCamera");
        return NO_INIT;
    }

    return HandleExceptions([&]() {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        camPtr->setCameraProperties({ CameraProperty::decodeFromJSONObject(nlohmann::json::parse(encodedOption)) });
    });
}

int GetFrameRate(char* cameraName, double* frameRate) {
    if (!gHaveInit) {
        PrintOutput("didn't init SCCamera");
        return NO_INIT;
    }

    return HandleExceptions([&]() {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        *frameRate = camPtr->getFrameRate();
    });
}

int IsConfiguredForHardwareTriggering(char* cameraName, int* isConfiguredForHardwareTriggering) {
    if (!gHaveInit) {
        PrintOutput("didn't init SCCamera");
        return NO_INIT;
    }

    return HandleExceptions([&]() {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        *isConfiguredForHardwareTriggering = camPtr->isConfiguredForHardwareTriggering();
    });
}

int SetImageOrientation(char* cameraName, int* orientationOps, int nOps) {
    return HandleExceptions([&]() {
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
                    throw std::runtime_error("Invalid orientation operation");
            }
        }

        camPtr->setImageOrientationOps(ops);
    });
}

std::vector<std::shared_ptr<std::uint16_t>> gImagesInFlight;
std::mutex gImagesInFlightMutex;

int AcquireSingleImage(char* cameraName, uint16_t** imagePtr, int* nRows, int* nCols) {
    if (!gHaveInit) {
        PrintOutput("didn't init SCCamera");
        return NO_INIT;
    }

    return HandleExceptions([&]() {
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
    });
}

int StartAsyncAcquisition(char* cameraName) {
    return StartBoundedAsyncAcquisition(cameraName, std::numeric_limits<std::uint64_t>::max());
}

int StartBoundedAsyncAcquisition(char* cameraName, std::uint64_t nImagesToAcquire) {
    if (!gHaveInit) {
        PrintOutput("didn't init SCCamera");
        return NO_INIT;
    }

    return HandleExceptions([&]() {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        camPtr->startAsyncAcquisition(BaseCameraClass::AcqFillAndStop, nImagesToAcquire);
    });
}

int GetOldestImageAsyncAcquired(char* cameraName, uint32_t timeoutMillis, uint16_t** imagePtr, int* nRows, int* nCols, double* timeStamp) {
    if (!gHaveInit) {
        PrintOutput("didn't init SCCamera");
        return NO_INIT;
    }

    return HandleExceptions([&]() {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        std::shared_ptr<std::uint16_t> imageData;
        std::optional<std::tuple<std::shared_ptr<std::uint16_t>, int, int, double>> result = camPtr->getOldestImageAsyncAcquiredWithTimeout(timeoutMillis);
        if (result.has_value()) {
            std::tie(imageData, *nRows, *nCols, *timeStamp) = result.value();
            *imagePtr = imageData.get();
            {
                std::lock_guard<std::mutex> lock(gImagesInFlightMutex);
                gImagesInFlight.push_back(imageData);
            }
        } else {
            *imagePtr = nullptr;
            *nRows = -1;
            *nCols = -1;
            *timeStamp = -1;
        }
    });
}

void ReleaseImageData(uint16_t* imagePtr) {
    if (!gHaveInit) {
        PrintOutput("didn't init SCCamera");
        return;
    }

    HandleExceptions([&]() {
        std::lock_guard<std::mutex> lock(gImagesInFlightMutex);
        auto it = std::find_if(gImagesInFlight.begin(), gImagesInFlight.end(), [=](const std::shared_ptr<std::uint16_t>& ptr) -> bool {
            return (ptr.get() == imagePtr);
        });
        if (it == gImagesInFlight.end()) {
            throw std::runtime_error("trying to release unknown image pointer");
        }
        gImagesInFlight.erase(it);
    });
}

int AbortAsyncAcquisition(char* cameraName) {
    if (!gHaveInit) {
        PrintOutput("didn't init SCCamera");
        return NO_INIT;
    }

    return HandleExceptions([&]() {
        std::shared_ptr<BaseCameraClass> camPtr;
        std::string identifier(cameraName);
        camPtr = gCameraManager->getCamera(identifier);
        camPtr->abortAsyncAcquisitionIfRunning();
    });
}

void GetLastSCCamError(char* msgBuf, size_t bufSize) {
    if (bufSize > 1) {
        size_t nBytesToCopy = std::min(gLastError.length(), bufSize - 1);
        memcpy(msgBuf, gLastError.c_str(), nBytesToCopy);
        msgBuf[nBytesToCopy] = 0;
    }
}
