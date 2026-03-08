#include "logger.h"
#include <iostream>
#include <sstream>
#if defined(_WIN32)
#include <windows.h>
#endif

Logger& Logger::Get()
{
    static Logger instance;
    return instance;
}

std::string Logger::GetLevelString(LogLevel level) const
{
    switch (level)
    {
    case LogLevel::Debug:
        return "[DEBUG]";
    case LogLevel::Info:
        return "[INFO]";
    case LogLevel::Warning:
        return "[WARNING]";
    case LogLevel::Error:
        return "[ERROR]";
    default:
        return "[UNKNOWN]";
    }
}

static bool s_consoleAnsiEnabled = false;
static void EnsureAnsiEnabled()
{
    if (s_consoleAnsiEnabled)
        return;
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD mode = 0;
        if (GetConsoleMode(hOut, &mode))
        {
            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, mode);
        }
    }
#endif
    s_consoleAnsiEnabled = true;
}

void Logger::Log(LogLevel level, const std::string_view& message)
{
    std::string msg(message);
    std::wostringstream wss;
    wss << GetLevelString(level).c_str() << " " << msg.c_str() << "\n";
#if defined(_WIN32)
    OutputDebugStringW(wss.str().c_str());
#endif

    EnsureAnsiEnabled();

    std::string prefix;
    std::string suffix;

    switch (level)
    {
    case LogLevel::Debug:
        prefix = "\x1b[36m"; // cyan
        break;
    case LogLevel::Info:
        prefix = "\x1b[32m"; // green
        break;
    case LogLevel::Warning:
        prefix = "\x1b[33m"; // yellow
        break;
    case LogLevel::Error:
        prefix = "\x1b[31m"; // red
        break;
    default:
        break;
    }
    suffix = "\x1b[0m";

    if (!prefix.empty())
        std::cout << prefix;

    std::cout << GetLevelString(level) << " " << message;

    if (!suffix.empty())
        std::cout << suffix;

    std::cout << std::endl;
}

void Logger::LogDebug(const std::string_view& message)
{
    if (!m_debugEnabled)
        return;
    Log(LogLevel::Debug, message);
}

void Logger::LogInfo(const std::string_view& message)
{
    Log(LogLevel::Info, message);
}

void Logger::LogWarning(const std::string_view& message)
{
    Log(LogLevel::Warning, message);
}

void Logger::LogError(const std::string_view& message)
{
    Log(LogLevel::Error, message);
}

void Logger::LogEngineState(double fps, int frameCount, double totalTime)
{
    if (!m_debugEnabled)
        return;
    std::ostringstream oss;
    oss.precision(2);
    oss << "Engine State - FPS: " << std::fixed << fps << " | Frame: " << frameCount
        << " | Total Time: " << totalTime << "s";
    LogInfo(oss.str());
}
