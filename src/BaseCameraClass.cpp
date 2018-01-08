
#include "BaseCameraClass.h"

#include <chrono>

#ifdef WITH_IGOR
#include "XOPStandardHeaders.h"
#endif

#include "Utils.h"

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

std::vector<std::pair<int, int>> BaseCameraClass::getSupportedCropSizes() const {
    int cropDimensions[] = { 4,16,32,64,128,256,512,1024,1280,1536,2048,3072,4096 };
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
        _croppedImageSize = crop;
        _haveImageCrop = (_croppedImageSize != getSensorSize());
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
        _binFactor = binningFactor;
        _haveBinning = (_binFactor != 1);
    }
}

int BaseCameraClass::getBinningFactor() {
    if (_haveBinning) {
        return _binFactor;
    } else {
        return 1;
    }
}

void BaseCameraClass::acquireImages(const int nImages, const unsigned int nImagesToAverage, std::uint16_t* outputBuffer) {
	startAsyncAcquisition(AcqFillAndStop, nImagesToAverage, outputBuffer, nImages);
    for (;;) {
        std::future_status status = waitForAsyncAcquisitionEnd(100);
        if (status == std::future_status::ready) {
            _asyncWorkerFuture.get();
            if (!_asyncErrorStr.empty()) {
                throw std::runtime_error(_asyncErrorStr);
            } else {
                return;
            }
        }
        #ifdef WITH_IGOR
        if (SpinProcess()) {
            abortAsyncAquisitionIfRunning();
            return;
        }
        #endif
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

int BaseCameraClass::startAsyncAcquisition(AcquisitionMode acqMode, unsigned int nImagesToAverage, std::uint16_t* outputBuffer, int nImagesInBuffer) {
    abortAsyncAquisitionIfRunning();
	_asyncErrorStr.clear();
	_asyncWantAbort = false;
	_asyncNImagesStored = 0;
	_imageIndicesWaitingToBeCopied.clear();

    _asyncWorkerFuture = std::async(std::launch::async, [=]() {
		_asyncAcquisitionWorker(acqMode, nImagesToAverage, outputBuffer, nImagesInBuffer);
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

int BaseCameraClass::getIndexOfLastImageAsyncAcquired() {
    if (!_asyncErrorStr.empty()) {
        throw std::runtime_error(_asyncErrorStr);
    }
	std::lock_guard<std::mutex> lock(this->_imageIndicesMutex);
	if (_imageIndicesWaitingToBeCopied.empty()) {
		return -1;
	} else {
		int index = _imageIndicesWaitingToBeCopied.front();
		_imageIndicesWaitingToBeCopied.erase(_imageIndicesWaitingToBeCopied.begin());
		return index;
	}
}

std::pair<double, double> BaseCameraClass::_derivedGetEMGainRange() {
	double currentGain = getEMGain();
	double minGain = 0.0;
	setEMGain(65000.0);
	double maxGain = getEMGain();
	setEMGain(currentGain);
	return std::pair<double, double>(minGain, maxGain);
}

void BaseCameraClass::_cropImage(const std::vector<std::uint16_t>& input, const std::pair<int, int>& inputSize, std::vector<std::uint16_t>& output, const std::pair<int, int>& outputSize) const {
    if (input.size() != (inputSize.first * inputSize.second)) {
        throw std::logic_error("_clipImage() with incorrect input size");
    }
    if (output.size() != (outputSize.first * outputSize.second)) {
        throw std::logic_error("_clipImage() with incorrect output size");
    }

    int rowOffset = (inputSize.first - outputSize.first) / 2;
    int colOffset = (inputSize.second - outputSize.second) / 2;
    std::uint16_t* inputPtr = (std::uint16_t*)input.data() + colOffset * inputSize.first + rowOffset;
    std::uint16_t* outputPtr = output.data();
    for (int col = 0; col < outputSize.second; col++) {
        memcpy(outputPtr, inputPtr, outputSize.first * sizeof(std::uint16_t));
        inputPtr += inputSize.first;
        outputPtr += outputSize.first;
    }
}

void BaseCameraClass::_binImage(const std::vector<std::uint16_t>& input, const std::pair<int, int>& inputSize, std::vector<std::uint16_t>& output, const int binFactor) const {
    if (((inputSize.first % binFactor) != 0) || ((inputSize.second % binFactor) != 0)) {
        throw std::logic_error("binning with non-divisor");
    }
    if ((binFactor > 4) || (binFactor < 1)) {
        throw std::logic_error("unsupported binning factor");
    }
    std::pair<int, int> outputSize(inputSize.first / binFactor, inputSize.second / binFactor);
    if (input.size() != (inputSize.first * inputSize.second)) {
        throw std::logic_error("_binImage() with incorrect input size");
    }
    if (output.size() != (outputSize.first * outputSize.second)) {
        throw std::logic_error("_binImage() with incorrect output size");
    }

    if (binFactor == 1) {
        memcpy(output.data(), input.data(), inputSize.first * inputSize.second * sizeof(std::uint16_t));
        return;
    }

    for (int col = 0; col < outputSize.second; col += 1) {
        std::uint16_t* rowPointers[4];
        for (int i = 0; i < binFactor; i += 1) {
            rowPointers[i] = (std::uint16_t*)input.data() + (col * binFactor + i) * inputSize.first;
        }
        std::uint16_t* outputPtr = (std::uint16_t*)output.data() + col * outputSize.first;
        for (int row = 0; row < outputSize.first; row += 1) {
            std::uint32_t accum = 0;
            for (int bCol = 0; bCol < binFactor; bCol += 1) {
                for (int bRow = 0; bRow < binFactor; bRow += 1) {
                    accum += *(rowPointers[bCol]);
                    rowPointers[bCol] += 1;
                }
            }
            *outputPtr = accum / (binFactor * binFactor);
            outputPtr += 1;
        }
    }
}

void BaseCameraClass::_asyncAcquisitionWorker(AcquisitionMode acqMode, unsigned int nImagesToAverage, std::uint16_t* outputBuffer, int nImagesInBuffer) {
    auto desiredImageSize = getActualImageSize();
    auto sensorSize = getSensorSize();
	int nPixelsInDesiredImage = desiredImageSize.first * desiredImageSize.second;
    bool needSoftwareCrop = (_usesSoftwareCroppingAndBinning() && _haveImageCrop);
    bool needSoftwareBinning = (_usesSoftwareCroppingAndBinning() && _haveBinning);
    bool needAveraging = nImagesToAverage > 1;

	try {
        std::vector<std::uint16_t> fullSensorImage;
        if (needSoftwareCrop || needSoftwareBinning) {
            fullSensorImage.resize(sensorSize.first * sensorSize.second);
        }
		std::vector<std::uint16_t> desiredImage;
        if (needSoftwareBinning || needSoftwareCrop || needAveraging) {
            desiredImage.resize(desiredImageSize.first * desiredImageSize.second);
        }
        std::vector<std::uint16_t> croppedImage;
        if (needSoftwareCrop) {
            croppedImage.resize(_croppedImageSize.first * _croppedImageSize.second);
        }
		std::vector<std::uint32_t> avgImage;
		if (needAveraging) {
			avgImage.resize(desiredImageSize.first * desiredImageSize.second);
			memset(avgImage.data(), 0, avgImage.size() * sizeof(avgImage[0]));
		}

		_derivedStartAsyncAcquisition();
        CleanupRunner runner([&]() {
            this->_derivedAbortAsyncAcquisition();
        });

		std::uint32_t nImagesAccumulatedInAvg = 0;
		int indexOfNextImage = 0;

		for ( ; ;) {
			if (this->_asyncWantAbort) {
				return;
			}

            bool haveImage = _waitForNewImageWithTimeout(100);
            if (haveImage) {
                if (!needSoftwareCrop && !needSoftwareBinning && !needAveraging) {
                    _derivedStoreNewImageInBuffer(outputBuffer + nPixelsInDesiredImage * indexOfNextImage, nPixelsInDesiredImage * sizeof(std::uint16_t));
                } else {
                    std::vector<std::uint16_t>* processedDataPtr = nullptr;
                    if (!needSoftwareCrop && !needSoftwareBinning) {
                        _derivedStoreNewImageInBuffer(desiredImage.data(), desiredImage.size() * sizeof(std::uint16_t));
                        processedDataPtr = &desiredImage;
                    } else {
                        _derivedStoreNewImageInBuffer(fullSensorImage.data(), fullSensorImage.size() * sizeof(std::uint16_t));
                        processedDataPtr = &fullSensorImage;
                        std::pair<int, int> latestSize = sensorSize;
                        if (needSoftwareCrop) {
                            _cropImage(fullSensorImage, sensorSize, croppedImage, _croppedImageSize);
                            latestSize = _croppedImageSize;
                            processedDataPtr = &croppedImage;
                        }
                        if (needSoftwareBinning) {
                            _binImage(*processedDataPtr, latestSize, desiredImage, _binFactor);
                            processedDataPtr = &desiredImage;
                        }
                    }
                    if (!needAveraging) {
                        memcpy(outputBuffer + nPixelsInDesiredImage * indexOfNextImage, processedDataPtr->data(), nPixelsInDesiredImage * sizeof(std::uint16_t));
                    } else {
                        for (int i = 0; i < nPixelsInDesiredImage; i += 1) {
                            avgImage[i] += (*processedDataPtr)[i];
                        }
                        nImagesAccumulatedInAvg += 1;
                        if (nImagesAccumulatedInAvg == nImagesToAverage) {
                            for (int i = 0; i < nPixelsInDesiredImage; i += 1) {
                                outputBuffer[nPixelsInDesiredImage * indexOfNextImage + i] = avgImage[i] / nImagesAccumulatedInAvg;
                            }
                            nImagesAccumulatedInAvg = 0;
                            memset(avgImage.data(), 0, avgImage.size() * sizeof(avgImage[0]));
                        } else {
                            continue;
                        }
                    }
                }

				_asyncNImagesStored += 1;
				{
					std::lock_guard<std::mutex> lock(this->_imageIndicesMutex);
					if (_imageIndicesWaitingToBeCopied.size() == nImagesInBuffer) {
						// buffer overflow
						_imageIndicesWaitingToBeCopied.clear();
					}
					_imageIndicesWaitingToBeCopied.push_back(indexOfNextImage);
				}
				if ((acqMode == AcqFillAndStop) && (_asyncNImagesStored == nImagesInBuffer)) {
					return;
				}

				indexOfNextImage = (indexOfNextImage + 1) % nImagesInBuffer;
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
