#ifndef DUMMYCAMERA_H
#define DUMMYCAMERA_H

#include <thread>
#include <memory>
#include <queue>

#include "BaseCameraClass.h"
#include "Utils.h"

class DummyCamera : public BaseCameraClass {
public:
	DummyCamera();
	~DummyCamera() { ; }

	std::string getIdentifierStr() const override { return std::string("ZZ__DummyCam"); };

	std::vector<CameraProperty> getCameraProperties() override;
	void setCameraProperty(const CameraProperty& prop) override;

	void setExposureTime(const double exposureTime) override;
	void setEMGain(const double emGain) override { _emGain = emGain; };

	double getExposureTime() const override { return _exposureTime; };
	double getEMGain() const override { return _emGain; };
	double getTemperature() const override;
	double getTemperatureSetpoint() const override { return _temperature; };
	std::pair<int, int> getSensorSize() const override { return std::pair<int, int>(2048, 2048); };

private:
    std::shared_ptr<std::vector<uint16_t>> _generateNewImage();

	void _derivedSetTemperature(const double temperature) override { _temperature = temperature; }
	void _setCoolerOn(const bool on) override { _coolerOn = on; }

	void _derivedStartAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	bool _derivedNewAsyncAcquisitionImageAvailable() override;
	void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

	double _exposureTime;
	double _emGain;
	bool _coolerOn;
	double _temperature;
	std::thread _timerThread;
	bool _abortTimerThread;
    std::mutex _imagesQueueMutex;
    std::queue<std::shared_ptr<std::vector<uint16_t>>> _imagesQueue;
	std::uint16_t _frameCounter;
};

#endif
