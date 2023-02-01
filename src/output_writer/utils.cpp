#include "../output_writer/body.h"
#include "../output_writer/utils.h"
#include <sstream>
#include <string>


void utils::calculate_slice(int size, int rank, int processNo, MPI_Vars& vars)
{
	vars.processNo = processNo;
	vars.rank = rank;
	vars.slice = size / processNo;
	vars.rest = size % processNo;

	if (rank < vars.rest)
	{
		vars.arraystart = rank * vars.slice + rank * ((rank - 1) < vars.rest);
	}
	else
	{
		vars.arraystart = rank * vars.slice + vars.rest;
	}

	vars.arrayend = vars.arraystart + vars.slice + (rank < vars.rest);
	/*
	for (int i = 0; i < processNo; i++){

		int l_slice = vars.slice + (i < vars.rest);
		MPI_Datatype& dtype = vars.dtype.emplace_back();
		MPI_Type_contiguous(l_slice*body_dynamic_size, MPI_DOUBLE, &dtype);
		MPI_Type_commit(&dtype);
		if (i < vars.rest)
		{
			vars.recv_start.push_back(i * vars.slice + i * ((i - 1) < vars.rest));
		}
		else
		{
			vars.recv_start.push_back(i * vars.slice + vars.rest);
		}
	}*/
}
