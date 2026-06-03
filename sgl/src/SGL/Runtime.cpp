#include "Runtime.h"
#include <SGL/Util/Memory.h>

#ifdef _WIN32
#include <Windows.h>
#endif

void sgl_Runtime_Init()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
}

void sgl_Runtime_Shutdown()
{
    sgl_Memory_ReportLeaks();
}