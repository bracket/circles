#pragma once

#include <cmath>
#include <math/MatrixOps.hpp>

class Camera {
	public:
		Camera() :
			near_clip_(1.0f),
			far_clip_(10.0f),
			aspect_ratio_(4.0f/3.0f),
			field_of_view_(static_cast<float>(2 * std::atan(1)))
		{ }

		Camera(float near_clip, float far_clip, float aspect_ratio, float field_of_view) {
			set_projection_matrix(near_clip, far_clip, aspect_ratio, field_of_view);
		}

		Matrix<4, 4, float> const & get_projection_matrix() const {
			return projection_matrix_;
		}

		void set_projection_matrix(float near_clip, float far_clip, float aspect_ratio, float field_of_view) {
			near_clip_ = near_clip;
			far_clip_ = far_clip;
			aspect_ratio_ = aspect_ratio;
			field_of_view_ = field_of_view;

			calc_projection_matrix();
		}

		Matrix<4, 4, float> const & get_camera_matrix() const {
			return camera_matrix_;
		}

		void set_camera_matrix(Matrix<4, 4, float> const & matrix) {
			camera_matrix_ = camera_inverse_ = matrix;
			invert_matrix(&camera_inverse_);
			camera_projection_matrix_ = camera_inverse_ * projection_matrix_;
		}

		Matrix<4, 4, float> const & get_camera_inverse() const {
			return camera_inverse_;
		}

		Vec2 project_to_normalized(Vec4 v) const {
			v = v * camera_projection_matrix_;
			return Vec2(v.x() / v.w(), v.y() / v.w());
		}

		Vec2 project_to_device_independent(Vec4 v) const {
			float height = std::tan(field_of_view_ / 2);

			v = v * camera_inverse_;
			Vec2 out = Vec2(v.x(), v.y()) / (-v.z() * v.w() * height);
            
			return out;
		}

		Ray<3, float> unproject_device_independent(Vec2 const & v) const {
			float height = std::tan(field_of_view_ / 2);
			Vec4 start4 = Vec4(0, 0, 0, 1) * camera_matrix_,
				end4 = Vec4(v.x() * height, v.y() * height, -1, 1) * camera_matrix_;

			Vec3 start = Vec3(start4.x(), start4.y(), start4.z()) / start4.w(),
				end = Vec3(end4.x(), end4.y(), end4.z()) / end4.w();

			return Ray<3, float>(start, end - start);
		}

		float get_near_clip() const {
			return near_clip_;
		}

		float get_far_clip() const {
			return far_clip_;
		}

		float get_field_of_view() const {
			return field_of_view_;
		}

	private:
		float near_clip_, far_clip_;
		float aspect_ratio_;
		float field_of_view_;
		Matrix<4, 4, float> projection_matrix_, camera_matrix_, camera_inverse_, camera_projection_matrix_;

		void calc_projection_matrix() {
			projection_matrix_ = perspective(near_clip_, far_clip_, aspect_ratio_, field_of_view_);
			camera_projection_matrix_ = camera_inverse_ * projection_matrix_;
		}

};
