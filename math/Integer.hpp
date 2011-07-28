#pragma once

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

//! @brief floored division - integer division with result truncated towards -\infty
inline int fdiv(int const & m, int const & n) {
	int q = m / n, r = m - q * n;
	return r < 0 ? (n < 0 ? ++q : --q ) : q;
}

//! @brief floored modulus - returns modulus from floored division
inline int fmod(int const & m, int const & n) {
	int q = m / n, r = m - q * n;
	return r < 0 ? (n < 0 ? r - n : r + n) : r;
}

//! @brief ceilinged division - division with result truncated towards +\infty
inline int cdiv(int const & m, int const & n) {
	int q = m / n, r = m - q * n;
	return r > 0 ? (n < 0 ? --q : ++q) : q;
}

//! @brief ceilinged modulus - returns modulus from ceilinged division
inline int cmod(int const & m, int const & n) {
	int q = m / n, r = m - q * n;
	return r > 0 ? (n < 0 ? r + n : r - n) : r;
}
