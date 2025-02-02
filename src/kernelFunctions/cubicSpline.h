#pragma once

#include "kernelFunction.h"

class CubicSpline : public KernelFunction
{
  public:
	explicit CubicSpline(double smoothing);

	double W(Vec2d difference) const override;
	Vec2d gradW(Vec2d difference) const override;
	double laplaceW(Vec2d difference) const override;
	double effectiveRadius() const override;

  private:
	double alpha;
};
