#pragma once

#include <memory>
#include <algorithm>
#include <renderer/RenderingEngine.hpp>

class ApplicationEngine {
	public:
		static ApplicationEngine * constuct() {
			std::auto_ptr<ApplicationEngine> ptr(new ApplicationEngine());
			if (!ptr->init()) { return 0; }
			return ptr.release();
		}

		~ApplicationEngine();

		void set_rendering_engine(RenderingEngine * rendering_engine) {
			std::swap(rendering_engine_, rendering_engine);
			delete rendering_engine;
		}

		void render_frame();

		void zoom_canvas(float scale) {
			printf("scale: %f\n", scale);
		}
	private:
		ApplicationEngine() : rendering_engine_(0), current_zoom_level_(1) { };

		bool init();

		RenderingEngine * rendering_engine_;
		float current_zoom_level_;
};
