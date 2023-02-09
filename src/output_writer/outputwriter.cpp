#include "../datastructures/particle.h"
#include "../output_writer/outputwriter.h"
#include <sstream>
#include <string>
#include <filesystem>
#include <utility>

OutputWriter::OutputWriter(double vs_dt, std::string path) :  m_vs_dt(vs_dt), m_dir(std::move(path))
{
	create_dirs();
	m_step = 0;
	m_path = std::string("time_series/") + std::string("sim_");
}

void OutputWriter::create_dirs()
{
	// create directories for the output files
	if (!std::filesystem::create_directories(m_dir + "/time_series"))
	{
		std::cout << "Did not create output directory!" << std::endl;
	}
}

void OutputWriter::build_tree()
{
	pugi::xml_node vtkfile = m_doc.append_child("VTKFile");
	vtkfile.append_attribute("type") = "PolyData";
	vtkfile.append_attribute("version") = "0.1";
	vtkfile.append_attribute("byte_order") = "LittleEndian";
	vtkfile.append_attribute("header_type") = "UInt64";
	pugi::xml_node polydata = vtkfile.append_child("PolyData");
	pugi::xml_node piece = polydata.append_child("Piece");
	m_numberOfPoints = piece.append_attribute("NumberOfPoints") = 0;
	m_numberOfVertices = piece.append_attribute("NumberOfVerts") = 0;

	// position
	pugi::xml_node points = piece.append_child("Points");
	m_position = points.append_child("DataArray");
	m_position.append_attribute("type") = "Float64";
	m_position.append_attribute("Name") = "position";
	m_position.append_attribute("NumberOfComponents") = "3";
	m_position.append_attribute("format") = "ascii";


	pugi::xml_node pointdata = piece.append_child("PointData");

	// velocity
	m_velocity = pointdata.append_child("DataArray");
	m_velocity.append_attribute("type") = "Float64";
	m_velocity.append_attribute("Name") = "velocity";
	m_velocity.append_attribute("NumberOfComponents") = "3";
	m_velocity.append_attribute("format") = "ascii";


	// acceleration
	m_forces = pointdata.append_child("DataArray");
	m_forces.append_attribute("type") = "Float64";
	m_forces.append_attribute("Name") = "force";
	m_forces.append_attribute("NumberOfComponents") = "3";
	m_forces.append_attribute("format") = "ascii";


	// angle
	m_mass = pointdata.append_child("DataArray");
	m_mass.append_attribute("type") = "Float64";
	m_mass.append_attribute("Name") = "mass";
	m_mass.append_attribute("NumberOfComponents") = "1";
	m_mass.append_attribute("format") = "ascii";


	// omega
	m_rho = pointdata.append_child("DataArray");
	m_rho.append_attribute("type") = "Float64";
	m_rho.append_attribute("Name") = "rho";
	m_rho.append_attribute("NumberOfComponents") = "1";
	m_rho.append_attribute("format") = "ascii";


	// alpha
	m_pressure = pointdata.append_child("DataArray");
	m_pressure.append_attribute("type") = "Float64";
	m_pressure.append_attribute("Name") = "pressure";
	m_pressure.append_attribute("NumberOfComponents") = "1";
	m_pressure.append_attribute("format") = "ascii";


	//verts
	pugi::xml_node verts = piece.append_child("Verts");
	m_verts = verts.append_child("DataArray");
	m_verts.append_attribute("type") = "Int64";
	m_verts.append_attribute("Name") = "offsets";


	//connectivity
	m_conn = verts.append_child("DataArray");
	m_conn.append_attribute("type") = "Int64";
	m_conn.append_attribute("Name") = "connectivity";
}


void OutputWriter::write_vtp(std::vector<Particle>& particles, std::vector<Particle>& boundary)
{
    m_numberOfPoints.set_value(particles.size() + boundary.size());
    m_numberOfVertices.set_value(particles.size() + boundary.size());

	// position
	std::string pos = "\n";
	std::string vel = "\n";
	std::string force = "\n";
	std::string mass = "\n";
	std::string rho = "\n";
	std::string pressure = "\n";
    std::string verts = "\n";
    std::string conn = "\n";
	for (int i = 0; i < particles.size(); i++)
	{
		Particle& particle = particles[i];
		pos += particle.position.serialize();
		vel += particle.velocity.serialize();
		force += particle.forces.serialize();
		mass += std::to_string(particle.mass);
		rho += std::to_string(particle.density);
		pressure += std::to_string(particle.pressure);
		pos += "\n";
		vel += "\n";
		force += "\n";
		mass += "\n";
		rho += "\n";
		pressure += "\n";
        verts += std::to_string(i+1) + " ";
        conn += std::to_string(i) + " ";
	}


	for (int i = 0; i < boundary.size(); i++)
	{
		Particle& particle = boundary[i];
		pos += particle.position.serialize();
		vel += particle.velocity.serialize();
		force += particle.forces.serialize();
		mass += std::to_string(particle.mass);
		rho += std::to_string(particle.density);
		pressure += std::to_string(particle.pressure);
		pos += "\n";
		vel += "\n";
		force += "\n";
		mass += "\n";
		rho += "\n";
		pressure += "\n";
        verts += std::to_string(i+1+particles.size()) + " ";
        conn += std::to_string(i+particles.size()) + " ";
    }



	m_position.text() = pos.c_str();
	m_velocity.text() = vel.c_str();
	m_forces.text() = force.c_str();
	m_mass.text() = mass.c_str();
	m_rho.text() = rho.c_str();
	m_pressure.text() = pressure.c_str();
    m_verts.text() = verts.c_str();
    m_conn.text() = conn.c_str();


	std::ofstream outFile;
	outFile.open(m_dir + m_path + std::to_string(m_step) + std::string(".vtp"));
	m_doc.save(outFile);
	outFile.close();

	m_step++;
}

std::string scientific(double number)
{
	std::ostringstream out;
	out.precision(4);
	out << std::scientific << number;
	return out.str();
}

void OutputWriter::write_pvd(const std::string& filename)
{
	pugi::xml_document doc;

	pugi::xml_node vtkfile = doc.append_child("VTKFile");
	vtkfile.append_attribute("type") = "Collection";
	vtkfile.append_attribute("version") = "0.1";
	vtkfile.append_attribute("byte_order") = "LittleEndian";
	vtkfile.append_attribute("compressor") = "vtkZLibDataCompressor";

	pugi::xml_node collection = vtkfile.append_child("Collection");
	pugi::xml_node dataset;

	double time = 0;
	for (int i = 0; i < m_step; i++)
	{
		time += m_vs_dt;
        dataset = collection.append_child("DataSet");
        dataset.append_attribute("timestep") = scientific(time).c_str();
        dataset.append_attribute("group") = "";
        dataset.append_attribute("part") = "";
        std::string out = m_path + std::to_string(i) + ".vtp";
        dataset.append_attribute("file") = out.c_str();
	}

	std::ofstream outFile;
	outFile.open(m_dir + filename);
	doc.save(outFile);
	outFile.close();
}
