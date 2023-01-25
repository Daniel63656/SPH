#pragma once

#include "kernelFunction.h"

template<unsigned int N>
class Gaussian : public KernelFunction<N>
{
public:
    Gaussian(float smoothing) : KernelFunction<N>(smoothing)
    {
        switch (N) {
            case 1:
                alpha = 1/this->h; break;
            case 2:
                alpha = 15/(float)(7*M_PI*this->h*this->h); break;
            case 3:
                alpha = 3/(float)(2*M_PI*pow(this->h, 3)); break;
            default:
                throw std::invalid_argument("Number of dimensions not supported!");
        }
    }


    double W(Vector<N> difference) override {
        float R = difference.magnitude()/this->h;
        if (R <= 3) {
            return alpha * std::exp(-R * R);
        }
        else return 0;
    }

    Vector<N> gradW(Vector<N> difference) override {
        float R = difference.magnitude()/this->h;
        if (R <= 3) {
            double factor = -2 * alpha / (this->h*this->h) * std::exp(-R * R);
            Vector<N> res = Vector<N>();
            for (int i = 0; i < N; i++)
            {
                res[i] = factor * difference[i];
            }
            return res;
        }
        else {
            Vector<N> n = Vector<N>();
            return n;
        };
    }

    double laplaceW(Vector<N> difference) override {
        float R = difference.magnitude()/this->h;
        if (R <= 3) {
            double factor = 4 * alpha/pow(this->h, 4) * std::exp(-R * R);
            double res = 0;
            for (int i = 0; i < N; i++)
            {
                res += factor * difference[i];
            }
            return res;
        }
        else return 0;
    }

    double effectiveRadius() override {
        return 3*this->h;
    }

private:
    float alpha;
};