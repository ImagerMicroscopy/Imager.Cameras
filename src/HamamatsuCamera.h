#ifndef HAMAMATSUCAMERA_H
#define HAMAMATSUCAMERA_H

#include "BaseCameraClass.h"

#include <string>
#include <vector>

#include "windows.h"
#include "Hamamatsu/dcamapi4.h"

const int kHamamatsuImagesInBuffer = 10;

class HamamatsuCamera : public BaseCameraClass {
public:
	enum GetOrSetProperty {
		GetProperty,
		SetProperty
	};

	enum HamamatsuPropIDs {
		PropEMMode = CameraProperty::FirstAvailablePropertyID,
		PropEMGain,
		PropReadoutSpeed,
		PropTemperatureSetPoint,
		PropCoolerOn,
		PropTriggerSource,
		PropTriggerMode,
		PropTriggerActive,
		PropTriggerPolarity
	};

	HamamatsuCamera(HDCAM camHandle);
	virtual ~HamamatsuCamera();

	std::string getIdentifierStr() const override;

	double getFrameRate() const override;

private:
	std::vector<CameraProperty> _derivedGetCameraProperties() override;
	void _derivedSetCameraProperties(const std::vector<CameraProperty>& properties) override;

	bool _derivedIsConfiguredForHardwareTriggering() override;

	std::pair<int, int> _getSizeOfRawImages() const override;

	std::pair<int, int> _getSensorSize() const { return _sensorSize; }
	CameraProperty _getSetEMMode(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetEMGain(GetOrSetProperty getOrSet, const double value);
	CameraProperty _getSetReadoutSpeed(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetTemperatureSetPoint(GetOrSetProperty getOrSet, const double setPoint);
	CameraProperty _getSetCoolerOn(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetTriggerSource(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetTriggerMode(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetTriggerActive(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty _getSetTriggerPolarity(GetOrSetProperty getOrSet, const std::string& mode);

	void _setExposureTime(const double exposureTime);
	double _getExposureTime() const;
    void _setImageCrop(const std::pair<int, int>& crop);
	std::pair<int, int> _getImageCrop();
    void _setBinningFactor(const int binningFactor);
	int _getBinningFactor();

	void _setDefaults();

	bool _hasCustomAcquireSingleImage() const override { return true; }
	void _derivedAcquireSingleImage(std::uint16_t* bufferForThisImage, int nBytes) override;

	void _derivedStartAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	bool _waitForNewImageWithTimeout(int timeoutMillis) override;
	bool _derivedNewAsyncAcquisitionImageAvailable() override;
	void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

	void _attachBuffers(std::uint16_t** bufPtrs, int nBuffers);
	void _initializeCamWaitHandle();
	void _releaseCamWaitHandle();
	std::string _getDCAMString(HDCAM camHandle, int stringID) const;
	bool _propertyIsSupported(HDCAM camHandle, int propertyID) const;
	double _getPropertyValue(HDCAM camHandle, int propertyID, bool ignoreErrors = false) const;
	void _setPropertyValue(HDCAM camHandle, int propertyID, double value, bool ignoreErrors = false) const;
	std::pair<double, double> _getPropertyLimits(HDCAM camHandle, int propertyID) const;

	HDCAM _camHandle;
	std::string _camName;
	std::pair<int, int> _sensorSize;
	HDCAMWAIT _camWaitHandle;
	int _numberOfImagesDelivered;
};

#endif
