#include "AndorSDK3CameraHandler.h"

#include <memory>
#include <stdexcept>
#include <vector>

#include "Andor/atcore.h"
#include "Andor/atutility.h"

#include "AndorSDK3Camera.h"
#include "AndorSDK3APIWrapper.h"
#include "SCPrinter.h"

std::vector<std::shared_ptr<BaseCameraClass>> OpenAndorSDK3Cameras() {
    AndorSDK3APIWrapper apiWrapper = GetAndorSDK3APIWrapper();
    if (!apiWrapper.areAllFunctionsLoaded()) {
        return std::vector<std::shared_ptr<BaseCameraClass>>();
    }

    Print("Found Andor3 runtime");

    int err = 0;
    err = apiWrapper.AT_InitialiseLibrary();
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't initialize Andor 3 library");
    }
    err = apiWrapper.AT_InitialiseUtilityLibrary();
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't initialize Andor 3 utility library");
    }

    AT_64 iNumberDevices = 0;
    apiWrapper.AT_GetInt(AT_HANDLE_SYSTEM, L"Device Count", &iNumberDevices);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor 3 device count");
    }
    
    std::vector<std::shared_ptr<BaseCameraClass>> cameras;
    for (int i = 0; i < iNumberDevices; ++i) {
        AT_H camHandle = 0;
        err = apiWrapper.AT_Open(i, &camHandle);
        if (err != AT_SUCCESS) {
            throw std::runtime_error("can't open Andor3 camera");
        }
        cameras.emplace_back(new AndorSDK3Camera(camHandle));
    }
        
    return cameras;
}

void CloseAndorSDK3Library() {
    AndorSDK3APIWrapper apiWrapper = GetAndorSDK3APIWrapper();
    if (apiWrapper.areAllFunctionsLoaded()) {
        apiWrapper.AT_FinaliseLibrary();
        apiWrapper.AT_FinaliseUtilityLibrary();
    }
}
