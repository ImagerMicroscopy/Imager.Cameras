
#include <cstdint>

#define MAX_CAMERAS_10
#define MAX_CAMERA_NAME_LENGTH 128

#define GENERIC_ERROR -1
#define NO_INIT -2

int InitCameraDLL();
int ShutdownCameraDLL();

int ListConnectedCameraNames(char** namesPtr);

int GetImageDimensions(char *cameraName, int* rows, int* cols);
int SetEMGain(char *cameraName, double emGain);
int ReadEMGain(char* cameraName, double* emGain);
int SetExposureTime(char* cameraName, double exposureTime);
int ReadExposureTime(char* cameraName, double* exposureTime);

int AcquireImages(char* cameraName, int nImages, uint16_t* buffer, uint64_t bufferSize);
