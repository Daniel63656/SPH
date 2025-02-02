#include "grid.h"
#include "neighbourhood.h"
#include "particle.h"

Grid::Grid(const Settings& settings) : m_settings(settings)
{
	// the grid encloses all simulated and boundary particles
	Vec2d size = m_settings.physicalSize;
	size.x += m_settings.left.m_thickness + m_settings.right.m_thickness;
	size.y += m_settings.top.m_thickness + m_settings.bottom.m_thickness;
	m_meshWidth = size / m_settings.nCells;

	nTotal = m_settings.nCells.x * m_settings.nCells.y;

	// initialize vectors for each gridCell
	grid.reserve(nTotal);
	for (int i = 0; i < nTotal; i++)
	{
		grid.emplace_back();
	}
}

void Grid::clear()
{
	// reset cell vectors, but don't deallocate the memory
	for (int i = 0; i < nTotal; i++)
		grid[i].clear();
}

void Grid::add(Particle* p)
{
	Vec2i d = discretizedPosition(p->position);
	int idx = pos2idx(d);
	grid[idx].push_back(p);
}

Neighbourhood Grid::neighbours(const Vec2d& center, double radius)
{
	return Neighbourhood(this, center, radius);
}

Vec2i Grid::discretizedPosition(Vec2d v) const
{
	Vec2i discretePos;
	discretePos.x = floor(v.x / m_meshWidth.x);
	discretePos.y = floor(v.y / m_meshWidth.y);

	discretePos.x = (discretePos.x < 0) ? 0 : discretePos.x;
	discretePos.y = (discretePos.y < 0) ? 0 : discretePos.y;

	discretePos.x = (discretePos.x >= m_settings.nCells.x) ? m_settings.nCells.x - 1 : discretePos.x;
	discretePos.y = (discretePos.y >= m_settings.nCells.y) ? m_settings.nCells.y - 1 : discretePos.y;

	return discretePos;
}

//! map 2-dimensional index-vector into a unique, seamless scalar index
int Grid::pos2idx(Vec2i pos) const
{
	return pos.x + m_settings.nCells.x * pos.y;
}
