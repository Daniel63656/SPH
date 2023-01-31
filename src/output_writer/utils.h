#pragma once

#include "../output_writer/body.h"
#include "../pch.h"
#include "../simulation/simulation.h"
#include <vector>

namespace utils
{
	void calculate_slice(int size, int rank, int processNo, MPI_Vars& vars);
};
