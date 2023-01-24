#include "CubicSpline.h"
#include <math.h>
#include <stdexcept>


//! CubicSpline function with effective radius of 2*smoothness
CubicSpline::CubicSpline(float smoothing, int dimensions) : KernelFunction(smoothing)
{
    switch (dimensions) {
        case 1:
            alpha = 1/h; break;
        case 2:
            alpha = 15/(float)(7*M_PI*h*h); break;
        case 3:
            alpha = 3/(float)(2*M_PI*pow(h, 3)); break;
        default:
            throw std::invalid_argument("Number of dimensions not supported!");
    }
}

float CubicSpline::W(float distance) {
    float R = distance/h;
    if (R <= 1) {
        return alpha * 2.0f/3 - R*R + (float)pow(R, 3)/2;
    }
    else if (R <= 2) {
        return alpha * 1.0f/6 * (float)pow(2-R, 3);
    }
    else return 0;
}

float CubicSpline::dW(float distance) {
    float R = distance/h;
    if (R <= 1) {
        return alpha * -2*distance/(h*h) + R*R*3/(2*h);
    }
    else if (R <= 2) {
        return alpha * -(2-R)*(2-R)/(2*h);
    }
    else return 0;
}

float CubicSpline::d2W(float distance) {
    float R = distance/h;
    if (R <= 1) {
        return alpha * -2/(h*h) + 3*distance/(h*h*h);
    }
    else if (R <= 2) {
        return alpha * (2-R)/(h*h);
    }
    else return 0;
}
