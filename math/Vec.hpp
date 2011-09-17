#pragma once

#include <cmath>
#include <math/Integer.hpp>
#include <math/ScalarType.hpp>

template <bool, class T = void> struct enable_if { };
template <class T> struct enable_if<true, T> { typedef T type; };

template <int n, class T = float>
struct Vec {
	typedef T value_type;

	typedef value_type * pointer;
	typedef value_type const * const_pointer;

	typedef value_type & reference;
	typedef value_type const & const_reference;

	typedef pointer iterator;
	typedef const_pointer const_iterator;

	Vec() { clear(); }

	Vec(Vec const & right) {
		for (int i = 0; i < n; ++i) { values[i] = right[i]; }
	}

	template <class U>
	explicit Vec(Vec<n, U> const & right) {
		for (int i = 0; i < n; ++i) { values[i] = static_cast<T>(right[i]); }
	}

	template <class U>
	explicit Vec(U const (&array)[n]) {
		for (int i = 0; i < n; ++i) { values[i] = static_cast<T>(array[i]); }
	}

	template <class T0>
	explicit Vec(T0 const & x,
		typename enable_if<(1 <= n), T0>::type * = 0)
	{
		clear(); 
		values[0] = x;
	}

	template <class T0, class T1>
	Vec(T0 const & x, T1 const & y,
		typename enable_if<(2 <= n), T0>::type * = 0)
	{
		clear();
		values[0] = x;
		values[1] = y;
	}

	template <class T0, class T1, class T2>
	Vec(T0 const & x, T1 const & y, T2 const & z,
		typename enable_if<(3 <= n), T0>::type * = 0)
	{
		clear();
		values[0] = x;
		values[1] = y;
		values[2] = z;
	}

	template <class T0, class T1, class T2, class T3>
	Vec(T0 const & x, T1 const & y,
		T2 const & z, T3 const & w,
		typename enable_if<(4 <= n), T0>::type * = 0)
	{
		clear();
		values[0] = x;
		values[1] = y;
		values[2] = z;
		values[3] = w;
	}

	T & operator [] (int i) { return values[i]; }
	T const & operator [] (int i) const { return values[i]; }

	T & x() { return values[0]; }
	T const & x() const { return values[0]; }

	T & y() { return values[1]; }
	T const & y() const { return values[1]; }

	T & z() { return values[2]; }
	T const & z() const { return values[2]; }

	T & w() { return values[3]; }
	T const & w() const { return values[3]; }

	iterator begin() { return values; }
	const_iterator begin() const { return values; }

	iterator end() { return values + n; }
	const_iterator end() const { return values + n; }

	bool operator == (Vec const & right) const {
		for (int i = 0; i < n; ++i) {
			if (values[i] != right[i]) { return false; }
		}
		return true;
	}

	bool operator != (Vec const & right) const {
		for (int i = 0; i < n; ++i) {
			if (values[i] == right[i]) { return false; }
		}
		return true;
	}

	Vec operator + (Vec const & right) const {
		Vec out;
		for (int i = 0; i < n; ++i) { out[i] = values[i] + right[i]; }
		return out;
	}

	Vec operator + (T const & right) const {
		Vec out;
		for (int i = 0; i < n; ++i) { out[i] = values[i] + right; }
		return out;
	}

	friend Vec operator + (T const & left, Vec const & right) {
		Vec out;
		for (int i = 0; i < n; ++i) { out[i] = left + right[i]; }
		return out;
	}

	Vec & operator += (Vec const & right) {
		for (int i = 0; i < n; ++i) { values[i] += right[i]; }
		return *this;
	}

	Vec & operator += (T const & right) {
		for (int i = 0; i < n; ++i) { values[i] += right; }
		return *this;
	}

	Vec operator - (Vec const & right) const {
		Vec out;
		for (int i = 0; i < n; ++i) { out[i] = values[i] - right[i]; }
		return out;
	}

	Vec operator - (T const & right) const {
		Vec out;
		for (int i = 0; i < n; ++i) { out[i] = values[i] - right; }
		return out;
	}

	friend Vec operator - (T const & left, Vec const & right) {
		Vec out;
		for (int i = 0; i < n; ++i) { out[i] = left - right[i]; }
		return out;
	}

	Vec & operator -= (Vec const & right) {
		for (int i = 0; i < n; ++i) { values[i] -= right[i]; }
		return *this;
	}

	Vec & operator -= (T const & right) {
		for (int i = 0; i < n; ++i) { values[i] -= right; }
	}

	Vec operator - () const {
		Vec out;
		for (int i = 0; i < n; ++i) { out[i] = -values[i]; }
		return out;
	}

	Vec operator * (T const & right) const {
		Vec out;
		for (int i = 0; i < n; ++i) { out[i] = values[i] * right; }

		return out;
	}

	friend inline Vec operator * (T const & left, Vec const & right) {
		Vec out;
		for (int i = 0; i < n; ++i) { out[i] = left * right[i]; }
		return out;
	}

	Vec & operator *= (T const & right) {
		for (int i = 0; i < n; ++i) { values[i] *= right; }
		return *this;
	}

	Vec operator * (Vec const & right) const {
		Vec out;
		for (int i = 0; i < n; ++i) { out[i] = values[i] * right[i]; }
		return out;
	}

