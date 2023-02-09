#include "scenario_liddrivencavity.h"
#include "simulation/simulation.h"

LidDrivenCavity::LidDrivenCavity(const Settings &settings, const std::shared_ptr<KernelFunction> &kernel
                                 ) : Simulation(settings, kernel)
                                 {}
