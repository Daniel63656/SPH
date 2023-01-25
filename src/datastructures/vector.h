#pragma once

#include <array>
#include <cmath>
#include <iostream>

//! vector class to do basic vvector operations and hold N entries
template <unsigned int N> class Vector;
template <unsigned int N> std::ostream& operator<< (std::ostream&, const Vector<N>&);


template <unsigned int N> class Vector {
public:
    Vector() = default;
    Vector(std::initializer_list<double> elements) {
        if(N != elements.size())
            throw std::length_error("Number of elements wrong for specified vector!");

        for(int i = 0; i != elements.size(); i++)
            data[i] = *(elements.begin() + i);
    }

    //! get the number of elements
    int size() const { return N; };

    //! access value at idx, declared not const, so the value can be changed
    double& operator[](int idx) { return data[idx]; }

    //! access value at idx, declared const, so it is not possible to change the value
    const double& operator[](int idx) const { return data[idx]; }

    //! non symmetric mathematical operators
    Vector<N>& operator+=(const Vector<N>& v){
        for(int i = 0; i < N; i++)
            data[i] += v.data[i];
        return *this;
    }

    Vector<N>& operator-=(const Vector<N>& v){
        for(int i = 0; i < N; i++)
            data[i] -= v.data[i];
        return *this;
    }

    Vector<N>& operator*=(double scalar){
        for(int i = 0; i < N; i++)
            data[i] *= scalar;
        return *this;
    }

    Vector<N>& operator/=(double scalar){
        for(int i = 0; i < N; i++)
            data[i] /= scalar;
        return *this;
    }

    Vector<N>& operator=(double d) {
        for(int i = 0; i < N; i++)
            data[i] = d;
        return *this;
    }

    //! for printing on console. Implemented as friend so one can access private data outside class definition
    friend std::ostream& operator<< <N>(std::ostream&, const Vector&);

private:
    std::array<double, N> data = {};
};


//! symmetric mathematical operators (outside class definition to make it a non member function)
template<unsigned int N>
Vector<N> operator+(const Vector<N>& v1, const Vector<N>& v2) {
    Vector<N> res = v1;
    res += v2;
    return res;
}
template<unsigned int N>
Vector<N> operator-(const Vector<N>& v1, const Vector<N>& v2) {
    Vector<N> res = v1;
    res -= v2;
    return res;
}

template <unsigned int N>
Vector<N> operator*(const Vector<N>& v, double d) {
    Vector<N> res = v;
    res *= d;
    return res;
}
template <unsigned int N>
Vector<N> operator*(double d, const Vector<N>& v){
    return v*d;
}

template <unsigned int N>
Vector<N> operator/(const Vector<N>& v, double d) {
    Vector<N> res = v;
    res /= d;
    return res;
}

//! get the 2-Norm between two vectors
template<unsigned int N>
double euclideanDistance(const Vector<N>& v1, const Vector<N>& v2)
{
    double sum = 0;
    for(int i = 0; i < N; i++)
        sum += (v1[i]-v2[i])*(v1[i]-v2[i]);
    return sqrt(sum);
}

//! print on console
template <unsigned N> std::ostream& operator<<(std::ostream& os, const Vector<N>& v){
    os << "[";
    for(unsigned i = 0; i < N; i++){
        os << v.data[i];
        if(i != N - 1) os << ", ";
    }
    os << "]";
    return os;
}