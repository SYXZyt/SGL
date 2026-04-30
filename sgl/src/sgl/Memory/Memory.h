#pragma once
#include <sgl/sgl.h>
#include <new>
#include <typeinfo>
#include <utility>

#define SGL_ALLOCATOR_FRIEND friend struct sgl::Memory;

#ifdef _DEBUG
#define SGL_DISALLOW_UNOWNED_POINTERS
#define SGL_MEMORY_TRACK
#endif

namespace sgl
{
    struct SGL_API Memory final
    {
        struct Track final
        {
            void* ptr;
            size_t size;
            const char* T;
        };

        static size_t GetTotalAllocated();

        static void ReportLeaks();

        static void AddTrack(void* ptr, size_t size, const char* T);
        static void PopTrack();

        static void* Malloc(size_t size);
        static void* Realloc(void* ptr, size_t newSize);
        static void Free(void* ptr) noexcept;

        template <typename T, typename... Args>
        static T* New(Args&&... args)
        {
            void* ptr = Malloc(sizeof(T));
            if (!ptr)
                return nullptr;

#ifdef SGL_MEMORY_TRACK
            // Malloc will have added a track, so we can remove that and add one with the correct type name
            PopTrack();
            AddTrack(ptr, sizeof(T), typeid(T).name());
#endif

            return new (ptr) T(std::forward<Args>(args)...);
        }

        template <typename T>
        static void Delete(T* ptr)
        {
            if (!ptr)
                return;

            ptr->~T();
            Free(ptr);
        }

        Memory() = delete;
    };
}