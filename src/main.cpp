#include "kernelFunctions/cubicSpline.h"
#include "kernelFunctions/gaussian.h"
#include "output_writer/outputwriter.h"
#include "pch.h"
#include "scenario/scenario_karmanvortex.h"
#include "scenario/scenario_liddrivencavity.h"
#include "settings/settings.h"
#include "simulation/simulation.h"
#include "utils/profiler.h"

int main(int argc, char* argv[])
{
	// try to read simulation parameters
	if (argc == 1)
	{
		std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
		return EXIT_FAILURE;
	}

	// read in the first argument
	std::string filename = argv[1];
	std::cout << "Parameters imported from: \"" << filename << "\"" << std::endl;

	// instantiate settings object
	Settings settings(filename);
	settings.calculateParameters();
	settings.printSettings();

	// tell omp to use as many threads as there are cores, so it doesn't use hyperthreading
	const auto processor_count = std::thread::hardware_concurrency();
	omp_set_num_threads(processor_count / 2);
	std::cout << "processor_count: " << processor_count << " , using " << processor_count / 2 << std::endl;

	OutputWriter writer(settings.vs_dt, "out/");

	// instantiate kernel function
	std::shared_ptr<KernelFunction> kernel;
	if (settings.kernelType == GAUSSIAN)
		kernel = std::make_shared<Gaussian>(settings.smoothness);
	else if (settings.kernelType == CUBIC_SPLINE)
		kernel = std::make_shared<CubicSpline>(settings.smoothness);

	// instantiate simulation scenario
	std::shared_ptr<Simulation> simulation;
	if (settings.scenario == LID_DRIVEN_CAVITY)
		simulation = std::make_shared<LidDrivenCavity>(settings, kernel);
	else if (settings.scenario == KARMAN_VORTEX)
		simulation = std::make_shared<KarmanVortex>(settings, kernel);

	// start profiler and run simulation
	double timing = 0.0;
	{
		ProfilerScoped p(timing);
		simulation->run(writer);
	}
	auto NsToMs = [&](double ms) { return ms * 1e-6; };

	std::cout << "total simulation time: " << NsToMs(timing) << std::endl;

	writer.write_pvd("sim.pvd");

	return EXIT_SUCCESS;
}
