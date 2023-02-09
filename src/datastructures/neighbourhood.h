#pragma once

#include "grid.h"

// implements the neighbourhood search in the linked cell algorithm
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

		Vec2i min;	   // 2d index into our grid, marks the lower left cell of our cells in the search area
		Vec2i max;	   // 2d index into our grid, marks the upper right cell of our search area
		int end_index; // cell index, marks our end
	};

  public:
	using Iterator = NeighbourhoodIterator;

	Neighbourhood(Grid* grid, const Vec2d& center, double radius) : m_grid(grid), m_center(center), m_radius(radius)
	{
		nCells = grid->m_settings.nCells;
	}

	Iterator begin();

	Iterator end();

	std::vector<std::vector<Particle*>>& getGrid();

	inline Vec2d getCenter() const;

	inline double getRadius() const;

  public:
	Vec2i nCells;

  private:
	Grid* m_grid;
	Vec2d m_center;
	double m_radius;
};
