
#include <vector>
#include <string>
#include <cstdint>

#include "XOPStandardHeaders.h"
#include "PVCAM/master.h"
#include "PVCAM/pvcam.h"

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
	double nImages_exposureTime;
	int nImagesParamsSet[1];

	// Parameters for cameraID keyword group.
	int cameraIDEncountered;
	double cameraID_cameraID;
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
	std::vector<std::string> cameraNames;

	std::int16_t nCameras;
	int err = pl_cam_get_total(&nCameras);
	for (std::int16_t i = 0; i < nCameras; i++) {
		char camName[CAM_NAME_LEN + 1];
		pl_cam_get_name(i, camName);
		cameraNames.push_back(std::string(camName));
	}

	for (const std::string& str : cameraNames) {
		XOPNotice(str.c_str());
		XOPNotice("\r");
	}

	return 0;
}

extern "C" int
ExecuteSCAcquireCameraImages(SCAcquireCameraImagesRuntimeParamsPtr p)
{
	int err = 0;

	// Main parameters.

	if (p->nImagesEncountered) {
		// Parameter: p->nImages_exposureTime
	}

	if (p->cameraIDEncountered) {
		// Parameter: p->cameraID_cameraID
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
	runtimeStrVarList = "";
	return RegisterOperation(cmdTemplate, runtimeNumVarList, runtimeStrVarList, sizeof(SCListAvailableCamerasRuntimeParams), (void*)ExecuteSCListAvailableCameras, 0);
}

static int
RegisterSCAcquireCameraImages(void)
{
	const char* cmdTemplate;
	const char* runtimeNumVarList;
	const char* runtimeStrVarList;

	// NOTE: If you change this template, you must change the SCAcquireCameraImagesRuntimeParams structure as well.
	cmdTemplate = "SCAcquireCameraImages nImages=number:exposureTime, cameraID=number:cameraID ";
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
			pl_pvcam_init();
			break;
		case CLEANUP:
			pl_pvcam_uninit();
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
