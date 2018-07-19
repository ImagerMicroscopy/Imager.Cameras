#ifndef HAMAMATSUCAMERA_H
#define HAMAMATSUCAMERA_H

#include "BaseCameraClass.h"

#include <string>
#include <vector>

#include "windows.h"
#include "Hamamatsu/dcamapi4.h"

const int kHamamatsuImagesInBuffer = 15;

class HamamatsuCamera : public BaseCameraClass {
public:
	enum GetOrSetProperty {
		GetProperty,
		SetProperty
	};

	enum HamamatsuPropIDs {
		PropEMMode = BaseCameraClass::FirstAvailablePropertyID,
		PropEMGain,
		PropReadoutSpeed,
		PropTemperatureSetPoint,
		PropCoolerOn
	};

	HamamatsuCamera(HDCAM camHandle);
	virtual ~HamamatsuCamera();

	std::string getIdentifierStr() const override;

	std::vector<CameraProperty> getCameraProperties() override;
	void setCameraProperty(const CameraProperty& prop) override;

    std::pair<int, int> getActualImageSize() const override;
	double getFrameRate() const override;

private:
	std::pair<int, int> _getSensorSize() const override { return _sensorSize; }
	int _getBinningFactor() const override;
	CameraProperty _getSetEMMode(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetEMGain(GetOrSetProperty getOrSet, const double value);
	CameraProperty _getSetReadoutSpeed(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetTemperatureSetPoint(GetOrSetProperty getOrSet, const double setPoint);
	CameraProperty _getSetCoolerOn(GetOrSetProperty getOrSet, const std::string& mode);

	void _setExposureTime(const double exposureTime) override;
	double _getExposureTime() const override;
    void _derivedSetImageCrop(const std::pair<int, int>& crop) override;
    void _derivedSetBinningFactor(const int binningFactor) override;

    bool _usesSoftwareCroppingAndBinning() const override  { return false; };

	void _derivedStartAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	bool _waitForNewImageWithTimeout(int timeoutMillis) override;
	bool _derivedNewAsyncAcquisitionImageAvailable() override;
	void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

	std::string _getDCAMString(HDCAM camHandle, int stringID) const;
	bool _propertyIsSupported(HDCAM camHandle, int propertyID) const;
	double _getPropertyValue(HDCAM camHandle, int propertyID, bool ignoreErrors = false) const;
	void _setPropertyValue(HDCAM camHandle, int propertyID, double value, bool ignoreErrors = false) const;
	std::pair<double, double> _getPropertyLimits(HDCAM camHandle, int propertyID) const;

	HDCAM _camHandle;
	std::string _camName;
	std::pair<int, int> _sensorSize;
	std::vector<std::uint16_t> _frameBuffer;
	HDCAMWAIT _camWaitHandle;
	int _numberOfImagesDelivered;
};

#endif
