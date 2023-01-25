#pragma once

//! all kernel functions based of the paper "Smoothed Particle Hydrodynamics (SPH): an Overview and Recent Developments"
//! https://link.springer.com/article/10.1007/s11831-010-9040-7
class KernelFunction
{
public:
    KernelFunction(float smoothing);

    virtual float  W(float distance) = 0;
    virtual float  dW(float distance) = 0;
    virtual float d2W(float distance) = 0;
    virtual float effectiveRadius() = 0;

protected:
    float h;
};
