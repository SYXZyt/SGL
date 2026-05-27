#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
    #ifdef SGL_EXPORTS
        #define SGL_API __declspec(dllexport)
    #else
        #define SGL_API __declspec(dllimport)
    #endif
#else
    #define SGL_API
#endif

#ifdef __cplusplus
#define SGL_BEGIN extern "C" {
#define SGL_END }
#else
#define SGL_BEGIN
#define SGL_END
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif

#ifdef _MSC_VER
#define sgl_alignas(alignment) __declspec(align(alignment))
#else
#define sgl_alignas(alignment) __attribute__((aligned(alignment)))
#endif

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;

typedef uint32 gluint;
typedef int32 glint;

#define SGL_ENUM_FLAG(i) = (1 << (i))

#define sglFuncPtr(name, returnType, ...) returnType(*name)(__VA_ARGS__)

#ifdef __cplusplus
#define sgl_sealed final
#else
#define sgl_sealed
#endif