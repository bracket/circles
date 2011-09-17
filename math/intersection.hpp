#pragma once

#include <boost/optional.hpp>
#include <math/Plane.hpp>
#include <math/Ray.hpp>
#include <math/Vec.hpp>

template <int n, class T>
inline boost::optional<Vec<n, T> > intersection(Ray<n, T> const & ray, Plane<n, T> const & plane) {
	T denom = dot(ray.get_dir(), plane.get_normal());
	if (denom == 0) {
        if (plane.contains(ray.get_start())) { return ray.get_start(); }
        else { return boost::none; }
    }

	T t = (plane.get_dist() - dot(ray.get_start(), plane.get_normal())) / denom;
    if (t < 0) { return boost::none; }
	return ray(t);
}

template <int n, class T>
inline boost::optional<Vec<n, T> > intersection(Plane<n, T> const & plane, Ray<n, T> const & ray) {
	return intersection(ray, plane);
}
