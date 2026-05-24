#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

typedef struct sgl_String sgl_sealed
{
    char* str;
    size_t len;
    size_t capacity;
} sgl_String;

SGL_API extern sgl_String sgl_MakeString(const char* str);
SGL_API extern void sgl_FreeString(sgl_String str);

SGL_API extern void sgl_AppendString(sgl_String* str, const char* toAppend);
SGL_API extern void sgl_AppendSglString(sgl_String* str, sgl_String toAppend);

SGL_END

#ifdef __cplusplus
#include <string>
#define sgl_MakeStringCpp(str) sgl_MakeString(str.c_str())
#define sgl_AppendStringCpp(str, cppstr) sgl_AppendString(str, cppstr.c_str())
#endif