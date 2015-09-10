
#include "SCConfigure.h"

#include <vector>
#include <string>
#include <cassert>
#include <cmath>

#include "XOPStandardHeaders.h"
#include "CameraManager.h"
#include "BaseCameraClass.h"

CameraManager* gCameraManager = nullptr;

bool StartCameraManager() {
	try {
		if (gCameraManager == nullptr) {
			gCameraManager = new CameraManager();
			gCameraManager->discoverCameras();
		}
	}
	catch (std::runtime_error e) {
		XOPNotice(e.what());
		XOPNotice("\r");
		if (gCameraManager != nullptr) {
			delete gCameraManager;
			gCameraManager = nullptr;
		}
	}

	return (gCameraManager != nullptr);
}

void StopCameraManager() {
	if (gCameraManager != nullptr) {
		delete gCameraManager;
		gCameraManager = nullptr;
	}
}

bool CameraManagerIsAvailable() {
	return StartCameraManager();
}

// Runtime param structure for SCListAvailableCameras operation.
#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct SCListAvailableCamerasRuntimeParams {
	// Flag parameters.

	// Main parameters.

	// These are postamble fields that Igor sets.
	int calledFromFunction;					// 1 if called from a user function, 0 otherwise.
	int calledFromMacro;					// 1 if called from a macro, 0 otherwise.
};
typedef struct SCListAvailableCamerasRuntimeParams SCListAvailableCamerasRuntimeParams;
typedef struct SCListAvailableCamerasRuntimeParams* SCListAvailableCamerasRuntimeParamsPtr;
#pragma pack()	// Reset structure alignment to default.

// Runtime param structure for SCAcquireCameraImages operation.
#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct SCAcquireCameraImagesRuntimeParams {
	// Flag parameters.

	// Main parameters.

	// Parameters for nImages keyword group.
	int nImagesEncountered;
	double nImages_nImages;
	int nImagesParamsSet[1];

	// Parameters for cameraID keyword group.
	int cameraIDEncountered;
	Handle cameraID_cameraID;
	int cameraIDParamsSet[1];

	// These are postamble fields that Igor sets.
	int calledFromFunction;					// 1 if called from a user function, 0 otherwise.
	int calledFromMacro;					// 1 if called from a macro, 0 otherwise.
};
typedef struct SCAcquireCameraImagesRuntimeParams SCAcquireCameraImagesRuntimeParams;
typedef struct SCAcquireCameraImagesRuntimeParams* SCAcquireCameraImagesRuntimeParamsPtr;
#pragma pack()	// Reset structure alignment to default.

// Runtime param structure for SCSetCameraSettings operation.
#pragma pack(2)	// All structures passed to Igor are two-byte aligned.
struct SCSetCameraSettingsRuntimeParams {
	// Flag parameters.

	// Main parameters.

	// Parameters for exposureTime keyword group.
	int exposureTimeEncountered;
	double exposureTime_exposureTime;
	int exposureTimeParamsSet[1];

	// Parameters for emGain keyword group.
	int emGainEncountered;
	double emGain_emGain;
	int emGainParamsSet[1];

	// Parameters for coolerOn keyword group.
	int coolerOnEncountered;
	double coolerOn_coolerOnOff;
	int coolerOnParamsSet[1];

	// Parameters for coolerTemperature keyword group.
	int coolerTemperatureEncountered;
	double coolerTemperature_setpoint;
	int coolerTemperatureParamsSet[1];

	// Parameters for cameraID keyword group.
	int cameraIDEncountered;
	Handle cameraID_cameraID;
	int cameraIDParamsSet[1];

	// These are postamble fields that Igor sets.
	int calledFromFunction;					// 1 if called from a user function, 0 otherwise.
	int calledFromMacro;					// 1 if called from a macro, 0 otherwise.
};
typedef struct SCSetCameraSettingsRuntimeParams SCSetCameraSettingsRuntimeParams;
typedef struct SCSetCameraSettingsRuntimeParams* SCSetCameraSettingsRuntimeParamsPtr;
#pragma pack()	// Reset structure alignment to default.

#pragma pack(2) // All Igor structures are two-byte-aligned. 
struct SCGetCameraEMGainParams {
	Handle identifier; // parameter: camera identifier.
	double result;
};
typedef struct SCGetCameraEMGainParams SCGetCameraEMGainParams; 
#pragma pack() // Reset structure alignment.

