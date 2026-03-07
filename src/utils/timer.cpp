// ------------------------------------------------------------------------------
// ----------------------------- TIMER IMPLEMENTATION ----------------------------
// ------------------------------------------------------------------------------
#include "utils/timer.h"

void Timer::Start()
{
    m_prev = std::chrono::high_resolution_clock::now();
    m_delta = 0.0;
}

double Timer::Tick()
{
    const auto now = std::chrono::high_resolution_clock::now();
    m_delta = std::chrono::duration<double>(now - m_prev).count();
    m_prev = now;
    return m_delta;
}

double Timer::GetDelta() const
{
    return m_delta;
}

// ------------------
