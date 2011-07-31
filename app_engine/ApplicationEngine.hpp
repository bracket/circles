#pragma once

#include <memory>

class RenderingEngine;

class ApplicationEngine {
	public:
		static ApplicationEngine * constuct() {
			std::auto_ptr<ApplicationEngine> ptr(new ApplicationEngine());
			if (!ptr->init()) { return 0; }
			return ptr.release();
		}

		~ApplicationEngine();

		void set_rendering_engine(RenderingEngine * rendering_engine) {
			rendering_engine_ = rendering_engine;
		}

		void zoom_canvas(float scale) {
			printf("scale: %f\n", scale);
		}
	private:
		ApplicationEngine() : current_zoom_level_(1) { };

		bool init();

		RenderingEngine * rendering_engine_;
		float current_zoom_level_;
};
