
#include "BaseCameraClass.h"

class AndorCamera : public BaseCameraClass {
public:
	AndorCamera();
	~AndorCamera();

	std::string getIdentifierStr() const override;

	bool setExposureTime(const double exposureTime) override;
	bool setEMGain(const double emGain) override;
	bool setCoolerOnOff(const bool on);
	bool setTemperature(const double temperature) override;

	double getExposureTime() const override;
	double getEMGain() const override;
	double getTemperature() const override;
	std::pair<int, int> getSensorSize() const override;

	std::vector<uint16_t> acquireImages(const int nImages) override;

private:
	void _setDefaults();
	void _selectFastestRecommendedReadoutSpeed();
	std::string _andorErrorCodeToMessage(int errorCode) const;

	bool _coolerOn;
};