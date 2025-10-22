#define COMPILING_IMAGERPLUGIN
#include "SCCameraPlugin.h"
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
#include "SCConfigurationFile.h"
#include "SCPrinter.h"

const char* gEquipmentName = "SCCamera";        // adjust to the name of your equipment
std::string gLastError = std::string();
CameraManager* gCameraManager = nullptr;

// Define a function to handle exceptions
int HandleExceptions(const std::function<void()>& func) {
    try {
        func(); // Execute the provided function
        return 0; // Return success
    } catch (const std::exception& e) {
        Print(e.what());
        gLastError = e.what();
        return -1; // Return error code
    }
}

int StoreStringListInBuffers(const std::vector<std::string>& stringList, char** stringBuffers, int nBuffers, int maxNBytesPerName);

void StopCameraManager() {
    if (gCameraManager != nullptr) {
        delete gCameraManager;
        gCameraManager = nullptr;
    }
}

void StartCameraManager(const fs::path& configDirPath) {
    if (gCameraManager == nullptr) {
        SCConfigurationFile configFile(configDirPath);
        gCameraManager = new CameraManager(configFile);
        gCameraManager->discoverCameras();
    }
}

bool gHaveInit = false;

int InitImagerPlugin(char* configurationDirPath, void(*printer)(const char*)) {
    if (printer == nullptr) {
        return -1;
    }

    return HandleExceptions([&]() {
        InitPrinter([=] (const std::string& str) -> void {
            printer(str.c_str());
        });

        StartCameraManager(configurationDirPath);
        gHaveInit = true;
    });
}

void ShutdownImagerPlugin() {
    StopCameraManager();
    gHaveInit = false;
}

int ImagerPluginAPIVersion(int* version) {
    *version = IMAGER_API_VERSION;
    return 0;
}

int EquipmentName(char* name, int maxNBytesPerName) {
    return HandleExceptions([&]() {
        if (strlen(gEquipmentName) > maxNBytesPerName - 1) {
            throw std::runtime_error("buffer too small to store the EquipmentName");
        }
        snprintf(name, maxNBytesPerName, "%s", gEquipmentName);
    });
}

int ListAvailableLightSources(char **namesPtr, int nNames, int maxNBytesPerName, int *nNamesReturned) {
    *nNamesReturned = 0;
    return 0;
}

int ListAvailableChannels(char *lightSourceName, char **namesPtr, int nNames, int maxNBytesPerName,
                          int *nNamesReturned, int *canControlPower, int *allowMultipleChannelsAtOnce) {
    return -1;
}

int ActivateLightSource(char *lightSourceName, char **channelNames, double *illuminationPowers, int nChannels) {
    return -1;
}

int DeactivateLightSource() {
    return HandleExceptions([&]() {
        // call a function you created
    });
}

int ListDiscreteMovableComponents(char **namesPtr, int nNames, int maxNBytesPerName, int *nNamesReturned) {
    *nNamesReturned = 0;
    return 0;
}

int ListContinuouslyMovableComponents(char **namesPtr, int nNames, int maxNBytesPerName, int *nNamesReturned) {
    *nNamesReturned = 0;
    return 0;
}

int ListDiscreteMovableComponentSettings(char *discreteComponentName, char **namesPtr, int nNames,
                                         int maxNBytesPerName, int *nNamesReturned) {
    return -1;
}

int ListContinuouslyMovableComponentRange(char *discreteComponentName, double *minValue, double *maxValue,
    double *increment) {
        // set minValue, maxValue, and increment as appropriate
    return -1;
}

int SetMovableComponents(int nDiscreteComponentNames, char **discreteComponentNames, char **discreteSettings,
                         int nContinuousComponentNames, char **continuousComponentNames, double *continuousSettings) {
    return -1;
}

int HasMotorizedStage(int *hasIt) {
    *hasIt = 0;
    return 0;
}

int MotorizedStageName(char *name, int maxNBytesPerName) {
    return -1;
}

int SupportedStageAxes(int *x, int *y, int *z) {
    return -1;
}

int GetStagePosition(double *x, double *y, double *z, int *usingHardwareAF, int *afOffset) {
    return -1;
}

int SetStagePosition(double x, double y, double z, int usingHardwareAF, int afOffset) {
    return -1;
}

int ListRobots(char** namesPtr, int nNames, int maxNBytesPerName, int* nNamesReturned) {
    return HandleExceptions([&] {
        std::vector<std::string> robotNames; // = <a call to a function you created>
        *nNamesReturned = StoreStringListInBuffers(robotNames, namesPtr, nNames, maxNBytesPerName);
        if (*nNamesReturned != robotNames.size()) {
            throw std::runtime_error("Could not return all available robots");
        }
    });
}

int ListRobotPrograms(char* robotName, char** namesPtr, int nNames, int maxNBytesPerName, int* nNamesReturned) {
    return HandleExceptions([&] {
        std::vector<std::string> programNames; // = <a call to a function you created>
        *nNamesReturned = StoreStringListInBuffers(programNames, namesPtr, nNames, maxNBytesPerName);
        if (*nNamesReturned != programNames.size()) {
            throw std::runtime_error("Could not return all available robot programs");
        }
    });
}

int ListRobotProgramArgumentsInfo(char* robotName, char* programName, char** encodedArgumentsPtr) {
    return HandleExceptions([&]() {

    });
}

