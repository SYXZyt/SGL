#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

SGL_API extern const char* SGL_VERSION_ID;
SGL_API extern uint32 SGL_VERSION_MAJOR;
SGL_API extern uint32 SGL_VERSION_MINOR;
SGL_API extern uint32 SGL_VERSION_PATCH;

/// @brief Get the current version as a string
SGL_API extern const char* sgl_VersionString();

#define SGL_VERSION_NUMERICAL(MAJOR, MINOR, PATCH) (MAJOR * 10000u + MINOR * 100u + PATCH * 1u)
#define SGL_VERSION_GET_MAJOR(VERSION) ((VERSION) / 10000)
#define SGL_VERSION_GET_MINOR(VERSION) (((VERSION) % 10000) / 100)
#define SGL_VERSION_GET_PATCH(VERSION) ((VERSION) % 100)

SGL_END