
#include "SCConfigure.h"
#include "CameraManager.h"
#include "BaseCameraClass.h"
#include "SCPrinter.h"

#include "PhotometricsCameraHandler.h"

#ifdef WITH_ANDOR
#include "AndorCamera.h"
#endif

#include "AndorSDK3CameraHandler.h"

#include "HamamatsuCameraHandler.h"

#ifdef WITH_IDS
#include "IDSCamera.h"
#endif

#include "PCOCamera.h"

#ifdef WITH_OCEANOPTICS
#include "OceanOpticsCamera.h"
#endif

#ifdef WITH_DUMMYCAM
#include "DummyCamera.h"
#endif

#include "IDSPeakCameraHandler.h"

CameraManager::~CameraManager() {
    CloseAndorSDK3Library();
    ClosePhotometricsLibrary();
    CloseHamamatsuLibrary();
    CloseIDSPeakLibrary();
}

void CameraManager::discoverCameras() {
    _availableCameras.clear();

    std::vector<std::shared_ptr<BaseCameraClass>> photometricsCameras = OpenPhotometricsCameras();
    for (auto c : photometricsCameras) {
        _availableCameras.insert({c->getIdentifierStr(), c});
    }

#ifdef WITH_ANDOR
    try {
        std::shared_ptr<BaseCameraClass> andorCamera(new AndorCamera());
        std::string identifier = andorCamera->getIdentifierStr();
        _availableCameras.insert({andorCamera->getIdentifierStr(), andorCamera});
    }
    catch (const std::runtime_error& e) {
        // no Andor camera available
    }
#endif

    std::vector<std::shared_ptr<BaseCameraClass>> andorCameras = OpenAndorSDK3Cameras();
    for (auto& c : andorCameras) {
        _availableCameras.insert({c->getIdentifierStr(), c});
    }

    std::vector<std::shared_ptr<BaseCameraClass>> hamamatsuCameras = OpenHamamatsuCameras();
    for (auto c : hamamatsuCameras) {
        _availableCameras.insert({c->getIdentifierStr(), c});
    }

#ifdef WITH_IDS
    is_SetErrorReport(0, IS_DISABLE_ERR_REP);
    int nIDSCams;
    int idsErr = is_GetNumberOfCameras(&nIDSCams);
    if (idsErr != IS_SUCCESS) {
        throw std::runtime_error("unable to determine number of IDS cameras");
    }
    for (int i = 0; i < nIDSCams; i += 1) {
        HIDS camHandle = 0;
        idsErr = is_InitCamera(&camHandle, nullptr);
        if (idsErr != IS_SUCCESS) {

            throw std::runtime_error("unable to open IDS camera");
        }
        std::shared_ptr<BaseCameraClass> idsCamera(new IDSCamera(camHandle));
        _availableCameras.insert({idsCamera->getIdentifierStr(), idsCamera});
    }
#endif

    std::vector<std::shared_ptr<BaseCameraClass>> idsCams = OpenIDSPeakCameras();
    for (auto& cam : idsCams) {
        _availableCameras.insert({cam->getIdentifierStr(), cam});
    }

    PCOAPIWrapper pcoAPIWrapper = GetPCOAPIWrapper();
    if (pcoAPIWrapper.areAllFunctionsLoaded()) {
        Print("Found PCO runtime libraries");
        for (; ; ) {
            HANDLE pcoCamHandle = nullptr;
            int pcoErr = pcoAPIWrapper.PCO_OpenCamera(&pcoCamHandle, 0);
            if (pcoErr) {
                std::string errMessage = PCOCamera::pcoErrorAsString(pcoErr);
                //throw std::runtime_error(errMessage);
            }
            if (pcoCamHandle == nullptr) {
                break;
            }
            std::shared_ptr<BaseCameraClass> pcoCamera(new PCOCamera(pcoCamHandle));
            _availableCameras.insert(std::make_pair(pcoCamera->getIdentifierStr(), pcoCamera));
        }
    }

#ifdef WITH_OCEANOPTICS
    SeaBreezeAPI* seabreezeAPI = SeaBreezeAPI::getInstance();
    long ooDeviceIDs[32];
    seabreezeAPI->probeDevices();
    int nOODevices = seabreezeAPI->getDeviceIDs(ooDeviceIDs, sizeof(ooDeviceIDs));
    for (int i = 0; i < nOODevices; i += 1) {
        std::shared_ptr<BaseCameraClass> ooCamera(new OceanOpticsCamera(ooDeviceIDs[i]));
        _availableCameras.emplace(ooCamera->getIdentifierStr(), ooCamera);
    }

#endif

#ifdef WITH_DUMMYCAM
    _availableCameras.emplace("DummyCam1", new DummyCamera());
    _availableCameras.emplace("DummyCam2", new DummyCamera());
#endif


    _applyCameraOrientationOptions();
}

std::vector<std::string> CameraManager::getCameraIdentifiers() const {
    std::vector<std::string> identifiers;

    for (auto it = _availableCameras.cbegin(); it != _availableCameras.cend(); it++) {
        identifiers.push_back(it->first);
    }

    return identifiers;
}

std::shared_ptr<BaseCameraClass> CameraManager::getCamera(const std::string& identifier) const {
    if (_availableCameras.count(identifier) == 0)
        throw std::runtime_error("Camera not found");
    return _availableCameras.at(identifier);
}

std::shared_ptr<BaseCameraClass> CameraManager::getFirstCamera() const {
    if (_availableCameras.size() == 0)
        throw std::runtime_error("No cameras found");
    return _availableCameras.begin()->second;
}

void CameraManager::abortRunningAcquisitions() {
    for (auto& it : _availableCameras) {
        it.second->abortAsyncAcquisitionIfRunning();
    }
}

void CameraManager::_applyCameraOrientationOptions() {
    for (const auto& [name, camera] : _availableCameras) {
        std::vector<std::shared_ptr<ImageProcessingDescriptor>> imageProcessingDescriptors = _configFile.getProcessingOptionsForCamera(name);
        camera->setImageOrientationOps(imageProcessingDescriptors);
    }
}