	Vec & operator *= (Vec const & right) {
		for (int i = 0; i < n; ++i) { values[i] *= right[i]; }
		return *this;
	}

	Vec operator / (T const & right) const {
		Vec out;
		for (int i = 0; i < n; ++i) { out[i] = values[i] / right; }
		return out;
	}

	Vec & operator /= (T const & right) {
		for (int i = 0; i < n; ++i) { values[i] /= right; }
		return *this;
	}

	Vec operator / (Vec const & right) const {
		Vec out;
		for (int i = 0; i < n; ++i) { out[i] = values[i] / right[i]; }
		return out;
	}

	Vec & operator /= (Vec const & right) {
		for (int i = 0; i < n; ++i) { values[i] /= right[i]; }
		return *this;
	}

	Vec operator % (T const & right) const {
		Vec out;
		for (int i = 0; i < n; ++i) { out[i] = values[i] % right; }
		return out;
	}

	Vec & operator %= (T const & right) {
		for (int i = 0; i < n; ++i) { values[i] %= right; }
	}

	void clear() { for(int i = 0; i < n; ++i) { values[i] = 0; } }

	T values[n];
};

template <class T, int n> struct ScalarType<Vec<n, T> > { typedef T type; };

template <int n, class T, class stream>
inline stream & operator << (stream & out, Vec<n, T> const & v) {
	out << "(" << v[0];
	for (int i = 1; i < n; ++i) { out << ", " << v[i]; }
	out << ")";
	return out;
}

template <int n, class T>
inline T dot(Vec<n, T> const & left, Vec<n, T> const & right) {
	T out = 0;
	for (int i = 0; i < n; ++i) { out += left[i] * right[i]; }
	return out;
}

template <int n, class T>
inline T length_sq(Vec<n, T> const & v) {
	return dot(v, v);
}

template <int n, class T>
inline T length(Vec<n, T> const & v) {
	return sqrt(length_sq(v));
}

template <int n, class T>
inline T dist_sq(Vec<n, T> const & from, Vec<n, T> const & to) {
	return length_sq(to - from);
}

template <int n, class T>
inline T dist(Vec<n, T> const & from, Vec<n, T> const & to) {
	return length(to - from);
}

template <int n, class T>
inline Vec<n, T> normalize(Vec<n, T> const & v) {
	T l = length_sq(v);
	if (!l) { return v; }
	return v / sqrt(l);
}

template <int n, class T>
inline T scale_length_to(Vec<n, T> & v, T new_length) {
	T l = length_sq(v);
	if (!l) { return l; }

	l = sqrt(l);
	v *= (new_length / l);
	return l;
}

template <int n, class T>
inline T projection_ratio(Vec<n, T> const & v, Vec<n, T> const & onto) {
	return dot(v, onto) / length_sq(onto);
}

template <int n, class T>
inline Vec<n, T> proj_onto(Vec<n, T> const & v, Vec<n, T> const & onto) {
	return projection_ratio(v, onto) * onto;
}

template <int n, class T>
inline Vec<n, T> proj_ortho(Vec<n, T> const & v, Vec<n, T> const & offof) {
	return v - proj_onto(v, offof);
}

typedef Vec<2, float> Vec2;
typedef Vec<3, float> Vec3;
typedef Vec<4, float> Vec4;

template <class T>
inline Vec<2, T> rot_90(Vec<2, T> const & v) {
	return Vec<2, T>(-v.y(), v.x());
}

template <class T>
inline bool is_left_of(Vec<2, T> const & self, Vec<2, T> const & other) {
	return dot(self, rot_90(other)) > 0.0f;
}

template <class T>
inline bool is_left_of_or_on(Vec<2, T> const & self, Vec<2, T> const & other) {
	return dot(self, rot_90(other)) >= 0.0f;
}

template <class T>
inline T cross(Vec<2, T> const & left, Vec<2, T> const & right) {
	return left.x() * right.y() - left.y() * right.x();
}

template <class T>
inline Vec<3, T> cross(Vec<3, T> const & left, Vec<3, T> const & right) {
	return Vec<3, T>(
		left.y() * right.z() - left.z() * right.y(),
		- (left.x() * right.z() - left.z() * right.x()),
		left.x() * right.y() - left.y() * right.x()
	);
}

template <int n>
inline Vec<n, int> fdiv(Vec<n, int> const & left, int const & right) {
	Vec<n, int> out;
	for (int i = 0; i < n; ++i) { out[i] = fdiv(left[i], right); }
	return out;
}

inline Vec4 rhw_position(Vec2 const & v) { return Vec4(v.x(), v.y(), 1.0f, 1.0f); }

inline Vec4 rhw_position(Vec3 const & v) { return Vec4(v.x(), v.y(), v.z(), 1.0f); }

inline Vec4 const & rhw_position(Vec4 const & v) { return v; }

template <int n, class T, class U>
inline Vec<n, T> min(Vec<n, T> const & left, Vec<n, U> const & right) {
	Vec<n, T> out;
	for (int i = 0; i < n; ++i) { out[i] = right[i] < left[i] ? right[i] : left[i]; }
	return out;
}

template <int n, class T, class U>
inline Vec<n, T> max(Vec<n, T> const & left, Vec<n, U> const & right) {
	Vec<n, T> out;
	for (int i = 0; i < n; ++i) { out[i] = right[i] < left[i] ? left[i] : right[i]; }
	return out;
}
