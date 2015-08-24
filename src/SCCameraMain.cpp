
#include "XOPStandardHeaders.h"

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

extern "C" int
ExecuteSCListAvailableCameras(SCListAvailableCamerasRuntimeParamsPtr p)
{
	int err = 0;

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

/*	XOPEntry()

This is the entry point from the host application to the XOP for all
messages after the INIT message.
*/
static void XOPEntry(void) {
	long result = 0;

	switch (GetXOPMessage()) {
		// We don't need to handle any messages for this XOP.
	}
	SetXOPResult(result);
}

static int RegisterOperations(void)		// Register any operations with Igor.
{
	int result;

	if ((result = RegisterSCListAvailableCameras()))
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
