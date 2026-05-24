#include "Error.h"
#include <iostream>
#include <cstdlib>
#include <string>

#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__)
#include <csignal>
#else
#error "Platform has no implementation"
#endif

static std::wstring ToWString(const std::string& str)
{
    if (str.empty())
        return L"";

#ifdef _WIN32

    int size = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0);

    std::wstring result(size, 0);

    MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), result.data(), size);

    return result;

#else

    std::mbstate_t state{};
    const char* src = str.data();

    size_t size = mbsrtowcs(nullptr, &src, 0, &state);
    if (size == (size_t)-1)
        return {};

    std::wstring result(size, 0);

    state = std::mbstate_t{};
    src = str.data();

    mbsrtowcs(result.data(), &src, size, &state);

    return result;

#endif
}

static std::string ToString(const std::wstring& str)
{
    if (str.empty())
        return "";

#ifdef _WIN32

    int size = WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0, nullptr, nullptr);

    std::string result(size, 0);

    WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), result.data(), size, nullptr, nullptr);

    return result;

#else

    std::mbstate_t state{};
    const wchar_t* src = str.data();

    size_t size = wcsrtombs(nullptr, &src, 0, &state);
    if (size == (size_t)-1)
        return {};

    std::string result(size, 0);

    state = std::mbstate_t{};
    src = str.data();

    wcsrtombs(result.data(), &src, size, &state);

    return result;

#endif
}

void sgl_ReportError(const char* message, const char* file, int line)
{
    std::wstring wMsg = ToWString(message);

    if (file)
    {
        std::wstring fname = ToWString(file);

#ifdef SGL_PATH
        {
            std::wstring projectPath = ToWString(SGL_PATH);

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
        std::string msg = ToString(wMsg);

        std::cerr << "\n=== Error ===\n" << msg << '\n';
        std::cerr << "[a]bort, [r]etry (debug), [i]gnore: ";

        char input = 'i';
        std::cin >> input;

        switch (input)
        {
            case 'a':
                std::abort();
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