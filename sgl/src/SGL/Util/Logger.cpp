#include "Logger.h"
#include <osbridge.h>
#include <print>
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <chrono>
#include <SGL/Util/String.h>

enum class LogType : uint8
{
    INFO,
    SUCCESS,
    WARNING,
    ERR,
};

struct Message final
{
    std::string message;
    LogType type;
};

static std::jthread gThread;
static std::mutex gMutex;
static std::queue<Message> gMessages;
static bool gIsRunning = false;

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

static const char* GetLogTypeString(LogType type)
{
    switch (type)
    {
        case LogType::INFO:
            return "INFO";
        case LogType::SUCCESS:
            return "SUCCESS";
        case LogType::WARNING:
            return "WARNING";
        case LogType::ERR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

static const char* GetColour(const LogType type)
{
    if (!sgl_os_SupportAnsi())
        return "";

    switch (type)
    {
        case LogType::INFO:
            return sgl_ANSI_Logger_Info;
        case LogType::SUCCESS:
            return sgl_ANSI_Logger_Success;
        case LogType::WARNING:
            return sgl_ANSI_Logger_Warning;
        case LogType::ERR:
            return sgl_ANSI_Logger_Error;
        default:
            return "";
    }
}

static const char* ResetColour() {
    return sgl_os_SupportAnsi() ? sgl_ANSI_Reset : "";
}

static void LogMessage()
{
    if (gMessages.empty())
        return;

    std::lock_guard lock(gMutex);

    const auto& [message, type] = gMessages.front();
    const char* typeStr = GetLogTypeString(type);

    std::string colouredOutput = GetColour(type) + std::string("[") + typeStr + "] " + ResetColour() + message;
    std::string whiteOutput = std::string("[") + typeStr + "]" + message;

    std::println("{}", colouredOutput);

#ifdef _WIN32
    std::wstring wide = ToWString(message);

    OutputDebugStringW(wide.c_str());
    OutputDebugStringW(L"\r\n");
#endif

    gMessages.pop();
}

static void LogThread()
{
    while (gIsRunning)
    {
        LogMessage();

        if (gMessages.empty())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void sgl_Logger_Init()
{
    if (gIsRunning)
        return;

    gIsRunning = true;
    gThread = std::jthread(LogThread);

    sgl_LogSuccess("Logger initialised");
}

void sgl_Logger_Shutdown()
{
    if (!gIsRunning)
        return;

    sgl_LogSuccess("Shutting down logger");
    gIsRunning = false;

    while (!gMessages.empty())
        LogMessage();
}

bool sgl_Logger_IsLogging() {
    return gIsRunning;
}

void sgl_Log(const char* message)
{
    std::lock_guard lock(gMutex);
    gMessages.push({ message, LogType::INFO });
}

void sgl_LogWarning(const char* message)
{
    std::lock_guard lock(gMutex);
    gMessages.push({ message, LogType::WARNING });
}

void sgl_LogError(const char* message)
{
    std::lock_guard lock(gMutex);
    gMessages.push({ message, LogType::ERR });
}

void sgl_LogSuccess(const char* message)
{
    std::lock_guard lock(gMutex);
    gMessages.push({ message, LogType::SUCCESS });
}