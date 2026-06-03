#pragma once
#include <SGL/SGL.h>
#include <stddef.h>

#ifdef _DEBUG
#define SGL_DISALLOW_UNOWNED_POINTERS
#define SGL_MEMORY_TRACK
#endif

#ifdef __cplusplus
#define SGL_ALLOCATOR_FRIEND friend struct sgl::Memory;
#else
#define SGL_ALLOCATOR_FRIEND
#endif

#define SGL_MEMORY_MAX_STACKFRAMES 16

SGL_BEGIN

typedef struct sgl_MemoryTrack sgl_sealed
{
    void* ptr;
    size_t size;
    const char* T;

    void* stack[SGL_MEMORY_MAX_STACKFRAMES];
    size_t stackSize;
} sgl_MemoryTrack;

SGL_API extern void sgl_Memory_StackTrace(bool enable);
SGL_API extern size_t sgl_Memory_GetTotalAllocated();
SGL_API extern void sgl_Memory_ReportLeaks();
SGL_API extern void sgl_Memory_AddTrack(void* ptr, size_t size, const char* T);
SGL_API extern void sgl_Memory_PopTrack();

SGL_API extern void* sgl_Malloc(size_t size);
SGL_API extern void sgl_Free(void* ptr);
SGL_API extern void* sgl_Realloc(void* ptr, size_t newSize);


SGL_END

#ifdef __cplusplus
#include <utility>
#include <typeinfo>

namespace sgl
{
    struct Memory final
    {
        template<typename T, typename... Args>
        static T* New(Args&&... args)
        {
            void* ptr = sgl_Malloc(sizeof(T));
            if (!ptr)
                return nullptr;

#ifdef SGL_MEMORY_TRACK
            // Malloc will have added a track, so we can remove that and add one with the correct type name
            sgl_Memory_PopTrack();
            sgl_Memory_AddTrack(ptr, sizeof(T), typeid(T).name());
#endif

            return new (ptr) T(std::forward<Args>(args)...);
        }

        template<typename T>
        static void Delete(T* ptr)
        {
            if (!ptr)
                return;

            ptr->~T();
            sgl_Free(ptr);
        }
    };
}

#endif