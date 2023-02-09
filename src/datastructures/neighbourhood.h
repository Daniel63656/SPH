#pragma once

#include "grid.h"

class Neighbourhood
{
public:
	using ValueType = Particle;
public:

	class NeighbourhoodIterator
	{
	public:
		using ValueType = typename Neighbourhood::ValueType;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;
	public:
		NeighbourhoodIterator(Neighbourhood* n, size_t outer, size_t inner);

		void findNextElement();

		NeighbourhoodIterator& operator++()
		{
			findNextElement();
			return *this;
		}

		NeighbourhoodIterator operator++(int)
		{
			NeighbourhoodIterator iter = *this;
			++(*this);
			return iter;
		}

		ReferenceType operator*()
		{
			return *(m_ptr->getGrid())[index_outer][index_inner];
		}

		bool operator==(const NeighbourhoodIterator& other) const
		{
			return (index_inner == other.index_inner) && (index_outer == other.index_outer);
		}

		bool operator!=(const NeighbourhoodIterator& other) const
		{
			return (index_inner != other.index_inner) || (index_outer != other.index_outer);
		}

	private:
		Neighbourhood* m_ptr;
		int index_outer; // current cell
		int index_inner; // current index in our cell

		Vec2i min; // 2d index into our grid, marks the lower left cell of our cells in the search area
		Vec2i max; // 2d index into our grid, marks the upper right cell of our search area
		int end_index; // cell index, marks our end
	};

public:
	using Iterator = NeighbourhoodIterator;

	Neighbourhood(Grid* grid, const Vec2d& center, double radius) : m_grid(grid), m_center(center), m_radius(radius)
	{
		nCells = grid->m_settings.nCells;
	}

	Iterator begin()
	{
		int min = m_grid->pos2idx( m_grid->discretizedPosition(m_center - Vec2d(m_radius)));
		Iterator it(this, min, 0);
		if (!getGrid()[min].empty() && euclideanDistance(getCenter(), (getGrid())[min][0]->position) <= getRadius())
		{
			return it;
		}
		else
		{
			it.findNextElement();
			return it;
		}
	}

	Iterator end()
	{
		// get the positions of our "to be searched" cells
		Vec2i min = m_grid->discretizedPosition(m_center - Vec2d(m_radius));
		Vec2i max = m_grid->discretizedPosition(m_center + Vec2d(m_radius));

		// calculate the end index
		int end = m_grid->pos2idx(Vec2i(min.x, max.y + 1));

		return Iterator(this, end, 0);
	}

	std::vector<std::vector<Particle*>>& getGrid()
	{
		return m_grid->grid;
	}

	inline Vec2d getCenter() const
	{
		return m_center;
	}

	inline double getRadius() const
	{
		return m_radius;
	}

public:
	Vec2i nCells;
private:
	Grid* m_grid;
	Vec2d m_center;
	double m_radius;
};