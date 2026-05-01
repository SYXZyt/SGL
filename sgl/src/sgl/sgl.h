#pragma once
#include <cstdint>

#ifdef _WIN32
    #ifdef SGL_EXPORTS
        #define SGL_API __declspec(dllexport)
    #else
        #define SGL_API __declspec(dllimport)
    #endif
#else
    #define SGL_API
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using byte = uint8_t;
using word = uint16_t;
using dword = uint32_t;
using qword = uint64_t;

#define SGL_ENUM_FLAG(i) = (1 << (i))

#define sglFuncPtr(returnType, ...) returnType(*)(__VA_ARGS__)

#define cbuffer struct alignas(16)

namespace sgl
{
    // If we use singletons, then this can be used to get any singleton
    template <typename T>
    T& Get() {
        return T::Get();
    }
}