#ifndef IDSCAMERA_H
#define IDSCAMERA_H

#include "BaseCameraClass.h"

#include <string>
#include <vector>

#include "IDS/uEye.h"

const int kIDSImagesInBuffer = 15;

class IDSCamera : public BaseCameraClass {
public:
	enum GetOrSetProperty {
		GetProperty,
		SetProperty
	};

	enum IDSPropIDs {
		PropGainBoost = BaseCameraClass::FirstAvailablePropertyID,
		PropHardwareGain,
		PropReadoutSpeed,
		PropTemperatureSetPoint,
		PropCoolerOn
	};

    IDSCamera(HIDS camHandle);
    ~IDSCamera();

    std::string getIdentifierStr() const override;

	std::vector<CameraProperty> getCameraProperties() override;
	void setCameraProperty(const CameraProperty& prop) override;

	double getFrameRate() const override;

private:
	std::pair<int, int> _getSensorSize() const override;
	double _getExposureTime() const override;
	void _setExposureTime(const double exposureTime) override;

	CameraProperty _getSetGainBoost(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetHardwareGain(GetOrSetProperty getOrSet, const double value);

	bool _haveGainBoost() const;
	bool _haveHotPixelCorrection() const;

    void _derivedStartAsyncAcquisition() override;
    void _derivedAbortAsyncAcquisition() override;
    bool _waitForNewImageWithTimeout(int timeoutMillis) override;
    bool _derivedNewAsyncAcquisitionImageAvailable() override;
    void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

    void _setDefaults();

    HIDS _camHandle;
    std::vector<std::uint16_t> _frameBuffer;
    std::vector<char*> _frameBufferPointers;
    std::vector<int> _frameBufferIDs;
    int _nBytesPerImage;
    int _numberOfImagesDelivered;
    int _idOfMemoryWithOldestImage;
    char* _ptrToMemoryWithOldestImage;
};

#endif
