#pragma once

#include "../pch.h"

template<typename T>
struct Vec2
{
	T x = 0.0, y = 0.0;

	Vec2() = default;
	Vec2(T value)
	{
		x = value;
		y = value;
	}

	Vec2(T x, T y)
	{
		this->x = x;
		this->y = y;
	}

	double magnitude() {
		return sqrt(x*x + y*y);
	}

	//! non symmetric mathematical operators
	Vec2& operator+=(const Vec2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2& operator-=(const Vec2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vec2& operator*=(double scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vec2& operator/=(double scalar) {
		x /= scalar;
		y /= scalar;
		return *this;
	}

	Vec2& operator=(double value) {
		x = value;
		y = value;
		return *this;
	}

	//! for printing on console. Implemented as friend so one can access private data outside class definition
	//friend std::ostream& operator<< <N>(std::ostream&, const Vector&);

	std::string serialize(const int n = 5) const
	{
		std::ostringstream out;
		out.precision(n);
		out << std::scientific << x << " " << y << " 0";
		return out.str();
	}
};

//! symmetric mathematical operators (outside class definition to make it a non member function)
template<typename T>
Vec2<T> operator+(const Vec2<T>& v1, const Vec2<T>& v2) {
	Vec2<T> res = v1;
	res += v2;
	return res;
}
template<typename T>
Vec2<T> operator-(const Vec2<T>& v1, const Vec2<T>& v2) {
	Vec2<T> res = v1;
	res -= v2;
	return res;
}

template<typename T>
Vec2<T> operator*(const Vec2<T>& v, double d) {
	Vec2<T> res = v;
	res *= d;
	return res;
}

template<typename T>
Vec2<T> operator*(double d, const Vec2<T>& v) {
	return v * d;
}

template<typename T>
Vec2<T> operator/(const Vec2<T>& v, double d) {
	Vec2<T> res = v;
	res /= d;
	return res;
}

// componentwise
template<typename T1, typename T2>
Vec2<T1> operator/(const Vec2<T1>& v1, const Vec2<T2>& v2) {
	Vec2<T1> res;
	res.x = v1.x / v2.x;
	res.y = v1.y / v2.y;
	return res;
}

//! get the 2-Norm between two vectors
template<typename T>
double euclideanDistance(const Vec2<T>& v1, const Vec2<T>& v2)
{
	auto diff = v1 - v2;
	return sqrt(diff.x * diff.x + diff.y * diff.y);
}

//! print on console
template<typename T> std::ostream& operator<<(std::ostream& os, const Vec2<T>& v)
{
	return os << "[" << v.x << ", " << v.y << "]";
}

typedef Vec2<double> Vec2d; //two dimensional vector with double type per component
typedef Vec2<int> Vec2i; //two dimensional vector with int type per component
typedef Vec2<float> Vec2f; //two dimensional vector with float type per component
