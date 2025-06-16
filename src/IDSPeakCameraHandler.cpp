#include <memory>
#include <stdexcept>

#include "IDSPeakCameraHandler.h"
#include "IDSPeakCamera.h"
#include "SCPrinter.h"

std::vector<std::shared_ptr<BaseCameraClass>> OpenIDSPeakCameras() {
    IDSPeakAPIWrapper apiWrapper;
    if (!apiWrapper.areAllFunctionsLoaded()) {
        return std::vector<std::shared_ptr<BaseCameraClass>>();
    }

    Print("Found IDS Peak runtime");
    peak_status status = PEAK_STATUS_SUCCESS;
    status = apiWrapper.peak_Library_Init();
    if (PEAK_ERROR(status)) {
        throw std::runtime_error("can't init IDS Peak library");
    }

    status = apiWrapper.peak_CameraList_Update(nullptr);

    size_t nCameras = 20;
    std::unique_ptr<peak_camera_descriptor[]> camDescriptors(new peak_camera_descriptor[20]);
    status = apiWrapper.peak_CameraList_Get(camDescriptors.get(), &nCameras);
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
    IDSPeakAPIWrapper apiWrapper;
    if (apiWrapper.areAllFunctionsLoaded()) {
        apiWrapper.peak_Library_Exit();
    }
}
