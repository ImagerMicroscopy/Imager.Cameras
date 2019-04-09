
#include "BaseCameraClass.h"

#include <chrono>

#include "Windows.h"

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
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    _performanceCounterFrequency = freq.QuadPart;
}

BaseCameraClass::~BaseCameraClass() {
    abortAsyncAquisitionIfRunning();
}

std::pair<int, int> BaseCameraClass::getActualImageSize() const {
    auto size = _getSensorSize();
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

std::tuple<std::shared_ptr<uint16_t>, int, int> BaseCameraClass::acquireSingleImage() {
	abortAsyncAquisitionIfRunning();
	if (!_hasCustomAcquireSingleImage()) {
		std::shared_ptr<std::uint16_t> imageData;
		int nRows, nCols;
		double timeStamp;

		startAsyncAcquisition(AcqFillAndStop, 1);
		std::tie(imageData, nRows, nCols, timeStamp) = getOldestImageAsyncAcquired();
		abortAsyncAquisitionIfRunning();

		return std::tuple<std::shared_ptr<uint16_t>, int, int>(imageData, nRows, nCols);
	} else {
		static std::vector<std::uint16_t> imageBuffer;
		auto imageSize = getActualImageSize();
		int nPixels = imageSize.first * imageSize.second;
		if (imageBuffer.size() < nPixels) {
			imageBuffer.resize(nPixels);
		}
		std::vector<std::shared_ptr<ImageProcessingDescriptor>> imageProcessingDescriptors = _getImageProcessingDescriptors();
		_derivedAcquireSingleImage(imageBuffer.data(), nPixels * sizeof(std::uint16_t));

		size_t nOutputRows, nOutputCols;
		std::shared_ptr<std::uint16_t> imageData(imageBuffer.data(), [=](auto ptr) {; });
		imageData = _processImage(imageSize.first, imageSize.second, imageData, imageProcessingDescriptors, nOutputRows, nOutputCols);
		return std::tuple<std::shared_ptr<uint16_t>, int, int>(imageData, (int)nOutputRows, (int)nOutputCols);
	}
}

int BaseCameraClass::startAsyncAcquisition(AcquisitionMode acqMode, unsigned int nImagesToAcquire) {
    abortAsyncAquisitionIfRunning();
	_asyncErrorStr.clear();
	_asyncWantAbort = false;
	_asyncNImagesStored = 0;
    _clearAvailableImagesQueue();

    _acquisitionStartTimeStamp = _getTimeStamp();

	_asyncWorkerFuture = std::async(std::launch::async, [=]() {
		_asyncAcquisitionWorker(acqMode, nImagesToAcquire);
	});

	return 0;
}

bool BaseCameraClass::isAsyncAcquisitionRunning() const {
	if (!_asyncWorkerFuture.valid()) {
		return false;
	}
	std::future_status status = _asyncWorkerFuture.wait_for(std::chrono::seconds(0));
	return (status != std::future_status::ready);
}

void BaseCameraClass::abortAsyncAquisitionIfRunning() {
	if (isAsyncAcquisitionRunning()) {
		_asyncWantAbort = true;
		_asyncWorkerFuture.wait();
		_asyncWorkerFuture.get();
	}
}

int BaseCameraClass::getNImagesAsyncAcquired() {
    return _asyncNImagesStored;
}

std::tuple<std::shared_ptr<std::uint16_t>, int, int, double> BaseCameraClass::getOldestImageAsyncAcquired() {
    std::tuple<std::shared_ptr<std::uint16_t>, int, int, double> imageData;
    for ( ; ; ) {
        bool haveImage = _availableImagesQueue.wait_dequeue_timed(imageData, std::chrono::milliseconds(500));
        if (haveImage) {
            return imageData;
        }
        if (!haveImage && !isAsyncAcquisitionRunning()) {
			if (!_asyncErrorStr.empty()) {
				throw std::runtime_error(std::string("async worker found error: ") + _asyncErrorStr);
			} else {
				throw std::runtime_error("waiting for new image but no acquisition running");
			}
        }
    }
}

std::vector<CameraProperty> BaseCameraClass::getRequiredProperties() {
	std::vector<CameraProperty> properties;
	// exposure time
	{
		CameraProperty prop(BaseCameraClass::ReqPropExposureTime, "Exposure time");
		prop.setNumeric(_getExposureTime());
		properties.push_back(prop);
	}
	// cropping
	{
		int first, second;
		std::vector<std::string> strCropSizes;
		auto cropSizes = _getSupportedCropSizes();
		for (const auto& size : cropSizes) {
			char buf[128];
			sprintf(buf, "%dx%d", size.first, size.second);
			strCropSizes.emplace_back(buf);
		}
		std::tie(first, second) = getActualImageSize();
		first *= _getBinningFactor();
		second *= _getBinningFactor();
		char actual[128];
		sprintf(actual, "%dx%d", first, second);
		CameraProperty prop(BaseCameraClass::ReqPropCropping, "Sensor cropping");
		prop.setDiscrete(actual, strCropSizes);
		properties.push_back(prop);
	}
	// binning
	{
		CameraProperty prop(BaseCameraClass::ReqPropBinning, "Binning");
		prop.setDiscrete(std::string(1, (char)_getBinningFactor() + 48), { "1", "2", "4" });
		properties.push_back(prop);
	}

	return properties;
}

bool BaseCameraClass::setIfRequiredProperty(const CameraProperty& prop) {
	int propertyCode = prop.getPropertyCode();
	switch (propertyCode) {
		case ReqPropExposureTime:
			_setExposureTime(prop.getValue());
			break;
		case ReqPropCropping:
		{
			int first, second;
			if (sscanf(prop.getCurrentOption().c_str(), "%dx%d", &first, &second) != 2) {
				throw std::runtime_error("decoding cropping from invalid string");
			}
			_setImageCrop(std::pair<int, int>(first, second));
			break;
		}
		case ReqPropBinning:
		{
			int binFactor = (char)prop.getCurrentOption().at(0) - 48;
			_setBinningFactor(binFactor);
			break;
		}
		default:
			return false;
			break;
	}
	return true;
}

std::vector<std::pair<int, int>> BaseCameraClass::_getSupportedCropSizes() const {
	int cropDimensions[] = { 16,32,64,128,256,512,1024,1280,1536,2048,3072,4096 };
	std::pair<int, int> sensorSize = _getSensorSize();
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

void BaseCameraClass::_setImageCrop(const std::pair<int, int>& crop) {
	auto supportedCropSizes = _getSupportedCropSizes();
	auto it = std::find(supportedCropSizes.cbegin(), supportedCropSizes.cend(), crop);
	if (it != supportedCropSizes.cend()) {
		_derivedSetImageCrop(crop);
	}
}

std::vector<int> BaseCameraClass::_getSupportedBinningFactors() const {
	std::vector<int> binningFactors = { 1, 2, 4 };
	return binningFactors;
}

void BaseCameraClass::_setBinningFactor(const int binningFactor) {
	auto supportedBinningFactors = _getSupportedBinningFactors();
	auto it = std::find(supportedBinningFactors.cbegin(), supportedBinningFactors.cend(), binningFactor);
	if (it != supportedBinningFactors.cend()) {
		_derivedSetBinningFactor(binningFactor);
	}
}

int BaseCameraClass::_getBinningFactor() const {
	if (_haveBinning) {
		return _binFactor;
	} else {
		return 1;
	}
}

void BaseCameraClass::_derivedSetImageCrop(const std::pair<int, int>& crop) {
    _croppedImageSize = crop;
    _haveImageCrop = (_croppedImageSize != _getSensorSize());
}

void BaseCameraClass::_derivedSetBinningFactor(const int binningFactor) {
    _binFactor = binningFactor;
    _haveBinning = (_binFactor != 1);
}

void BaseCameraClass::_asyncAcquisitionWorker(AcquisitionMode acqMode, unsigned int nImagesToAcquire) {
    auto desiredImageSize = getActualImageSize();
    auto sensorSize = _getSensorSize();
    auto inputImageSize = (_usesSoftwareCroppingAndBinning()) ? sensorSize : desiredImageSize;

	try {
		std::vector<std::shared_ptr<ImageProcessingDescriptor>> imageProcessingDescriptors = _getImageProcessingDescriptors();

        moodycamel::BlockingReaderWriterQueue<std::pair<std::shared_ptr<std::uint16_t>, double>> processingQueue;
        _processingAsyncHasError = false;
        std::future<void> imageProcessingFuture = std::async(std::launch::async, [&]() {
            _imageProcessingWorker(inputImageSize.first, inputImageSize.second, imageProcessingDescriptors, processingQueue);
        });
        CleanupRunner ipRunner([&]() {
            processingQueue.enqueue(std::pair<std::shared_ptr<std::uint16_t>, double>());
			imageProcessingFuture.wait();
            imageProcessingFuture.get();
        });

		_derivedStartAsyncAcquisition();
        CleanupRunner runner([&]() {
            this->_derivedAbortAsyncAcquisition();
        });

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
                double acqTimeStamp = static_cast<double>(_getTimeStamp() - _acquisitionStartTimeStamp) / static_cast<double>(_performanceCounterFrequency);
                std::shared_ptr<std::uint16_t> theImage = NewRecycledImage(inputImageSize);
                _derivedStoreNewImageInBuffer(theImage.get(), inputImageSize.first * inputImageSize.second * sizeof(std::uint16_t));

                processingQueue.enqueue(std::pair<std::shared_ptr<std::uint16_t>, double>(theImage, acqTimeStamp));
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

void BaseCameraClass::_clearAvailableImagesQueue() {
    std::tuple<std::shared_ptr<std::uint16_t>, int, int, double> dummy;
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

std::vector<std::shared_ptr<ImageProcessingDescriptor>> BaseCameraClass::_getImageProcessingDescriptors() {
	bool needSoftwareCrop = (_usesSoftwareCroppingAndBinning() && _haveImageCrop);
	bool needSoftwareBinning = (_usesSoftwareCroppingAndBinning() && _haveBinning);

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
	return imageProcessingDescriptors;
}

void BaseCameraClass::_imageProcessingWorker(const size_t nRows, const size_t nCols, const std::vector<std::shared_ptr<ImageProcessingDescriptor>>& processingDescriptors,
											 moodycamel::BlockingReaderWriterQueue<std::pair<std::shared_ptr<std::uint16_t>, double>> &queue) {
	try {
		std::shared_ptr<std::uint16_t> inputImage;
		double timeStamp;
		std::pair<std::shared_ptr<std::uint16_t>, double> queuedData;

		for (; ; ) {
			queue.wait_dequeue(queuedData);
			std::tie(inputImage, timeStamp) = queuedData;
			if (inputImage.get() == nullptr) {
				return;
			}

			size_t nInputRows = nRows, nInputCols = nCols;
			size_t nOutputRows = nRows, nOutputCols = nCols;
			std::shared_ptr<std::uint16_t> outputImage = _processImage(nInputRows, nInputCols, inputImage, processingDescriptors, nOutputRows, nOutputCols);
			std::tuple<std::shared_ptr<std::uint16_t>, int, int, double> imageData(outputImage, nOutputRows, nOutputCols, timeStamp);
			_availableImagesQueue.enqueue(imageData);
		}
	}
	catch (...) {
		_processingAsyncHasError = true;
		return;
	}
}

std::shared_ptr<std::uint16_t> BaseCameraClass::_processImage(const size_t nRows, const size_t nCols, std::shared_ptr<std::uint16_t> inputImage, const std::vector<std::shared_ptr<ImageProcessingDescriptor>>& processingDescriptors,
															  size_t& nOutputRows, size_t& nOutputCols) {
	size_t nInputRows = nRows, nInputCols = nCols;
	nOutputRows = nRows;
	nOutputCols = nCols;
	std::shared_ptr<std::uint16_t> outputImage = inputImage;
	for (const auto& pd : processingDescriptors) {
		nInputRows = nOutputRows;
		nInputCols = nOutputCols;
		inputImage = outputImage;
		outputImage = _doProcessingStep(pd, inputImage, nInputRows, nInputCols, nOutputRows, nOutputCols);
	}
	return outputImage;
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

std::uint64_t BaseCameraClass::_getTimeStamp() const {
    LARGE_INTEGER ts;
    QueryPerformanceCounter(&ts);
    return ts.QuadPart;
}
