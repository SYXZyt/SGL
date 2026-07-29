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

/// @brief Contains info about a memory allocation
typedef struct sgl_MemoryTrack sgl_sealed
{
    void* ptr;
    size_t size;

#ifdef SGL_MEMORY_TRACK
    const char* T;
    void* stack[SGL_MEMORY_MAX_STACKFRAMES];
    size_t stackSize;
#endif
} sgl_MemoryTrack;

/// @brief Enable or disale stack tracing. Only works if SGL_MEMORY_TRACK is defined
/// @param enable True to enable or false to disable
SGL_API extern void sgl_Memory_StackTrace(bool enable);

/// @brief Get the total amount of bytes currently allocated
/// @return The total size in bytes
SGL_API extern size_t sgl_Memory_GetTotalAllocated();

/// @brief Display all memory leaks to the console
SGL_API extern void sgl_Memory_ReportLeaks();

/// @brief Push a memory track to the allocator
/// @param ptr The address allocated
/// @param size How many bytes are allocated
/// @param T The type name
SGL_API extern void sgl_Memory_AddTrack(void* ptr, size_t size, const char* T);

/// @brief Pop a track off from the allocator
SGL_API extern void sgl_Memory_PopTrack();

/// @brief Allocate memory
/// @param size How many bytes to allocate
/// @return An address to the allocated region
SGL_API extern void* sgl_Malloc(size_t size);

/// @brief Free a memory allocation
/// @param ptr The address to free
SGL_API extern void sgl_Free(void* ptr);

/// @brief Reallocate an address with a new size
/// @param ptr The address to reallocate
/// @param newSize How many bytes the new size should be
/// @return A pointer to the memory. May be the old pointer if reallocation failed
SGL_API extern void* sgl_Realloc(void* ptr, size_t newSize);


SGL_END

#ifdef __cplusplus
#include <utility>
#include <typeinfo>
#include <new>

namespace sgl
{
    /// @brief Helpers for handling ctors and dtors
    struct Memory final
    {
        /// @brief Instantiate a new object
        /// @tparam T The object type
        /// @tparam ...Args The ctor parameters
        /// @param ...args Parameters to forward
        /// @return An address to the object
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

        /// @brief Delete an object
        /// @tparam T The object type
        /// @param ptr The object to delete
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