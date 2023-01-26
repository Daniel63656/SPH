#include "gaussian.h"
#include <cmath>


//! Gaussian function with effective radius of 3*smoothing
Gaussian::Gaussian(double smoothing) : KernelFunction(smoothing)
{
    //1d    alpha = 1/(sqrt(M_PI)*h);
    alpha = 1/(M_PI*h*h);
    //3d    alpha = 1/(sqrt(pow(M_PI, 3))*pow(h, 3));
}

double Gaussian::W(Vector<2> difference) const {
    double R = difference.magnitude()/h;
    if (R <= 3)
    {
        return alpha * exp(-R * R);
    }
    else return 0;
}


Vector<2> Gaussian::gradW(Vector<2> difference) const {
    double R = difference.magnitude()/h;
    Vector<2> res = Vector<2>();
    if (R <= 3) {
        double factor = -2 / (h*h) * exp(-R * R);
        for (int i = 0; i < 2; i++)
        {
            res[i] = alpha*factor * difference[i];
        }
        return res;
    }
    else {
        res = 0;
        return res;
    }
}


double Gaussian::laplaceW(Vector<2> difference) const {
    double R = difference.magnitude()/h;
    if (R <= 3) {
        double res = 0;
        for (int i = 0; i < 2; i++)
        {
            res += alpha * (4*difference[i]*difference[i] - 2*h*h)/pow(h, 4) * exp(-R * R);
        }
        return res;
    }
    else return 0;
}


double Gaussian::effectiveRadius() const {
    return 3*h;
}
