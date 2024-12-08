#include "SCConfigure.h"

#ifdef WITH_IDS_PEAK

#include <memory>
#include <stdexcept>

#include "IDSPeakCameraHandler.h"
#include "IDSPeakCamera.h"

std::vector<std::shared_ptr<BaseCameraClass>> OpenIDSPeakCameras() {
    peak_status status = PEAK_STATUS_SUCCESS;

    status = peak_Library_Init();
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't init IDS Peak library");
    }

    status = peak_CameraList_Update(nullptr);

    size_t nCameras = 20;
    std::unique_ptr<peak_camera_descriptor[]> camDescriptors(new peak_camera_descriptor[20]);
    status = peak_CameraList_Get(camDescriptors.get(), &nCameras);
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't list cameras");
    }

    std::vector<std::shared_ptr<BaseCameraClass>> cameras;
    for (size_t i = 0; i < nCameras; ++i) {
        cameras.emplace_back(new IDSPeakCamera(camDescriptors[i]));
    }

    return cameras;
}

void CloseIDSPeakLibrary() {
    peak_Library_Exit();
}

#endif
