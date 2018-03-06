#include "SCConfigure.h"

#ifdef WITH_HAMAMATSU

#include "HamamatsuCamera.h"

#include "Hamamatsu/dcamprop.h"

HamamatsuCamera::HamamatsuCamera(HDCAM camHandle) :
	_camHandle(camHandle),
	_camWaitHandle(nullptr)
{
	_camName = _getDCAMString(_camHandle, DCAM_IDSTR_MODEL) + " (" + _getDCAMString(_camHandle, DCAM_IDSTR_CAMERAID) + ")";
	
	_setPropertyValue(_camHandle, DCAM_IDPROP_CCDMODE, DCAMPROP_CCDMODE__EMCCD, true);

	_setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYMODE, DCAMPROP_MODE__OFF);
	_sensorSize.first = _getPropertyValue(_camHandle, DCAM_IDPROP_IMAGE_WIDTH);
	_sensorSize.second = _getPropertyValue(_camHandle, DCAM_IDPROP_IMAGE_HEIGHT);

	std::pair<double, double> readoutLimits = _getPropertyLimits(_camHandle, DCAM_IDPROP_READOUTSPEED);
	_setPropertyValue(_camHandle, DCAM_IDPROP_READOUTSPEED, readoutLimits.second);

    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYMODE, DCAMPROP_MODE__ON);
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYHSIZE, _sensorSize.first);
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYVSIZE, _sensorSize.second);

	_setPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_POLARITY, DCAMPROP_OUTPUTTRIGGER_POLARITY__POSITIVE);
	_setPropertyValue(_camHandle, DCAM_IDPROP_OUTPUTTRIGGER_KIND, DCAMPROP_OUTPUTTRIGGER_KIND__EXPOSURE);
}

HamamatsuCamera::~HamamatsuCamera() {
	if (_camWaitHandle != nullptr) {
		dcamwait_close(_camWaitHandle);
		_camWaitHandle = nullptr;
	}
	if (_camHandle != nullptr) {
		dcamdev_close(_camHandle);
		_camHandle = nullptr;
	}
}

std::string HamamatsuCamera::getIdentifierStr() const {
	return _camName;
}

void HamamatsuCamera::setExposureTime(const double exposureTime) {
	_setPropertyValue(_camHandle, DCAM_IDPROP_EXPOSURETIME, exposureTime);
}

void HamamatsuCamera::setEMGain(const double emGain) {
	_setPropertyValue(_camHandle, DCAM_IDPROP_SENSITIVITY, emGain, true);
}

double HamamatsuCamera::getExposureTime() const {
	return _getPropertyValue(_camHandle, DCAM_IDPROP_EXPOSURETIME);
}

double HamamatsuCamera::getEMGain() const {
	return _getPropertyValue(_camHandle, DCAM_IDPROP_SENSITIVITY);
}

double HamamatsuCamera::getTemperature() const {
    return _getPropertyValue(_camHandle, DCAM_IDPROP_SENSORTEMPERATURE, true);
}

double HamamatsuCamera::getTemperatureSetpoint() const {
    return _getPropertyValue(_camHandle, DCAM_IDPROP_SENSORTEMPERATURETARGET, true);
}

std::pair<int, int> HamamatsuCamera::getActualImageSize() const {
    std::pair<int, int> size;
    size.first = _getPropertyValue(_camHandle, DCAM_IDPROP_IMAGE_WIDTH);
    size.second = _getPropertyValue(_camHandle, DCAM_IDPROP_IMAGE_HEIGHT);
    return size;
}

int HamamatsuCamera::getBinningFactor() const {
    int fact = _getPropertyValue(_camHandle, DCAM_IDPROP_BINNING);
    switch (fact) {
        case DCAMPROP_BINNING__1:
            return 1;
            break;
        case DCAMPROP_BINNING__2:
            return 2;
            break;
        case DCAMPROP_BINNING__4:
            return 4;
            break;
        default:
            throw std::runtime_error("unknown hamamatsu binning factor");
            break;
    }
}

