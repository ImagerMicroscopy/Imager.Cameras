#ifndef DUMMYCAMERA_H
#define DUMMYCAMERA_H

#include "BaseCameraClass.h"

class DummyCamera : public BaseCameraClass {
public:
	DummyCamera();
	~DummyCamera() { ; }

	std::string getIdentifierStr() const override { return std::string("ZZ__DummyCam"); };

	void setExposureTime(const double exposureTime) override { _exposureTime = exposureTime; };
	void setEMGain(const double emGain) override { _emGain = emGain; };
	bool setTemperature(const double temperature) override { _temperature = temperature; return true; };

	double getExposureTime() const override { return _exposureTime; };
	double getEMGain() const override { return _emGain; };
	double getTemperature() const override;
	double getTemperatureSetpoint() const override { return _temperature; };
	std::pair<int, int> getSensorSize() const override { return std::pair<int, int>(512, 512); };

private:
	void _setCoolerOn(const bool on) override { _coolerOn = on; }

	void _derivedStartAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	bool _derivedNewAsyncAcquisitionImageAvailable() override;
	void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

	double _exposureTime;
	double _emGain;
	bool _coolerOn;
	double _temperature;
};

#endif
