#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

typedef struct sgl_EngineConfig sgl_sealed
{
    const char* title;
    uint32 width;
    uint32 height;
    bool resizable;
} sgl_EngineConfig;

typedef struct sgl_Window
{
    sgl_EngineConfig cfg;


} sgl_Window;

SGL_END