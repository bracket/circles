#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MathTest
#include <boost/test/unit_test.hpp>

#include <math/Vec.hpp>
#include <math/MatrixOps.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE(TEST_MATRIX_MULTIPLY) {
	float m[]= {
		1 , 2 , 3 , 4 , 
		5 , 6 , 7 , 8 , 
		9 , 10, 11, 12, 
		13, 14, 15, 16
	};

	Matrix<4,4,float> M(m);
	Vec4 v(1, 1, 1, 1);

	BOOST_CHECK(M * v == Vec4(10, 26, 42, 58));
	BOOST_CHECK(v * M == Vec4(28, 32, 36, 40));
}

BOOST_AUTO_TEST_CASE(TEST_MATRIX_INVERSE) {
	float F[][3] = {
		{ 1, 2,  4 },
		{ 1, 3,  9 },
		{ 1, 4, 16 },
	};
	
	Matrix<3, 3, float> M(F), N = M;
	invert_matrix(&N);

	Matrix<3, 3, float> X = N * M, Y = M * N;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			BOOST_CHECK(fabsf(static_cast<float>(i == j) - X[i][j]) < 1e-5);
			BOOST_CHECK(fabsf(static_cast<float>(i == j) - Y[i][j]) < 1e-5);
		}
	}
}

BOOST_AUTO_TEST_CASE(TEST_FRUSTUM_CALC) {
	Matrix<4, 4, float> M = frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 2.0f);

	Vec4 v(-1.5f, -1.5f, -1.5f, 1.0f);
	v = v * M;
	v /= v.w();

	BOOST_CHECK(v.x() == -1.0f);
	BOOST_CHECK(v.y() == -1.0f);
}

