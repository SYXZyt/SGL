#include "Logger.h"
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

static sgl_Logger_Callback_ptr gCallback = nullptr;
static void* gCallbackUserdata = nullptr;

#ifdef _WIN32
static std::wstring ToWString(const std::string& str)
{
    if (str.empty())
        return L"";


    int size = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0);

    std::wstring result(size, 0);

    MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), result.data(), size);

    return result;
}
#endif

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

static sgl_LogLevel ToLogLevel(LogType type)
{
    switch (type)
    {
        case LogType::INFO:
            return sgl_LogLevel_INFO;
        case LogType::SUCCESS:
            return sgl_LogLevel_SUCCESS;
        case LogType::WARNING:
            return sgl_LogLevel_WARNING;
        case LogType::ERR:
            return sgl_LogLevel_ERROR;
        default:
            return sgl_LogLevel_INFO;
    }
}

static void LogMessage()
{
    std::lock_guard lock(gMutex);

    if (gMessages.empty())
        return;

    const auto& [message, type] = gMessages.front();

    if (gCallback)
    {
        gCallback(message.c_str(), ToLogLevel(type), gCallbackUserdata);
    }
    else
    {
        std::println("[{}] {}", GetLogTypeString(type), message);

#ifdef _WIN32
        std::wstring wide = ToWString(message);

        OutputDebugStringW(wide.c_str());
        OutputDebugStringW(L"\r\n");
#endif
    }

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

void sgl_Logger_SetCallback(sgl_Logger_Callback_ptr callback, void* userdata)
{
    std::lock_guard lock(gMutex);
    gCallback = callback;
    gCallbackUserdata = userdata;
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