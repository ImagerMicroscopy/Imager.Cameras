#ifndef DUMMYCAMERA_H
#define DUMMYCAMERA_H

#include <format>
#include <thread>
#include <memory>

#include "ReaderWriterQueue/atomicops.h"
#include "ReaderWriterQueue/readerwriterqueue.h"

#include "ImagerPluginCore/BaseCameraClass.h"

class DummyCamera : public BaseCameraClass {
public:
    DummyCamera();
    virtual ~DummyCamera() { ; }

    std::string getIdentifierStr() override { return std::format("ZZ__DummyCam_{}", _cameraID); };

    double getFrameRate() override;

private:
    std::vector<CameraProperty> _derivedGetCameraProperties() override;
    void _derivedSetCameraProperties(const std::vector<CameraProperty>& properties) override;

    std::pair<int, int> _getSizeOfRawImages() override;

    std::pair<int, int> _getSensorSize() const;
    std::shared_ptr<std::vector<uint16_t>> _generateNewImage();
    void _fillImage(std::uint16_t* data, size_t nPixels);

    bool _hasCustomAcquireSingleImage() const override { return true; }
    void _derivedAcquireSingleImage(std::uint16_t* bufferForThisImage, int nBytes) override;

    void _derivedStartUnboundedAsyncAcquisition() override;
    void _derivedAbortAsyncAcquisition() override;
    NewImageResult _waitForNewImageWithTimeout(int timeoutMillis, std::uint16_t* bufferForThisImage, int nBytes) override;

    std::vector<std::shared_ptr<ImageProcessingDescriptor>> _derivedGetAdditionalImageProcessingDescriptors() override;

    double _getExposureTime() const { return _exposureTime; }
    void _setExposureTime(const double exposureTime);
    std::pair<int, int> _getCurrentCropping() const { return _currentCropping; }
    void _setCurrentCropping(const std::optional<int> crop1, const std::optional<int> crop2);
    int _getCurrentBinning() const { return _currentBinFactor; }
    void _setCurrentBinning(int binFactor);
    double _limitExposureTime(const double reqExposureTime) const;

    double _exposureTime;
    std::pair<int, int> _currentCropping;
    int _currentBinFactor;
    double _emGain;
    bool _coolerOn;
    double _temperature;
    std::thread _timerThread;
    volatile bool _abortTimerThread;

    moodycamel::BlockingReaderWriterQueue<std::shared_ptr<std::vector<uint16_t>>> _imagesQueue;
    std::uint16_t _frameCounter;

    int _cameraID = 0;
    static inline int _camCounter = 0;
};

#endif
