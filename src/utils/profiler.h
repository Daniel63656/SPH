#pragma once
#include <chrono>

// Measures time spent inside a scope
class ProfilerScoped
{
public:
    ProfilerScoped(double& out) : output(out)
    {
        startTime = std::chrono::high_resolution_clock::now();
    }

    ~ProfilerScoped()
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
        output = elapsed;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    double& output;
};

// Same as ProfilerScoped but adds time instead
class ProfilerCumulative
{
public:
    ProfilerCumulative(double& out) : output(out)
    {
        startTime = std::chrono::high_resolution_clock::now();
    }

    ~ProfilerCumulative()
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
        output += elapsed;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    double& output;
};
