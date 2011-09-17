#pragma once

#include <math/Vec.hpp>

template <int n, class T>
class Plane {
	public:
		typedef Vec<n, T> VecType;

		Plane(VecType const & normal, T const & dist) :
			normal_(normal), dist_(dist) { }

		Plane(VecType const & position, VecType const & normal) :
			normal_(normal),
			dist_(dot(position, normal))
		{ }

		bool contains(VecType const & v, T const & tolerance = 0) const {
			T diff = dot(v, normal_) - dist_;
			return tolerance <= diff && diff <= tolerance;
		}

		VecType const & get_normal() const { return normal_; }
		T const & get_dist() const { return dist_; }

	private:
		VecType normal_;
		T dist_;
};
