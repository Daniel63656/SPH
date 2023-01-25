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

    // instantiate settings object
    Settings<2> settings;
    //settings.loadFromFile(filename);
    //settings.printSettings();

    std::shared_ptr<KernelFunction<2>> kernel = std::make_shared<Gaussian<2>>(6);
    Simulation<2, Gaussian<2>> simulation(settings, kernel);
    //simulation.run();

    return 0;
}
