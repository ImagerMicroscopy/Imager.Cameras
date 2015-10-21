
#ifndef DUMMYCAMERA_H
#define DUMMYCAMERA_H

#include "BaseCameraClass.h"

class DummyCamera : public BaseCameraClass {
public:
	DummyCamera();
	~DummyCamera() { ; }

	std::string getIdentifierStr() const override { return std::string("ZZ__DummyCam"); };

	bool setExposureTime(const double exposureTime) override { _exposureTime = exposureTime; return true; };
	bool setEMGain(const double emGain) override { _emGain = emGain; return true; };
	bool setCoolerOn(const bool on) override { _coolerOn = on; return true; }
	bool setTemperature(const double temperature) override { _temperature = temperature; return true; };

	double getExposureTime() const override { return _exposureTime; };
	double getEMGain() const override { return _emGain; };
	double getTemperature() const override { return _temperature; }
	std::pair<int, int> getSensorSize() const override { return std::pair<int, int>(512, 512); };

	std::vector<uint16_t> acquireImages(const int nImages) override;

private:
	double _exposureTime;
	double _emGain;
	bool _coolerOn;
	double _temperature;
};

#endif
