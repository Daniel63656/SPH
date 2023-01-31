#include "datastructures/particle.h"
#include "outputwriter/outputwriter.h"
#include <sstream>
#include <string>

OutputWriter::OutputWriter(double vs_dt, std::vector<Particle>& particles, std::string path): m_vs_dt(vs_dt),m_particles(particles), m_dir(path)
{
	create_dirs();
	build_tree();
    m_step = 0;
    m_path = m_dir+std::string("time_series/")+std::to_string(m_mpi_info.rank)+std::string("/sim_");
}

void OutputWriter::create_dirs()
{

	// create directories for the output files
	bool created_dir = false;
	created_dir = std::filesystem::create_directories(m_dir + "/time_series");
	if (!created_dir)
	{
		std::cout << "Could not create directories!" << std::endl;
	}
	for (int i = 0; i < m_mpi_info.processNo; i++)
	{
		bool created_subdirs = std::filesystem::create_directories(m_dir + "/time_series/" + std::to_string(i));
		if (created_subdirs)
		{
			std::cout << "Could not create subdirectory! " << i << std::endl;
		}
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
	piece.append_attribute("NumberOfPoints") = m_mpi_info.arrayend - m_mpi_info.arraystart;
	piece.append_attribute("NumberOfVerts") = m_mpi_info.arrayend - m_mpi_info.arraystart;

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
	m_acceleration = pointdata.append_child("DataArray");
	m_acceleration.append_attribute("type") = "Float64";
	m_acceleration.append_attribute("Name") = "acceleration";
	m_acceleration.append_attribute("NumberOfComponents") = "3";
	m_acceleration.append_attribute("format") = "ascii";


	// angle
	m_phi = pointdata.append_child("DataArray");
	m_phi.append_attribute("type") = "Float64";
	m_phi.append_attribute("Name") = "phi";
	m_phi.append_attribute("NumberOfComponents") = "3";
	m_phi.append_attribute("format") = "ascii";

	
	// omega
	m_omega = pointdata.append_child("DataArray");
	m_omega.append_attribute("type") = "Float64";
	m_omega.append_attribute("Name") = "omega";
	m_omega.append_attribute("NumberOfComponents") = "3";
	m_omega.append_attribute("format") = "ascii";

	
	// alpha
	m_alpha = pointdata.append_child("DataArray");
	m_alpha.append_attribute("type") = "Float64";
	m_alpha.append_attribute("Name") = "alpha";
	m_alpha.append_attribute("NumberOfComponents") = "3";
	m_alpha.append_attribute("format") = "ascii";


	//
	pugi::xml_node verts = piece.append_child("Verts");
	m_verts = verts.append_child("DataArray");
	m_verts.append_attribute("type") = "Int64";
	m_verts.append_attribute("Name") = "offsets";

	std::string out = "\n";
	for (int i = m_mpi_info.arraystart; i < m_mpi_info.arrayend; i++)
	{
		out += std::to_string(i + 1 - m_mpi_info.arraystart) + " ";
	}
	m_verts.text() = out.c_str();

	// connectivity
	m_conn = verts.append_child("DataArray");
 	m_conn.append_attribute("type") = "Int64";
	m_conn.append_attribute("Name") = "connectivity";

	out = "\n";
	for (int i = m_mpi_info.arraystart; i < m_mpi_info.arrayend; i++)
	{
		out += std::to_string(i - m_mpi_info.arraystart) + " ";
	}
	m_conn.text() = out.c_str();


/*
	pugi::xml_node fielddata = polydata.append_child("FieldData");
	m_kin_e = fielddata.append_child("DataArray");
	m_kin_e.append_attribute("type") = "Float64";
	m_kin_e.append_attribute("Name") = "kinetic energy";
	m_kin_e.append_attribute("NumberOfTuples") = "1";
	m_kin_e.append_attribute("format") = "ascii";

	m_rot_e = fielddata.append_child("DataArray");
	m_rot_e.append_attribute("type") = "Float64";
	m_rot_e.append_attribute("Name") = "rotational energy";
	m_rot_e.append_attribute("NumberOfTuples") = "1";
	m_rot_e.append_attribute("format") = "ascii";

	m_pot_e = fielddata.append_child("DataArray");
	m_pot_e.append_attribute("type") = "Float64";
	m_pot_e.append_attribute("Name") = "potential energy";
	m_pot_e.append_attribute("NumberOfTuples") = "1";
	m_pot_e.append_attribute("format") = "ascii";

	m_total_e = fielddata.append_child("DataArray");
	m_total_e.append_attribute("type") = "Float64";
	m_total_e.append_attribute("Name") = "total energy";
	m_total_e.append_attribute("NumberOfTuples") = "1";
	m_total_e.append_attribute("format") = "ascii";*/
}


void OutputWriter::write_vtp()
{
	// position
	std::string pos = "\n";
	std::string vel = "\n";
	std::string acc = "\n";
	std::string phi = "\n";
	std::string omega = "\n";
	std::string alpha = "\n";
	for (int i = m_mpi_info.arraystart; i < m_mpi_info.arrayend; i++)
	{
		Particle& body = m_mol_arr[i];
		pos   += body.position.serialize().c_str();
		vel   += body.velocity.serialize().c_str();
		acc   += body.acceleration.serialize().c_str();
		phi   += body.phi.serialize().c_str();
		omega += body.omega.serialize().c_str();
		alpha += body.alpha.serialize().c_str();
		pos += "\n";
		vel += "\n";
		acc += "\n";
		phi += "\n";
		omega += "\n";
		alpha += "\n";
	}
	m_position.text() = pos.c_str();
	m_velocity.text() = vel.c_str();
	m_acceleration.text() = acc.c_str();
	m_phi.text() = phi.c_str();
	m_omega.text() = omega.c_str();
	m_alpha.text() = alpha.c_str();


	//m_kin_e.text() = m_glob.kin_e;
	//m_rot_e.text() = m_glob.rot_e;
	//m_pot_e.text() = m_glob.pot_e;
	//m_total_e.text() = m_glob.total;

	std::ofstream outFile;
	outFile.open(m_path + std::to_string(m_step)+std::string(".vtp"));
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

void OutputWriter::write_pvd(std::string filename)
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
	for (int i = 0; i < m_step; i ++)
	{
		time += m_vs_dt;
		for (int j = 0; j < m_mpi_info.processNo; j++)
		{
			dataset = collection.append_child("DataSet");
			dataset.append_attribute("timestep") = scientific(time).c_str();
			dataset.append_attribute("group") = "";
			dataset.append_attribute("part") = j;
			std::string out = "time_series/" + std::to_string(j) + "/sim_" + std::to_string(i) + ".vtp";
			dataset.append_attribute("file") = out.c_str();
		}
	}

	std::ofstream outFile;
	outFile.open(m_dir+filename);
	doc.save(outFile);
	outFile.close();
}


