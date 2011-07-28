#pragma once

#include <algorithm>
#include <utility>
#include <math/Vec.hpp>

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

	row_type & operator [] (int i) { return data_[i]; }
	row_type const & operator [] (int i) const { return data_[i]; }

	reference operator () (int i, int j) { return data_[i][j]; }
	const_reference operator () (int i, int j) const { return data_[i][j]; }

	range_type row_range(int i) { return range_type(data_[i], data_[i] + cols); }
	const_range_type row_range(int i) const { return range_type(data_[i], data_[i] + cols); }

	range_type column_range(int j) { return range_type(data_[0] + j, data_[rows] + j); }
	const_range_type column_range(int j) const { return range_type(data_[0] + j, data_[rows] + j); }

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
			out[i] = dot(data_[i], v);
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
		template <class R>
		static inline T dot(R const & left, int stride_left, R const & right, int stride_right) {
			T out = 0;
			iterator l = left.first, r = right.first;
			for (; l < left.second && r < right.second; l += stride_left, r += stride_right) 
				{ out += *l * *r; }
			return out;
		}

		template <class U, int n>
		static inline T dot(row_type const & row, Vec<n, U> const & v) {
			T out = 0;
			const_pointer it = row + 0, end = row + cols;
			for (int i = 0; i < n && it != end; ++i, ++it) {
				out += *it * v[i];
			}

			return out;
		}

		storage_type data_;
};
