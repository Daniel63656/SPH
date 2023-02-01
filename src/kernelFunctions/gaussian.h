#pragma once

#include "kernelFunction.h"

class Gaussian : public KernelFunction
{
public:
    explicit Gaussian(double smoothing);

    double W(Vec2 difference) const override;
    Vec2 gradW(Vec2 difference) const override;
    double laplaceW(Vec2 difference) const override;
    double effectiveRadius() const override;

private:
    double alpha;
};