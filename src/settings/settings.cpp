#include <fstream>
#include "settings.h"

void Settings::loadFromFile(const std::string& filename)
{
	// open file
	std::ifstream file(filename.c_str(), std::ios::in);

	// check if file is open
	if (!file.is_open())
	{
		std::cout << "Could not open parameter file \"" << filename << "\"." << std::endl;
		return;
	}

	int lineNo = 0;
	// flag to evaluate presence of a parameter
	int noValue;
	// loop over lines of file
	while (!file.eof())
	{
		noValue = 0;
		// read line
		std::string line;
		getline(file, line);

		// comment detected
		if (line[0] == '#')
			noValue = 1;

		// no =-sign found
		if (line.find('=') == std::string::npos)
			noValue = 1;

		// print line
		if (!noValue)
		{
			// remove all spaces
			while (line.find_first_of(" \t") != std::string::npos)
			{
				line.erase(line.find_first_of(" \t"), 1);
			}

			// remove every comment at the end of a line
			if (line.find_first_of('#') != std::string::npos)
			{
				line.erase(line.find_first_of('#'));
			}

			// split on equal sign and extract name + value
			std::string paramName;
			std::string paramValue;
			int pos_eq = line.find_first_of('=');
			paramName = line.substr(0, pos_eq);
			paramValue = line.substr(pos_eq + 1);

			if (paramName == "kappa")
				kappa = atof(paramValue.c_str());
			else if (paramName == "rho0")
				rho_0 = atof(paramValue.c_str());
			else if (paramName == "mass")
				mass = atof(paramValue.c_str());
			else if (paramName == "mu")
				mu = atof(paramValue.c_str());
			else if (paramName == "numberOfParticles")
				numberOfParticles = atoi(paramValue.c_str());

			else if (paramName == "physicalSizeX")
				physicalSize.x = atof(paramValue.c_str());
			else if (paramName == "physicalSizeY")
				physicalSize.y = atof(paramValue.c_str());
			else if (paramName == "nCellsX")
				nCells.x = atoi(paramValue.c_str());
			else if (paramName == "nCellsY")
				nCells.y = atoi(paramValue.c_str());
            else if (paramName == "boundaryDensityX")
                boundaryDensity.x = atof(paramValue.c_str());
            else if (paramName == "boundaryDensityY")
                boundaryDensity.y = atof(paramValue.c_str());

			else if (paramName == "endTime")
				endTime = atof(paramValue.c_str());
			else if (paramName == "dt")
				dt = atof(paramValue.c_str());

			else if (paramName == "gX")
				g.x = atof(paramValue.c_str());
			else if (paramName == "gY")
				g.y = atof(paramValue.c_str());
			else if (paramName == "kernelFunction")
				kernelFunction = paramValue;
			else if (paramName == "smoothness")
				smoothness = atof(paramValue.c_str());

			else {
				std::cout << "unknown parameter";
			}
		}
		lineNo++;
	}
}

void Settings::printSettings() {
	std::cout << "Settings: " << std::endl
		<< "kappa: " << kappa << ", rho0: " << rho_0 << ", mass: " << mass << ", mu: " << mu << ", nParticles: " << numberOfParticles << std::endl
		<< "physicalSize: " << physicalSize.x << " x " << physicalSize.y << std::endl
		<< "cells: " << nCells.x << " x " << nCells.y << std::endl
		<< "time: " << endTime << " in " << dt << " steps\n"
		<< "g: (" << g.x << "," << g.y << "), kernel: " << kernelFunction << "(" << smoothness << ")\n";
}
