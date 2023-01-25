#pragma once

#include "kernelFunction.h"

template<unsigned int N>
class CubicSpline : public KernelFunction<N>
{
public:
    CubicSpline(float smoothing);
    double W(Vector<N> difference) override;
    Vector<N> gradW(Vector<N> difference) override;
    double laplaceW(Vector<N> difference) override;
    double effectiveRadius() override;

private:
    float alpha;
};