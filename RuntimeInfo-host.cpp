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
#include <android-base/logging.h>

#include <vintf/RuntimeInfo.h>

namespace android {
namespace vintf {

status_t RuntimeInfo::fetchAllInformation(RuntimeInfo::FetchFlags /* flags */) {
    LOG(ERROR) << "Should not run fetchAllInformation on host.";
    return UNKNOWN_ERROR;
}

} // namespace vintf
} // namespace android
