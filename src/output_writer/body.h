#pragma once

#include "../pch.h"
#include <cmath>
#include <sys/resource.h>



struct MPI_Vars
{
	int rank;
	int processNo;
	int arraystart;
	int arrayend;
	int slice;
	int rest;

    //std::vector<int> recv_start;
    //std::vector<MPI_Datatype> dtype;
};
