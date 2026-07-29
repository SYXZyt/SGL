#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

/// @brief Startup the SGL runtime. Should be the first call in your app. For now, only does things on Windows
SGL_API extern void sgl_Runtime_Init();

/// @brief Shutdown the SGL runtime. Should be the last call in your app. Will report leaks
SGL_API extern void sgl_Runtime_Shutdown();

SGL_END