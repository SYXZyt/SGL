#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

/// @brief Log severity
typedef uint8 sgl_LogLevel;
enum
{
    sgl_LogLevel_INFO,
    sgl_LogLevel_SUCCESS,
    sgl_LogLevel_WARNING,
    sgl_LogLevel_ERROR,
};

typedef sglFuncPtr(sgl_Logger_Callback_ptr, void, const char* message, sgl_LogLevel level, void* userdata);

/// @brief Initialise and start the logger
SGL_API extern void sgl_Logger_Init();

/// @brief Shutdown the logger and flush any cached messages
SGL_API extern void sgl_Logger_Shutdown();

/// @brief Check if the logger is running or not
SGL_API extern bool sgl_Logger_IsLogging();

/// @brief Set a callback to receive log messages
/// @param callback The function to call upon logging. Pass NULL to restore to default logger
SGL_API extern void sgl_Logger_SetCallback(sgl_Logger_Callback_ptr callback, void* userdata);

/// @brief Log an info message
/// @param message The message
SGL_API extern void sgl_Log(const char* message);

/// @brief Log a warning message
/// @param message The message
SGL_API extern void sgl_LogWarning(const char* message);

/// @brief Log an error message
/// @param message The message
SGL_API extern void sgl_LogError(const char* message);

/// @brief Log a success message
/// @param message The message
SGL_API extern void sgl_LogSuccess(const char* message);

SGL_END