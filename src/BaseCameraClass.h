#ifndef BASECAMERACLASS_H
#define BASECAMERACLASS_H

#include <string>
#include <vector>
#include <cstdint>
#include <limits>
#include <thread>

class BaseCameraClass {
public:
	BaseCameraClass();
	~BaseCameraClass();

	virtual std::string getIdentifierStr() const = 0;

	virtual bool setExposureTime(const double exposureTime) = 0;
	virtual bool setEMGain(const double emGain) = 0;
	virtual bool setCoolerOn(const bool on) = 0;
	virtual bool setTemperature(const double temperature) = 0;

	virtual double getExposureTime() const = 0;
	virtual double getEMGain() const = 0;
	virtual double getTemperature() const = 0;
	virtual double getTemperatureSetpoint() const = 0;
	virtual std::pair<int, int> getSensorSize() const = 0;

	std::vector<std::uint16_t> acquireImages(const int nImages);

	int getAsyncStatus();
	int startAsyncAcquisition(bool freeRun, std::uint16_t* outputBuffer, int nImagesInBuffer);
	bool isAsyncAcquisitionRunning();
	void abortAsyncAquisition();
	int getNImagesAsyncAcquired();
	int getIndexOfLastImageAsyncAcquired();

private:
	void _asyncAcquisitionWorker(bool freeRun, std::uint16_t* outputBuffer, int nImagesInBuffer);

	virtual void _derivedStartAsyncAcquisition() = 0;
	virtual void _derivedAbortAsyncAcquisition() = 0;
	virtual bool _derivedNewAsyncAcquisitionImageAvailable() = 0;
	virtual void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) = 0;

	bool _asyncIsRunning;
	std::string _asyncErrorStr;
	int _asyncWantAbort;
	int _asyncNImagesStored;
	int _asyncIndexOfLastAcquisition;
	std::thread _asyncWorkerThread;
};

#endif
