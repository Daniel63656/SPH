#include "neighbourhood.h"

Neighbourhood::NeighbourhoodIterator::NeighbourhoodIterator(Neighbourhood* n, size_t outer, size_t inner)
	: m_ptr(n), index_inner(inner), index_outer(outer)
{
	min = n->m_grid->discretizedPosition(n->m_center - Vec2d(n->m_radius));
	max = n->m_grid->discretizedPosition(n->m_center + Vec2d(n->m_radius));
	end_index = n->m_grid->pos2idx(Vec2i(min.x, max.y + 1));
}

void Neighbourhood::NeighbourhoodIterator::findNextElement()
{
	while (true)
	{
		// check if there is a next element in this cell
		int lastindex = (m_ptr->getGrid())[index_outer].size() - 1;
		if (index_inner < lastindex)
		{
			index_inner++; // go to next element in this cell
		}
		else
		{
			// skip to next non empty cell or end
			index_inner = 0;
			index_outer++;
			if (index_outer % m_ptr->nCells.x > max.x || index_outer % m_ptr->nCells.x == 0)
			{
				index_outer = index_outer + m_ptr->nCells.x - (max.x - min.x + 1);
			}
		}

		// reached the position of the end iterator
		if (index_outer == end_index)
		{
			break;
		}

		// make sure the next element is inside our kernels influence
		if (!m_ptr->getGrid()[index_outer].empty() &&
			(euclideanDistance(m_ptr->getCenter(), (m_ptr->getGrid())[index_outer][index_inner]->position) <=
			 m_ptr->getRadius()))
		{
			break;
		}
	}
}

Neighbourhood::Iterator Neighbourhood::begin()
{
	int min = m_grid->pos2idx(m_grid->discretizedPosition(m_center - Vec2d(m_radius)));
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

Neighbourhood::Iterator Neighbourhood::end()
{
	// get the positions of our "to be searched" cells
	Vec2i min = m_grid->discretizedPosition(m_center - Vec2d(m_radius));
	Vec2i max = m_grid->discretizedPosition(m_center + Vec2d(m_radius));

	// calculate the end index
	int end = m_grid->pos2idx(Vec2i(min.x, max.y + 1));

	return Iterator(this, end, 0);
}

std::vector<std::vector<Particle*>>& Neighbourhood::getGrid()
{
	return m_grid->grid;
}

Vec2d Neighbourhood::getCenter() const
{
	return m_center;
}

inline double Neighbourhood::getRadius() const
{
	return m_radius;
}
