#pragma once
#include <SGL/SGL.h>
#include <stddef.h>

SGL_BEGIN

/// @brief A custom string wrapper
typedef struct sgl_String sgl_sealed
{
    char* str;
    size_t len;
    size_t capacity;
} sgl_String;

/// @brief Create a string from a char pointer
/// @param str A c string to convert
/// @return A new sgl_String
SGL_API extern sgl_String sgl_MakeString(const char* str);

/// @brief Free an sgl_String
/// @param str The string to free
SGL_API extern void sgl_FreeString(sgl_String str);

/// @brief Append a c string to an sgl_String
/// @param str The string to append to
/// @param toAppend The c string to append
SGL_API extern void sgl_AppendString(sgl_String* str, const char* toAppend);

/// @brief Append an sgl_String to another sgl_String
/// @param str The string to append to
/// @param toAppend The string to append
SGL_API extern void sgl_AppendSglString(sgl_String* str, sgl_String toAppend);

SGL_END

#ifdef __cplusplus
#include <string>
/// @brief Make an sgl_String from a C++ string
/// @param str A std::string
/// @return A converted sgl_String
#define sgl_MakeStringCpp(str) sgl_MakeString(str.c_str())

/// @brief Append a C++ string to an sgl_String
/// @param str The string to append to
/// @param cppstr The std::string to append to
#define sgl_AppendStringCpp(str, cppstr) sgl_AppendString(str, cppstr.c_str())
#endif