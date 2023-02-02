#pragma once

#include "../pch.h"

//! vector class to do basic vector operations and hold N entries
//template <unsigned int N> class Vector;
//template <unsigned int N> std::ostream& operator<< (std::ostream&, const Vector<N>&);
/*
template <unsigned int N>
class Vector {
public:
	Vector() = default;
	Vector(std::initializer_list<double> elements) {
		if (N != elements.size())
			throw std::length_error("Number of elements wrong for specified vector!");

		for (int i = 0; i != elements.size(); i++)
			data[i] = *(elements.begin() + i);
	}

	//! get the number of elements
	int size() const { return N; };

	double magnitude() {
		double res = 0;
		for (int i = 0; i < N; i++)
			res += data[i] * data[i];
		return sqrt(res);
	}

	//! access value at idx, declared not const, so the value can be changed
	//double& operator[](int idx) { return data[idx]; }

	//! access value at idx, declared const, so it is not possible to change the value
	//const double& operator[](int idx) const { return data[idx]; }

	//! non symmetric mathematical operators
	Vector<N>& operator+=(const Vector<N>& v) {


		for (int i = 0; i < N; i++)
			data[i] += v.data[i];
		return *this;
	}

	Vector<N>& operator-=(const Vector<N>& v) {
		for (int i = 0; i < N; i++)
			data[i] -= v.data[i];
		return *this;
	}

	Vector<N>& operator*=(double scalar) {
		for (int i = 0; i < N; i++)
			data[i] *= scalar;
		return *this;
	}

	Vector<N>& operator/=(double scalar) {
		for (int i = 0; i < N; i++)
			data[i] /= scalar;
		return *this;
	}

	Vector<N>& operator=(double d) {
		for (int i = 0; i < N; i++)
			data[i] = d;
		return *this;
	}

	//! for printing on console. Implemented as friend so one can access private data outside class definition
	friend std::ostream& operator<< <N>(std::ostream&, const Vector&);

	std::string serialize(const int n = 5) const
	{
		std::ostringstream out;
		out.precision(n);
		out << std::scientific << data[0] << " " << data[1];
		return out.str();
	}


private:
	std::array<double, N> data = {};
};
typedef Vector<2> Vec2;
*/
/*
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
Vector<N> operator*(double d, const Vector<N>& v) {
	return v * d;
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
	for (int i = 0; i < N; i++)
		sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
	return sqrt(sum);
}

//! print on console
template <unsigned N> std::ostream& operator<<(std::ostream& os, const Vector<N>& v)
{
	os << "[";
	for (unsigned i = 0; i < N; i++)
	{
		os << v.data[i];
		if (i != N - 1) os << ", ";
	}
	os << "]";
	return os;
}
*/
//--------------------

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

	//Vec2(std::initializer_list<double> elements) {
	//	if (N != elements.size())
	//		throw std::length_error("Number of elements wrong for specified vector!");

	//	for (int i = 0; i != elements.size(); i++)
	//		data[i] = *(elements.begin() + i);
	//}

	//! get the number of elements
	int size() const { return N; };

	double magnitude() {
		return sqrt(x*x + y*y);
	}

	//! access value at idx, declared not const, so the value can be changed
	//double& operator[](int idx) { return data[idx]; }

	//! access value at idx, declared const, so it is not possible to change the value
	//const double& operator[](int idx) const { return data[idx]; }

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
		out << std::scientific << x << " " << y;
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
	return os << "[" << v.x << v.y << "]";
}


typedef Vec2<double> Vec2d;
typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;