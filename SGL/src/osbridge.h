#pragma once
#include <SGL/SGL.h>
#include <stddef.h>

SGL_BEGIN

/// @brief Get the size of a pointer in bytes
SGL_API extern size_t sgl_os_GetPointerSize(void* ptr);

/// @brief Check if this platform supports ANSI escape codes
SGL_API extern bool sgl_os_SupportAnsi();

SGL_END