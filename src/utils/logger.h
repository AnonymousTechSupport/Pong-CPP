#pragma once
#include <string>

// --- Logger
// -------------------------------------------------

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

    void SetDebugEnabled(bool enabled);
    bool IsDebugEnabled() const;

    void Log(LogLevel level, const std::string_view& message);
    void LogDebug(const std::string_view& message);
    void LogInfo(const std::string_view& message);
    void LogWarning(const std::string_view& message);
    void LogError(const std::string_view& message);

    void LogEngineState(double fps, int frameCount, double totalTime);

  private:
    Logger() = default;

    std::string GetLevelString(LogLevel level) const;

    bool m_debugEnabled = false;
};

// Convenience macros
#define LOG_DEBUG(msg) Logger::Get().LogDebug(msg)
#define LOG_INFO(msg) Logger::Get().LogInfo(msg)
#define LOG_WARNING(msg) Logger::Get().LogWarning(msg)
#define LOG_ERROR(msg) Logger::Get().LogError(msg)
#define LOG_ENGINE_STATE(fps, frameCount, totalTime)                                               \
    Logger::Get().LogEngineState(fps, frameCount, totalTime)