#pragma pack(2) // All Igor structures are two-byte-aligned. 
struct SCGetCameraExposureTimeParams {
	Handle identifier; // parameter: camera identifier.
	double result;
};
typedef struct SCGetCameraExposureTimeParams SCGetCameraExposureTimeParams;
#pragma pack() // Reset structure alignment.

#pragma pack(2) // All Igor structures are two-byte-aligned. 
struct SCGetCameraTemperatureParams {
	Handle identifier; // parameter: camera identifier.
	double result;
};
typedef struct SCGetCameraTemperatureParams SCGetCameraTemperatureParams;
#pragma pack() // Reset structure alignment.

extern "C" int
ExecuteSCListAvailableCameras(SCListAvailableCamerasRuntimeParamsPtr p)
{
	if (!CameraManagerIsAvailable()) {
		return NOMEM;	// todo: better message
	}

	std::vector<std::string> cameraIdentifiers = gCameraManager->getCameraIdentifiers();
	std::string identifierList;

	for (const std::string& id : cameraIdentifiers) {
		identifierList += id + ";";
	}

	return SetOperationStrVar("S_CameraIdentifiers", identifierList.c_str());
}

extern "C" int
ExecuteSCAcquireCameraImages(SCAcquireCameraImagesRuntimeParamsPtr p)
{
	int err = 0;

	if (!CameraManagerIsAvailable()) {
		return NOMEM;	// todo: better message
	}

	// Main parameters.

	int nImages = 1;
	if (p->nImagesEncountered) {
		// Parameter: p->nImages_nImages
		nImages = std::round(p->nImages_nImages);
		if (nImages <= 0)
			return EXPECT_POS_NUM;
	}

	std::string identifier;
	if (p->cameraIDEncountered) {
		// Parameter: p->cameraID_cameraID (test for NULL handle before using)
		if (p->cameraID_cameraID == nullptr)
			return EXPECTED_STRING;
		char buf[128];
		err = GetCStringFromHandle(p->cameraID_cameraID, buf, 128 - 1);
		if (err)
			return err;
		identifier = buf;
	}

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		if (!identifier.empty()) {
			camPtr = gCameraManager->getCamera(identifier);
		} else {
			camPtr = gCameraManager->getFirstCamera();
		}
		std::pair<int, int> chipDimensions = camPtr->getSensorSize();
		std::vector<std::uint16_t> acquiredImages = camPtr->acquireImages(nImages);
		int nElements = chipDimensions.first * chipDimensions.second * nImages;
		assert(nElements == acquiredImages.size());

		waveHndl waveH;
		CountInt dimensionSizes[MAX_DIMENSIONS + 1];
		dimensionSizes[0] = chipDimensions.first;
		dimensionSizes[1] = chipDimensions.second;
		dimensionSizes[2] = nImages;
		dimensionSizes[3] = 0;
		err = MDMakeWave(&waveH, "M_AcquiredImages", NULL, dimensionSizes, NT_I16 | NT_UNSIGNED, 1);
		if (err)
			return err;

		memcpy(WaveData(waveH), acquiredImages.data(), nElements * sizeof(std::uint16_t));
	}
	catch (std::runtime_error e) {
		XOPNotice(e.what());
		XOPNotice("\r");
		return NOMEM;
	}
	catch (int e) {
		err = e;
	}
	catch (...) {
		XOPNotice("Unknown exception\r");
		return NOMEM;
	}

	return err;
}

