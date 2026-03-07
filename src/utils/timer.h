#pragma once
#include <chrono>

// --- Timer
// --------------------------------------------

class Timer
{
  public:
    Timer() = default;

    // Record the start time.
    void Start();

    // Returns elapsed seconds since the last Tick() (or Start()).
    double Tick();

    // Returns the most recent delta without advancing the clock.
    double GetDelta() const;

  private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint m_prev;
    double m_delta = 0.0;
};
