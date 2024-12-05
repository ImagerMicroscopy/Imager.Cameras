#include "SCConfigure.h"

#ifdef WITH_IDS_PEAK

#include <memory>
#include <stdexcept>

#include "IDSPeakCameraHandler.h"

#endif

std::vector<std::shared_ptr<BaseCameraClass>> OpenIDSPeakCameras() {
    peak_status status = PEAK_STATUS_SUCCESS;

    status = peak_Library_Init();
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't init IDS Peak library");
    }

    size_t nCameras = 20;
    std::unique_ptr<peak_camera_descriptor[]> camDescriptors(new peak_camera_descriptor[20]);
    status = peak_CameraList_Get(camDescriptors.get(), &nCameras);




    return std::vector<std::shared_ptr<BaseCameraClass>>();
}

void CloseIDSPeakLibrary() {
    peak_Library_Exit();
}
