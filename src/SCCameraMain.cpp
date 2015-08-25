
#include <vector>
#include <string>
#include <cstdint>
#include <cassert>

#include "XOPStandardHeaders.h"
#include "CameraManager.h"
#include "BaseCameraClass.h"

CameraManager* gCameraManager = nullptr;

bool StartCameraManager() {
	if (gCameraManager == nullptr) {
		gCameraManager = new CameraManager();
		if (gCameraManager != nullptr)
			gCameraManager->discoverCameras();
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
		nImages = p->nImages_nImages;
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
	} else {
		return EXPECTED_STRING;
	}

	try {
		std::shared_ptr<BaseCameraClass> camPtr = gCameraManager->getCamera(identifier);
		std::pair<int, int> chipDimensions = camPtr->getSensorSize();
		std::vector<std::uint16_t> acquiredImages = camPtr->acquireImages(nImages);
		int nElements = chipDimensions.first * chipDimensions.second * nImages;
		assert(nElements == acquiredImages.size());

		char buf[128];
		sprintf_s(buf, "have %d elements\r", nElements);
		XOPNotice(buf);
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
	catch (...) {
		XOPNotice("Unknown exception\r");
		return NOMEM;
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
