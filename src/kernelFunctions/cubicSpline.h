#pragma once

#include "kernelFunction.h"

class CubicSpline : public KernelFunction
{
public:
    explicit CubicSpline(double smoothing);

    double W(Vector<2> difference) const override;
    Vector<2> gradW(Vector<2> difference) const override;
    double laplaceW(Vector<2> difference) const override;
    double effectiveRadius() const override;

private:
    double alpha;
};