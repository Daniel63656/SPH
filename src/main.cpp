#include <iostream>
#include <array>
#include "settings/settings.h"
#include "simulation/simulation.h"
#include "kernelFunctions/gaussian.h"
//#include "output_writer/vtk_writer.h"
#include "output_writer/outputwriter.h"


int main(int argc, char *argv[])
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

    OutputWriter writer;


    Simulation simulation(settings, &kernel);
//    VtkWriter vtkWriter(simulation.getGrid());

    simulation.run(&writer);

    return EXIT_SUCCESS;
}
