
#include "SCConfigure.h"

#ifdef WITH_DUMMYCAM

#include "DummyCamera.h"

#include <random>

#include "ImagerPluginCore/StandardCameraProperties.h"

DummyCamera::DummyCamera() :
    _exposureTime(50.0e-3),
    _currentBinFactor(1),
    _emGain(5.0),
    _coolerOn(false),
    _temperature(-50.0),
    _frameCounter(0),
    _abortTimerThread(false)
{
    _currentCropping = _getSensorSize();
    _cameraID = _camCounter;
    _camCounter += 1;
}

double DummyCamera::getFrameRate() {
    return (1.0 / _getExposureTime());
}

std::vector<CameraProperty> DummyCamera::_derivedGetCameraProperties() {
    const auto sensorSize = _getSensorSize();
    return GetStandardProperties(_getExposureTime(), _getCurrentCropping(), StandardCroppingOptions(sensorSize.first), StandardCroppingOptions(sensorSize.second),
                                 _getCurrentBinning(), StandardBinningOptions());
}

void DummyCamera::_derivedSetCameraProperties(const std::vector<CameraProperty>& properties) {
    auto propsCopy(properties);

    DecodedStandardProperties decodedStandardProperties = DecodeAndRemoveStandardProperties(propsCopy);

    if (decodedStandardProperties.crop1.has_value() || decodedStandardProperties.crop2.has_value()) {
        _setCurrentCropping(decodedStandardProperties.crop1, decodedStandardProperties.crop2);
    }
    if (decodedStandardProperties.binningFactor.has_value()) {
        _setCurrentBinning(decodedStandardProperties.binningFactor.value());
    }
    if (decodedStandardProperties.exposureTime.has_value()) {
        _setExposureTime(decodedStandardProperties.exposureTime.value());
    }

    if (!propsCopy.empty()) {
        throw std::runtime_error("DummyCamera::setCameraProperties() but non-standard");
    }
}

std::pair<int, int> DummyCamera::_getSizeOfRawImages() {
    return _getSensorSize();
}

std::vector<std::shared_ptr<ImageProcessingDescriptor>> DummyCamera::_derivedGetAdditionalImageProcessingDescriptors() {
    std::vector<std::shared_ptr<ImageProcessingDescriptor>> descs;
    if (_getSensorSize() != _getCurrentCropping()) {
        descs.push_back(std::shared_ptr<ImageProcessingDescriptor>(new IPDCrop(_getCurrentCropping().first, _getCurrentCropping().second)));
    }
    if (_getCurrentBinning() != 1) {
        descs.push_back(std::shared_ptr<ImageProcessingDescriptor>(new IPDBin(_getCurrentBinning())));
    }
    return descs;
}

void DummyCamera::_setExposureTime(const double exposureTime) {
    _exposureTime = _limitExposureTime(exposureTime);
}

void DummyCamera::_setCurrentCropping(std::optional<int> crop1, std::optional<int> crop2) {
    auto supportedCrop1 = StandardCroppingOptions(_getSensorSize().first);
    auto supportedCrop2 = StandardCroppingOptions(_getSensorSize().second);

    if (crop1.has_value()) {
        auto it = std::find(supportedCrop1.begin(), supportedCrop1.end(), crop1);
        if (it == supportedCrop1.end()) {
            throw std::runtime_error("DummyCamera::_setCurrentCropping() but invalid cropping");
        }
    }
    if (crop2.has_value()) {
        auto it = std::find(supportedCrop2.begin(), supportedCrop2.end(), crop2);
        if (it == supportedCrop2.end()) {
            throw std::runtime_error("DummyCamera::_setCurrentCropping() but invalid cropping");
        }
    }

    auto currentCropping = _getCurrentCropping();
    if (crop1.has_value()) {
        currentCropping.first = crop1.value();
    }
    if (crop2.has_value()) {
        currentCropping.second = crop2.value();
    }
    _currentCropping = currentCropping;

    _exposureTime = _limitExposureTime(_exposureTime);
}

void DummyCamera::_setCurrentBinning(const int binFactor) {
    auto supportedBinFactors = StandardBinningOptions();
    auto it = std::find(supportedBinFactors.cbegin(), supportedBinFactors.cend(), binFactor);
    if (it != supportedBinFactors.cend()) {
        _currentBinFactor = binFactor;
    } else {
        throw std::runtime_error("DummyCamera::_setCurrentBinning() but invalid binning");
    }
    _exposureTime = _limitExposureTime(_exposureTime);
}

double DummyCamera::_limitExposureTime(const double reqExposureTime) const {
    auto sensorSize = _getSensorSize();
    auto currentSize = _getCurrentCropping();
    currentSize.first /= _getCurrentBinning();
    currentSize.second /= _getCurrentBinning();
    double minExposureTime = 10e-3 * (static_cast<double>(currentSize.first * currentSize.second) / (sensorSize.first * sensorSize.second));
    return clamp(reqExposureTime, minExposureTime, 1.0);
}

std::pair<int, int> DummyCamera::_getSensorSize() const {
    return std::make_pair(2048, 2048);
}

AcquiredImage DummyCamera::_generateNewImage() {
    std::pair<int, int> imageDimensions = _getSizeOfRawImages();
    AcquiredImage image = NewRecycledImage(imageDimensions.first, imageDimensions.second);
    _fillImage(image.getData().get(), imageDimensions.first * imageDimensions.second);
    return image;
}

void DummyCamera::_fillImage(std::uint16_t * data, size_t nPixels) {
    for (size_t i = 0; i < nPixels; i++) {
        data[i] = _frameCounter + i;
    }
    _frameCounter += 1;
}

AcquiredImage DummyCamera::_derivedAcquireSingleImage() {
    auto imageDimensions = _getSizeOfRawImages();
    AcquiredImage image(imageDimensions.first, imageDimensions.second, 0.0);
    _fillImage(image.getData().get(), imageDimensions.first * imageDimensions.second);
    return image;
}

void DummyCamera::_derivedStartBoundedAsyncAcquisition(std::uint64_t nImagesToAcquire) {
    _abortTimerThread = false;
    AcquiredImage dummy;
    while (_imagesQueue.try_dequeue(dummy)) {
        ;
    }
     _timerThread = std::thread([=]() {
        std::int64_t exposureTimeMillis = this->_getExposureTime() * 1000.0;
        std::uint64_t nImagesAcquired = 0;
        for ( ; ; ) {
            if (nImagesAcquired >= nImagesToAcquire) {
                return;
            }
            if (this->_abortTimerThread)
                return;
            std::this_thread::sleep_for(std::chrono::milliseconds(exposureTimeMillis));
            auto newImage = _generateNewImage();
            _imagesQueue.enqueue(std::move(newImage));
            nImagesAcquired += 1;
        }
    });
}

void DummyCamera::_derivedAbortAsyncAcquisition() {
    _abortTimerThread = true;
    if (_timerThread.joinable()) {
        _timerThread.join();
    }
    AcquiredImage dummy;
    while (_imagesQueue.try_dequeue(dummy)) {
        ;
    }
}

std::optional<AcquiredImage> DummyCamera::_waitForNewImageWithTimeout(int timeoutMillis) {
    AcquiredImage newImage;
    bool hadImage = _imagesQueue.wait_dequeue_timed(newImage, std::chrono::milliseconds(timeoutMillis));
    if (!hadImage) {
        return std::nullopt;
    }
    return newImage;
}

#endif
