#pragma once

#include "kernelFunction.h"

class Gaussian : public KernelFunction
{
public:
    explicit Gaussian(double smoothing);

    double W(Vec2d difference) const override;
    Vec2d gradW(Vec2d difference) const override;
    double laplaceW(Vec2d difference) const override;
    double effectiveRadius() const override;

private:
    double alpha;
};