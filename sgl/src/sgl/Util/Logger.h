#pragma once
#include <sgl/sgl.h>
#include <sgl/Util/ANSI.h>

namespace sgl::Logger
{
    SGL_API extern void Init();
    SGL_API extern void Shutdown();

    SGL_API extern bool IsLogging();

    SGL_API extern void Log(const String& message);
    SGL_API extern void LogWarning(const String& message);
    SGL_API extern void LogError(const String& message);
    SGL_API extern void LogSuccess(const String& message);
}