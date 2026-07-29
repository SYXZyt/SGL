#pragma once
#include <SGL/SGL.h>

#ifdef _DEBUG
#define SGL_REPORT_ERROR(msg) do { sgl_ReportError((msg), __FILE__, __LINE__); } while (0)
#else
#define SGL_REPORT_ERROR(msg) do { sgl_ReportError((msg), NULL, 0); } while (0)
#endif

SGL_BEGIN

/// @brief Report an error to a message box (Windows only. Linux will use the terminal)
/// @param message The message to show
/// @param file The file where the error occurred. Use NULL to hide this
/// @param line The line where the error occurred. Will not show if file is NULL
SGL_API extern void sgl_ReportError(const char* message, const char* file, int line);

#ifdef _WIN32
/// @brief Convert a HRESULT to a string
/// @param hr The HRESULT to convert
/// @return A sgl_String containing the result. Remember to free this
SGL_API extern const char* sgl_HResultToString(HRESULT hr);
#endif

SGL_END