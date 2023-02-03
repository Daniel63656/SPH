#include "grid.h"
#include "particle.h"
#include "neighbourhood.h"

Grid::Grid(const Settings& settings) : m_settings(settings)
{
	//nTotal = 1;
	//for (int i = 0; i < 2; i++)
	//{
	//	meshWidth[i] = m_settings.physicalSize[i] / (float)m_settings.nCells[i];
	//	nTotal *= m_settings.nCells[i];
	//}
    Vec2d size = m_settings.physicalSize;
    size.x += 2*m_settings.boundaryThickness;
    size.y += 2*m_settings.boundaryThickness;
	m_meshWidth = size / m_settings.nCells;
	nTotal = m_settings.nCells.x * m_settings.nCells.y;

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


void Grid::add(Particle* p)
{
	int idx = pos2idx(discretizedPosition(p->position));
	grid[idx].push_back(p);
}

/*Neighbourhood Grid::neighbours(const Vec2d& center, double radius)
{
	return Neighbourhood(this, center, radius);
}*/


std::vector<Particle*> Grid::neighbours(const Vec2d& center, double radius)
{
	//TODO implement as iterator

	std::vector<Particle*> neighbours;
	//made x inner loop, so we traverse particle collection in the correct order
	for (int y = std::max(0, (int)(center.y - radius)); y < std::min(m_settings.nCells.y, (int)(center.y + radius) + 1); y++)
	{
		for (int x = std::max(0, (int)(center.x - radius)); x < std::min(m_settings.nCells.x, (int)(center.x + radius) + 1); x++)
		{
			for (Particle* p : grid[pos2idx({ x, y })])
			{
				if (euclideanDistance(p->position, center) <= radius)
				{
					neighbours.push_back(p);
				}
			}
		}
	}
	return neighbours;
}

Vec2i Grid::discretizedPosition(Vec2d v) {
	Vec2i discretePos;
	discretePos.x = int(v.x / m_meshWidth.x);
	discretePos.y = int(v.y / m_meshWidth.y);

	discretePos.x = (discretePos.x < 0) ? 0 : discretePos.x;
	discretePos.y = (discretePos.y < 0) ? 0 : discretePos.y;

	discretePos.x = (discretePos.x >= m_settings.nCells.x) ? m_settings.nCells.x - 1 : discretePos.x;
	discretePos.y = (discretePos.y >= m_settings.nCells.y) ? m_settings.nCells.y - 1 : discretePos.y;

	return discretePos;

	//for (int i = 0; i < 2; i++)
	//{
	//	discretePos[i] = int(v[i] / meshWidth[i]);
	//	//TODO this is just a temporary hack. Particles need to be stopped from leaving the domain in the first place!
	//	discretePos[i] = (discretePos[i] < 0) ? 0 : discretePos[i];
	//	discretePos[i] = (discretePos[i] >= m_settings.nCells[i]) ? m_settings.nCells[i] - 1 : discretePos[i];
	//}
	//return discretePos;
}

//! map 2-dimensional index-vector into a unique, seamless scalar index
int Grid::pos2idx(Vec2i pos)
{
	return pos.x + m_settings.nCells.x * pos.y;
}
