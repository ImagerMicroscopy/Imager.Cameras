#ifndef BASECAMERACLASS_H
#define BASECAMERACLASS_H

#include <string>
#include <vector>
#include <cstdint>
#include <limits>
#include <thread>
#include <mutex>
#include <future>
#include <vector>

#include "ReaderWriterQueue/atomicops.h"
#include "ReaderWriterQueue/readerwriterqueue.h"

#include "CameraPropertiesEncoding.h"
#include "ImageProcessingUtils.h"

class BaseCameraClass {
public:
	enum AcquisitionMode {
		AcqFreeRunMode,
		AcqFillAndStop
	};

	enum RequiredCameraPropertyIDs {
		ReqPropExposureTime,
		ReqPropCropping,
		ReqPropBinning,
		FirstAvailablePropertyID
	};

	BaseCameraClass();
	virtual ~BaseCameraClass();

	virtual std::string getIdentifierStr() const = 0;

	virtual std::vector<CameraProperty> getCameraProperties() = 0;
	virtual void setCameraProperty(const CameraProperty& prop) = 0;
	
    virtual std::pair<int, int> getActualImageSize() const;
	virtual double getFrameRate() const = 0;
    virtual void setImageOrientationOps(const std::vector<std::shared_ptr<ImageProcessingDescriptor>>& ops);

	void acquireImages(const unsigned int nImagesToAcquire, std::uint16_t* outputBuffer);

	int startAsyncAcquisition(AcquisitionMode acqMode, unsigned int nImagesToAcquire);
    bool isAsyncAcquisitionRunning() const;
	void abortAsyncAquisitionIfRunning();
	int getNImagesAsyncAcquired();
    std::tuple<std::shared_ptr<std::uint16_t>, int, int, double> getOldestImageAsyncAcquired();

protected:
	std::vector<CameraProperty> getRequiredProperties();
	bool setIfRequiredProperty(const CameraProperty& prop);

private:
	std::vector<std::pair<int, int>> _getSupportedCropSizes() const;
	virtual void _setExposureTime(const double exposureTime) = 0;
	virtual double _getExposureTime() const = 0;
	void _setImageCrop(const std::pair<int, int>& crop);
	std::vector<int> _getSupportedBinningFactors() const;
	void _setBinningFactor(const int binningFactor);
	virtual int _getBinningFactor() const;

	virtual std::pair<int, int> _getSensorSize() const = 0;
    virtual void _derivedSetImageCrop(const std::pair<int, int>& crop);
    virtual void _derivedSetBinningFactor(const int binningFactor);
    virtual bool _usesSoftwareCroppingAndBinning() const { return true; }

    void _asyncAcquisitionWorker(AcquisitionMode acqMode, unsigned int nImagesToAcquire);
    void _imageProcessingWorker(const size_t nRows, const size_t nCols, std::vector<std::shared_ptr<ImageProcessingDescriptor>> processingDescriptors,
                                moodycamel::BlockingReaderWriterQueue<std::pair<std::shared_ptr<std::uint16_t>, double>> &queue);
    void _clearAvailableImagesQueue();
    virtual void _derivedStartAsyncAcquisition() = 0;
	virtual void _derivedAbortAsyncAcquisition() = 0;
	virtual bool _derivedNewAsyncAcquisitionImageAvailable() = 0;
    virtual bool _waitForNewImageWithTimeout(int timeoutMillis);
	virtual void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) = 0;

    std::shared_ptr<std::uint16_t> _doProcessingStep(std::shared_ptr<ImageProcessingDescriptor> descriptor, std::shared_ptr<std::uint16_t> inputImage, size_t nRowsInput, size_t nColsInput, size_t& nRowsOutput, size_t& nColsOutput);

    std::uint64_t _getTimeStamp() const;

    std::vector<std::shared_ptr<ImageProcessingDescriptor>> _imageOrientationOps;
    bool _haveImageCrop;
    std::pair<int, int> _croppedImageSize;
    bool _haveBinning;
    int _binFactor;

    std::uint64_t _acquisitionStartTimeStamp;
    std::uint64_t _performanceCounterFrequency;
	std::string _asyncErrorStr;
	volatile bool _asyncWantAbort;
    volatile bool _processingAsyncHasError;
	int _asyncNImagesStored;
    moodycamel::BlockingReaderWriterQueue<std::tuple<std::shared_ptr<std::uint16_t>, int, int, double>> _availableImagesQueue;
    std::future<void> _asyncWorkerFuture;
};

#endif
