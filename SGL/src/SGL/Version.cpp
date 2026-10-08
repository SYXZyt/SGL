#include "Version.h"
#include <string>

const char* SGL_VERSION_ID = "Beta";
uint32 SGL_VERSION_MAJOR = 0;
uint32 SGL_VERSION_MINOR = 6;
uint32 SGL_VERSION_PATCH = 0;
static std::string gVersionString = std::string(SGL_VERSION_ID) + " " + std::to_string(SGL_VERSION_MAJOR) + "." + std::to_string(SGL_VERSION_MINOR) + "." + std::to_string(SGL_VERSION_PATCH);

const char* sgl_VersionString() {
    return gVersionString.c_str();
}