#include "SCConfigure.h"

#ifdef WITH_ANDORSDK3

#include "AndorSDK3CameraHandler.h"

#include <memory>
#include <stdexcept>
#include <vector>

#include "Andor/atcore.h"
#include "Andor/atutility.h"

#include "AndorSDK3Camera.h"

std::vector<std::shared_ptr<BaseCameraClass>> OpenAndorSDK3Cameras() {
    int err = 0;
    err = AT_InitialiseLibrary();
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't initialize Andor 3 library");
    }
    err = AT_InitialiseUtilityLibrary();
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't initialize Andor 3 utility library");
    }

    AT_64 iNumberDevices = 0;
    AT_GetInt(AT_HANDLE_SYSTEM, L"DeviceCount", &iNumberDevices);
    if (err != AT_SUCCESS) {
        throw std::runtime_error("can't get Andor 3 device count");
    }
    
    std::vector<std::shared_ptr<BaseCameraClass>> cameras;
    for (AT_64 i = 0; i < iNumberDevices; ++i) {
        AT_H camHandle = 0;
        err = AT_Open(i, &camHandle);
        if (err != AT_SUCCESS) {
            throw std::runtime_error("can't open Andor3 camera");
        }
        cameras.emplace_back(new AndorSDK3Camera(camHandle));
    }
        
    return cameras;
}

void CloseAndorSDK3Library() {
    AT_FinaliseLibrary();
    AT_FinaliseUtilityLibrary();
}

#endif
