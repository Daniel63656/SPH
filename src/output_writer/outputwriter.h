#pragma once

#include "../datastructures/particle.h"
#include "pch.h"

class OutputWriter
{
public:
	OutputWriter(double vs_dt,  std::string path);

	void write_vtp(std::vector<Particle>& particles, std::vector<Particle>& boundary);

	void write_pvd(const std::string& filename);
	void build_tree();

private:
	void create_dirs();

	pugi::xml_document m_doc;
    pugi::xml_attribute m_numberOfPoints;
    pugi::xml_attribute m_numberOfVertices;
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

	std::string m_dir;
	std::string m_path;
};
