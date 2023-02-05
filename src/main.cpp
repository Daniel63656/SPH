#include <iostream>
#include <array>
#include <thread>
#include <omp.h>
#include "output_writer/body.h"
#include "settings/settings.h"
#include "simulation/simulation.h"
#include "kernelFunctions/gaussian.h"
#include "output_writer/outputwriter.h"
#include "pch.h"
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
    settings.calculateSettings();
	settings.printSettings();

	// tell omp to use as many threads as there are cores so it doesnt use hyperthreading
	const auto processor_count = std::thread::hardware_concurrency();
	omp_set_num_threads(processor_count / 2);
	std::cout << "processor_count: " << processor_count << " , using " << processor_count / 2 << std::endl;

	Gaussian kernel(settings.smoothness);
    MPI_Vars info{0, 1, 0, settings.nParticles, settings.nParticles, 0};

    OutputWriter writer(info, settings.vs_dt, "out/");


    Simulation simulation(settings, &kernel, info);
	double taiming = 0.0;
	{
		ProfilerScoped p(taiming);
		simulation.run(writer);
	}
	auto NsToMs = [&](double ms) { return ms * 1e-6; };

	std::cout << "time -> " << NsToMs(taiming) << std::endl;

    writer.write_pvd("sim");

	return EXIT_SUCCESS;
}
