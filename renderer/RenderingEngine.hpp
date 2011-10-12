#pragma once

#include <boost/unordered_map.hpp>
#include <math/Matrix.hpp>
#include <math/Vec.hpp>
#include <math/Ray.hpp>
#include <memory>
#include <renderer/Camera.hpp>
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

		Vec2 project_to_screen(Vec4 const & v) const {
			return (Vec2(.5f, .5f) + camera_.project_to_normalized(v) / 2.0f)
				* Vec2(view_width_, view_height_);
		}

		Camera const & get_camera() const { return camera_; }
		void set_camera(Camera const & camera) { camera_ = camera; }


	private:
		RenderingEngine(float view_width, float view_height) :
			view_width_(view_width),
			view_height_(view_height)
		{ }
		
		bool init();

		Camera camera_;

		RenderMap render_map_;
		float view_width_, view_height_;
};
