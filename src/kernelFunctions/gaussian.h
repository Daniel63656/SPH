#pragma once

#include "kernelFunction.h"

class Gaussian : public KernelFunction
{
public:
    explicit Gaussian(double smoothing);

    double W(Vector<2> difference) const override;
    Vector<2> gradW(Vector<2> difference) const override;
    double laplaceW(Vector<2> difference) const override;
    double effectiveRadius() const override;

private:
    double alpha;
};