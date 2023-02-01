#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "particle.h"
#include "../settings/settings.h"

class Grid
{
public:
    template<typename Neighbourhood>
    class NeighbourhoodIterator
    {
    public:
        using ValueType = typename Neighbourhood::ValueType;
        using PointerType = ValueType*;
        using ReferenceType = ValueType&;
    public:
        NeighbourhoodIterator(Neighbourhood* n, size_t outer, size_t inner) : m_ptr(n), index_inner(inner), index_outer(outer)
        {
            index_lowerbound_x = std::max(0, (int)(n->getCenter()[0] - n->getRadius()));
            index_upperbound_x = std::min(n->nCells[0], (int)(n->getCenter()[0] + n->getRadius() + 1));
            index_lowerbound_y = std::max(0, (int)(n->getCenter()[1] - n->getRadius()));
            index_upperbound_y = std::min(n->nCells[1], (int)(n->getCenter()[1] + n->getRadius() + 1));
        }

        void findNextElement()
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
                int currentcell_y = index_outer / m_ptr->nCells[1];
                int currentcell_x = index_outer % m_ptr->nCells[1];

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

        const ReferenceType operator*() const
        {
            return *this;
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
        int index_inner;
        int index_outer;

        size_t index_lowerbound_x;
        size_t index_upperbound_x;
        size_t index_lowerbound_y;
        size_t index_upperbound_y;

    };

    class Neighbourhood
    {
    public:
        using ValueType = Particle;
        using Iterator = NeighbourhoodIterator<Neighbourhood>;
    public:
        Neighbourhood(Grid* grid, const Vec2& center, double radius) : m_grid(grid), m_center(center), m_radius(radius)
        {
            nCells = grid->m_settings.nCells;
        }

        Iterator begin()
        {
            //find the first element in the grid
            //for (int i = 0; i < m_grid->grid.size(); i++)
            //{
            //    if (!m_grid->grid[i].empty())
            //    {
            //        return Iterator(this, i, 0);
            //    }
            //}
            Iterator it(this, 0, 0);
            it.findNextElement();
            return it;
        }

        Iterator end()
        {
            return Iterator(this, m_grid->grid.size(), 0);
        }

        auto& getGrid()
        {
            return m_grid->grid;
        }

        inline Vec2 getCenter() const
        {
            return m_center;
        }

        inline double getRadius() const
        {
            return m_radius;
        }

    public:
        std::array<int, 2> nCells{};
    private:
        Grid* m_grid;
        Vec2 m_center;
        double m_radius;
    };

    Grid(const Settings& settings);

    void clear();
    void add(Particle* p);

    Neighbourhood neighbours(const Vec2& center, double radius)
    {
        return Neighbourhood(this, center, radius);
    }

private:
    const Settings& m_settings;

    int nTotal;                             //total number of cells
    std::array<double, 2> meshWidth{};      //width of the cells in all spacial directions
    std::vector<std::vector<Particle*>> grid;

    std::array<int, 2> discretizedPosition(Vec2 v);
    int pos2idx(std::array<int, 2> pos);
};