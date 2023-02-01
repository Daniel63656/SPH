#pragma once

#include "../datastructures/particle.h"
#include "../output_writer/body.h"
#include <vector>
#include <pugixml.hpp>

class OutputWriter
{
public:
	OutputWriter(MPI_Vars mpi_info, double vs_dt,  std::string path);

	void write_vtp(std::vector<Particle>& particles);

	void write_pvd(std::string filename);

	int steps;
private:
	void build_tree();
	void create_dirs();
	pugi::xml_document m_doc;
	pugi::xml_node m_position;
	pugi::xml_node m_velocity;
	pugi::xml_node m_forces;
	pugi::xml_node m_mass;
	pugi::xml_node m_rho;
	pugi::xml_node m_pressure;
	pugi::xml_node m_verts;
	pugi::xml_node m_conn;

	int m_step;
	double m_vs_dt;
	double m_endtime;

    MPI_Vars m_mpi_info;
	std::string m_dir;
	std::string m_path;
};
