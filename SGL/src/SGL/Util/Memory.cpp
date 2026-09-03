#include "Memory.h"
#include <vector>
#include <osbridge.h>
#include <print>
#include <algorithm>
#include <atomic>
#include <mutex>

#if defined(_WIN32)
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")
#elif defined(__linux__)
#include <execinfo.h>
#endif

#ifdef SGL_MEMORY_TRACK
static std::vector<sgl_MemoryTrack> gTracks;
static std::atomic<size_t> gTotalAllocated = 0;
static bool gStackTrace = false;
static std::mutex gMemoryMutex;
static bool gTrack = false;
#endif

#ifdef SGL_DISALLOW_UNOWNED_POINTERS
#include <unordered_set>
static std::unordered_set<void*> gOwnedPointers;
#endif

static void PrintSymbol(void* addr)
{
#ifdef _WIN32
    char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME]{ 0 };
    SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;

    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = MAX_SYM_NAME;

    DWORD64 displacement = 0;

    if (SymFromAddr(GetCurrentProcess(), (DWORD64)addr, &displacement, symbol))
        std::println("  {} + 0x{:x}", symbol->Name, displacement);
    else
        std::println("  {}", addr);
#else
    char** symbols = backtrace_symbols(&addr, 1);

    if (symbols)
    {
        std::println("  {}", symbols[0]);
        free(symbols);
    }
    else
        std::println("  {}", addr);
#endif
}

static size_t CaptureStack(void** outFrames, size_t maxFrames)
{
#ifdef _WIN32
    USHORT frames = CaptureStackBackTrace(2, (DWORD)maxFrames, outFrames, nullptr);
    return (size_t)frames;
#else
    int frames = backtrace(outFrames, (int)maxFrames);
    return (size_t)frames;
#endif
}

void sgl_Memory_StackTrace(bool enable)
{
#ifdef SGL_MEMORY_TRACK
    gStackTrace = enable;
#endif
}

void sglIntern_Memory_SetTrack(bool enable) {
    gTrack = enable;
}

size_t sgl_Memory_GetTotalAllocated()
{
#ifdef SGL_MEMORY_TRACK
    return gTotalAllocated.load();
#else
    return 0;
#endif
}

void sgl_Memory_ReportLeaks()
{
#ifdef SGL_MEMORY_TRACK
    std::lock_guard lock(gMemoryMutex);

    for (const sgl_MemoryTrack& t : gTracks)
    {
        std::println("Memory Leak Detected: Ptr = {}, Size = {}, Type = '{}'", t.ptr, t.size, t.T);

        if (gStackTrace)
        {
            for (size_t i = 0; i < t.stackSize; ++i)
                PrintSymbol(t.stack[i]);
        }
    }
#endif
}

void sgl_Memory_AddTrack(void* ptr, size_t size, const char* T)
{
#ifdef SGL_MEMORY_TRACK
    if (!gTrack)
        return;

    sgl_MemoryTrack track
    {
        .ptr = ptr,
        .size = size,
        .T = T,
        .stackSize = 0
    };

    if (gStackTrace)
        track.stackSize = CaptureStack(track.stack, SGL_MEMORY_MAX_STACKFRAMES);

    std::lock_guard lock(gMemoryMutex);
    gTracks.push_back(track);
#endif
}

void sgl_Memory_RetagTrack(void* ptr, const char* T)
{
#ifdef SGL_MEMORY_TRACK
    if (!gTrack)
        return;

    std::lock_guard lock(gMemoryMutex);

    auto it = std::find_if(gTracks.begin(), gTracks.end(),
        [ptr](const sgl_MemoryTrack& t) { return t.ptr == ptr; });

    if (it != gTracks.end())
        it->T = T;
#endif
}

void* sgl_Malloc(size_t size)
{
    if (size == 0)
        return nullptr;

    void* ptr = std::malloc(size);
    if (!ptr)
        return nullptr;

#ifdef SGL_DISALLOW_UNOWNED_POINTERS
    {
        std::lock_guard lock(gMemoryMutex);
        gOwnedPointers.insert(ptr);
    }
#endif

#ifdef SGL_MEMORY_TRACK
    sgl_Memory_AddTrack(ptr, size, "RawMemory");

    gTotalAllocated += size;
#endif

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

    void* newPtr = std::realloc(ptr, newSize);
    if (!newPtr)
        return nullptr;

#ifdef SGL_DISALLOW_UNOWNED_POINTERS
    if (newPtr != ptr)
    {
        std::lock_guard lock(gMemoryMutex);

        auto it = gOwnedPointers.find(ptr);
        if (it != gOwnedPointers.end())
        {
            gOwnedPointers.erase(it);
            gOwnedPointers.insert(newPtr);
        }
    }
#endif

#ifdef SGL_MEMORY_TRACK
    {
        std::lock_guard lock(gMemoryMutex);

        auto it = std::find_if(gTracks.begin(), gTracks.end(),
            [ptr](const sgl_MemoryTrack& t) { return t.ptr == ptr; });

        if (it != gTracks.end())
        {
            it->ptr = newPtr;
            it->size = newSize;
        }
    }
#endif

    return newPtr;
}

void sgl_Free(void* ptr)
{
    if (!ptr)
        return;

#ifdef SGL_DISALLOW_UNOWNED_POINTERS
    if (gTrack)
    {

        std::lock_guard lock(gMemoryMutex);

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
    if (gTrack)
    {
        std::lock_guard lock(gMemoryMutex);

        auto it = std::find_if(gTracks.begin(), gTracks.end(), [ptr](const sgl_MemoryTrack& track) { return ptr == track.ptr; });

        if (it != gTracks.end())
            gTracks.erase(it);
        else
        {
            std::println("Attempted to free untracked pointer: {}", ptr);
            return;
        }
    }

    gTotalAllocated -= sgl_os_GetPointerSize(ptr);
#endif
    std::free(ptr);
}