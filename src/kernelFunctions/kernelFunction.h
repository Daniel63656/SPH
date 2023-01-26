#pragma once

//! all m_kernel functions based of the paper "Smoothed Particle Hydrodynamics (SPH): an Overview and Recent Developments"
//! https://link.springer.com/article/10.1007/s11831-010-9040-7

#include "../datastructures/vector.h"

template<unsigned int N>
class KernelFunction
{
public:
    explicit KernelFunction(float smoothing);

    virtual double W(Vector<N> difference) const = 0;
    virtual Vector<N> gradW(Vector<N> difference) const = 0;
    virtual double laplaceW(Vector<N> difference) const = 0;
    virtual double effectiveRadius() const = 0;

protected:
    double h;
};

template<unsigned int N>
KernelFunction<N>::KernelFunction(float smoothing) :
    h{smoothing}
    {}
