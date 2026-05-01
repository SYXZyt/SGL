#include  "osbridge.h"
#include <malloc.h>

size_t osbridge::GetPointerSize(void* ptr)
{
#ifdef _WIN32
    return _msize(ptr);
#else
    return malloc_usable_size(ptr);
#endif
}