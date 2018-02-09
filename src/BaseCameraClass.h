#ifndef BASECAMERACLASS_H
#define BASECAMERACLASS_H

#include <string>
#include <vector>
#include <cstdint>
#include <limits>
#include <thread>
#include <mutex>
#include <future>

class BaseCameraClass {
public:
	enum AcquisitionMode {
		AcqFreeRunMode,
		AcqFillAndStop
	};
	BaseCameraClass();
	~BaseCameraClass();

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
    void _cropImage(const std::vector<std::uint16_t>& input, const std::pair<int, int>& inputSize, std::vector<std::uint16_t>& output, const std::pair<int, int>& outputSize) const;
    void _binImage(const std::vector<std::uint16_t>& input, const std::pair<int, int>& inputSize, std::vector<std::uint16_t>& output, const int binFactor) const;
    bool _accumulateIntoAverage(const std::vector<std::uint16_t>& inputImage, std::vector <std::uint32_t>& averageImage, const int nImagesAccumulated, const int nImagesToAccumulate) const;

    void _asyncAcquisitionWorker(AcquisitionMode acqMode, unsigned int nImagesToAverage, std::uint16_t* outputBuffer, int nImagesInBuffer);
    virtual void _derivedStartAsyncAcquisition() = 0;
	virtual void _derivedAbortAsyncAcquisition() = 0;
	virtual bool _derivedNewAsyncAcquisitionImageAvailable() = 0;
    virtual bool _waitForNewImageWithTimeout(int timeoutMillis);
	virtual void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) = 0;

    bool _haveImageCrop;
    std::pair<int, int> _croppedImageSize;
    bool _haveBinning;
    int _binFactor;

	std::string _asyncErrorStr;
	volatile int _asyncWantAbort;
	int _asyncNImagesStored;
	std::vector<int> _imageIndicesWaitingToBeCopied;
	std::mutex _imageIndicesMutex;
    std::future<void> _asyncWorkerFuture;
};

#endif
