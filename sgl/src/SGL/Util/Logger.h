#pragma once
#include <SGL/SGL.h>
#include <SGL/Util/ANSI.h>

SGL_BEGIN

SGL_API extern void sgl_Logger_Init();
SGL_API extern void sgl_Logger_Shutdown();

SGL_API extern bool sgl_Logger_IsLogging();

SGL_API extern void sgl_Log(const char* message);
SGL_API extern void sgl_LogWarning(const char* message);
SGL_API extern void sgl_LogError(const char* message);
SGL_API extern void sgl_LogSuccess(const char* message);

SGL_END