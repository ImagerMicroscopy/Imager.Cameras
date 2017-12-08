#ifndef IDSCAMERA_H
#define IDSCAMERA_H

#include "BaseCameraClass.h"

#include <string>
#include <vector>

#include "IDS/uEye.h"

const int kIDSImagesInBuffer = 50;

class IDSCamera : public BaseCameraClass {
public:
    IDSCamera(HIDS camHandle);
    ~IDSCamera();

    std::string getIdentifierStr() const override;

    void setExposureTime(const double exposureTime) override;
    void setEMGain(const double emGain) override;

    double getExposureTime() const override;
    double getEMGain() const override;
    double getTemperature() const override;
    double getTemperatureSetpoint() const override;
    std::pair<int, int> getSensorSize() const override;

private:
    void _derivedSetTemperature(const double temperature) override { ; }
    void _setCoolerOn(const bool on) override { ; }

    void _derivedStartAsyncAcquisition() override;
    void _derivedAbortAsyncAcquisition() override;
    bool _waitForNewImageWithTimeout(int timeoutMillis) override;
    bool _derivedNewAsyncAcquisitionImageAvailable() override;
    void _derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) override;

    void _setDefaults();

    HIDS _camHandle;
    std::vector<std::uint16_t> _frameBuffer;
    std::vector<char*> _frameBufferPointers;
    std::vector<int> _frameBufferIDs;
    int _nBytesPerImage;
    int _numberOfImagesDelivered;
    int _idOfMemoryWithOldestImage;
    char* _ptrToMemoryWithOldestImage;
};

#endif
