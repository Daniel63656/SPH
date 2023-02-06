#include "scenario_liddrivencavity.h"
#include "simulation/simulation.h"

LidDrivenCavity::LidDrivenCavity(const Settings &settings, const std::shared_ptr<KernelFunction> &kernel,
                                 MPI_Vars &mpiInfo) : Simulation(settings, kernel, mpiInfo)
                                 {}