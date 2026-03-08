#pragma once
#include <spdlog/spdlog.h>

class EngineLogger
{
  public:
    EngineLogger(const EngineLogger&) = delete;
    EngineLogger& operator=(const EngineLogger&) = delete;

    static EngineLogger& Get();

    void SetDebugEnabled(bool enabled);
    bool IsDebugEnabled() const;

    template <typename... Args> void Info(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        spdlog::info(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args> void Debug(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (IsDebugEnabled())
        {
            spdlog::debug(fmt, std::forward<Args>(args)...);
        }
    }

    template <typename... Args> void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        spdlog::warn(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args> void Error(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        spdlog::error(fmt, std::forward<Args>(args)...);
    }

  private:
    EngineLogger() = default;

    bool m_debugEnabled = false;
};

#define LOG_INFO(...) EngineLogger::Get().Info(__VA_ARGS__)
#define LOG_DEBUG(...) EngineLogger::Get().Debug(__VA_ARGS__)
#define LOG_WARNING(...) EngineLogger::Get().Warn(__VA_ARGS__)
#define LOG_ERROR(...) EngineLogger::Get().Error(__VA_ARGS__)

#define LOG_ENGINE_STATE(fps, frameCount, totalTime)                                               \
    EngineLogger::Get().Info("Engine State - FPS: {:.2f} | Frame: {} | Total Time: {:.2f}s",       \
                             fps,                                                                  \
                             frameCount,                                                           \
                             totalTime)
