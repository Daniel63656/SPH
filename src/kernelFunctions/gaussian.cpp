#include "gaussian.h"
#include <cmath>

//! Gaussian function with effective radius of 3*smoothing
Gaussian::Gaussian(double smoothing) : KernelFunction(smoothing)
{
    //1d    alpha = 1/(sqrt(M_PI)*h);
    alpha = 1.0/(M_PI*h*h);
    //3d    alpha = 1/(sqrt(pow(M_PI, 3))*pow(h, 3));
}

double Gaussian::W(Vec2d difference) const {
    double R = difference.magnitude()/h;
    if (R <= 3)
    {
        return alpha * exp(-R * R);
    }
    else return 0;
}


Vec2d Gaussian::gradW(Vec2d difference) const {
    double R = difference.magnitude()/h;
    Vec2d res = Vec2d();
    if (R <= 3) {
        double factor = -2 / (h*h) * exp(-R * R);
        //for (int i = 0; i < 2; i++)
        //{
        //    res[i] = alpha*factor * difference[i];
        //}
        res.x = alpha * factor * difference.x;
        res.y = alpha * factor * difference.y;
        return res;
    }
    else {
        res = 0;
        return res;
    }
}


double Gaussian::laplaceW(Vec2d difference) const {
    double R = difference.magnitude()/h;
    if (R <= 3) {
        double res = 0;
        //for (int i = 0; i < 2; i++)
        //{
        //    res += alpha * (4*difference[i]*difference[i] - 2*h*h)/pow(h, 4) * exp(-R * R);
        //}
        res += alpha * (4 * difference.x * difference.x - 2 * h * h) / pow(h, 4) * exp(-R * R);
        res += alpha * (4 * difference.y * difference.y - 2 * h * h) / pow(h, 4) * exp(-R * R);
        return res;
    }
    else return 0;
}


double Gaussian::effectiveRadius() const {
    return 3*h;
}
