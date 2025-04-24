#include "SCConfigure.h"

#ifdef WITH_OCEANOPTICS

#include "OceanOpticsCamera.h"

#include "Utils.h"

OceanOpticsCamera::OceanOpticsCamera(long deviceID) :
    _seabreezeAPI(SeaBreezeAPI::getInstance()),
    _deviceID(deviceID),
    _nSpectraToAverage(1),
    _offsetToSubtract(0.0),
    _spectraGrabberShouldAbort(false),
    _spectraGrabberHasError(false),
    _availableSpectraQueue(16)
{
    int errorCode = 0, err = 0;

    err = _seabreezeAPI->openDevice(_deviceID, &errorCode);
    if (err) {
        throw std::runtime_error("error opening ocean optics");
    }
    int nFeatures = _seabreezeAPI->getNumberOfSpectrometerFeatures(_deviceID, &errorCode);
    if ((errorCode != 0) || (nFeatures < 1)) {
        throw std::runtime_error("no features found");
    }
    _spectrometerFeatures.resize(nFeatures);
    _seabreezeAPI->getSpectrometerFeatures(_deviceID, &errorCode, _spectrometerFeatures.data(), _spectrometerFeatures.size());
    if (errorCode != 0) {
        throw std::runtime_error("no features found");
    }
    _spectrumLength = _seabreezeAPI->spectrometerGetFormattedSpectrumLength(_deviceID, _spectrometerFeatures.at(0), &errorCode);
    if (errorCode != 0) {
        throw std::runtime_error("error getting spectrum length");
    }
    _minIntegrationTimeMicros = _seabreezeAPI->spectrometerGetMinimumIntegrationTimeMicros(_deviceID, _spectrometerFeatures.at(0), &errorCode);
    if (errorCode != 0) {
        throw std::runtime_error("error getting min exposure time");
    }
    _maxIntegrationTimeMicros = _seabreezeAPI->spectrometerGetMaximumIntegrationTimeMicros(_deviceID, _spectrometerFeatures.at(0), &errorCode);
    if (errorCode != 0) {
        throw std::runtime_error("error getting max exposure time");
    }
    _setDefaults();
}

OceanOpticsCamera::~OceanOpticsCamera() {
    int error = 0;
    _seabreezeAPI->closeDevice(_deviceID, &error);
}

std::string OceanOpticsCamera::getIdentifierStr() const {
    int err = 0, errorCode = 0;
    char buf[512];
    _seabreezeAPI->getDeviceType(_deviceID, &errorCode, buf, sizeof(buf));
    if (err) {
        throw std::runtime_error("error getting identifier string");
    }
    return std::string(buf);
}

double OceanOpticsCamera::getFrameRate() const {
    return (1.0 / _getExposureTime());
}

std::vector<CameraProperty> OceanOpticsCamera::_derivedGetCameraProperties() {
    return { _getSetExposureTime(GetProperty, 0.0), _getSetNSpectraToAverage(GetProperty, 0.0), _getSetOffsetToSubtract(GetProperty, 0.0) };
}

void OceanOpticsCamera::_derivedSetCameraProperties(const std::vector<CameraProperty>& properties) {
    for (const auto& prop : properties) {
        switch (prop.getPropertyCode()) {
        case PropExposureTime:
            _getSetExposureTime(SetProperty, prop.getValue());
            break;
        case PropNSpectraToAverage:
            _getSetNSpectraToAverage(SetProperty, prop.getValue());
            break;
        case PropOffsetToSubtract:
            _getSetOffsetToSubtract(SetProperty, prop.getValue());
            break;
        default:
            throw std::runtime_error("setting unrecognized option");
        }
    }
}

std::pair<int, int> OceanOpticsCamera::_getSizeOfRawImages() const {
    return std::pair<int, int>(_spectrumLength, 1);
}

CameraProperty OceanOpticsCamera::_getSetExposureTime(GetOrSetProperty getOrSet, const double exposureTime) {
    if (getOrSet == SetProperty) {
        _setExposureTime(exposureTime);
    }
    CameraProperty prop(PropExposureTime, "Exposure time");
    prop.setNumeric(_getExposureTime());
    return prop;
}

CameraProperty OceanOpticsCamera::_getSetNSpectraToAverage(GetOrSetProperty getOrSet, const double nSpectra) {
    if (getOrSet == SetProperty) {
        _setNSpectraToAverage(nSpectra);
    }
    CameraProperty prop(PropNSpectraToAverage, "N spectra to average");
    prop.setNumeric(_getNSpectraToAverage());
    return prop;
}

CameraProperty OceanOpticsCamera::_getSetOffsetToSubtract(GetOrSetProperty getOrSet, const double offset) {
    if (getOrSet == SetProperty) {
        _setOffsetToSubtract(offset);
    }
    CameraProperty prop(PropOffsetToSubtract, "Offset to subtract");
    prop.setNumeric(_getOffsetToSubtract());
    return prop;
}

void OceanOpticsCamera::_setDefaults() {
    _setExposureTime(0.1);
    _setNSpectraToAverage(1);
}

