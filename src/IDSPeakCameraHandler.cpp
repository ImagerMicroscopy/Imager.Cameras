#include <memory>
#include <stdexcept>

#include "IDSPeakCameraHandler.h"
#include "IDSPeakCamera.h"
#include "SCPrinter.h"

#ifndef _WIN32
#include <cstdlib>
#include <dlfcn.h>
#include <filesystem>
#include <glob.h>
#include <string>

namespace fs = std::filesystem;

static bool DirectoryHasCtiFiles(const fs::path& dir) {
    glob_t matches{};
    const std::string pattern = (dir / "*.cti").string();
    const bool found = (glob(pattern.c_str(), 0, nullptr, &matches) == 0 && matches.gl_pathc > 0);
    globfree(&matches);
    return found;
}

// peak_Library_Init fails when GENICAM_GENTL64_PATH is not set. If the variable is
// missing, point it at the GenTL producer directory that belongs to the IDS Peak SDK
// whose library was actually loaded, so producers and library always match versions.
static void EnsureGenTLPathIsSet(const IDSPeakAPIWrapper& apiWrapper) {
    if (std::getenv("GENICAM_GENTL64_PATH") != nullptr) {
        return;
    }

    // Locate the loaded comfortC library and look for the producer directory
    // relative to it.
    Dl_info info{};
    if (dladdr(reinterpret_cast<const void*>(apiWrapper.peak_Library_Init), &info) != 0
        && info.dli_fname != nullptr) {
        std::error_code ec;
        const fs::path libDir = fs::canonical(fs::path(info.dli_fname), ec).parent_path();
        if (!ec) {
            const fs::path candidates[] = {
                libDir / "ids-peak" / "cti",
                libDir / "cti",
                libDir / ".." / "ids-peak" / "cti",
            };
            for (const fs::path& candidate : candidates) {
                std::error_code candidateEc;
                const fs::path canonicalCandidate = fs::canonical(candidate, candidateEc);
                if (!candidateEc && DirectoryHasCtiFiles(canonicalCandidate)) {
                    setenv("GENICAM_GENTL64_PATH", canonicalCandidate.c_str(), 0);
                    return;
                }
            }
        }
    }

    // Fall back to well-known install locations.
    const char* searchPatterns[] = {
        "/opt/ids-peak*/lib/*/ids-peak/cti",
        "/opt/ids/peak/lib/*/ids-peak/cti",
    };
    for (const char* pattern : searchPatterns) {
        glob_t matches{};
        if (glob(pattern, 0, nullptr, &matches) == 0 && matches.gl_pathc > 0) {
            setenv("GENICAM_GENTL64_PATH", matches.gl_pathv[0], 0);
            globfree(&matches);
            return;
        }
        globfree(&matches);
    }
}
#endif

std::vector<std::unique_ptr<BaseCameraClass>> OpenIDSPeakCameras() {
    IDSPeakAPIWrapper apiWrapper;
    if (!apiWrapper.areAllFunctionsLoaded()) {
        return std::vector<std::unique_ptr<BaseCameraClass>>();
    }

    Print("Found IDS Peak runtime");
#ifndef _WIN32
    EnsureGenTLPathIsSet(apiWrapper);
#endif

    bool libraryInitialized = false;
    try {
        peak_status status = PEAK_STATUS_SUCCESS;
        status = apiWrapper.peak_Library_Init();
        if (PEAK_ERROR(status)) {
            throw std::runtime_error("can't init IDS Peak library");
        }
        libraryInitialized = true;

        status = apiWrapper.peak_CameraList_Update(nullptr);

        size_t nCameras = 20;
        std::unique_ptr<peak_camera_descriptor[]> camDescriptors(new peak_camera_descriptor[20]);
        status = apiWrapper.peak_CameraList_Get(camDescriptors.get(), &nCameras);
        if (PEAK_ERROR(status)) {
            throw std::runtime_error("can't list cameras");
        }

        std::vector<std::unique_ptr<BaseCameraClass>> cameras;
        for (size_t i = 0; i < nCameras; ++i) {
            try {
                cameras.emplace_back(new IDSPeakCamera(camDescriptors[i]));
            } catch (const std::exception& e) {
                Print("Failed to open IDS Peak camera: " + std::string(e.what()));
            }
        }

        return cameras;
    } catch (const std::exception& e) {
        // A broken IDS Peak runtime must not abort camera discovery for the other vendors.
        if (libraryInitialized) {
            apiWrapper.peak_Library_Exit();
        }
        Print("IDS Peak camera detection failed: " + std::string(e.what()));
        return std::vector<std::unique_ptr<BaseCameraClass>>();
    }
}

void CloseIDSPeakLibrary() {
    IDSPeakAPIWrapper apiWrapper;
    if (apiWrapper.areAllFunctionsLoaded()) {
        apiWrapper.peak_Library_Exit();
    }
}
