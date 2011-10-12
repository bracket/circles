#pragma once

#include <boost/range.hpp>
#include <math/Vec.hpp>
#include <OpenGLES/ES2/gl.h>
#include <renderer/Camera.hpp>
#include <renderer/Renderable.hpp>

struct Vertex {
	GLfloat pos[3];
	GLfloat tex[2];

	Vec4 get_pos() const { return Vec4(pos[0], pos[1], pos[2], 1.0f); }
};

namespace detail {
	struct ApplyTransformAndProject {
		ApplyTransformAndProject(Camera const * camera, Matrix<4, 4, float> * M) :
			camera_(camera), M_(M) { }

		Vec2 operator () (Vertex const & v) const {
			return camera_->project_to_device_independent(v.get_pos() *  *M_);
		}

		private:
			Camera const * camera_;
			Matrix<4, 4, float> * M_;
	};
}

template <class Iterator>
inline Rectangle<float> get_bounding_rectangle(Camera const & camera, Renderable::FrameType const & f, Iterator begin, Iterator end) {
	float F[] = {
		f[0][0], f[0][1], f[0][2], 0,
		f[1][0], f[1][1], f[1][2], 0,
			  0,       0,       1, 0,
		f[2][0], f[2][1], f[2][2], 1
	};

	Matrix<4, 4, float> M(F);

	return get_bounding_rectangle(begin, end, detail::ApplyTransformAndProject(&camera, &M));
}

template <class Container>
inline Rectangle<float> get_bounding_rectangle(Camera const & camera, Renderable::FrameType const & f, Container const & container) {
	return get_bounding_rectangle(camera, f, boost::begin(container), boost::end(container));
}
