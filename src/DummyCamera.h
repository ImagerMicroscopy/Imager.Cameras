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
	virtual ~DummyCamera() { ; }

	std::string getIdentifierStr() const override { return std::string("ZZ__DummyCam"); };

	std::vector<CameraProperty> getCameraProperties() override;
	void setCameraProperty(const CameraProperty& prop) override;

	double getFrameRate() const override;

private:
	std::pair<int, int> _getSensorSize() const override { return std::pair<int, int>(2048, 2048); };
	void _setExposureTime(const double exposureTime) override;
	double _getExposureTime() const override { return _exposureTime; };
    std::shared_ptr<std::vector<uint16_t>> _generateNewImage();

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
