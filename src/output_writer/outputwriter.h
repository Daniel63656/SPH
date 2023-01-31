#pragma once

#include "../datastructures/particle.h"
#include "../output_writer/body.h"
#include <vector>
#include <pugixml.hpp>

class OutputWriter
{
public:
	OutputWriter(MPI_Vars mpi_info, double vs_dt, std::vector<Particle>& particles, std::string path);

	void write_vtp();

	void write_pvd(std::string filename);

	int steps;
private:
	void build_tree();
	void create_dirs();
	pugi::xml_document m_doc;
	pugi::xml_node m_position;
    pugi::xml_node m_body_id;
	pugi::xml_node m_velocity;
	pugi::xml_node m_acceleration;
	pugi::xml_node m_phi;
	pugi::xml_node m_omega;
	pugi::xml_node m_alpha;
	pugi::xml_node m_verts;
	pugi::xml_node m_conn;
	pugi::xml_node m_kin_e;
	pugi::xml_node m_rot_e;
	pugi::xml_node m_pot_e;
	pugi::xml_node m_total_e;

	int m_step;
	double m_vs_dt;
	double m_endtime;

    MPI_Vars m_mpi_info;
	std::vector<Particle>& m_particles;
	std::string m_dir;
	std::string m_path;
};
