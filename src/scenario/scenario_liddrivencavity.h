#pragma once

#include "simulation/simulation.h"

class LidDrivenCavity : public Simulation
{
  public:
	LidDrivenCavity(const Settings& settings, const std::shared_ptr<KernelFunction>& kernel);
};
