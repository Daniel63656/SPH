#include "gaussian.h"
#include <cmath>
#include <stdexcept>


//! Gaussian function with effective radius of 3*smoothing
Gaussian::Gaussian(float smoothing, int dimensions) : KernelFunction(smoothing)
{
    switch (dimensions) {
        case 1:
            alpha = 1/(float)(sqrt(M_PI)*h); break;
        case 2:
            alpha = 1/(float)(M_PI*h*h); break;
        case 3:
            alpha = 1/(float)(sqrt(pow(M_PI, 3))*pow(h, 3)); break;
        default:
            throw std::invalid_argument("Number of dimensions not supported!");
    }
}

float Gaussian::W(float distance) {
    float R = distance/h;
    if (R <= 3) {
        return alpha * std::exp(-R * R);
    }
    else return 0;
}

float Gaussian::dW(float distance) {
    float R = distance/h;
    if (R <= 3) {
        return -2*distance/(h*h) * alpha * std::exp(-R * R);
    }
    else return 0;
}

float Gaussian::d2W(float distance) {
    float R = distance/h;
    if (R <= 3) {
        return 4*(distance*distance)/(float)pow(h, 4) * alpha * std::exp(-R * R);
    }
    else return 0;
}
