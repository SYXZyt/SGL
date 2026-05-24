#include  "osbridge.h"
#include <malloc.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

size_t sgl_os_GetPointerSize(void* ptr)
{
#ifdef _WIN32
    return _msize(ptr);
#else
    return malloc_usable_size(ptr);
#endif
}

bool sgl_os_SupportAnsi()
{
#ifdef _WIN32
    DWORD mode = 0;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleMode(handle, &mode))
        return false;

    return (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
#else
    return isatty(STDOUT_FILENO);
#endif
}