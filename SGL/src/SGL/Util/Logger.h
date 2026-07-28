#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

typedef uint8 sgl_LogLevel;
enum
{
    sgl_LogLevel_INFO,
    sgl_LogLevel_SUCCESS,
    sgl_LogLevel_WARNING,
    sgl_LogLevel_ERROR,
};

typedef sglFuncPtr(sgl_Logger_Callback_ptr, void, const char* message, sgl_LogLevel level, void* userdata);

SGL_API extern void sgl_Logger_Init();
SGL_API extern void sgl_Logger_Shutdown();

SGL_API extern bool sgl_Logger_IsLogging();

/// @brief Set a callback to receive log messages
/// @param callback The function to call upon logging. Pass NULL to restore to default logger
SGL_API extern void sgl_Logger_SetCallback(sgl_Logger_Callback_ptr callback, void* userdata);

SGL_API extern void sgl_Log(const char* message);
SGL_API extern void sgl_LogWarning(const char* message);
SGL_API extern void sgl_LogError(const char* message);
SGL_API extern void sgl_LogSuccess(const char* message);

SGL_END