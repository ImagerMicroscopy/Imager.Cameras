
#include "SCConfigure.h"
#include "CameraManager.h"
#include "BaseCameraClass.h"

#ifdef WITH_PHOTOMETRICS
#include "PhotometricsCameraHandler.h"
#endif

#ifdef WITH_ANDOR
#include "Andor/ATMCD32D.H"
#include "AndorCamera.h"
#endif

#ifdef WITH_HAMAMATSU
#include "HamamatsuCameraHandler.h"
#endif

#ifdef WITH_IDS
#include "IDSCamera.h"
#endif

#ifdef WITH_PCO
#include "windows.h"
#include "PCO/SC2_SDKStructures.h"
#include "PCO/SC2_CamExport.h"
#include "PCOCamera.h"
#endif

#ifdef WITH_OCEANOPTICS
#include "api/seabreezeapi/SeaBreezeAPI.h"
#include "OceanOpticsCamera.h"
#endif

#ifdef WITH_DUMMYCAM
#include "DummyCamera.h"
#endif

#ifdef WITH_IDS_PEAK
#include "IDSPeakCameraHandler.h"
#endif

CameraManager::CameraManager() {
}

CameraManager::~CameraManager() {
#ifdef WITH_PHOTOMETRICS
	ClosePhotometricsLibrary();
#endif

#ifdef WITH_HAMAMATSU
	CloseHamamatsuLibrary();
#endif

#ifdef WITH_IDS_PEAK
	CloseIDSPeakLibrary();
#endif
}

void CameraManager::discoverCameras() {
	_availableCameras.clear();

#ifdef WITH_PHOTOMETRICS
	std::vector<std::shared_ptr<BaseCameraClass>> photometricsCameras = OpenPhotometricsCameras();
	for (auto c : photometricsCameras) {
		_availableCameras.insert(std::pair<std::string, std::shared_ptr<BaseCameraClass>>(c->getIdentifierStr(), c));
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
	std::vector<std::shared_ptr<BaseCameraClass>> hamamatsuCameras = OpenHamamatsuCameras();
	for (auto c : hamamatsuCameras) {
		_availableCameras.insert(std::pair<std::string, std::shared_ptr<BaseCameraClass>>(c->getIdentifierStr(), c));
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

#ifdef WITH_IDS_PEAK

#endif

#ifdef WITH_PCO
	for (; ; ) {
		HANDLE pcoCamHandle = nullptr;
		int pcoErr = PCO_OpenCamera(&pcoCamHandle, 0);
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
#endif

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
        it.second->abortAsyncAquisitionIfRunning();
	}
}