extern "C" int
ExecuteSCSetCameraSettings(SCSetCameraSettingsRuntimeParamsPtr p)
{
	int err = 0;

	if (!CameraManagerIsAvailable()) {
		return NOMEM;	// todo: better message
	}

	// Main parameters.

	bool haveExposureTime = false;
	double exposureTime;
	if (p->exposureTimeEncountered) {
		// Parameter: p->exposureTime_exposureTime
		exposureTime = p->exposureTime_exposureTime;
		if (exposureTime <= 0.0)
			return EXPECT_POS_NUM;
		haveExposureTime = true;
	}

	bool haveEMGain = false;
	double emGain;
	if (p->emGainEncountered) {
		// Parameter: p->emGain_emGain
		emGain = p->emGain_emGain;
		if (emGain < 0.0)
			return EXPECT_POS_NUM;
		haveEMGain = true;
	}

	bool haveCoolerOn = false;
	bool coolerOn;
	if (p->coolerOnEncountered) {
		// Parameter: p->coolerOn_coolerOnOff
		coolerOn = (p->coolerOn_coolerOnOff != 0.0);
		haveCoolerOn = true;
	}

	bool haveCoolerTemperature = false;
	double coolerTemperature;
	if (p->coolerTemperatureEncountered) {
		// Parameter: p->coolerTemperature_setpoint
		coolerTemperature = p->coolerTemperature_setpoint;
		haveCoolerTemperature = true;
	}

	std::string identifier;
	if (p->cameraIDEncountered) {
		// Parameter: p->cameraID_cameraID (test for NULL handle before using)
		if (p->cameraID_cameraID == nullptr)
			return EXPECTED_STRING;
		char buf[128];
		err = GetCStringFromHandle(p->cameraID_cameraID, buf, 128 - 1);
		if (err)
			return err;
		identifier = buf;
	}

	bool success, atLeastOneFailure = false;
	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		if (!identifier.empty()) {
			camPtr = gCameraManager->getCamera(identifier);
		}
		else {
			camPtr = gCameraManager->getFirstCamera();
		}

		if (haveExposureTime) {
			success = camPtr->setExposureTime(exposureTime);
			if (!success)
				atLeastOneFailure = true;
		}
		if (haveEMGain) {
			success = camPtr->setEMGain(emGain);
			if (!success)
				atLeastOneFailure = true;
		}
		if (haveCoolerOn) {
			success = camPtr->setCoolerOnOff(coolerOn);
			if (!success)
				atLeastOneFailure = true;
		}
		if (haveCoolerTemperature) {
			success = camPtr->setTemperature(coolerTemperature);
			if (!success)
				atLeastOneFailure = true;
		}
	}
	catch (std::runtime_error e) {
		XOPNotice(e.what());
		XOPNotice("\r");
		atLeastOneFailure = true;
	}
	catch (int e) {
		err = e;
		atLeastOneFailure = true;
	}
	catch (...) {
		XOPNotice("Unknown error\r");
		atLeastOneFailure = true;
	}

	SetOperationNumVar("V_flag", atLeastOneFailure);

	return err;
}

extern "C"
int ExecuteSCGetCameraEMGain(SCGetCameraEMGainParams* p) {
	int err = 0;

	if (!CameraManagerIsAvailable()) {
		return NOMEM;	// todo: better message
	}

	std::string identifier;
	if (p->identifier != nullptr) {
		char buf[128];
		err = GetCStringFromHandle(p->identifier, buf, 128 - 1);
		if (err)
			return err;
		identifier = buf;
	} else {
		return EXPECTED_STRING;
	}

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		if (!identifier.empty()) {
			camPtr = gCameraManager->getCamera(identifier);
		}
		else {
			camPtr = gCameraManager->getFirstCamera();
		}
		double emGain = camPtr->getEMGain();
		p->result = emGain;
	}
	catch (std::runtime_error e) {
		XOPNotice(e.what());
		XOPNotice("\r");
	}
	catch (...) {
		XOPNotice("Unknown error\r");
	}

	return err;
}

extern "C"
int ExecuteSCGetCameraExposureTime(SCGetCameraExposureTimeParams* p) {
	int err = 0;

	if (!CameraManagerIsAvailable()) {
		return NOMEM;	// todo: better message
	}

	std::string identifier;
	if (p->identifier != nullptr) {
		char buf[128];
		err = GetCStringFromHandle(p->identifier, buf, 128 - 1);
		if (err)
			return err;
		identifier = buf;
	}
	else {
		return EXPECTED_STRING;
	}

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		if (!identifier.empty()) {
			camPtr = gCameraManager->getCamera(identifier);
		}
		else {
			camPtr = gCameraManager->getFirstCamera();
		}
		double exposureTime = camPtr->getExposureTime();
		p->result = exposureTime;
	}
	catch (std::runtime_error e) {
		XOPNotice(e.what());
		XOPNotice("\r");
	}
	catch (...) {
		XOPNotice("Unknown error\r");
	}

	return err;
}

extern "C"
int ExecuteSCGetCameraTemperature(SCGetCameraTemperatureParams* p) {
	int err = 0;

	if (!CameraManagerIsAvailable()) {
		return NOMEM;	// todo: better message
	}

	std::string identifier;
	if (p->identifier != nullptr) {
		char buf[128];
		err = GetCStringFromHandle(p->identifier, buf, 128 - 1);
		if (err)
			return err;
		identifier = buf;
	}
	else {
		return EXPECTED_STRING;
	}

	try {
		std::shared_ptr<BaseCameraClass> camPtr;
		if (!identifier.empty()) {
			camPtr = gCameraManager->getCamera(identifier);
		}
		else {
			camPtr = gCameraManager->getFirstCamera();
		}
		double temperature = camPtr->getTemperature();
		p->result = temperature;
	}
	catch (std::runtime_error e) {
		XOPNotice(e.what());
		XOPNotice("\r");
	}
	catch (...) {
		XOPNotice("Unknown error\r");
	}

	return err;
}

