#include <iostream>
#include <array>
#include "output_writer/body.h"
#include "settings/settings.h"
#include "simulation/simulation.h"
#include "kernelFunctions/gaussian.h"
//#include "output_writer/vtk_writer.h"
#include "output_writer/outputwriter.h"
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
	Settings settings;
	settings.loadFromFile(filename);
	settings.printSettings();

	Gaussian kernel(settings.smoothness);

    MPI_Vars info{0,1,0, settings.numberOfParticles, settings.numberOfParticles, 0};

    OutputWriter writer(info, settings.vs_dt, "out/");

    Simulation simulation(settings, &kernel, info);
//    VtkWriter vtkWriter(simulation.getGrid());
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
