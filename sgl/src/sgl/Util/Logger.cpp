#include "Logger.h"
#include <osbridge.h>
#include <print>
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <chrono>

enum class LogType : uint8
{
    INFO,
    SUCCESS,
    WARNING,
    ERR,
};

struct Message final
{
    sgl::String message;
    LogType type;
};

static std::jthread gThread;
static std::mutex gMutex;
static std::queue<Message> gMessages;
static bool gIsRunning = false;

static const char8_t* GetLogTypeString(LogType type)
{
    switch (type)
    {
        case LogType::INFO:
            return u8"INFO";
        case LogType::SUCCESS:
            return u8"SUCCESS";
        case LogType::WARNING:
            return u8"WARNING";
        case LogType::ERR:
            return u8"ERROR";
        default:
            return u8"UNKNOWN";
    }
}

static sgl::String GetColour(const LogType type)
{
    if (!osbridge::SupportAnsi())
        return u8"";

    switch (type)
    {
        case LogType::INFO:
            return sgl::ANSI::Colours::Logger::Info;
        case LogType::SUCCESS:
            return sgl::ANSI::Colours::Logger::Success;
        case LogType::WARNING:
            return sgl::ANSI::Colours::Logger::Warning;
        case LogType::ERR:
            return sgl::ANSI::Colours::Logger::Error;
        default:
            return u8"";
    }
}

static sgl::String ResetColour() {
    return osbridge::SupportAnsi() ? sgl::ANSI::Reset : u8"";
}

static void LogMessage()
{
    if (gMessages.empty())
        return;

    std::lock_guard lock(gMutex);

    const auto& [message, type] = gMessages.front();

    const sgl::String typeStr = GetLogTypeString(type);

    sgl::String colouredOutput = GetColour(type) + u8"[" + typeStr + u8"] " + ResetColour() + message;
    sgl::String whiteOutput = u8"[" + typeStr + u8"] " + message;

    std::println("{}", sgl::ConvertString<sgl::String, std::string>(colouredOutput));

#ifdef _WIN32
    OutputDebugStringW(sgl::ConvertString<st::String, std::wstring>(whiteOutput).c_str());
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

void sgl::Logger::Init()
{
    if (gIsRunning)
        return;

    gIsRunning = true;
    gThread = std::jthread(LogThread);

    LogSuccess(u8"Logger initialised");
}

void sgl::Logger::Shutdown()
{
    if (!gIsRunning)
        return;

    LogSuccess(u8"Shutting down logger");
    gIsRunning = false;

    while (!gMessages.empty())
        LogMessage();
}

void sgl::Logger::Log(const String& message)
{
    std::lock_guard lock(gMutex);
    gMessages.push({ message, LogType::INFO });
}

void sgl::Logger::LogWarning(const String& message)
{
    std::lock_guard lock(gMutex);
    gMessages.push({ message, LogType::WARNING });
}

void sgl::Logger::LogError(const String& message)
{
    std::lock_guard lock(gMutex);
    gMessages.push({ message, LogType::ERR });
}

void sgl::Logger::LogSuccess(const String& message)
{
    std::lock_guard lock(gMutex);
    gMessages.push({ message, LogType::SUCCESS });
}