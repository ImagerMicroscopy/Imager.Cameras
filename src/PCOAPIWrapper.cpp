#include "PCOAPIWrapper.h"

PCOAPIWrapper gAPIWrapper("sc2_cam");

PCOAPIWrapper GetPCOAPIWrapper() {
    return PCOAPIWrapper(gAPIWrapper);
}


