
#include "BaseCameraClass.h"

#include <chrono>

#ifdef WITH_IGOR
#include "XOPStandardHeaders.h"
#endif

#include "Utils.h"
#include "ImageRecycler.h"

template <typename T>
class ScopedSetter {
public:
    ScopedSetter(T* valToModify, T value) :
        _valToModify(valToModify),
        _value(value) {
    }
    ~ScopedSetter() {
        *_valToModify = _value;
    }
private:
    T* _valToModify;
    T _value;
};

BaseCameraClass::BaseCameraClass() :
    _haveImageCrop(false)
  , _haveBinning(false)
{
}

BaseCameraClass::~BaseCameraClass() {
    abortAsyncAquisitionIfRunning();
}

void BaseCameraClass::setTemperature(const double temperature) {
    _derivedSetTemperature(temperature);
    _setCoolerOn(temperature < 15.0);
}

void BaseCameraClass::getAllowableExposureTimes(double* minExposureTime, double* maxExposureTime) {
    double currentExposureTime = getExposureTime();
    setExposureTime(1.0e-6);
    *minExposureTime = getExposureTime();
    *maxExposureTime = 1.0;
    setExposureTime(currentExposureTime);
}

void BaseCameraClass::getAllowableEMGains(double* minGain, double* maxGain) {
    std::pair<double, double> range = _derivedGetEMGainRange();
    *minGain = range.first;
    *maxGain = range.second;
}

std::pair<int, int> BaseCameraClass::getActualImageSize() const {
    auto size = getSensorSize();
    if (_usesSoftwareCroppingAndBinning() && _haveImageCrop) {
        size = _croppedImageSize;
    }
    if (_usesSoftwareCroppingAndBinning() && _haveBinning) {
        size.first /= _binFactor;
        size.second /= _binFactor;
    }
    return size;
}

void BaseCameraClass::setImageOrientationOps(const std::vector<std::shared_ptr<ImageProcessingDescriptor>>& ops) {
    _imageOrientationOps = ops;
}

std::vector<std::pair<int, int>> BaseCameraClass::getSupportedCropSizes() const {
    int cropDimensions[] = { 16,32,64,128,256,512,1024,1280,1536,2048,3072,4096 };
    std::pair<int, int> sensorSize = getSensorSize();
    std::vector<std::pair<int, int>> result;
    for (int s : cropDimensions) {
        std::pair<int, int> croppedSize(s, s);
        if ((croppedSize.first < sensorSize.first) && (croppedSize.second < sensorSize.second)) {
            result.push_back(croppedSize);
        }
    }
    result.push_back(sensorSize);
    return result;
}

void BaseCameraClass::setImageCrop(const std::pair<int, int>& crop) {
    auto supportedCropSizes = getSupportedCropSizes();
    auto it = std::find(supportedCropSizes.cbegin(), supportedCropSizes.cend(), crop);
    if (it != supportedCropSizes.cend()) {
        _derivedSetImageCrop(crop);
    }
}

std::vector<int> BaseCameraClass::getSupportedBinningFactors() const {
    std::vector<int> binningFactors = { 1, 2, 4 };
    return binningFactors;
}

void BaseCameraClass::setBinningFactor(const int binningFactor) {
    auto supportedBinningFactors = getSupportedBinningFactors();
    auto it = std::find(supportedBinningFactors.cbegin(), supportedBinningFactors.cend(), binningFactor);
    if (it != supportedBinningFactors.cend()) {
        _derivedSetBinningFactor(binningFactor);
    }
}

int BaseCameraClass::getBinningFactor() const {
    if (_haveBinning) {
        return _binFactor;
    } else {
        return 1;
    }
}

void BaseCameraClass::acquireImages(const int nImages, const unsigned int nImagesToAverage, const unsigned int nImagesToAcquire, std::uint16_t* outputBuffer) {
	startAsyncAcquisition(AcqFillAndStop, nImagesToAverage, nImagesToAcquire);
    size_t offsetInBytes = 0;
    for (int nImagesAcquired = 0; nImagesAcquired < nImagesToAcquire; ) {
        #ifdef WITH_IGOR
        if (SpinProcess()) {
            abortAsyncAquisitionIfRunning();
            return;
        }
        #endif

        std::shared_ptr<std::uint16_t> imageData;
        int nRows, nCols;
        std::tie(imageData, nRows, nCols) = getOldestImageAsyncAcquired();
        if (imageData.get() == nullptr) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }
        size_t nBytesInImage = nRows * nCols * sizeof(std::uint16_t);
        memcpy(reinterpret_cast<std::uint8_t*>(outputBuffer) + offsetInBytes, imageData.get(), nBytesInImage);
        offsetInBytes += nBytesInImage;
        nImagesAcquired += 1;
	}
}

