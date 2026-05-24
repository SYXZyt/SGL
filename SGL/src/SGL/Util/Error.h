#pragma once
#include <SGL/SGL.h>

#ifdef _DEBUG
#define SGL_REPORT_ERROR(msg) do { sgl_ReportError((msg), __FILE__, __LINE__); } while (0)
#else
#define SGL_REPORT_ERROR(msg) do { sgl_ReportError((msg), NULL, 0); } while (0)
#endif

SGL_BEGIN

SGL_API extern void sgl_ReportError(const char* message, const char* file, int line);

SGL_END