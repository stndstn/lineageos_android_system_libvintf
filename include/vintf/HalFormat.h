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


#ifndef ANDROID_VINTF_HAL_FORMAT_H
#define ANDROID_VINTF_HAL_FORMAT_H

#include <stdint.h>
#include <string>
#include <array>

namespace android {
namespace vintf {

enum class HalFormat : size_t {
    HIDL = 0,
    NATIVE,
    AIDL,
};

// clang-format off
static constexpr std::array<const char*, 3> gHalFormatStrings = {
    "hidl",
    "native",
    "aidl"
};
// clang-format on

} // namespace vintf
} // namespace android

#endif // ANDROID_VINTF_HAL_FORMAT_H
