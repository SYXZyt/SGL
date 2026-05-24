#pragma once
#include <sgl/sgl.h>
#include <stddef.h>

SGL_BEGIN

SGL_API extern size_t sgl_os_GetPointerSize(void* ptr);
SGL_API extern bool sgl_os_SupportAnsi();

SGL_END