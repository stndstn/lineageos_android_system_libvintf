/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef ANDROID_VINTF_FILE_SYSTEM_H
#define ANDROID_VINTF_FILE_SYSTEM_H

#include <memory>
#include <mutex>

#include <utils/Errors.h>
#include <vintf/RuntimeInfo.h>
#include <vintf/parse_xml.h>

namespace android {
namespace vintf {

// Queries the file system in the correct way. Files can come from
// an actual file system, a sub-directory, or from ADB, depending on the
// implementation.
//
// This class can be used to create a mock for overriding.
class FileSystem {
   public:
    virtual ~FileSystem(){};
    // Return NAME_NOT_FOUND if file is not found,
    //        OK if file is retrieved and written to "fetched".
    virtual status_t fetch(const std::string& path, std::string* fetched,
                           std::string* error) const = 0;
    // Return NAME_NOT_FOUND if directory is not found,
    //        OK if file names are retrieved and written to out.
    virtual status_t listFiles(const std::string& path, std::vector<std::string>* out,
                               std::string* error) const = 0;
};

namespace details {

// Initialize the global instance.
__attribute__((warn_unused_result)) bool initFileSystem(std::unique_ptr<FileSystem>&& value);

// Return the instance provided through init(), or a default implementation
// if init() is not called. The default implementation queries the actual
// file system on the device and does nothing on host.
// Once get(), cannot be init()-ed again.
FileSystem& getFileSystem();

// Class that actually queries the file system.
class FileSystemImpl : public FileSystem {
   public:
    status_t fetch(const std::string&, std::string*, std::string*) const;
    status_t listFiles(const std::string&, std::vector<std::string>*, std::string*) const;
};

// Class that does nothing.
class FileSystemNoOp : public FileSystem {
   public:
    status_t fetch(const std::string&, std::string*, std::string*) const;
    status_t listFiles(const std::string&, std::vector<std::string>*, std::string*) const;
};

}  // namespace details
}  // namespace vintf
}  // namespace android

#endif
