
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

BaseCameraClass::BaseCameraClass() {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    _performanceCounterFrequency = freq.QuadPart;
}

BaseCameraClass::~BaseCameraClass() {
    abortAsyncAquisitionIfRunning();
}

std::vector<CameraProperty> BaseCameraClass::getCameraProperties() {
	return _derivedGetCameraProperties();
}

void BaseCameraClass::setCameraProperties(const std::vector<CameraProperty>& properties) {
	if (isAsyncAcquisitionRunning()) {
		throw std::runtime_error("BaseCameraClass::setCameraProperties() but acquisition running");
	}
	_derivedSetCameraProperties(properties);
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
		std::pair<int, int> imageSize = _getSizeOfRawImages();
		int nPixels = imageSize.first * imageSize.second;
		std::shared_ptr<std::uint16_t> imageData(new std::uint16_t[nPixels], [](std::uint16_t* ptr) {delete[] ptr; });
		std::vector<std::shared_ptr<ImageProcessingDescriptor>> imageProcessingDescriptors = _getImageProcessingDescriptors();
		_derivedAcquireSingleImage(imageData.get(), nPixels * sizeof(std::uint16_t));

		size_t nOutputRows, nOutputCols;
		imageData = _processImage(imageSize.first, imageSize.second, imageData, imageProcessingDescriptors, nOutputRows, nOutputCols);
		return std::tuple<std::shared_ptr<uint16_t>, int, int>(imageData, (int)nOutputRows, (int)nOutputCols);
	}
}

int BaseCameraClass::startAsyncAcquisition(AcquisitionMode acqMode, unsigned int nImagesToAcquire) {
	_acquisitionStartTimeStamp = _getTimeStamp();
	
	abortAsyncAquisitionIfRunning();
	_asyncErrorStr.clear();
	_asyncWantAbort = false;
	_asyncNImagesStored = 0;
    _clearAvailableImagesQueue();
	std::shared_ptr<moodycamel::BlockingReaderWriterQueue<int>> startedNotificationQueue(new moodycamel::BlockingReaderWriterQueue<int>());

	_asyncWorkerFuture = std::async(std::launch::async, [=]() {
		_asyncAcquisitionWorker(acqMode, nImagesToAcquire, startedNotificationQueue);
	});

	int dummy = -1;
	bool hadValue = startedNotificationQueue->wait_dequeue_timed(dummy, std::chrono::seconds(5));	// wait until acquisition has really started
	if (!hadValue) {
		throw std::runtime_error("Waiting excessively long on camera acquisition start");
	}

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
	auto result = getOldestImageAsyncAcquiredWithTimeout(std::numeric_limits<uint32_t>::max());
	return result.value();
}

std::optional<std::tuple<std::shared_ptr<std::uint16_t>, int, int, double>> BaseCameraClass::getOldestImageAsyncAcquiredWithTimeout(const std::uint32_t timeoutMillis) {
	std::tuple<std::shared_ptr<std::uint16_t>, int, int, double> imageData;
	int nCompleteWaitLoops = timeoutMillis / 500;
	int remainder = timeoutMillis - (nCompleteWaitLoops * 500);

	for (int i = 0; i < (nCompleteWaitLoops + 1) ; i += 1) {
		int sleepDuration = (i == nCompleteWaitLoops) ? remainder : 500;
		bool haveImage = _availableImagesQueue.wait_dequeue_timed(imageData, std::chrono::milliseconds(sleepDuration));
		if (haveImage) {
			return std::optional<std::tuple<std::shared_ptr<std::uint16_t>, int, int, double>>(imageData);
		}
		if (!isAsyncAcquisitionRunning()) {
			if (!_asyncErrorStr.empty()) {
				throw std::runtime_error(std::string("async worker found error: ") + _asyncErrorStr);
			} else {
				throw std::runtime_error("waiting for new image but no acquisition running");
			}
		}
	}
	return std::optional<std::tuple<std::shared_ptr<std::uint16_t>, int, int, double>>();
}

void BaseCameraClass::_asyncAcquisitionWorker(AcquisitionMode acqMode, unsigned int nImagesToAcquire, std::shared_ptr<moodycamel::BlockingReaderWriterQueue<int>> startedNotificationQueue) {
    auto actualImageSize = _getSizeOfRawImages();

	try {
		std::vector<std::shared_ptr<ImageProcessingDescriptor>> imageProcessingDescriptors = _getImageProcessingDescriptors();

        moodycamel::BlockingReaderWriterQueue<std::pair<std::shared_ptr<std::uint16_t>, double>> processingQueue;
        _processingAsyncHasError = false;
        std::future<void> imageProcessingFuture = std::async(std::launch::async, [&]() {
            _imageProcessingWorker(actualImageSize.first, actualImageSize.second, imageProcessingDescriptors, processingQueue);
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

		startedNotificationQueue->enqueue(0);

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
                std::shared_ptr<std::uint16_t> theImage = NewRecycledImage(actualImageSize);
                _derivedStoreNewImageInBuffer(theImage.get(), actualImageSize.first * actualImageSize.second * sizeof(std::uint16_t));

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
	std::vector<std::shared_ptr<ImageProcessingDescriptor>> imageProcessingDescriptors;
	imageProcessingDescriptors = _derivedGetAdditionalImageProcessingDescriptors();
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
