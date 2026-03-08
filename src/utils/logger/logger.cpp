#include "logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

EngineLogger& EngineLogger::Get()
{
    static EngineLogger instance;
    static bool initialized = false;
    if (!initialized)
    {
        auto console = spdlog::stdout_color_mt("engine");
        spdlog::set_default_logger(console);
        spdlog::set_pattern("%^[%T] [%l] %v%$");
        spdlog::set_level(spdlog::level::info);
        initialized = true;
    }
    return instance;
}

void EngineLogger::SetDebugEnabled(bool enabled)
{
    m_debugEnabled = enabled;
    spdlog::set_level(enabled ? spdlog::level::debug : spdlog::level::info);
}

bool EngineLogger::IsDebugEnabled() const
{
    return m_debugEnabled;
}
