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

	HamamatsuCamera(HDCAM camHandle);
	~HamamatsuCamera();

	std::string getIdentifierStr() const override;

	std::vector<CameraProperty> getCameraProperties() override;
	void setCameraProperty(const CameraProperty& prop) override;

	void setExposureTime(const double exposureTime) override;
	void setEMGain(const double emGain) override;

	double getExposureTime() const override;
	double getEMGain() const override;
	double getTemperature() const override;
	double getTemperatureSetpoint() const override;

	std::pair<int, int> getSensorSize() const override { return _sensorSize; }
    std::pair<int, int> getActualImageSize() const override;
    int getBinningFactor() const override;

private:
	CameraProperty getSetEMMode(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty getSetReadoutSpeed(GetOrSetProperty getOrSet, const std::string& mode);
	CameraProperty getSetTemperatureSetPoint(GetOrSetProperty getOrSet, const double setPoint);
	CameraProperty getSetCoolerOn(GetOrSetProperty getOrSet, const std::string& mode);

	void _derivedSetTemperature(const double temperature) override;
	std::pair<double, double> _derivedGetEMGainRange() override;
	void _setCoolerOn(const bool on) override;

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
