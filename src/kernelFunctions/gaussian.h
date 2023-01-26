#pragma once

#include "kernelFunction.h"

template<unsigned int N>
class Gaussian : public KernelFunction<N>
{
public:
    Gaussian(float smoothing);


    virtual double W(Vector<N> difference);
    virtual Vector<N> gradW(Vector<N> difference);
    virtual double laplaceW(Vector<N> difference);
    virtual double effectiveRadius();

private:
    float alpha;
};