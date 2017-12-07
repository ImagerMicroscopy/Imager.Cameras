#include "SCConfigure.h"

#ifdef WITH_HAMAMATSU

#include "HamamatsuCamera.h"

#include "DCAM/dcamprop.h"

HamamatsuCamera::HamamatsuCamera(HDCAM camHandle) :
	_camHandle(camHandle),
	_camWaitHandle(nullptr),
	_haveAttachedBuffers(false)
{
	_camHandle = camHandle;
	_camName = _getDCAMString(_camHandle, DCAM_IDSTR_MODEL) + " (" + _getDCAMString(_camHandle, DCAM_IDSTR_CAMERAID) + ")";
	std::pair<int, int> sensorSize = getSensorSize();
	_nBytesPerImage = sensorSize.first * sensorSize.second * sizeof(std::uint16_t);
}

HamamatsuCamera::~HamamatsuCamera() {
	if (_haveAttachedBuffers) {
		dcambuf_release(_camHandle);
		_haveAttachedBuffers = false;
	}
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
	_setPropertyValue(_camHandle, DCAM_IDPROP_SENSITIVITY, emGain);
}

double HamamatsuCamera::getExposureTime() const {
	return _getPropertyValue(_camHandle, DCAM_IDPROP_EXPOSURETIME);
}

double HamamatsuCamera::getEMGain() const {
	return _getPropertyValue(_camHandle, DCAM_IDPROP_SENSITIVITY);
}

double HamamatsuCamera::getTemperature() const {
	return _getPropertyValue(_camHandle, DCAM_IDPROP_SENSORTEMPERATURE);
}

double HamamatsuCamera::getTemperatureSetpoint() const {
	return _getPropertyValue(_camHandle, DCAM_IDPROP_SENSORTEMPERATURETARGET);
}

std::pair<int, int> HamamatsuCamera::getSensorSize() const {
	std::pair<int, int> size;
	size.first = _getPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYHSIZE);
	size.second = _getPropertyValue(_camHandle, DCAM_IDPROP_SUBARRAYVSIZE);
	return size;
}

void HamamatsuCamera::_derivedSetTemperature(const double temperature) {
	_setPropertyValue(_camHandle, DCAM_IDPROP_SENSORTEMPERATURETARGET, temperature);
}

std::pair<double, double> HamamatsuCamera::_derivedGetEMGainRange() {
	return std::pair<double, double>(0.0, 255.0);
}

void HamamatsuCamera::_setCoolerOn(const bool on) {
	int action = on ? DCAMPROP_SENSORCOOLER__ON : DCAMPROP_SENSORCOOLER__OFF;
	_setPropertyValue(_camHandle, DCAM_IDPROP_SENSORCOOLER, action);
}

void HamamatsuCamera::_derivedStartAsyncAcquisition() {
	std::pair<int, int> sensorSize = getSensorSize();
	int nPixelsInImage = sensorSize.first * sensorSize.second;
	
	if (_frameBuffer.empty()) {
	    _frameBuffer.resize(sensorSize.first * sensorSize.second * kHamamatsuImagesInBuffer);
    }
	std::uint16_t* bufferPtrs[kHamamatsuImagesInBuffer];
	for (int i = 0; i < kHamamatsuImagesInBuffer; i++) {
		bufferPtrs[i] = _frameBuffer.data() + i * nPixelsInImage;
	}

	DCAMERR err;
	if (!_haveAttachedBuffers) {
		DCAMBUF_ATTACH attachParams = { 0 };
		attachParams.size = sizeof(attachParams);
		attachParams.iKind = DCAMBUF_ATTACHKIND_FRAME;
		attachParams.buffer = reinterpret_cast<void**>(&bufferPtrs);
		attachParams.buffercount = kHamamatsuImagesInBuffer;
		err = dcambuf_attach(_camHandle, &attachParams);
	if (err != 0) {
		throw std::runtime_error("couldn't attach buffers");
	}
    _haveAttachedBuffers = true;
}

	if (_camWaitHandle == nullptr) {
		DCAMWAIT_OPEN waitParams = { 0 };
		waitParams.size = sizeof(DCAMWAIT_OPEN);
		waitParams.hdcam = _camHandle;
		err = dcamwait_open(&waitParams);
		if (err != 0) {
			throw std::runtime_error("couldn't get wait handle");
		}
		_camWaitHandle = waitParams.hwait;
	}

	err = dcamcap_start(_camHandle, DCAMCAP_START_SEQUENCE);
	if (err != 0) {
		throw std::runtime_error("couldn't start async acq");
	}
	_numberOfImagesDelivered = 0;
}

