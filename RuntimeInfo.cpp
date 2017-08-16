/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#define LOG_TAG "libvintf"

#include "RuntimeInfo.h"

#include "CompatibilityMatrix.h"
#include "parse_string.h"

namespace android {
namespace vintf {

const std::string &RuntimeInfo::osName() const {
    return mOsName;
}

const std::string &RuntimeInfo::nodeName() const {
    return mNodeName;
}

const std::string &RuntimeInfo::osRelease() const {
    return mOsRelease;
}

const std::string &RuntimeInfo::osVersion() const {
    return mOsVersion;
}

const std::string &RuntimeInfo::hardwareId() const {
    return mHardwareId;
}

const KernelVersion &RuntimeInfo::kernelVersion() const {
    return mKernelVersion;
}

const std::map<std::string, std::string> &RuntimeInfo::kernelConfigs() const {
    return mKernelConfigs;
}

size_t RuntimeInfo::kernelSepolicyVersion() const {
    return mKernelSepolicyVersion;
}

const std::string &RuntimeInfo::cpuInfo() const {
    return mCpuInfo;
}

const Version &RuntimeInfo::bootVbmetaAvbVersion() const {
    return mBootVbmetaAvbVersion;
}

const Version &RuntimeInfo::bootAvbVersion() const {
    return mBootAvbVersion;
}

bool RuntimeInfo::checkCompatibility(const CompatibilityMatrix &mat,
            std::string *error) const {
    if (mat.mType != SchemaType::FRAMEWORK) {
        if (error != nullptr) {
            *error = "Should not check runtime info against " + to_string(mat.mType)
                    + " compatibility matrix.";
        }
        return false;
    }
    if (kernelSepolicyVersion() != mat.framework.mSepolicy.kernelSepolicyVersion()) {
        if (error != nullptr) {
            *error = "kernelSepolicyVersion = " + to_string(kernelSepolicyVersion())
                     + " but required " + to_string(mat.framework.mSepolicy.kernelSepolicyVersion());
        }
        return false;
    }

    // mat.mSepolicy.sepolicyVersion() is checked against static HalManifest.device.mSepolicyVersion

    const MatrixKernel *matrixKernel = mat.findKernel(this->mKernelVersion);
    if (matrixKernel == nullptr) {
        if (error != nullptr) {
            *error = "Cannot find suitable kernel entry for " + to_string(mKernelVersion);
        }
        return false;
    }
    for (const KernelConfig &matrixConfig : matrixKernel->configs()) {
        const std::string &key = matrixConfig.first;
        auto it = this->mKernelConfigs.find(key);
        if (it == this->mKernelConfigs.end()) {
            // special case: <value type="tristate">n</value> matches if the config doesn't exist.
            if (matrixConfig.second == KernelConfigTypedValue::gMissingConfig) {
                continue;
            }
            if (error != nullptr) {
                *error = "Missing config " + key;
            }
            return false;
        }
        const std::string &kernelValue = it->second;
        if (!matrixConfig.second.matchValue(kernelValue)) {
            if (error != nullptr) {
                *error = "For config " + key + ", value = " + kernelValue
                        + " but required " + to_string(matrixConfig.second);
            }
            return false;
        }
    }

    const Version &matAvb = mat.framework.mAvbMetaVersion;
    if (mBootAvbVersion.majorVer != matAvb.majorVer || mBootAvbVersion.minorVer < matAvb.minorVer) {
        if (error != nullptr) {
            std::stringstream ss;
            ss << "AVB version " << mBootAvbVersion << " does not match framework matrix "
               << matAvb;
            *error = ss.str();
        }
        return false;
    }
    if (mBootVbmetaAvbVersion.majorVer != matAvb.majorVer ||
        mBootVbmetaAvbVersion.minorVer < matAvb.minorVer) {
        if (error != nullptr) {
            std::stringstream ss;
            ss << "Vbmeta version " << mBootVbmetaAvbVersion << " does not match framework matrix "
               << matAvb;
            *error = ss.str();
        }
        return false;
    }

    return true;
}

} // namespace vintf
} // namespace android
