#include "HamamatsuAPIWrapper.h"

HamamatsuAPIWrapper gHamamatsuAPIWrapper("dcamapi.dll");

HamamatsuAPIWrapper GetHamamatsuAPIWrapper() {
    return HamamatsuAPIWrapper(gHamamatsuAPIWrapper);
}
