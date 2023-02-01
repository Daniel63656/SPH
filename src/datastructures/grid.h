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
        NeighbourhoodIterator(Neighbourhood* n, size_t outer, size_t inner) : m_ptr(n), index_inner(inner), index_outer(outer) {}

        NeighbourhoodIterator& operator++()
        {
            bool found = false;
            while (!found)
            {
                // check if there is a next element in this cell
                if (index_inner < (m_ptr->getGrid())[index_outer].size() - 1)
                {
                    index_inner++;  // go to next element in this cell
                }
                else
                {
                    // skip to next non empty cell or end
                    index_inner = 0;
                    do {} while (++index_outer < (m_ptr->getGrid()).size() && (m_ptr->getGrid())[index_outer].empty());
                }

                // found if next found element is inside the search radius or we reached the end
                if ((index_outer == m_ptr->getGrid().size()) || (euclideanDistance(m_ptr->getCenter(), (m_ptr->getGrid())[index_outer][index_inner]->position) <= m_ptr->getRadius()))
                {
                    found = true;
                }
            }
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
        size_t index_inner;
        size_t index_outer;
    };

    class Neighbourhood
    {
    public:
        using ValueType = Particle;
        using Iterator = NeighbourhoodIterator<Neighbourhood>;
    public:
        Neighbourhood(Grid* grid, const Vec2& center, double radius) : m_grid(grid), m_center(center), m_radius(radius) {}

        Iterator begin()
        {
            //find the first element in the grid
            for (int i = 0; i < m_grid->grid.size(); i++)
            {
                if (!m_grid->grid[i].empty())
                {
                    return Iterator(this, i, 0);
                }
            }

            return end();
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