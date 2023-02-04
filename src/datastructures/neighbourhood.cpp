#include "neighbourhood.h"

Neighbourhood::NeighbourhoodIterator::NeighbourhoodIterator(Neighbourhood* n, size_t outer, size_t inner) : m_ptr(n), index_inner(inner), index_outer(outer)
{
    min = n->m_grid->discretizedPosition(n->m_center - Vec2d(n->m_radius));
    max = n->m_grid->discretizedPosition(n->m_center + Vec2d(n->m_radius));
    //int lastrelevantouter = n->m_grid->pos2idx(max);
    end_index = n->m_grid->pos2idx(Vec2i(min.x, max.y + 1));

    //index_lowerbound_x = std::max(0, (int)(n->getCenter().x - n->getRadius()));
    //index_upperbound_x = std::min(n->nCells.x, (int)(n->getCenter().x + n->getRadius() + 1));
    //index_lowerbound_y = std::max(0, (int)(n->getCenter().y - n->getRadius()));
    //index_upperbound_y = std::min(n->nCells.y, (int)(n->getCenter().y + n->getRadius() + 1));
}

void Neighbourhood::NeighbourhoodIterator::findNextElement()
{
    //bool found = false;
    while (true)
    {
        // check if there is a next element in this cell
        int lastindex = (m_ptr->getGrid())[index_outer].size() - 1;
        if (index_inner < lastindex)
        {
            index_inner++;  // go to next element in this cell
        }
        else
        {
            // skip to next non empty cell or end
            index_inner = 0;
            index_outer++;
            if (index_outer % m_ptr->nCells.y > max.x || index_outer % m_ptr->nCells.y == 0)
            {
                index_outer = index_outer + m_ptr->nCells.x - (max.x - min.x + 1);
            }
            //while (++index_outer < (m_ptr->getGrid()).size() && (m_ptr->getGrid())[index_outer].empty());
        }

        if (index_outer == end_index)
        {
            //std::cout << "iterator returns (end)" << index_outer << ", " << index_inner << std::endl;
            break;
        }

        if (!m_ptr->getGrid()[index_outer].empty() &&
            (euclideanDistance(m_ptr->getCenter(), (m_ptr->getGrid())[index_outer][index_inner]->position) <= m_ptr->getRadius()))
        {
            //std::cout << "iterator returns " << index_outer << ", " << index_inner << std::endl;
            break;
        }
        //// get the x,y index from index_outer
        //int currentcell_y = index_outer / m_ptr->nCells.y;
        //int currentcell_x = index_outer % m_ptr->nCells.y;

        //// cell is skippable if its not inside our searchbounds
        //bool skipablecell = !(min.x <= currentcell_x && currentcell_x <= max.x &&
        //    min.y <= currentcell_y && currentcell_y <= max.y);
        //if ((index_outer == max.y))
        //{
        //    // found since we reached the end
        //    found = true;
        //}
        //else if (!skipablecell && (euclideanDistance(m_ptr->getCenter(), (m_ptr->getGrid())[index_outer][index_inner]->position) <= m_ptr->getRadius()))
        //{
        //    // found if next found element is inside the search radius
        //    found = true;
        //}
    }
}