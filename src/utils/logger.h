#pragma once
#include <string>

enum class LogLevel
{
    Debug,
    Info,
    Warning,
    Error
};

class Logger
{
  public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& Get();

    void SetDebugEnabled(bool enabled) { m_debugEnabled = enabled; }
    bool IsDebugEnabled() const { return m_debugEnabled; }

    void Log(LogLevel level, const std::string& message);
    void LogDebug(const std::string& message);
    void LogInfo(const std::string& message);
    void LogWarning(const std::string& message);
    void LogError(const std::string& message);

    void LogEngineState(double fps, int frameCount, double totalTime);

  private:
    Logger() = default;

    std::string GetLevelString(LogLevel level) const;

    bool m_debugEnabled = false;
};
