#ifndef BASECAMERACLASS_H
#define BASECAMERACLASS_H

#include <string>
#include <vector>
#include <cstdint>
#include <limits>
#include <thread>
#include <mutex>

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

	void acquireImages(const int nImages, unsigned int nImagesToAverage, std::uint16_t* outputBuffer);

	int getAsyncStatus();
	int startAsyncAcquisition(AcquisitionMode acqMode, unsigned int nImagesToAverage, std::uint16_t* outputBuffer, int nImagesInBuffer);
	bool isAsyncAcquisitionRunning();
	void abortAsyncAquisition();
	int getNImagesAsyncAcquired();
	int getIndexOfLastImageAsyncAcquired();

private:
	virtual void _derivedSetTemperature(const double temperature) = 0;
	virtual std::pair<double, double> _derivedGetEMGainRange();
	virtual void _setCoolerOn(const bool on) = 0;
	void _asyncAcquisitionWorker(AcquisitionMode acqMode, unsigned int nImagesToAverage, std::uint16_t* outputBuffer, int nImagesInBuffer);

	virtual void _derivedStartAsyncAcquisition() = 0;
	virtual void _derivedAbortAsyncAcquisition() = 0;
	virtual bool _derivedNewAsyncAcquisitionImageAvailable() = 0;
	virtual void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) = 0;

	bool _asyncIsRunning;
	std::string _asyncErrorStr;
	int _asyncWantAbort;
	int _asyncNImagesStored;
	std::vector<int> _imageIndicesWaitingToBeCopied;
	std::mutex _imageIndicesMutex;
	std::thread _asyncWorkerThread;
};

#endif
