#include "logger.h"
#include <sstream>
#include <windows.h>

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

void Logger::Log(LogLevel level, const std::string& message)
{
    std::wostringstream wss;
    wss << GetLevelString(level).c_str() << " " << message.c_str()
        << "\n";
    OutputDebugStringW(wss.str().c_str());
}

void Logger::LogDebug(const std::string& message)
{
    if (!m_debugEnabled)
        return;
    Log(LogLevel::Debug, message);
}

void Logger::LogInfo(const std::string& message)
{
    Log(LogLevel::Info, message);
}

void Logger::LogWarning(const std::string& message)
{
    Log(LogLevel::Warning, message);
}

void Logger::LogError(const std::string& message)
{
    Log(LogLevel::Error, message);
}

void Logger::LogEngineState(
    double fps, int frameCount, double totalTime)
{
    if (!m_debugEnabled)
        return;
    std::ostringstream oss;
    oss.precision(2);
    oss << "Engine State - FPS: " << std::fixed << fps
        << " | Frame: " << frameCount
        << " | Total Time: " << totalTime << "s";
    LogInfo(oss.str());
}