void OceanOpticsCamera::_setExposureTime(const double exposureTime) {
    int errorCode = 0;
    long integrationTimeMicros = static_cast<long>(exposureTime * 1.0e6);
    integrationTimeMicros = clamp(integrationTimeMicros, _minIntegrationTimeMicros, _maxIntegrationTimeMicros);
    integrationTimeMicros = std::min(integrationTimeMicros, 1000000L);	// limit to max 1 s
    _seabreezeAPI->spectrometerSetIntegrationTimeMicros(_deviceID, _spectrometerFeatures.at(0), &errorCode, integrationTimeMicros);
    if (errorCode != 0) {
        throw std::runtime_error("error setting exposure time");
    }
    _integrationTimeMicros = integrationTimeMicros;
}

double OceanOpticsCamera::_getExposureTime() const {
    return ((double)_integrationTimeMicros / 1.0e6);
}

void OceanOpticsCamera::_setNSpectraToAverage(const double nSpectra) {
    _nSpectraToAverage = clamp(nSpectra, 1.0, 1000.0);
}

double OceanOpticsCamera::_getNSpectraToAverage() const {
    return _nSpectraToAverage;
}

void OceanOpticsCamera::_setOffsetToSubtract(const double offset) {
    _offsetToSubtract = clamp(offset, 0.0, 65535.0);
}

double OceanOpticsCamera::_getOffsetToSubtract() const {
    return _offsetToSubtract;
}

void OceanOpticsCamera::_derivedStartUnboundedAsyncAcquisition() {
    _startAsyncSpectraGrabber();
}

void OceanOpticsCamera::_derivedAbortAsyncAcquisition() {
    _stopAsyncSpectraGrabber();
}

BaseCameraClass::NewImageResult OceanOpticsCamera::_waitForNewImageWithTimeout(int timeoutMillis, std::uint16_t* bufferForThisImage, int nBytes) {
    if (_spectraGrabberHasError) {
        _stopAsyncSpectraGrabber();
        throw std::runtime_error("error in Ocean Optics acquisition thread");
    }

    std::vector<double> spectrumInFlight;
    bool haveIt = _availableSpectraQueue.wait_dequeue_timed(spectrumInFlight, std::chrono::milliseconds(timeoutMillis));
    if (!haveIt) {
        return NoImageBeforeTimeout;
    }

    if (nBytes != (spectrumInFlight.size() * sizeof(std::uint16_t))) {
        throw std::runtime_error("buffer of wrong size");
    }
    for (size_t i = 0; i < spectrumInFlight.size(); i += 1) {
        bufferForThisImage[i] = clamp(std::round(spectrumInFlight[i]), 0.0, 65535.0);
    }

    return NewImageCopied;
}

void OceanOpticsCamera::_asyncSpectraGrabberWorker() {
    int nPixels = _getSizeOfRawImages().first;
    int nSpectraToAverage = _nSpectraToAverage;
    double offsetToSubtract = _offsetToSubtract;
    std::vector<double> accumulatedSpectrum(nPixels);
    std::vector<double> singleSpectrum(nPixels);

    // discard the first spectrum
    int errorCode = 0;
    _seabreezeAPI->spectrometerGetFormattedSpectrum(_deviceID, _spectrometerFeatures.at(0), &errorCode, singleSpectrum.data(), singleSpectrum.size());
    if (errorCode) {
        _spectraGrabberHasError = true;
        return;
    }

    for ( ; ; ) {
        for (auto& p : accumulatedSpectrum) {
            p = 0.0;
        }
        for (int n = 0; n < nSpectraToAverage; n += 1) {
            if (_spectraGrabberShouldAbort) {
                return;
            }

            _seabreezeAPI->spectrometerGetFormattedSpectrum(_deviceID, _spectrometerFeatures.at(0), &errorCode, singleSpectrum.data(), singleSpectrum.size());
            if (errorCode) {
                _spectraGrabberHasError = true;
                return;
            }


            for (size_t i = 0; i < accumulatedSpectrum.size(); i += 1) {
                accumulatedSpectrum[i] += (singleSpectrum[i] - offsetToSubtract);
            }
        }
        if (nSpectraToAverage > 1) {
            for (auto& p : accumulatedSpectrum) {
                p /= (double)nSpectraToAverage;
            }
        }

        _availableSpectraQueue.try_enqueue(accumulatedSpectrum);
    }
}

void OceanOpticsCamera::_startAsyncSpectraGrabber() {
    _stopAsyncSpectraGrabber();
    _spectraGrabberShouldAbort = false;
    _spectraGrabberHasError = false;
    while (_availableSpectraQueue.pop()) {}
    _spectraGrabberFuture = std::async(std::launch::async, [=]() {
        _asyncSpectraGrabberWorker();
    });
}

void OceanOpticsCamera::_stopAsyncSpectraGrabber() {
    _spectraGrabberShouldAbort = true;
    if (_spectraGrabberFuture.valid()) {
        _spectraGrabberFuture.wait();
        _spectraGrabberFuture.get();
    }
}

#endif