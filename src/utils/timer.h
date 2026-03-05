// Simple high-resolution timer using QueryPerformanceCounter
#pragma once
#include <windows.h>

class Timer
{
  public:
    Timer();
    void Start();
    // Tick updates internal time and returns delta seconds since last tick
    double Tick();
    double GetDelta() const;

  private:
    LARGE_INTEGER m_freq;
    LARGE_INTEGER m_prev;
    double m_delta;
};
