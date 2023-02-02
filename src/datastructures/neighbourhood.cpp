#include "neighbourhood.h"

Neighbourhood::NeighbourhoodIterator::NeighbourhoodIterator(Neighbourhood* n, size_t outer, size_t inner) : m_ptr(n), index_inner(inner), index_outer(outer)
{
    index_lowerbound_x = std::max(0, (int)(n->getCenter().x - n->getRadius()));
    index_upperbound_x = std::min(n->nCells.x, (int)(n->getCenter().x + n->getRadius() + 1));
    index_lowerbound_y = std::max(0, (int)(n->getCenter().y - n->getRadius()));
    index_upperbound_y = std::min(n->nCells.y, (int)(n->getCenter().y + n->getRadius() + 1));
}

void Neighbourhood::NeighbourhoodIterator::findNextElement()
{
    bool found = false;
    while (!found)
    {
        // check if there is a next element in this cell
        int adsa = (m_ptr->getGrid())[index_outer].size() - 1;
        if (index_inner < adsa)
        {
            index_inner++;  // go to next element in this cell
        }
        else
        {
            // skip to next non empty cell or end
            index_inner = 0;
            do {} while (++index_outer < (m_ptr->getGrid()).size() && (m_ptr->getGrid())[index_outer].empty());
        }

        // get the x,y index from index_outer
        int currentcell_y = index_outer / m_ptr->nCells.y;
        int currentcell_x = index_outer % m_ptr->nCells.y;

        // cell is skippable if its not inside our searchbounds
        bool skipablecell = !(index_lowerbound_x <= currentcell_x && currentcell_x <= index_upperbound_x &&
            index_lowerbound_y <= currentcell_y && currentcell_y <= index_upperbound_y);
        if ((index_outer == m_ptr->getGrid().size()))
        {
            // found since we reached the end
            found = true;
        }
        else if (!skipablecell && (euclideanDistance(m_ptr->getCenter(), (m_ptr->getGrid())[index_outer][index_inner]->position) <= m_ptr->getRadius()))
        {
            // found if next found element is inside the search radius
            found = true;
        }
    }
}