int BaseCameraClass::getAsyncStatus() {
	if (!_asyncErrorStr.empty()) {
		return -1;
	}
    if (!isAsyncAcquisitionRunning()) {
		return 0;
	} else {
		return 1;
	}
}

int BaseCameraClass::startAsyncAcquisition(AcquisitionMode acqMode, unsigned int nImagesToAverage, unsigned int nImagesToAcquire) {
    abortAsyncAquisitionIfRunning();
	_asyncErrorStr.clear();
	_asyncWantAbort = false;
	_asyncNImagesStored = 0;
    _clearAvailableImagesQueue();

    _asyncWorkerFuture = std::async(std::launch::async, [=]() {
		_asyncAcquisitionWorker(acqMode, nImagesToAverage, nImagesToAcquire);
	});

	return 0;
}

bool BaseCameraClass::wasAsyncAcquisitionStarted() const {
    return (_asyncWorkerFuture.valid());
}

bool BaseCameraClass::isAsyncAcquisitionRunning() const {
    return (wasAsyncAcquisitionStarted() && (_asyncWorkerFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout));
}

std::future_status BaseCameraClass::waitForAsyncAcquisitionEnd(int timeoutMillis) {
    if (!wasAsyncAcquisitionStarted()) {
        throw std::runtime_error("BaseCameraClass::waitForAsyncAcquisitionEnd() but no async acquisition has been started");
    }
    return _asyncWorkerFuture.wait_for(std::chrono::milliseconds(timeoutMillis));
}

void BaseCameraClass::abortAsyncAquisitionIfRunning() {
    if (wasAsyncAcquisitionStarted()) {
        _asyncWantAbort = true;
        _asyncWorkerFuture.get();
    }
}

int BaseCameraClass::getNImagesAsyncAcquired() {
    return _asyncNImagesStored;
}

std::tuple<std::shared_ptr<std::uint16_t>, int, int> BaseCameraClass::getOldestImageAsyncAcquired() {

    std::tuple<std::shared_ptr<std::uint16_t>, int, int> result;
    bool haveImage = _availableImagesQueue.try_dequeue(result);
    if (!haveImage) {
        if (!isAsyncAcquisitionRunning()) {
            throw std::runtime_error("waiting for new image but no acquisition running");
        } else {
            result = std::tuple<std::shared_ptr<std::uint16_t>, int, int>(std::shared_ptr<std::uint16_t>(), 0, 0);
        }
    }
    return result;
}

std::pair<double, double> BaseCameraClass::_derivedGetEMGainRange() {
    double currentGain = getEMGain();
    double minGain = 0.0;
    setEMGain(65000.0);
    double maxGain = getEMGain();
    setEMGain(currentGain);
    return std::pair<double, double>(minGain, maxGain);
}

void BaseCameraClass::_derivedSetImageCrop(const std::pair<int, int>& crop) {
    _croppedImageSize = crop;
    _haveImageCrop = (_croppedImageSize != getSensorSize());
}

void BaseCameraClass::_derivedSetBinningFactor(const int binningFactor) {
    _binFactor = binningFactor;
    _haveBinning = (_binFactor != 1);
}

bool BaseCameraClass::_accumulateIntoAverage(const std::shared_ptr<std::uint16_t>& inputImage, std::vector <std::uint32_t>& averageImage, const int nImagesAccumulated, const int nImagesToAccumulate) const {
    if (nImagesToAccumulate <= 0) {
        throw std::runtime_error("averaging <= 0 images");
    }

    if (nImagesAccumulated == 0) {
        memset(averageImage.data(), 0, averageImage.size() * sizeof(std::uint32_t));
    }

    const std::uint16_t* imagePtr = inputImage.get();
    if (nImagesAccumulated == (nImagesToAccumulate - 1)) {
        for (size_t i = 0; i < averageImage.size(); i += 1) {
            averageImage[i] += imagePtr[i];
            averageImage[i] /= nImagesToAccumulate;
        }
    } else {
        for (size_t i = 0; i < averageImage.size(); i += 1) {
            averageImage[i] += imagePtr[i];
        }
    }

    return (nImagesAccumulated == (nImagesToAccumulate - 1));   // return truth that we are done accumulating for this image
}

