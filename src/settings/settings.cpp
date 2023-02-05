#include <fstream>
#include <cassert>
#include "settings.h"

Settings::Settings(const std::string& filename)
{
    // open file
	std::ifstream file(filename.c_str(), std::ios::in);

	// check if file is open
	if (!file.is_open())
	{
		std::cout << "Could not open parameter file \"" << filename << "\"." << std::endl;
		return;
	}

	// loop over lines of file
	while (!file.eof())
	{
        // read line
        std::string line;
        getline(file, line);

		// comment detected
		if (line[0] == '#')
			continue;

		// no =-sign found
		if (line.find('=') == std::string::npos) {
            //check for keywords that can open blocks: [Boundary]
            std::stringstream ss(line);
            std::string word;
            // get first word
            std::getline(ss, word, ' ');

            if (word == "Boundary") {
                //get the type of boundary
                std::getline(ss, word, ' ');
                //remove '\r' if '{' is in new line
                if (word[word.size()-1] == '\r')
                    word.erase(word.size() - 1);

                Boundary* boundary = nullptr;
                if (word == "bottom")
                    boundary = &bottom;
                else if (word == "top")
                    boundary = &top;
                else if (word == "left")
                    boundary = &left;
                else if (word == "right")
                    boundary = &right;

                assert(boundary != nullptr);
                //preset particle mass in boundary to standard particle mass
                boundary->m_particleMass = mass;
                //read whole block
                while (line.find('}') == std::string::npos)
                {
                    getline(file, line);

                    if (line[0] != '#' && line.find('=') != std::string::npos) {
                        //if valid parameter line, get name and value
                        std::string paramName;
                        std::string paramValue;
                        line2nameAndValue(line, paramName, paramValue);

                        if (paramName == "nParticlesPerRow")
                            boundary->m_nParticlesPerRow = atoi(paramValue.c_str());
                        else if (paramName == "thickness")
                            boundary->m_thickness = atoi(paramValue.c_str());
                        else if (paramName == "particleMass")
                            boundary->m_particleMass = atof(paramValue.c_str());
                        else if (paramName == "velocityX")
                            boundary->m_velocity.x = atof(paramValue.c_str());
                        else if (paramName == "velocityY")
                            boundary->m_velocity.y = atof(paramValue.c_str());
                        else
                            std::cout << "unknown Boundary parameter";
                    }
                }
            }

            //still no = sign found so continue
            continue;
        }


		//parameter line
        std::string paramName;
        std::string paramValue;
        line2nameAndValue(line, paramName, paramValue);

        if (paramName == "kappa")
            kappa = atof(paramValue.c_str());
        else if (paramName == "rho0")
            rho_0 = atof(paramValue.c_str());
        else if (paramName == "mass")
            mass = atof(paramValue.c_str());
        else if (paramName == "mu")
            mu = atof(paramValue.c_str());
        else if (paramName == "numberOfParticles")
            nParticles = atoi(paramValue.c_str());

        else if (paramName == "physicalSizeX")
            physicalSize.x = atof(paramValue.c_str());
        else if (paramName == "physicalSizeY")
            physicalSize.y = atof(paramValue.c_str());
        else if (paramName == "nCellsX")
            nCells.x = atoi(paramValue.c_str());
        else if (paramName == "nCellsY")
            nCells.y = atoi(paramValue.c_str());
        else if (paramName == "boundaryThickness")
            boundaryThickness = atof(paramValue.c_str());

        else if (paramName == "endTime")
            endTime = atof(paramValue.c_str());
        else if (paramName == "dt")
            dt = atof(paramValue.c_str());
        else if (paramName == "vs_dt")
            vs_dt = atof(paramValue.c_str());

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
}

void Settings::printSettings() const {
	std::cout << "========= Settings ==========\n"
        << "kappa: " << kappa << ", rho0: " << rho_0 << ", mass: " << mass << ", mu: " << mu << ", nParticles: " << nParticles << std::endl
		<< "physicalSize: " << physicalSize.x << " x " << physicalSize.y << std::endl
		<< "cells: " << nCells.x << " x " << nCells.y << std::endl
        << "Bottom boundary: " << bottom << "\n"
        << "Top    boundary: " << top << "\n"
        << "Left   boundary: " << left << "\n"
        << "Right  boundary: " << right << "\n"
		<< "endTime: " << endTime << " in " << dt << " steps\n"
        << "Gravity: " << g <<"\n"
		<< "Kernel: " << kernelFunction << "(" << smoothness << ")\n"
        << "=============================\n\n";
}

void Settings::line2nameAndValue(std::string line, std::string& paramName, std::string& paramValue) {
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
    int pos_eq = line.find_first_of('=');
    paramName = line.substr(0, pos_eq);
    paramValue = line.substr(pos_eq + 1);
}