void HamamatsuCamera::_derivedSetTemperature(const double temperature) {
	_setPropertyValue(_camHandle, DCAM_IDPROP_SENSORTEMPERATURETARGET, temperature, true);
}

std::pair<double, double> HamamatsuCamera::_derivedGetEMGainRange() {
	return std::pair<double, double>(0.0, 255.0);
}

void HamamatsuCamera::_setCoolerOn(const bool on) {
	int action = on ? DCAMPROP_SENSORCOOLER__ON : DCAMPROP_SENSORCOOLER__OFF;
	_setPropertyValue(_camHandle, DCAM_IDPROP_SENSORCOOLER, action, true);
}

void HamamatsuCamera::_derivedSetImageCrop(const std::pair<int, int>& crop) {
    auto sensorSize = getSensorSize();
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYHPOS, 0);
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYVPOS, 0);
    int rowOffset = (sensorSize.first - crop.first) / 2;
    int colOffset = (sensorSize.second - crop.second) / 2;
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYHSIZE, crop.first);
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYVSIZE, crop.second);
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYHPOS, rowOffset);
    _setPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYVPOS, colOffset);
}

void HamamatsuCamera::_derivedSetBinningFactor(const int binningFactor) {
    int fact = 0;
    switch (binningFactor) {
        case 1:
            fact = DCAMPROP_BINNING__1;
            break;
        case 2:
            fact = DCAMPROP_BINNING__2;
            break;
        case 4:
            fact = DCAMPROP_BINNING__4;
            break;
        default:
            throw std::runtime_error("unknown hamamatsu binning factor");
            break;
    }
    _setPropertyValue(_camHandle, DCAM_IDPROP_BINNING, fact);
}

void HamamatsuCamera::_derivedStartAsyncAcquisition() {
	std::pair<int, int> imageSize = getActualImageSize();
	int nPixelsInImage = imageSize.first * imageSize.second;
	
	_frameBuffer.resize(nPixelsInImage * kHamamatsuImagesInBuffer);

    DCAMERR err;
    std::uint16_t* bufferPtrs[kHamamatsuImagesInBuffer];
    for (int i = 0; i < kHamamatsuImagesInBuffer; i++) {
        bufferPtrs[i] = _frameBuffer.data() + i * nPixelsInImage;
    }
    DCAMBUF_ATTACH attachParams = { 0 };
    attachParams.size = sizeof(attachParams);
    attachParams.iKind = DCAMBUF_ATTACHKIND_FRAME;
    attachParams.buffer = reinterpret_cast<void**>(&bufferPtrs);
    attachParams.buffercount = kHamamatsuImagesInBuffer;
    err = dcambuf_attach(_camHandle, &attachParams);
    if (err != DCAMERR_SUCCESS) {
        throw std::runtime_error("couldn't attach buffers");
    }

	if (_camWaitHandle == nullptr) {
		DCAMWAIT_OPEN waitParams = { 0 };
		waitParams.size = sizeof(DCAMWAIT_OPEN);
		waitParams.hdcam = _camHandle;
		err = dcamwait_open(&waitParams);
		if (err != DCAMERR_SUCCESS) {
			throw std::runtime_error("couldn't get wait handle");
		}
		_camWaitHandle = waitParams.hwait;
	}

	err = dcamcap_start(_camHandle, DCAMCAP_START_SEQUENCE);
	if (err != DCAMERR_SUCCESS) {
		throw std::runtime_error("couldn't start async acq");
	}
	_numberOfImagesDelivered = 0;
}

void HamamatsuCamera::_derivedAbortAsyncAcquisition() {
	DCAMERR err = dcamcap_stop(_camHandle);
	if (err != DCAMERR_SUCCESS) {
		throw std::runtime_error("couldn't abort async acq");
	}

    dcambuf_release(_camHandle);
    _frameBuffer.clear();
}

bool HamamatsuCamera::_waitForNewImageWithTimeout(int timeoutMillis) {
	if (_derivedNewAsyncAcquisitionImageAvailable()) {
		return true;
	}
	DCAMWAIT_START waitParams = { 0 };
	waitParams.size = sizeof(DCAMWAIT_START);
	waitParams.eventmask = DCAMWAIT_CAPEVENT_FRAMEREADY;
	waitParams.timeout = timeoutMillis;
	DCAMERR err = dcamwait_start(_camWaitHandle, &waitParams);
    return _derivedNewAsyncAcquisitionImageAvailable();
}

