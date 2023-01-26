#include <iostream>
#include <array>
#include <memory>
#include "datastructures/vector.h"
#include "settings/settings.h"
#include "simulation/simulation.h"
#include "kernelFunctions/gaussian.h"


int main(int argc, char *argv[]) {

    // try to read simulation parameters
    if (argc == 1)
    {
        std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    }
    // read in the first argument
    std::string filename = argv[1];
    std::cout << "Parameters imported from: \"" << filename << "\"" << std::endl;

    Vector<2> t = Vector<2>();
    t = 0;

    std::cout << t << std::endl;


    Vector<2> v = Vector<2>();
    Vector<3> w = Vector<3>();



    // instantiate settings object
    Settings settings;
    //settings.loadFromFile(filename);
    //settings.printSettings();

    Gaussian kernel(4);

    Simulation simulation(settings, &kernel);
    simulation.run();

    return 0;
}
