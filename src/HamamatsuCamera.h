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
	HamamatsuCamera(HDCAM camHandle);
	~HamamatsuCamera();

	std::string getIdentifierStr() const override;

	void setExposureTime(const double exposureTime) override;
	void setEMGain(const double emGain) override;

	double getExposureTime() const override;
	double getEMGain() const override;
	double getTemperature() const override;
	double getTemperatureSetpoint() const override;
	std::pair<int, int> getSensorSize() const override;

private:
	void _derivedSetTemperature(const double temperature) override;
	std::pair<double, double> _derivedGetEMGainRange() override;
	void _setCoolerOn(const bool on) override;

	void _derivedStartAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	bool _waitForNewImageWithTimeout(int timeoutMillis) override;
	bool _derivedNewAsyncAcquisitionImageAvailable() override;
	void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

	std::string _getDCAMString(HDCAM camHandle, int stringID) const;
	double _getPropertyValue(HDCAM camHandle, int propertyID) const;
	void _setPropertyValue(HDCAM camHandle, int propertyID, double value) const;

	HDCAM _camHandle;
	std::string _camName;
	std::vector<std::uint16_t> _frameBuffer;
	int _nBytesPerImage;
	HDCAMWAIT _camWaitHandle;
	int _numberOfImagesDelivered;
};

#endif