void BaseCameraClass::_asyncAcquisitionWorker(AcquisitionMode acqMode, unsigned int nImagesToAverage, unsigned int nImagesToAcquire) {
    auto desiredImageSize = getActualImageSize();
    auto sensorSize = getSensorSize();
    auto inputImageSize = (_usesSoftwareCroppingAndBinning()) ? sensorSize : desiredImageSize;
    bool needSoftwareCrop = (_usesSoftwareCroppingAndBinning() && _haveImageCrop);
    bool needSoftwareBinning = (_usesSoftwareCroppingAndBinning() && _haveBinning);
    bool needAveraging = nImagesToAverage > 1;

	try {
        std::vector<std::shared_ptr<ImageProcessingDescriptor>> imageProcessingDescriptors;
        if (needSoftwareCrop) {
            imageProcessingDescriptors.push_back(std::shared_ptr<ImageProcessingDescriptor>(new IPDCrop(_croppedImageSize.first, _croppedImageSize.second)));
        }
        if (needSoftwareBinning) {
            imageProcessingDescriptors.push_back(std::shared_ptr<ImageProcessingDescriptor>(new IPDBin(_binFactor)));
        }
        for (const auto& pd : _imageOrientationOps) {
            imageProcessingDescriptors.push_back(pd);
        }

        moodycamel::BlockingReaderWriterQueue<std::shared_ptr<std::uint16_t>> processingQueue;
        _processingAsyncHasError = false;
        std::future<void> imageProcessingFuture = std::async(std::launch::async, [&]() {
            _imageProcessingWorker(inputImageSize.first, inputImageSize.second, imageProcessingDescriptors, processingQueue);
        });
        CleanupRunner ipRunner([&]() {
            processingQueue.enqueue(std::shared_ptr<std::uint16_t>());
            imageProcessingFuture.get();
        });

		std::vector<std::uint32_t> avgImage;
		if (needAveraging) {
			avgImage.resize(inputImageSize.first * inputImageSize.second);
			memset(avgImage.data(), 0, avgImage.size() * sizeof(avgImage[0]));
		}

		_derivedStartAsyncAcquisition();
        CleanupRunner runner([&]() {
            this->_derivedAbortAsyncAcquisition();
        });

		std::uint32_t nImagesAccumulatedInAvg = 0;

		for ( ; ;) {
			if (this->_asyncWantAbort) {
				return;
			}
            if (this->_processingAsyncHasError) {
                _asyncErrorStr = "image processing async had error";
                return;
            }

            bool haveImage = _waitForNewImageWithTimeout(100);
            if (haveImage) {
                std::shared_ptr<std::uint16_t> theImage = NewRecycledImage(inputImageSize);
                _derivedStoreNewImageInBuffer(theImage.get(), inputImageSize.first * inputImageSize.second * sizeof(std::uint16_t));

                if (needAveraging) {
                    bool done = _accumulateIntoAverage(theImage, avgImage, nImagesAccumulatedInAvg, nImagesToAverage);
                    nImagesAccumulatedInAvg += 1;
                    if (!done) {
                        continue;
                    }
                    memcpy(theImage.get(), avgImage.data(), avgImage.size() * sizeof(std::uint16_t));
                    nImagesAccumulatedInAvg = 0;
                }

                processingQueue.enqueue(theImage);
				_asyncNImagesStored += 1;
				
				if ((acqMode == AcqFillAndStop) && (_asyncNImagesStored == nImagesToAcquire)) {
					return;
				}
			}
		}
	}
	catch (std::runtime_error& e) {
		_asyncErrorStr = e.what();
		return;
	}
	catch (...) {
		_asyncErrorStr = "unknown exception in async";
		return;
	}
}

