#pragma once

#include <algorithm>
#include <boost/range.hpp>
#include <math.h>
#include <math/Vec.hpp>
#include <utility>

template <int rows_, int cols_, class T = float>
struct Matrix {
	static const int rows = rows_;
	static const int cols = cols_;
	static const int size = rows * cols;

	typedef T value_type;

	typedef T & reference;
	typedef T const & const_reference;

	typedef T * pointer;
	typedef T const * const_pointer;

	typedef T row_type[cols];
	typedef row_type storage_type[rows];

	typedef pointer iterator;
	typedef const_pointer const_iterator;

	typedef std::pair<iterator, iterator> range_type;
	typedef std::pair<const_iterator, const_iterator> const_range_type;

	Matrix() {
		iterator end = this->end();
		for (iterator it = this->begin(); it != end; *it++ = 0) { ; }
	}

	template <class InIt>
	Matrix(InIt it, InIt end) {
		assign(it, end);
	}

	template <class U>
	Matrix(U (&array)[rows * cols]) {
		assign(boost::begin(array), boost::end(array));
	}

	template <class R>
	Matrix(R (&array)[rows]) {
		iterator out = begin();
		for (int i = 0; i < rows; ++i) {
			out = std::copy(boost::begin(array[i]), boost::end(array[i]), out);
		}
	}

	row_type & operator [] (int i) { return data_[i]; }
	row_type const & operator [] (int i) const { return data_[i]; }

	reference operator () (int i, int j) { return data_[i][j]; }
	const_reference operator () (int i, int j) const { return data_[i][j]; }

	range_type row_range(int i) { return range_type(data_[i], data_[i] + cols); }
	const_range_type row_range(int i) const { return const_range_type(data_[i], data_[i] + cols); }

	range_type column_range(int j) { return range_type(data_[0] + j, data_[rows] + j); }
	const_range_type column_range(int j) const { return const_range_type(data_[0] + j, data_[rows] + j); }

	iterator begin() { return data_[0]; }
	const_iterator begin() const { return data_[0]; }

	iterator end() { return data_[rows]; }
	const_iterator end() const { return data_[rows]; }

	template <class It>
	void assign(It it, It end) {
		iterator out_end = this->end();
		for (iterator out = begin(); out != out_end && it != end; *out++ = *it++) { ; }
	}

	template <class U>
	Vec<rows, T> operator * (Vec<cols, U> const & v) const {
		Vec<rows, T> out;
		for (int i = 0; i < rows; ++i) {
			out[i] = dot(row_range(i), 1, std::make_pair(v.begin(), v.end()), 1);
		}

		return out;
	}

	template <class U>
	friend Vec<cols, T> operator * (Vec<rows, U> const & v, Matrix const & M) {
		Vec<cols, T> out;
		for (int i = 0; i < cols; ++i) {
			out[i] = M.dot(std::make_pair(v.begin(), v.end()), 1, M.column_range(i), cols);
		}

		return out;
	}

	template <int c, class U>
	Matrix<rows, c, T> operator * (Matrix<cols, c, U> const & right) const {
		Matrix<rows, c> out;
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < right.cols; ++j) {
				out.data_[i][j] = dot(row_range(i), 1, right.column_range(j), right.cols);
			}
		}
		return out;
	}

	template <class U>
	Matrix & operator *= (Matrix<cols, cols, U> const & right) {
		*this = (*this) * right;
		return *this;
	}

	template <class stream>
	friend stream & operator << (stream & out, Matrix const & m) {
		out << "{ ";
		for (int i = 0; i < m.rows; ++i) {
			if (i) { out << ", "; }

			out << "{ ";
			for (int j = 0; j < m.cols; ++j) {
				if (j) { out << ", "; }
				out << m.data_[i][j];
			}
			out << " }";
		}
		out << " }";
		return out;
	}

	private:
		template <class L, class R>
		static inline T dot(L left, int stride_left, R right, int stride_right) {
			T out = 0;
			
			while (left.first < left.second && right.first < right.second) {
				out += *left.first * *right.first;
				left.first += stride_left;
				right.first += stride_right;
			}
			
			return out;
		}

		storage_type data_;
};

namespace detail {
	namespace MatrixInverter {
		int abs(int x) { return ::abs(x); }
		float abs(float x) { return fabsf(x); }
		double abs(double x) { return fabs(x); }
		long double abs(long double x) { return fabsl(x); }

		template <class T, int N>
		inline void swap(T (&left)[N], T (&right)[N]) {
			float *l = left, *r = right;
			for (int i = 0; i < N; ++i) { std::swap(*l++, *r++); }
		}

		template <class T>
		void div(T * begin, T * end, T d) {
			for (; begin != end; ++begin) { *begin /= d; }
		}

		template <class T>
		T max_abs(T * begin, T * end) {
			T m = 0;
			for (T * it = begin; it < end; ++it) {
				T v = abs(*it);
				if (it != begin && v <= m) { continue; }
				m = v;
			}

			return m;
		}

		template <class T>
		int find_pivot_row(T * begin, T * end, int stride) {
			T m = 0; int p = 0, i = 0;
			for (T * it = begin; it < end; ++i, it += stride) {
				T v = abs(*it); 
				if (i && v <= m) { continue; }
				m = v; p = i;
			}

			return p;
		}

		template <class T>
		void normalize_front(T * begin, T * end) {
			float d = *begin; *begin = 1; ++begin;
			while (begin < end) { *begin++ /= d; }
		}

		template <class T>
		void scale_and_subtract(T * begin, T * end, T * out) {
			T s = -*out / *begin++; *out++ = 0;
			while (begin < end) { *out++ += s * *begin++; }
		}

		template <class T, int N>
		void invert_matrix(Matrix<N, N, T> * M) {
			T S[N][2 * N];

			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < N; ++j) {
					S[i][j] = (*M)[i][j];
					S[i][j + N] = (i == j);
				}
			}

			for (int i = 0; i < N; ++i) { div(S[i], S[i] + 2 * N, max_abs(S[i], S[i] + N)); }

			for (int i = 0; i < N; ++i) {
				int pivot = find_pivot_row(S[i] + i, S[N] + i, 2 * N);
				if (pivot) { swap(S[i], S[i + pivot]); }

				normalize_front(S[i] + i, S[i] + 2 * N);
				for (int j = i + 1; j < N; ++j) { scale_and_subtract(S[i] + i, S[i] + 2 * N, S[j] + i); }
			}

			for (int i = N - 1; i > 0; --i) {
				for (int j = i - 1; j >= 0; --j) { scale_and_subtract(S[i] + i, S[i] + 2 * N, S[j] + i); }
			}

			for (int i = 0; i < N; ++i) {
				for (int j = 0; j < N; ++j) { (*M)[i][j] = S[i][j + N]; }
			}
		}
	}
}

using detail::MatrixInverter::invert_matrix;
