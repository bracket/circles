#pragma once

#include <boost/unordered_map.hpp>
#include <math/Matrix.hpp>
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

		Matrix<4, 4, float> const & get_projection_matrix() const {
			return projection_matrix_;
		}

		void set_projection_matrix(Matrix<4, 4, float> const & matrix) {
			projection_matrix_ = matrix;
		}

		Matrix<4, 4, float> const & get_camera_matrix() const {
			return camera_matrix_;
		}

		void set_camera_matrix(Matrix<4, 4, float> const & matrix) {
			camera_matrix_ = matrix;
		}

	private:
		RenderingEngine(float view_width, float view_height) :
			view_width_(view_width),
			view_height_(view_height)
		{ }
		
		bool init();

		RenderMap render_map_;
		float view_width_, view_height_;

		Matrix<4, 4, float> projection_matrix_, camera_matrix_;
};
