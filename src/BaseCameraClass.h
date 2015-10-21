#ifndef BASECAMERACLASS_H
#define BASECAMERACLASS_H

#include <string>
#include <vector>
#include <cstdint>
#include <limits>

class BaseCameraClass {
public:
	BaseCameraClass() { ; }
	~BaseCameraClass() { ; }

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

	virtual std::vector<std::uint16_t> acquireImages(const int nImages) = 0;
};

#endif