bool HamamatsuCamera::_derivedNewAsyncAcquisitionImageAvailable() {
	DCAMERR err;
	DCAMCAP_TRANSFERINFO transferInfo = { 0 };
	transferInfo.size = sizeof(DCAMCAP_TRANSFERINFO);
	err = dcamcap_transferinfo(_camHandle, &transferInfo);
	if (err != DCAMERR_SUCCESS) {
		throw std::runtime_error("error in dcam transferinfo");
	}
	return (_numberOfImagesDelivered != transferInfo.nFrameCount);
}

void HamamatsuCamera::_derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) {
	auto imageSize = getActualImageSize();
	int nPixelsInImage = imageSize.first * imageSize.second;
	int nBytesPerImage = nPixelsInImage * sizeof(std::uint16_t);
	if (nBytes != nBytesPerImage) {
		throw std::runtime_error("buffer of invalid size");
	}

	DCAMERR err;
	DCAMCAP_TRANSFERINFO transferInfo = { 0 };
	transferInfo.size = sizeof(DCAMCAP_TRANSFERINFO);
	err = dcamcap_transferinfo(_camHandle, &transferInfo);
	if (err != DCAMERR_SUCCESS) {
		throw std::runtime_error("error in dcam transferinfo");
	}

	int lastAcquiredImageIndex = transferInfo.nNewestFrameIndex;
	int nUnreadImagesInBuffer = transferInfo.nFrameCount - _numberOfImagesDelivered;
    int indexOfEarliestUnreadImage = lastAcquiredImageIndex - nUnreadImagesInBuffer + 1;
	if (indexOfEarliestUnreadImage < 0) {
		indexOfEarliestUnreadImage = kHamamatsuImagesInBuffer + indexOfEarliestUnreadImage;
	}

	std::uint16_t* startOfImage = _frameBuffer.data() + indexOfEarliestUnreadImage * nPixelsInImage;
	memcpy(bufferForThisImage, startOfImage, nBytesPerImage);
	_numberOfImagesDelivered += 1;
}

std::string HamamatsuCamera::_getDCAMString(HDCAM camHandle, int stringID) const {
	DCAMDEV_STRING	param = { 0 };
	char buf[512];
	param.size = sizeof(param);
	param.text = buf;
	param.textbytes = 512;
	param.iString = stringID;

	DCAMERR err = dcamdev_getstring(camHandle, &param);
	if (err != DCAMERR_SUCCESS) {
		throw std::runtime_error("error getting dcam string");
	}
	return std::string(buf);
}

double HamamatsuCamera::_getPropertyValue(HDCAM camHandle, int propertyID, bool ignoreErrors) const {
	double value = 0.0;
	DCAMERR err = dcamprop_getvalue(camHandle, propertyID, &value);
	if (err != DCAMERR_SUCCESS) {
        if (ignoreErrors) {
            return 0.0;
        } else {
            throw std::runtime_error("error getting dcam property value");
        }
	}
	return value;
}

void HamamatsuCamera::_setPropertyValue(HDCAM camHandle, int propertyID, double value, bool ignoreErrors) const {
	DCAMERR err = dcamprop_setvalue(camHandle, propertyID, value);
	if ((err != DCAMERR_SUCCESS) && !ignoreErrors) {
		throw std::runtime_error("error setting dcam property value");
	}
}

std::pair<double, double> HamamatsuCamera::_getPropertyLimits(HDCAM camHandle, int propertyID) const {
	DCAMERR err;
	DCAMPROP_ATTR attr = { 0 };
	attr.iProp = propertyID;
	err = dcamprop_getattr(camHandle, &attr);
	if (err != DCAMERR_SUCCESS) {
		throw std::runtime_error("error get dcam property limits");
	}
	return std::pair<double, double>(attr.valuemin, attr.valuemax);
}

#endif
