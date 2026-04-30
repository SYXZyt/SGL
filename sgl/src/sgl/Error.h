#pragma once
#include <sgl/sgl.h>
#include <sgl/String.h>

#ifdef _DEBUG
#define SGL_REPORT_ERROR(msg) do { sgl::ReportError((msg), __FILE__, __LINE__); } while (0)
#else
#define SGL_REPORT_ERROR(msg) do { sgl::ReportError((mag)); } while (0)
#endif

namespace sgl
{
    SGL_API extern void ReportError(const String& message, const char* file = nullptr, int line = 0);
}