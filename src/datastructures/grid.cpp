#include "grid.h"


Grid::Grid(const Settings& settings) : m_settings{settings}
{
    nCells = 1;
    for (int i = 0; i < 2; i++)
    {
        meshWidth[i] = m_settings.physicalSize[i] / m_settings.size[i];
        nCells *= m_settings.size[i];
    }

    grid.reserve(nCells);
    for (int i = 0; i < nCells; i++)
    {
        grid.emplace_back();
    }
}


void Grid::clear()
{
    for (int i = 0; i < nCells; i++)
        grid[i].clear();
}


void Grid::add(Particle p)
{
    int idx = pos2idx(discretizedPosition(p.position));
    grid[idx].push_back(p);
}


std::vector<Particle> Grid::neighbours(Particle center, double radius)
{
    std::vector<Particle> neighbours;
    //made x inner loop, so we traverse particle collection in the correct order
    for (int y = (int)(center.position[1]-radius); y < center.position[1]+radius; y++)
    {
        for (int x = (int)(center.position[0]-radius); x < center.position[0]+radius; x++)
        {
            for (Particle p : grid[pos2idx({x, y})])
            {
                if (euclideanDistance(p.position, center.position) <= radius)
                {
                    neighbours.push_back(p);
                }
            }
        }
    }
    return neighbours;
}




std::array<int, 2> Grid::discretizedPosition(Vector<2> v) {
    std::array<int, 2> discretePos{};
    for (int i = 0; i < 2; i++)
    {
        discretePos[i] = int(v[i]/meshWidth[i]);
        //TODO limiting necessary?
    }
    return discretePos;
}

//! map 2-dimensional index-vector into a unique, seamless scalar index
int Grid::pos2idx(std::array<int, 2> pos)
{
    int idx = pos[0];
    int factor = 1;
    for (int i = 1; i < pos.size(); i++)
    {
        factor *= m_settings.size[i-1];
        idx += factor*pos[i];
    }
    return idx;
}
