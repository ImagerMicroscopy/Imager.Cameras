#ifndef OCEANOPTICS_H
#define OCEANTOPICS_H

#include "BaseCameraClass.h"

#include <future>
#include <string>
#include <vector>

#include "ReaderWriterQueue/atomicops.h"
#include "ReaderWriterQueue/readerwriterqueue.h"

#include "api/seabreezeapi/SeaBreezeAPI.h"

class OceanOpticsCamera : public BaseCameraClass {
public:
	enum GetOrSetProperty {
		GetProperty,
		SetProperty
	};

	enum OOPropIDs {
		PropExposureTime = CameraProperty::FirstAvailablePropertyID,
		PropNSpectraToAverage
	};

	OceanOpticsCamera(long deviceID);
	virtual ~OceanOpticsCamera();

	std::string getIdentifierStr() const final;

private:
	std::vector<CameraProperty> _derivedGetCameraProperties() override;
	void _derivedSetCameraProperties(const std::vector<CameraProperty>& properties) override;

	std::pair<int, int> _getSizeOfRawImages() const override;

	CameraProperty _getSetExposureTime(GetOrSetProperty getOrSet, const double exposureTime);
	CameraProperty _getSetNSpectraToAverage(GetOrSetProperty getOrSet, const double nSpectra);

	void _setDefaults();

	void _setExposureTime(const double exposureTime);
	double _getExposureTime() const;
	void _setNSpectraToAverage(const double nSpectra);
	double _getNSpectraToAverage() const;

	void _derivedStartAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	bool _derivedNewAsyncAcquisitionImageAvailable() override;
	bool _waitForNewImageWithTimeout(int timeoutMillis) override;
	void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

	void _asyncSpectraGrabberWorker();
	void _startAsyncSpectraGrabber();
	void _stopAsyncSpectraGrabber();

	SeaBreezeAPI* _seabreezeAPI;
	long _deviceID;
	long _featureID;
	std::vector<long> _spectrometerFeatures;
	int _spectrumLength;
	long _integrationTimeMicros;
	long _minIntegrationTimeMicros;
	long _maxIntegrationTimeMicros;
	int _nSpectraToAverage;

	std::future<void> _spectraGrabberFuture;
	bool _spectraGrabberShouldAbort;
	bool _spectraGrabberHasError;
	moodycamel::BlockingReaderWriterQueue<std::vector<double>> _availableSpectraQueue;
	std::vector<double> _spectrumInFlight;
};

#endif
