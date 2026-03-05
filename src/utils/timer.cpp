#include "utils/timer.h"

Timer::Timer() : m_freq{}, m_prev{}, m_delta{0.0}
{
    QueryPerformanceFrequency(&m_freq);
}

void Timer::Start()
{
    QueryPerformanceCounter(&m_prev);
    m_delta = 0.0;
}

double Timer::Tick()
{
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    if (m_freq.QuadPart != 0)
        m_delta = double(now.QuadPart - m_prev.QuadPart) / double(m_freq.QuadPart);
    else
        m_delta = 0.0;
    m_prev = now;
    return m_delta;
}

double Timer::GetDelta() const
{
    return m_delta;
}
