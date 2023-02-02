#pragma once

#include "grid.h"

class Neighbourhood
{
public:
    using ValueType = Particle;
public:

    //template<typename Neighbourhood>
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
        int index_inner;
        int index_outer;

        size_t index_lowerbound_x;
        size_t index_upperbound_x;
        size_t index_lowerbound_y;
        size_t index_upperbound_y;
    };

public:
    using Iterator = NeighbourhoodIterator;

    Neighbourhood(Grid* grid, const Vec2d& center, double radius) : m_grid(grid), m_center(center), m_radius(radius)
    {
        nCells = grid->m_settings.nCells;
    }

    Iterator begin()
    {
        Iterator it(this, 0, 0);
        it.findNextElement();
        return it;
    }

    Iterator end()
    {
        return Iterator(this, m_grid->grid.size(), 0);
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