void BaseCameraClass::_imageProcessingWorker(const size_t nRows, const size_t nCols, std::vector<std::shared_ptr<ImageProcessingDescriptor>> processingDescriptors,
                                             moodycamel::BlockingReaderWriterQueue<std::shared_ptr<std::uint16_t>> &queue) {
    try {
        std::shared_ptr<std::uint16_t> inputImage;
        for (; ; ) {
            queue.wait_dequeue(inputImage);
            if (inputImage.get() == nullptr) {
                return;
            }

            size_t nInputRows = nRows, nInputCols = nCols;
            size_t nOutputRows = nRows, nOutputCols = nCols;
            std::shared_ptr<std::uint16_t> outputImage = inputImage;
            for (const auto& pd : processingDescriptors) {
                nInputRows = nOutputRows;
                nInputCols = nOutputCols;
                inputImage = outputImage;
                outputImage = _doProcessingStep(pd, inputImage, nInputRows, nInputCols, nOutputRows, nOutputCols);
            }

            std::tuple<std::shared_ptr<std::uint16_t>, int, int> imageData(outputImage, nOutputRows, nOutputCols);
            _availableImagesQueue.enqueue(imageData);
        }
    }
    catch (...) {
        _processingAsyncHasError = true;
        return;
    }
}

void BaseCameraClass::_clearAvailableImagesQueue() {
    std::tuple<std::shared_ptr<std::uint16_t>, int, int> dummy;
    bool hadValue = false;
    do {
        hadValue = _availableImagesQueue.try_dequeue(dummy);
    } while (hadValue);
}

bool BaseCameraClass::_waitForNewImageWithTimeout(int timeoutMillis) {
    auto start = std::chrono::high_resolution_clock::now();
    for ( ; ; ) {
        if (_derivedNewAsyncAcquisitionImageAvailable()) {
            return true;
        }
        std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - start;
        if (diff.count() > ((double)timeoutMillis / 1000.0)) {
            return false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

std::shared_ptr<std::uint16_t> BaseCameraClass::_doProcessingStep(std::shared_ptr<ImageProcessingDescriptor> descriptor, std::shared_ptr<std::uint16_t> inputImage, 
                                                                  size_t nRowsInput, size_t nColsInput, size_t& nRowsOutput, size_t& nColsOutput) {
    int processingType = descriptor->getType();
    switch (processingType) {
        case kRotateCW:
        case kRotateCCW:
        {
            nRowsOutput = nColsInput;
            nColsOutput = nRowsInput;
            std::shared_ptr<std::uint16_t> outputImage = NewRecycledImage(std::pair<size_t, size_t>(nRowsOutput, nColsOutput));
            if (processingType == kRotateCW) {
                RotateCW(inputImage.get(), nRowsInput, nColsInput, outputImage.get());
            } else {
                RotateCCW(inputImage.get(), nRowsInput, nColsInput, outputImage.get());
            }
            return outputImage;
            break;
        }
        case kFlipHorizontal:
        case kFlipVertical:
        {
            nRowsOutput = nRowsInput;
            nColsOutput = nColsInput;
            std::shared_ptr<std::uint16_t> outputImage = NewRecycledImage(std::pair<size_t, size_t>(nRowsOutput, nColsOutput));
            if (processingType == kFlipHorizontal) {
                FlipHorizontal(inputImage.get(), nRowsInput, nColsInput, outputImage.get());
            } else {
                FlipVertical(inputImage.get(), nRowsInput, nColsInput, outputImage.get());
            }
            return outputImage;
            break;
        }
        case kCrop:
        {
            IPDCrop* cropObj = reinterpret_cast<IPDCrop*>(descriptor.get());
            nRowsOutput = cropObj->nRows;
            nColsOutput = cropObj->nCols;
            std::shared_ptr<std::uint16_t> outputImage = NewRecycledImage(std::pair<size_t, size_t>(nRowsOutput, nColsOutput));
            CropImage(inputImage.get(), nRowsInput, nColsInput, nRowsOutput, nColsOutput, outputImage.get());
            return outputImage;
            break;
        }
        case kBin:
        {
            IPDBin* binObj = reinterpret_cast<IPDBin*>(descriptor.get());
            int binFactor = binObj->binFactor;
            nRowsOutput = nRowsInput / binFactor;
            nColsOutput = nColsInput / binFactor;
            std::shared_ptr<std::uint16_t> outputImage = NewRecycledImage(std::pair<size_t, size_t>(nRowsOutput, nColsOutput));
            BinImage(inputImage.get(), nRowsInput, nColsInput, outputImage.get(), binFactor);
            return outputImage;
            break;
        }
        default:
            throw std::logic_error("no processing in _doProcessingStep()");
            break;
    }
}
