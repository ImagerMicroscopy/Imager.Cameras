#ifndef PCOCAMERA_H
#define PCOCAMERA_H

#include "BaseCameraClass.h"

#include <string>
#include <vector>

#include "windows.h"
#include "PCO/sc2_SDKStructures.h"
#include "PCO/SC2_CamExport.h"

const int kPCOImagesInBuffer = 10;

class PCOCamera : public BaseCameraClass {
public:
	enum GetOrSetProperty {
		GetProperty,
		SetProperty
	};

	enum PCOPropIDs {
		PropReadoutSpeed = BaseCameraClass::FirstAvailablePropertyID,
	};

	PCOCamera(HANDLE camHandle);
	virtual ~PCOCamera();

	std::string getIdentifierStr() const override;

	std::vector<CameraProperty> getCameraProperties() override;
	void setCameraProperty(const CameraProperty& prop) override;

    std::pair<int, int> getActualImageSize() const override;
	double getFrameRate() const override;

private:
	std::pair<int, int> _getSensorSize() const override { return _sensorSize; }
	//int _getBinningFactor() const override;
	CameraProperty _getSetReadoutSpeed(GetOrSetProperty getOrSet, const std::string& mode);

	void _setExposureTime(const double exposureTime) override;
	double _getExposureTime() const override;
    //void _derivedSetImageCrop(const std::pair<int, int>& crop) override;
    //void _derivedSetBinningFactor(const int binningFactor) override;

    bool _usesSoftwareCroppingAndBinning() const override  { return true; };

	bool _hasCustomAcquireSingleImage() const override { return false; }
	//void _derivedAcquireSingleImage(std::uint16_t* bufferForThisImage, int nBytes) override;

	void _derivedStartAsyncAcquisition() override;
	void _derivedAbortAsyncAcquisition() override;
	bool _waitForNewImageWithTimeout(int timeoutMillis) override;
	void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

	void _fetchCameraInfo();
	void _initDefaults();

	double _pcoTimeToSeconds(DWORD time, DWORD timeBase) const;

	HANDLE _camHandle;
	PCO_Description _camDescription;
	std::string _camName;
	std::pair<int, int> _sensorSize;
	std::map < std::string , int> _readoutSpeeds;
	std::vector<std::uint16_t> _frameBuffer;
	std::vector<HANDLE> _waitObjects;
	std::vector<DWORD> _bufferStatuses;
	size_t _nextBufferToReadIndex;
	int _numberOfImagesDelivered;
};

#endif
