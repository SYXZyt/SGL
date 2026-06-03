#include "String.h"
#include <cstring>
#include <SGL/Util/Memory.h>

sgl_String sgl_MakeString(const char* str)
{
    sgl_String sglString{};

    size_t len = std::strlen(str);
    sglString.capacity = len + 1;
    sglString.str = (char*)sgl_Malloc(sglString.capacity);
    sglString.len = len;

    std::memcpy(sglString.str, str, len);
    sglString.str[sglString.len] = '\0';

    return sglString;
}

void sgl_FreeString(sgl_String str)
{
    sgl_Free(str.str);
    str.len = str.capacity = 0;
}

void sgl_AppendString(sgl_String* str, const char* toAppend)
{
    std::string us = str->str;
    std::string them = toAppend;

    std::string appended = us + them;

    str->capacity = appended.length() + 1;
    str->len = str->capacity - 1;
    str->str = (char*)sgl_Realloc(str->str, str->capacity);
    std::memcpy(str->str, appended.c_str(), str->capacity);
}

void sgl_AppendSglString(sgl_String* str, sgl_String toAppend) {
    sgl_AppendString(str, toAppend.str);
}