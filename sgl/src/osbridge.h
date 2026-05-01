#pragma once
#include <sgl/sgl.h>
#include <stddef.h>

namespace osbridge
{
    SGL_API extern size_t GetPointerSize(void* ptr);
    SGL_API extern bool SupportAnsi();
}