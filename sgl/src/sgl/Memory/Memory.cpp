#include "Memory.h"
#include <vector>
#include <print>
#include <osbridge.h>

static std::vector<sgl::Memory::Track> gTracks;
static size_t gTotalAllocated = 0;

#ifdef SGL_DISALLOW_UNOWNED_POINTERS
#include <unordered_set>
static std::unordered_set<void*> gOwnedPointers;
#endif

size_t sgl::Memory::GetTotalAllocated() {
    return gTotalAllocated;
}

void sgl::Memory::ReportLeaks()
{
    for (const Track& t : gTracks)
        std::println("Memory Leak Detected: Ptr = {}, Size = {}, Type = '{}'", t.ptr, t.size, t.T);
}

void sgl::Memory::AddTrack(void* ptr, size_t size, const char* T)
{
    Track track
    {
        .ptr = ptr,
        .size = size,
        .T = T,
    };

    gTracks.push_back(track);
}

void sgl::Memory::PopTrack()
{
    if (!gTracks.empty())
        gTracks.pop_back();
}

void* sgl::Memory::Malloc(size_t size)
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
    AddTrack(ptr, size, "RawMemory");
#endif

    gTotalAllocated += size;

    return ptr;
}

void* sgl::Memory::Realloc(void* ptr, size_t newSize)
{
    if (!ptr)
        return nullptr;

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
    // Update the size of the track
    {
        auto it = std::find_if(gTracks.begin(), gTracks.end(), [ptr](const Track& track) { return ptr == track.ptr; });

        if (it != gTracks.end())
            it->size = newSize;
        else
            std::println("Attempted to realloc untracked pointer: {}", ptr);
    }
#endif

    size_t currentSize = osbridge::GetPointerSize(ptr);
    gTotalAllocated -= currentSize;

    void* newPtr = std::realloc(ptr, newSize);
    gTotalAllocated += newSize;

    return newPtr;
}

void sgl::Memory::Free(void* ptr) noexcept
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
        auto it = std::find_if(gTracks.begin(), gTracks.end(), [ptr](const Track& track) { return track.ptr == ptr; });

        if (it != gTracks.end())
            gTracks.erase(it);
        else
            std::println("Attempted to free untracked pointer: {}", ptr);
    }
#endif

    gTotalAllocated -= osbridge::GetPointerSize(ptr);

    std::free(ptr);
}