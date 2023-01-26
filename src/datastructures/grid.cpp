#include "grid.h"

Grid::Grid(const Settings& settings) : m_settings(settings)
{
    nTotal = 1;
    for (int i = 0; i < 2; i++)
    {
        meshWidth[i] = m_settings.physicalSize[i] / (float)m_settings.nCells[i];
        nTotal *= m_settings.nCells[i];
    }

    //initialize vectors for each gridCell
    grid.reserve(nTotal);
    for (int i = 0; i < nTotal; i++)
    {
        grid.emplace_back();
    }
}


void Grid::clear()
{
    for (int i = 0; i < nTotal; i++)
        grid[i].clear();
}


void Grid::add(const std::shared_ptr<Particle>& p)
{
    int idx = pos2idx(discretizedPosition(p->position));
    grid[idx].push_back(p);
}


std::vector<std::shared_ptr<Particle>> Grid::neighbours(std::shared_ptr<Particle> center, double radius)
{
    std::vector<std::shared_ptr<Particle>> neighbours;
    //made x inner loop, so we traverse particle collection in the correct order
    for (int y = std::max(0, (int)(center->position[1]-radius)); y < std::min(m_settings.nCells[1], (int)(center->position[1]+radius)+1); y++)
    {
        for (int x = std::max(0, (int)(center->position[0]-radius)); x < std::min(m_settings.nCells[0], (int)(center->position[0]+radius)+1); x++)
        {
            for (const std::shared_ptr<Particle>& p : grid[pos2idx({x, y})])
            {
                if (euclideanDistance(p->position, center->position) <= radius)
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
    return pos[0] + m_settings.nCells[0]*pos[1];
}
