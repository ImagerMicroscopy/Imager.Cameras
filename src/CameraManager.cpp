
#include "SCConfigure.h"
#include "CameraManager.h"

#ifdef WITH_PHOTOMETRICS
#include "PVCAM/master.h"
#include "PVCAM/pvcam.h"
#include "PhotometricsCamera.h"
#endif

#ifdef WITH_ANDOR
#include "Andor/ATMCD32D.H"
#include "AndorCamera.h"
#endif

#ifdef WITH_HAMAMATSU
#include "windows.h"
#include "Hamamatsu/dcamapi4.h"
#include "HamamatsuCamera.h"
#endif

#ifdef WITH_IDS
#include "IDSCamera.h"
#endif

#ifdef WITH_DUMMYCAM
#include "DummyCamera.h"
#endif

CameraManager::CameraManager() {
#ifdef WITH_PHOTOMETRICS
	int err = pl_pvcam_init();
	if (!err)
		throw std::runtime_error(PhotometricsCamera::getPVCAMErrorMessage());
#endif
    // Hamamatsu api will be init'ed when discovering cameras
}

CameraManager::~CameraManager() {
#ifdef WITH_PHOTOMETRICS
	pl_pvcam_uninit();
#endif

#ifdef WITH_HAMAMATSU
	dcamapi_uninit();
#endif
}

void CameraManager::discoverCameras() {
	_availableCameras.clear();

#ifdef WITH_PHOTOMETRICS
	std::vector<std::string> cameraNames;
	std::int16_t nCameras;
	int pvcamErr = pl_cam_get_total(&nCameras);
	if (!pvcamErr)
		throw std::runtime_error(PhotometricsCamera::getPVCAMErrorMessage());

	for (std::int16_t i = 0; i < nCameras; i++) {
		char camName[CAM_NAME_LEN + 1];
		pvcamErr = pl_cam_get_name(i, camName);
		if (!pvcamErr)
			throw std::runtime_error(PhotometricsCamera::getPVCAMErrorMessage());
		cameraNames.push_back(std::string(camName));
	}
	
	for (const std::string& name : cameraNames) {
		std::shared_ptr<BaseCameraClass> newCamera(new PhotometricsCamera(name));
		_availableCameras.insert(std::pair<std::string, std::shared_ptr<BaseCameraClass>>(name, newCamera));
	}
#endif

#ifdef WITH_ANDOR
	try {
		std::shared_ptr<BaseCameraClass> andorCamera(new AndorCamera());
		std::string identifier = andorCamera->getIdentifierStr();
		_availableCameras.insert(std::pair<std::string, std::shared_ptr<BaseCameraClass>>(identifier, andorCamera));
	}
	catch (std::runtime_error e) {
		// no Andor camera available
	}
#endif

#ifdef WITH_HAMAMATSU
	DCAMAPI_INIT paraminit;
	memset(&paraminit, 0, sizeof(paraminit));
	paraminit.size = sizeof(paraminit);
	DCAMERR	dcamErr;
	dcamErr = dcamapi_init( &paraminit );
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
		dcamErr = dcamdev_open(&devOpen);
		if (dcamErr != DCAMERR_SUCCESS) {
			throw std::runtime_error("unable to open DCAM camera");
		}
		HDCAM hdCam = devOpen.hdcam;
		std::shared_ptr<BaseCameraClass> hamamatsuCamera(new HamamatsuCamera(hdCam));
		_availableCameras.insert(std::pair<std::string, std::shared_ptr<BaseCameraClass>>(hamamatsuCamera->getIdentifierStr(), hamamatsuCamera));
	}
#endif

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
        _availableCameras.insert(std::pair<std::string, std::shared_ptr<BaseCameraClass>>(idsCamera->getIdentifierStr(), idsCamera));
    }
#endif

#ifdef WITH_DUMMYCAM
	_availableCameras.emplace("DummyCam1", new DummyCamera());
	_availableCameras.emplace("DummyCam2", new DummyCamera());

#endif
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
	for (auto it : _availableCameras) {
        it.second->abortAsyncAquisitionIfRunning();
	}
}
