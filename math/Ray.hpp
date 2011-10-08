#pragma once

#include <math/Vec.hpp>

template <int n, class T>
class Ray {
	typedef Vec<n, T> VecType;

	public:
		Ray(VecType const & start, VecType const & dir)
			: start_(start), dir_(normalize(dir)) { }

		void set_start(VecType const & start) { start_ = start; }
		VecType const & get_start() const { return start_; }

		void set_dir(VecType const & dir) { dir_ = normalize(dir); }
		VecType const & get_dir() const { return dir_; }

		VecType operator () (T const & t) const { return start_ + t * dir_; }

	private:
		VecType start_, dir_;
};
