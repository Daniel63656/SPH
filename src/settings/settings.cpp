#include <fstream>
#include <cassert>
#include "pch.h"
#include "settings.h"
#include "scenario/scenario_liddrivencavity.h"

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


                        if (paramName == "thickness")
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
        if (paramName == "scenario") {
            if (paramValue == "LidDrivenCavity") {
                std::cout << "*************** Running LidDrivenCavity **********\n\n";
                scenario = std::make_shared<LidDrivenCavity>();
            }
            else
                throw std::invalid_argument("Scenario not recognized!");
        }

        else if (paramName == "kappa")
            kappa = atof(paramValue.c_str());
        else if (paramName == "rho0")
            rho_0 = atof(paramValue.c_str());
        else if (paramName == "mass") {
            if (paramValue == "auto")
                mass = -1;      //mark for auto calculation
            else
                mass = atof(paramValue.c_str());
        }
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
        else if (paramName == "dt") {
            if (paramValue == "auto")
                dt = -1;      //mark for auto calculation
            else
                dt = atof(paramValue.c_str());
        }
        else if (paramName == "vs_dt")
            vs_dt = atof(paramValue.c_str());

        else if (paramName == "gX")
            g.x = atof(paramValue.c_str());
        else if (paramName == "gY")
            g.y = atof(paramValue.c_str());
        else if (paramName == "kernelFunction") {
            if (paramValue == "Gaussian")
                kernelType = GAUSSIAN;
            else if (paramValue == "CubicSpline")
                kernelType = CUBIC_SPLINE;
            else {
                kernelType = GAUSSIAN;
                std::cout << "Kernel function not recognized. Defaulting to Gaussian...\n";
            }
        }
        else {
            std::cout << "unknown parameter";
        }
	}

    //calculate parameters marked as auto
    calculateParameters();
}

//! calculate parameters marked as auto and boundary particles
void Settings::calculateParameters()
{
    double area = physicalSize.x * physicalSize.y;
    if (mass < 0)
        mass = rho_0 * area / nParticles;

    //Set smoothness according to kernel
    if (kernelType == GAUSSIAN)
        smoothness = sqrt((area * 20)/(M_PI * nParticles))/3;
    else if (kernelType == CUBIC_SPLINE)
        smoothness = sqrt((area * 20)/(M_PI * nParticles))/2;

    //set dt to a reasonable value
    if (dt < 0) {
        const double safety = 0.2;
        dt = ((smoothness) / 1.0) * safety;
    }


    //set boundary number of particles to match density of particles inside domain
    Vec2i vParticles;
    vParticles.x = (int)std::lround(physicalSize.x * sqrt(nParticles / area));
    vParticles.y = nParticles / vParticles.x;

    bottom.m_nParticlesPerRow = vParticles.y;
    top.m_nParticlesPerRow = vParticles.y;
    left.m_nParticlesPerRow = vParticles.x;
    right.m_nParticlesPerRow = vParticles.x;
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
              << "Gravity: " << g << "\n"
              << "Kernel: " << kernelType << "(" << smoothness << ")\n"
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

    //remove '\r' if '{' is in new line
    if (paramValue[paramValue.size()-1] == '\r')
        paramValue.erase(paramValue.size() - 1);
}
