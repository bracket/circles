#pragma once

#include <boost/range.hpp>
#include <cmath>
#include <math/Matrix.hpp>
#include <math/Vec.hpp>

template <class T>
Matrix<4, 4, T> frustum(T left, T right, T bottom, T top, T near, T far) {
	T a = 2 * near / (right - left),
		b = 2 * near / (top - bottom),
		c = (right + left) / (right - left),
		d = (top + bottom) / (top - bottom),
		e = - (far + near) / (far - near),
		f = -2 * far * near / (far - near);

	T out[] = {
		a, 0, 0,  0,
		0, b, 0,  0,
		c, d, e, -1,
		0, 0, f,  0
	};
	return Matrix<4, 4, T>(boost::begin(out), boost::end(out));
}

template <class T>
Matrix<4, 4, T> ortho(T max_x, T max_y) {
	T a = 1.0f / max_x;
	T b = 1.0f / max_y;

	T out[] = {
		a, 0,  0, 0, 
		0, b,  0, 0,
		0, 0, -1, 0,
		0, 0,  0, 1
	};
	return Matrix<4, 4, T>(boost::begin(out), boost::end(out));
}

template <class T>
Matrix<4, 4, T> identity_matrix() {
	T out[] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	return Matrix<4, 4, T>(boost::begin(out), boost::end(out));
}

template <class T>
Matrix<4, 4, T> perspective(T near, T far, T aspect_ratio, T field_of_view) {
	T height = std::tan(field_of_view / 2) * near,
		width = aspect_ratio * height;

	return frustum(-width, width, -height, height, near, far);
}

template <class T>
Matrix<4, 4, T> rotate_around_z(T angle) {
	T s = std::sin(angle);
	T c = std::cos(angle);

	T out[] = {
		 c, s, 0, 0,
		-s, c, 0, 0,
		 0, 0, 1, 0,
		 0, 0, 0, 1
	};

	return Matrix<4, 4, T>(boost::begin(out), boost::end(out));
}

template <class T>
Matrix<4, 4, T> translate_matrix(Vec<3, T> const & position) {
	T values[] = {
		1,             0,             0,             0, 
		0,             1,             0,             0, 
		0,             0,             1,             0, 
		position.x(),  position.y(),  position.z(),  1
	};

	return Matrix<4, 4, T>(values);
}
