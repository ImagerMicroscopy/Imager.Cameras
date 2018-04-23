#ifndef BASECAMERACLASS_H
#define BASECAMERACLASS_H

#include <string>
#include <vector>
#include <cstdint>
#include <limits>
#include <thread>
#include <mutex>
#include <future>

#include "ReaderWriterQueue/atomicops.h"
#include "ReaderWriterQueue/readerwriterqueue.h"

#include "ImageProcessingUtils.h"

class BaseCameraClass {
public:
	enum AcquisitionMode {
		AcqFreeRunMode,
		AcqFillAndStop
	};
	BaseCameraClass();
	virtual ~BaseCameraClass();

	virtual std::string getIdentifierStr() const = 0;

	virtual void setExposureTime(const double exposureTime) = 0;
	virtual void setEMGain(const double emGain) = 0;
	void setTemperature(const double temperature);

	virtual double getExposureTime() const = 0;
	virtual double getEMGain() const = 0;
	void getAllowableExposureTimes(double* minExposureTime, double* maxExposureTime);
	void getAllowableEMGains(double* minGain, double* maxGain);
	virtual double getTemperature() const = 0;
	virtual double getTemperatureSetpoint() const = 0;
	
    virtual std::pair<int, int> getSensorSize() const = 0;
    virtual std::pair<int, int> getActualImageSize() const;
    virtual void setImageOrientationOps(const std::vector<std::shared_ptr<ImageProcessingDescriptor>>& ops);
    std::vector<std::pair<int, int>> getSupportedCropSizes() const;
    void setImageCrop(const std::pair<int, int>& crop);
    std::vector<int> getSupportedBinningFactors() const;
    void setBinningFactor(const int binningFactor);
    virtual int getBinningFactor() const;

	void acquireImages(const int nImages, unsigned int nImagesToAverage, std::uint16_t* outputBuffer);

	int getAsyncStatus();
	int startAsyncAcquisition(AcquisitionMode acqMode, unsigned int nImagesToAverage, std::uint16_t* outputBuffer, int nImagesInBuffer);
	bool wasAsyncAcquisitionStarted() const;
    bool isAsyncAcquisitionRunning() const;
    std::future_status waitForAsyncAcquisitionEnd(int timeoutMillis);
	void abortAsyncAquisitionIfRunning();
	int getNImagesAsyncAcquired();
	int getIndexOfLastImageAsyncAcquired();

private:
	virtual void _derivedSetTemperature(const double temperature) = 0;
	virtual std::pair<double, double> _derivedGetEMGainRange();
	virtual void _setCoolerOn(const bool on) = 0;

    virtual void _derivedSetImageCrop(const std::pair<int, int>& crop);
    virtual void _derivedSetBinningFactor(const int binningFactor);

    virtual bool _usesSoftwareCroppingAndBinning() const { return true; }
    std::vector<uint16_t>* _performCroppingAndBinning(std::vector<std::uint16_t>& fullSensorImage, const std::pair<int,int>& sensorSize, std::vector<std::uint16_t>& croppedImage, std::vector<std::uint16_t>& desiredImage) const;
    bool _accumulateIntoAverage(const std::shared_ptr<std::uint16_t>& inputImage, std::vector <std::uint32_t>& averageImage, const int nImagesAccumulated, const int nImagesToAccumulate) const;

    void _asyncAcquisitionWorker(AcquisitionMode acqMode, unsigned int nImagesToAverage, std::uint16_t* outputBuffer, int nImagesInBuffer);
    void _imageProcessingWorker(const size_t nRows, const size_t nCols, std::vector<std::shared_ptr<ImageProcessingDescriptor>> processingDescriptors,
                                moodycamel::BlockingReaderWriterQueue<std::shared_ptr<std::uint16_t>> &queue, std::uint16_t* outputBuffer, const size_t nImagesInOutputBuffer);
    virtual void _derivedStartAsyncAcquisition() = 0;
	virtual void _derivedAbortAsyncAcquisition() = 0;
	virtual bool _derivedNewAsyncAcquisitionImageAvailable() = 0;
    virtual bool _waitForNewImageWithTimeout(int timeoutMillis);
	virtual void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) = 0;

    std::shared_ptr<std::uint16_t> _doProcessingStep(std::shared_ptr<ImageProcessingDescriptor> descriptor, std::shared_ptr<std::uint16_t> inputImage, size_t nRowsInput, size_t nColsInput, size_t& nRowsOutput, size_t& nColsOutput);

    std::vector<std::shared_ptr<ImageProcessingDescriptor>> _imageOrientationOps;
    bool _haveImageCrop;
    std::pair<int, int> _croppedImageSize;
    bool _haveBinning;
    int _binFactor;

	std::string _asyncErrorStr;
	volatile bool _asyncWantAbort;
    volatile bool _processingAsyncHasError;
	int _asyncNImagesStored;
	std::vector<int> _imageIndicesWaitingToBeCopied;
	std::mutex _imageIndicesMutex;
    std::future<void> _asyncWorkerFuture;
};

#endif