void HamamatsuCamera::_derivedAbortAsyncAcquisition() {
	DCAMERR err = dcamcap_stop(_camHandle);
	if (err != 0) {
		throw std::runtime_error("couldn't abort async acq");
	}
}

bool HamamatsuCamera::_waitForNewImageWithTimeout(int timeoutMillis) {
	DCAMWAIT_START waitParams = { 0 };
	waitParams.size = sizeof(DCAMWAIT_START);
	waitParams.eventmask = DCAMCAP_EVENT_FRAMEREADY;
	waitParams.timeout = timeoutMillis;
	DCAMERR err = dcamwait_start(_camHandle, &waitParams);
	return ((err == 0) && (waitParams.eventHappened == DCAMCAP_EVENT_FRAMEREADY));
}

bool HamamatsuCamera::_derivedNewAsyncAcquisitionImageAvailable() {
	DCAMERR err;
	DCAMCAP_TRANSFERINFO transferInfo = { 0 };
	transferInfo.size = sizeof(DCAMCAP_TRANSFERINFO);
	err = dcamcap_transferinfo(_camHandle, &transferInfo);
	if (err != 0) {
		throw std::runtime_error("error in dcam transferinfo");
	}
	return (_numberOfImagesDelivered != transferInfo.nFrameCount);
}

void HamamatsuCamera::_derivedStoreNewImageInBuffer(std::uint16_t* bufferForThisImage, int nBytes) {
	int nPixelsInImage  = _nBytesPerImage / 2;
	if (nBytes != _nBytesPerImage) {
		throw std::runtime_error("buffer of invalid size");
	}

	DCAMERR err;
	DCAMCAP_TRANSFERINFO transferInfo = { 0 };
	transferInfo.size = sizeof(DCAMCAP_TRANSFERINFO);
	err = dcamcap_transferinfo(_camHandle, &transferInfo);
	if (err != 0) {
		throw std::runtime_error("error in dcam transferinfo");
	}

	int lastAcquiredImageIndex = transferInfo.nNewestFrameIndex;
	int nUnreadImagesInBuffer = transferInfo.nFrameCount - _numberOfImagesDelivered;
    int indexOfEarliestUnreadImage = lastAcquiredImageIndex - nUnreadImagesInBuffer + 1;
	if (indexOfEarliestUnreadImage < 0) {
		indexOfEarliestUnreadImage = kHamamatsuImagesInBuffer + indexOfEarliestUnreadImage;
	}

	std::uint16_t* startOfImage = _frameBuffer.data() + indexOfEarliestUnreadImage * nPixelsInImage;
	memcpy(bufferForThisImage, startOfImage, _nBytesPerImage);
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
	if (err != 0) {
		throw std::runtime_error("error getting dcam string");
	}
	return std::string(buf);
}

double HamamatsuCamera::_getPropertyValue(HDCAM camHandle, int propertyID) const {
	double value = 0.0;
	DCAMERR err = dcamprop_queryvalue(camHandle, propertyID, &value, DCAMPROP_OPTION_NONE);
	if (err != 0) {
		throw std::runtime_error("error getting dcam property value");
	}
	return value;
}

void HamamatsuCamera::_setPropertyValue(HDCAM camHandle, int propertyID, double value) const {
	DCAMERR err = dcamprop_setvalue(camHandle, propertyID, value);
	if (err != 0) {
		throw std::runtime_error("error setting dcam property value");
	}
}

#endif
