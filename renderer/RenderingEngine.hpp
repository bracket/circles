#pragma once

#include <boost/unordered_map.hpp>
#include <math/Matrix.hpp>
#include <math/Vec.hpp>
#include <memory>
#include <renderer/Program.hpp>
#include <renderer/Renderable.hpp>
#include <vector>

class RenderingEngine {
	public:
		typedef std::vector<Renderable*> RenderList;
		typedef boost::unordered_map<Program*, RenderList> RenderMap;

		static RenderingEngine * construct(float width, float height) {
			std::auto_ptr<RenderingEngine> ptr(new RenderingEngine(width, height));
			if (!ptr->init()) { return 0; }
			return ptr.release();
		}

		void submit(Renderable * renderable) {
			Program * program = renderable->get_program();
			render_map_[program].push_back(renderable);
		}

		void render();

		float get_view_width() const { return view_width_; }
		float get_view_height() const { return view_height_; }

		Matrix<4, 4, float> const & get_projection_matrix() const {
			return projection_matrix_;
		}

		void set_projection_matrix(Matrix<4, 4, float> const & matrix) {
			projection_matrix_ = matrix;
			camera_projection_matrix_ = camera_matrix_ * projection_matrix_;
		}

		Matrix<4, 4, float> const & get_camera_matrix() const {
			return camera_matrix_;
		}

		void set_camera_matrix(Matrix<4, 4, float> const & matrix) {
			camera_matrix_ = matrix;
			camera_projection_matrix_ = camera_matrix_ * projection_matrix_;
		}

		Vec2 project_to_normalized(Vec4 v) const {
			v = v * camera_projection_matrix_;
			return Vec2(v.x() / v.w(), v.y() / v.w());
		}

		Vec2 project_to_screen(Vec4 const & v) const {
			return (Vec2(.5f, .5f) + project_to_normalized(v) / 2.0f)
				* Vec2(view_width_, view_height_);
		}

	private:
		RenderingEngine(float view_width, float view_height) :
			view_width_(view_width),
			view_height_(view_height)
		{ }
		
		bool init();

		RenderMap render_map_;
		float view_width_, view_height_;

		Matrix<4, 4, float> projection_matrix_, camera_matrix_, camera_projection_matrix_;
};
