#pragma once

#include "../settings/settings.h"
#include "particle.h"
#include "pch.h"

class Neighbourhood;

class Grid
{
  public:
	Grid(const Settings& settings);

	void clear();
	void add(Particle* p);

	friend class Neighbourhood;
	Neighbourhood neighbours(const Vec2d& center, double radius);

  private:
	std::vector<std::vector<Particle*>> grid;
	const Settings& m_settings;

	int nTotal;			 // total number of cells
	Vec2d m_meshWidth{}; // width of the cells in all spacial directions

	Vec2i discretizedPosition(Vec2d v) const;
	int pos2idx(Vec2i pos) const;
};
