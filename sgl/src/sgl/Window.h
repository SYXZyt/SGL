#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

typedef struct sgl_EngineConfig
{
    const char* title = nullptr;
    uint32 width = 1920;
    uint32 height = 1080;
    bool resizable = true;
} sgl_EngineConfig;

typedef struct sgl_Window
{
    sgl_EngineConfig cfg;


} sgl_Window;

SGL_END