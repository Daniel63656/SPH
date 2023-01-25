#pragma once

#include "kernelFunction.h"

class Gaussian : public KernelFunction
{
public:
    Gaussian(float smoothing, int dimensions);
    float   W(float distance) override;
    float  dW(float distance) override;
    float d2W(float distance) override;
    float effectiveRadius() override;

private:
    float alpha;
};