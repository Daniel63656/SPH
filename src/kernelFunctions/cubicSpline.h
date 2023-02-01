#pragma once

#include "kernelFunction.h"

class CubicSpline : public KernelFunction
{
public:
    explicit CubicSpline(double smoothing);

    double W(Vec2 difference) const override;
    Vec2 gradW(Vec2 difference) const override;
    double laplaceW(Vec2 difference) const override;
    double effectiveRadius() const override;

private:
    double alpha;
};