void ReleaseRobotProgramArgumentsInfo(char* data) {

}

int ExecuteRobotProgram(char* robotName, char* programName, char* programArguments) {
    return HandleExceptions([&]() {

    });
}

int StopRobot() {
    return HandleExceptions([&]() {

    });
}

int ListConnectedCameraNames(char **namesPtr, int nNames, int maxNBytesPerName, int *nNamesReturned) {
    return HandleExceptions([&]() {
        if (!gHaveInit) {
            throw std::logic_error("The camera manager is unavailable");
        }
        std::vector<std::string> cameraIdentifiers = gCameraManager->getCameraIdentifiers();
        *nNamesReturned = StoreStringListInBuffers(cameraIdentifiers, namesPtr, nNames, maxNBytesPerName);
        if (*nNamesReturned != (int)cameraIdentifiers.size()) {
            throw std::runtime_error("Can't return all camera names");
        }
    });
}

int GetCameraOptions(char* cameraName, char** encodedOptionsPtr) {
    return HandleExceptions([&]() {
        if (!gHaveInit) {
            throw std::logic_error("The camera manager is unavailable");
        }

        *encodedOptionsPtr = nullptr;
        std::shared_ptr<BaseCameraClass> camPtr = gCameraManager->getCamera(cameraName);
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
    return HandleExceptions([&]() {
        if (!gHaveInit) {
            throw std::logic_error("The camera manager is unavailable");
        }
        std::shared_ptr<BaseCameraClass> camPtr = gCameraManager->getCamera(cameraName);
        camPtr->setCameraProperties({ CameraProperty::decodeFromJSONObject(nlohmann::json::parse(encodedOption)) });
    });
}

int GetFrameRate(char* cameraName, double* frameRate) {
    return HandleExceptions([&]() {
        if (!gHaveInit) {
            throw std::logic_error("The camera manager is unavailable");
        }
        std::shared_ptr<BaseCameraClass> camPtr = gCameraManager->getCamera(cameraName);
        *frameRate = camPtr->getFrameRate();
    });
}

int IsConfiguredForHardwareTriggering(char* cameraName, int* isConfiguredForHardwareTriggering) {
    return HandleExceptions([&]() {
        if (!gHaveInit) {
            throw std::logic_error("The camera manager is unavailable");
        }
        std::shared_ptr<BaseCameraClass> camPtr = gCameraManager->getCamera(cameraName);
        *isConfiguredForHardwareTriggering = camPtr->isConfiguredForHardwareTriggering();
    });
}

std::vector<std::shared_ptr<std::uint16_t>> gImagesInFlight;
std::mutex gImagesInFlightMutex;

int AcquireSingleImage(char* cameraName, uint16_t** imagePtr, int* nRows, int* nCols) {
    return HandleExceptions([&]() {
        if (!gHaveInit) {
            throw std::logic_error("The camera manager is unavailable");
        }
        std::shared_ptr<BaseCameraClass> camPtr = gCameraManager->getCamera(cameraName);

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
    return HandleExceptions([&]() {
        if (!gHaveInit) {
            throw std::logic_error("The camera manager is unavailable");
        }
        StartBoundedAsyncAcquisition(cameraName, std::numeric_limits<std::uint64_t>::max());
    });
}

int StartBoundedAsyncAcquisition(char* cameraName, std::uint64_t nImagesToAcquire) {
    return HandleExceptions([&]() {
        if (!gHaveInit) {
            throw std::logic_error("The camera manager is unavailable");
        }
        std::shared_ptr<BaseCameraClass> camPtr = gCameraManager->getCamera(cameraName);
        camPtr->startAsyncAcquisition(BaseCameraClass::AcqFillAndStop, nImagesToAcquire);
    });
}

int GetOldestImageAsyncAcquired(char* cameraName, uint32_t timeoutMillis, uint16_t** imagePtr, int* nRows, int* nCols, double* timeStamp) {
    return HandleExceptions([&]() {
        if (!gHaveInit) {
            throw std::logic_error("The camera manager is unavailable");
        }
        std::shared_ptr<BaseCameraClass> camPtr = gCameraManager->getCamera(cameraName);
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
    HandleExceptions([&]() {
        if (!gHaveInit) {
            throw std::logic_error("The camera manager is unavailable");
        }
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
    return HandleExceptions([&]() {
        if (!gHaveInit) {
            throw std::logic_error("The camera manager is unavailable");
        }
        std::shared_ptr<BaseCameraClass> camPtr = gCameraManager->getCamera(cameraName);
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

// A utility function to store a list of strings in buffers passed by Imager. Returns the number of items actually stored.
int StoreStringListInBuffers(const std::vector<std::string>& stringList, char** stringBuffers, int nBuffers, int maxNBytesPerName) {
    int nStrings = (int)stringList.size();
    int nItemsToStore = std::min(nStrings, nBuffers);
    for (int i = 0; i < nItemsToStore; ++i) {
        const std::string& item = stringList.at(i);
        if (item.size() > maxNBytesPerName - 1) {   // '-1' so the trailing nil can be stored
            throw std::runtime_error("buffer too small to store item \"" + item + "\"");
        }
        snprintf(stringBuffers[i], maxNBytesPerName, "%s", item.c_str());
    }
    return nItemsToStore;
}

