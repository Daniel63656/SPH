#include "cubicSpline.h"

//! CubicSpline function with effective radius of 2*smoothing
CubicSpline::CubicSpline(double smoothing) : KernelFunction(smoothing)
{
    //1d    alpha = 1/h;
    alpha = 15/(7*M_PI*h*h);
    //3d    alpha = 3/(2*M_PI*pow(h, 3))
}


double CubicSpline::W(Vec2d difference) const {
    double R = difference.magnitude()/h;
    if (R <= 1) {
        return alpha * 2.0f/3 - R*R + pow(R, 3)/2;
    }
    else if (R <= 2) {
        return alpha * 1.0f/6 * pow(2-R, 3);
    }
    else return 0;
}


Vec2d CubicSpline::gradW(Vec2d difference) const {
    double R = difference.magnitude() / h;
    Vec2 res = Vec2d();
    if (R <= 1) {
        double factor = -2/(h*h) + 3*R/(2*h*h);
        //for (int i = 0; i < 2; i++)
        //{
        //    res[i] = alpha*factor * difference[i];
        //}
        res.x = alpha * factor * difference.x;
        res.y = alpha * factor * difference.y;
        return res;
    }
    else if (R <= 2) {
        double factor = (2-R)*(2-R) / (2*R*h*h);
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


double CubicSpline::laplaceW(Vec2d difference) const {
    double R = difference.magnitude()/h;
    double res = 0;
    if (R <= 1) {
        double factor = -2/(h*h) + 3*R/(2*h*h) + 3/(2+R*pow(h, 4));
        //for (int i = 0; i < 2; i++)
        //{
        //    res += alpha*factor * difference[i]*difference[i];
        //}
        res += alpha * factor * difference.x * difference.x;
        res += alpha * factor * difference.y * difference.y;
        return res;
    }
    else if (R <= 2) {
        double factor = -(2-R)*(2-R)/(2*R*h*h) + (-R*R + 4)/(2*pow(R, 3)*pow(h, 4));
        //for (int i = 0; i < 2; i++)
        //{
        //    res += alpha*factor * difference[i]*difference[i];
        //}
        res += alpha * factor * difference.x * difference.x;
        res += alpha * factor * difference.y * difference.y;
        return res;
    }
    else return 0;
}


double CubicSpline::effectiveRadius() const {
    return 2*h;
}
