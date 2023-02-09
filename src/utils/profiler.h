#pragma once
#include "pch.h"

// Measures time spent inside a scope
class ProfilerScoped
{
  public:
	// writes the timing of this scope into the parameter out
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