static int
RegisterSCListAvailableCameras(void)
{
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the SCListAvailableCamerasRuntimeParams structure as well.
	cmdTemplate = "SCListAvailableCameras ";
	runtimeNumVarList = "";
	runtimeStrVarList = "S_CameraIdentifiers";
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(SCListAvailableCamerasRuntimeParams), (void*)ExecuteSCListAvailableCameras, 0);
}

static int
RegisterSCAcquireCameraImages(void)
{
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the SCAcquireCameraImagesRuntimeParams structure as well.
	cmdTemplate = "SCAcquireCameraImages nImages=number:nImages, cameraID=string:cameraID ";
	runtimeNumVarList = "";
	runtimeStrVarList = "";
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(SCAcquireCameraImagesRuntimeParams), (void*)ExecuteSCAcquireCameraImages, 0);
}

static int
RegisterSCSetCameraSettings(void)
{
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the SCSetCameraSettingsRuntimeParams structure as well.
	cmdTemplate = "SCSetCameraSettings exposureTime=number:exposureTime, emGain=number:emGain, coolerOn=number:coolerOnOff, coolerTemperature=number:setpoint, cameraID=string:cameraID ";
	runtimeNumVarList = "V_flag";
	runtimeStrVarList = "";
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(SCSetCameraSettingsRuntimeParams), (void*)ExecuteSCSetCameraSettings, 0);
}

static int
DoFunction()
{
	int funcIndex;
	void *p;				/* pointer to structure containing function parameters and result */
	int err;

	funcIndex = (int)GetXOPItem(0);	/* which function invoked ? */
	p = (void*)GetXOPItem(1);		/* get pointer to params/result */
	switch (funcIndex) {
	case 0:						/* XFUNC1Add(p1, p2) */
		err = ExecuteSCGetCameraEMGain((SCGetCameraEMGainParams*)p);
		break;
	case 1:						/* XFUNC1Div(p1, p2) */
		err = ExecuteSCGetCameraExposureTime((SCGetCameraExposureTimeParams*)p);
		break;
	case 2:						/* XFUNC1ComplexConjugate(p1) */
		err = ExecuteSCGetCameraTemperature((SCGetCameraTemperatureParams*)p);
		break;
	}
	return(err);
}

XOPIORecResult RegisterFunction(int funcIndex) {
	switch (funcIndex) {
		case 0:
			return (XOPIORecResult)ExecuteSCGetCameraEMGain;
		case 1:
			return (XOPIORecResult)ExecuteSCGetCameraExposureTime;
		case 2:
			return (XOPIORecResult)ExecuteSCGetCameraTemperature;
		default:
			return (XOPIORecResult)0;
	}
}

/*	XOPEntry()

This is the entry point from the host application to the XOP for all
messages after the INIT message.
*/
static void XOPEntry(void) {
	long result = 0;

	switch (GetXOPMessage()) {
		case INIT:
			StartCameraManager();
			break;
		case CLEANUP:
			StopCameraManager();
			break;
		case FUNCADDRS:
			//result = RegisterFunction(GetXOPItem(0));
			break;
		case FUNCTION:
			result = DoFunction();
			break;
	}
	SetXOPResult(result);
}

static int RegisterOperations(void)		// Register any operations with Igor.
{
	int result;

	if ((result = RegisterSCListAvailableCameras()))
		return result;
	if ((result = RegisterSCAcquireCameraImages()))
		return result;
	if ((result = RegisterSCSetCameraSettings()))
		return result;

	// There are no more operations added by this XOP.

	return 0;
}


/*	main(ioRecHandle)

This is the initial entry point at which the host application calls XOP.
The message sent by the host must be INIT.

main does any necessary initialization and then sets the XOPEntry field of the
ioRecHandle to the address to be called for future messages.
*/
HOST_IMPORT int XOPMain(IORecHandle ioRecHandle) {
	int result;

	XOPInit(ioRecHandle);							// Do standard XOP initialization.
	SetXOPEntry(XOPEntry);							// Set entry point for future calls.
	if (igorVersion < 620) {
		SetXOPResult(IGOR_OBSOLETE);
		return EXIT_FAILURE;
	}

	if ((result = RegisterOperations())) {
		SetXOPResult(result);
	}
	else {
		SetXOPResult(0);
	}
	return EXIT_SUCCESS;
}
