#pragma once

#include <vtkSmartPointer.h>
#include <vtkXMLImageDataWriter.h>
#include "../datastructures/grid.h"

/** Write *.vti files that can be viewed with ParaView.
 *  The mesh that can be visualized in ParaView corresponds to the mesh of the computational domain.
 *  All values are given for the nodes of the mesh, i.e., the corners of each cell.
 *  This means, values will be interpolated because the values are stored at positions given by the staggered grid.
 */
class VtkWriter
{
public:
    //! constructor
    VtkWriter(Grid& grid);

//! write current velocities to file, filename is output_<count>.vti
    void writeFile(double currentTime);

private:
    vtkSmartPointer<vtkXMLImageDataWriter> writer;      //< vtk writer to write ImageData
    const Grid m_grid;              //< reference to the simulation object
    int fileNo;                     //< a counter that increments for every file, this number is part of the file name of output files
};