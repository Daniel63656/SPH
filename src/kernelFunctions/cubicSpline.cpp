#include "cubicSpline.h"

//! CubicSpline function with effective radius of 2*smoothing
template<unsigned int N>
CubicSpline<N>::CubicSpline(float smoothing) : KernelFunction<N>(smoothing)
{
    switch (N) {
        case 1:
            alpha = 1/(float)(sqrt(M_PI)*this->h); break;
        case 2:
            alpha = 1/(float)(M_PI*this->h*this->h); break;
        case 3:
            alpha = 1/(float)(sqrt(pow(M_PI, 3))*pow(this->h, 3)); break;
        default:
            throw std::invalid_argument("Number of dimensions not supported!");
    }
}

template<unsigned int N>
double CubicSpline<N>::W(Vector<N> difference) {
    float R = difference.magnitude()/this->h;
    if (R <= 1) {
        return alpha * 2.0f/3 - R*R + (float)pow(R, 3)/2;
    }
    else if (R <= 2) {
        return alpha * 1.0f/6 * (float)pow(2-R, 3);
    }
    else return 0;
}

template<unsigned int N>
Vector<N> CubicSpline<N>::gradW(Vector<N> difference) {
    float R = difference.magnitude() / this->h;
    if (R <= 1) {
        double factor = alpha * (-2*R + 3*R*R/2) / (R*this->h*this->h);
        Vector<N> res = Vector<N>();
        for (int i = 0; i < N; i++)
        {
            res[i] = factor * difference[i];
        }
        return res;
    }
    else if (R <= 2) {
        double factor = alpha * (2-R)*(2-R) / (2*R*this->h*this->h);
        Vector<N> res = Vector<N>();
        for (int i = 0; i < N; i++)
        {
            res[i] = factor * difference[i];
        }
        return res;
    }
    else return 0;
}

template<unsigned int N>
double CubicSpline<N>::laplaceW(Vector<N> difference) {
    float R = difference.magnitude()/this->h;


    if (R <= 1) {

        double factor = 4 * alpha/pow(this->h, 4) * std::exp(-R * R);
        double res = 0;
        for (int i = 0; i < N; i++)
        {
            res += factor * difference[i];
        }
        return res;


        return alpha * 2.0f/3 - R*R + (float)pow(R, 3)/2;
    }
    else if (R <= 2) {
        return alpha * 1.0f/6 * (float)pow(2-R, 3);
    }
    else return 0;
}

template<unsigned int N>
double CubicSpline<N>::effectiveRadius() {
    return 2*this->h;
}
