#pragma once

//! all m_kernel functions based of the paper "Smoothed Particle Hydrodynamics (SPH): an Overview and Recent Developments"
//! https://link.springer.com/article/10.1007/s11831-010-9040-7

#include "../datastructures/vector.h"

class KernelFunction
{
public:
    explicit KernelFunction(double smoothing) : h{smoothing}
    {}

    virtual double W(Vector<2> difference) const = 0;
    virtual Vector<2> gradW(Vector<2> difference) const = 0;
    virtual double laplaceW(Vector<2> difference) const = 0;
    virtual double effectiveRadius() const = 0;

protected:
    double h;
};