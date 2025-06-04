#include "SCConfigure.h"

#ifdef WITH_HAMAMATSU

#include "HamamatsuCameraHandler.h"

#include "windows.h"
#include "Hamamatsu/dcamapi4.h"
#include "HamamatsuAPIWrapper.h"
#include "HamamatsuCamera.h"

std::vector<std::shared_ptr<BaseCameraClass>> OpenHamamatsuCameras() {
    HamamatsuAPIWrapper apiWrapper = GetHamamatsuAPIWrapper();
    if (!apiWrapper.allFunctionsLoaded) {
        return std::vector<std::shared_ptr<BaseCameraClass>>();
    }

    std::vector<std::shared_ptr<BaseCameraClass>> cameras;

    DCAMAPI_INIT paraminit;
    memset(&paraminit, 0, sizeof(paraminit));
    paraminit.size = sizeof(paraminit);
    DCAMERR	dcamErr;
    dcamErr = apiWrapper.dcamapi_init(&paraminit);
    if ((dcamErr != DCAMERR_SUCCESS) && (dcamErr != DCAMERR_NOCAMERA)) {
        throw std::runtime_error("can't initialize DCAM api");
    }
    int nCameras = paraminit.iDeviceCount;
    if (dcamErr == DCAMERR_NOCAMERA) {
        nCameras = 0;
    }
    for (int i = 0; i < nCameras; ++i) {
        DCAMDEV_OPEN devOpen = { 0 };
        devOpen.index = i;
        devOpen.size = sizeof(DCAMDEV_OPEN);
        dcamErr = apiWrapper.dcamdev_open(&devOpen);
        if (dcamErr != DCAMERR_SUCCESS) {
            throw std::runtime_error("unable to open DCAM camera");
        }
        HDCAM hdCam = devOpen.hdcam;
        cameras.emplace_back(new HamamatsuCamera(hdCam));
    }

    return cameras;
}

void CloseHamamatsuLibrary() {
    HamamatsuAPIWrapper apiWrapper = GetHamamatsuAPIWrapper();
    if (apiWrapper.allFunctionsLoaded) {
        apiWrapper.dcamapi_uninit();
    }
}

#endif
