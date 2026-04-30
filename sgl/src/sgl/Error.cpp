#include "Error.h"
#include <iostream>
#include <cstdlib>

#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__)
#include <csignal>
#else
#error "Platform has no implementation"
#endif

void sgl::ReportError(const String& message, const char* file, int line)
{
    std::wstring wMsg = ConvertString<String, std::wstring>(message);

    if (file)
    {
        std::wstring fname = ConvertString<const char*, std::wstring>(file);

#ifdef SGL_PATH
        {
            std::wstring projectPath = ConvertString<const char*, std::wstring>(SGL_PATH);

            size_t pos = fname.find(projectPath);
            if (pos != std::wstring::npos)
                fname = fname.substr(pos + projectPath.length());
        }

        wMsg += L"\n\nFile: " + fname;
        if (line != 0)
            wMsg += L"\nLine: " + std::to_wstring(line);
#endif

#ifdef _WIN32
        wMsg += L"\n\nAbort - Quit application\nRetry - Debug application\nIgnore - Continue execution";
#endif

#if defined(_WIN32)
        MessageBeep(MB_ICONERROR);

        enum PromptResponse : uint8
        {
            ABORT = 3,
            RETRY = 4,
            IGNRE = 5,
        };

        int response = MessageBoxW(nullptr, wMsg.c_str(), L"An Error Occurred", MB_ICONERROR | MB_ABORTRETRYIGNORE | MB_DEFBUTTON2);

        switch (response)
        {
            case PromptResponse::ABORT:
                abort();
            case PromptResponse::IGNRE:
                break;

            case PromptResponse::RETRY:
            default:
                DebugBreak();
                break;
        }
#elif defined(__linux__)
        std::string msg = ConvertString<std::wstring, std::string>(wMsg);

        std::cerr << "\n=== Error ===\n" << msg << '\n';
        std::cerr << "[a]bort, [r]etry (debug), [i]gnore: ";

        char input = 'i';
        std::cin >> input;

        switch (input)
        {
            case 'a':
                std::abort();
                [[fallthrough]]
            case 'i':
                break;
            case 'r':
            default:
                raise(SIGTRAP);
                break;
        }
#endif
    }
}