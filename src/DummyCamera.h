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

	double getFrameRate() const override;

private:
	std::vector<CameraProperty> _derivedGetCameraProperties() override;
	void _derivedSetCameraProperties(const std::vector<CameraProperty>& properties) override;

	std::pair<int, int> _getSizeOfRawImages() const override;

	std::pair<int, int> _getSensorSize() const;
	std::shared_ptr<std::vector<uint16_t>> _generateNewImage();
	void _fillImage(std::uint16_t* data, size_t nPixels);

	bool _hasCustomAcquireSingleImage() const override { return true; }
	void _derivedAcquireSingleImage(std::uint16_t* bufferForThisImage, int nBytes) override;

	void _derivedStartAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	bool _derivedNewAsyncAcquisitionImageAvailable() override;
	void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

	std::vector<std::shared_ptr<ImageProcessingDescriptor>> _derivedGetAdditionalImageProcessingDescriptors() override;

	double _getExposureTime() const { return _exposureTime; }
	void _setExposureTime(const double exposureTime);
	std::pair<int, int> _getCurrentCropping() const { return _currentCropping; }
	void _setCurrentCropping(const std::pair<int, int>& cropping);
	int _getCurrentBinning() const { return _currentBinFactor; }
	void _setCurrentBinning(int binFactor);

	double _exposureTime;
	std::pair<int, int> _currentCropping;
	int _currentBinFactor;
	double _emGain;
	bool _coolerOn;
	double _temperature;
	std::thread _timerThread;
	volatile bool _abortTimerThread;
    std::mutex _imagesQueueMutex;
    std::queue<std::shared_ptr<std::vector<uint16_t>>> _imagesQueue;
	std::uint16_t _frameCounter;
};

#endif
