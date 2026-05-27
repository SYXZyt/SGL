#include "Memory.h"
#include <vector>
#include <osbridge.h>
#include <print>

static std::vector<sgl_MemoryTrack> gTracks;
static size_t gTotalAllocated = 0;

#ifdef SGL_DISALLOW_UNOWNED_POINTERS
#include <unordered_set>
static std::unordered_set<void*> gOwnedPointers;
#endif

size_t sgl_Memory_GetTotalAllocated() {
    return gTotalAllocated;
}

void sgl_Memory_ReportLeaks()
{
    for (const sgl_MemoryTrack& t : gTracks)
        std::println("Memory Leak Detected: Ptr = {}, Size = {}, Type = '{}'", t.ptr, t.size, t.T);
}

void sgl_Memory_AddTrack(void* ptr, size_t size, const char* T)
{
    sgl_MemoryTrack track
    {
        .ptr = ptr,
        .size = size,
        .T = T
    };

    gTracks.push_back(track);
}

void sgl_Memory_PopTrack()
{
    if (!gTracks.empty())
        gTracks.pop_back();
}

void* sgl_Malloc(size_t size)
{
    if (size == 0)
        return nullptr;

    void* ptr = std::malloc(size);
    if (!ptr)
        return nullptr;

#ifdef SGL_DISALLOW_UNOWNED_POINTERS
    gOwnedPointers.insert(ptr);
#endif

#ifdef SGL_MEMORY_TRACK
    sgl_Memory_AddTrack(ptr, size, "RawMemory");
#endif

    gTotalAllocated += size;

    return ptr;
}

void* sgl_Realloc(void* ptr, size_t newSize)
{
    if (!ptr)
        return sgl_Malloc(newSize);

    if (newSize == 0)
    {
        sgl_Free(ptr);
        return nullptr;
    }

#ifdef SGL_DISALLOW_UNOWNED_POINTERS
    {
        auto it = gOwnedPointers.find(ptr);

        if (it == gOwnedPointers.end())
        {
            std::println("Cannot realloc unowned pointer: {}", ptr);
            return nullptr;
        }
    }
#endif

#ifdef SGL_MEMORY_TRACK
    {
        auto it = std::find_if(gTracks.begin(), gTracks.end(), [ptr](const sgl_MemoryTrack& track) { return ptr == track.ptr; });

        if (it != gTracks.end())
            it->size = newSize;
        else
        {
            std::println("Attempted to realloc untracked pointer");
            return nullptr;
        }
    }
#endif

    size_t currentSize = sgl_os_GetPointerSize(ptr);
    gTotalAllocated -= currentSize;

    void* newPtr = std::realloc(ptr, newSize);
    gTotalAllocated += newSize;

    return newPtr;
}

void sgl_Free(void* ptr)
{
    if (!ptr)
        return;

#ifdef SGL_DISALLOW_UNOWNED_POINTERS
    {
        auto it = gOwnedPointers.find(ptr);

        if (it == gOwnedPointers.end())
        {
            std::println("Attempted to free unowned pointer: {}", ptr);
            return;
        }

        gOwnedPointers.erase(it);
    }
#endif

#ifdef SGL_MEMORY_TRACK
    {
        auto it = std::find_if(gTracks.begin(), gTracks.end(), [ptr](const sgl_MemoryTrack& track) { return ptr == track.ptr; });

        if (it != gTracks.end())
            gTracks.erase(it);
        else
        {
            std::println("Attempted to free untracked pointer: {}", ptr);
            return;
        }
    }
#endif

    gTotalAllocated -= sgl_os_GetPointerSize(ptr);
    std::free(ptr);
}