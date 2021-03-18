#ifndef OCEANOPTICS_H
#define OCEANTOPICS_H

#include "BaseCameraClass.h"

#include <string>
#include <vector>

#include "api/seabreezeapi/SeaBreezeAPI.h"

class OceanOpticsCamera : public BaseCameraClass {
public:
	OceanOpticsCamera(long deviceID);
	virtual ~OceanOpticsCamera();

	std::string getIdentifierStr() const final;

private:
	void _setDefaults();

	void _setExposureTime(const double exposureTime);
	double _getExposureTime() const;

	SeaBreezeAPI* _seabreezeAPI;
	long _deviceID;
	long _featureID;
	std::vector<long> _spectrometerFeatures;
	int _spectrumLength;
	long _integrationTimeMicros;
	long _minIntegrationTimeMicros;
	long _maxIntegrationTimeMicros;
};

#endif
