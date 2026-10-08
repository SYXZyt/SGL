#include "Runtime.h"
#include <SGL/Util/Memory.h>

#ifdef _WIN32
#include <Windows.h>
#include <DbgHelp.h>
#endif

void sgl_Runtime_Init()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SymInitialize(GetCurrentProcess(), nullptr, TRUE);
#endif

    sglIntern_Memory_SetTrack(true);
}

void sgl_Runtime_Shutdown()
{
    sglIntern_Memory_SetTrack(false);
    sgl_Memory_ReportLeaks();
}