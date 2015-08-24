
#include <string>
#include <vector>

#include <Eigen/Dense>

class BaseCameraClass {
public:
	BaseCameraClass() { ; }
	~BaseCameraClass() { ; }

	virtual std::string getIdentifierStr() const = 0;

	virtual bool setExposureTime(const double exposureTime);
	virtual bool setEMGain(const double emGain);
	virtual bool setTemperature(const double temperature);

	virtual double getExposureTime() const;
	virtual double getEMGain() const;
	virtual double getTemperature() const;

	virtual std::vector<Eigen::ArrayXXd> acquireImages(const int nImages);
};