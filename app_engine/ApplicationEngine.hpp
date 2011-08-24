#pragma once

#include <algorithm>
#include <math/Vec.hpp>
#include <memory>
#include <renderer/RenderingEngine.hpp>
#include <input/TouchHandler.hpp>
#include <app_engine/CommandQueue.hpp>

class ApplicationEngine {
	public:
		static ApplicationEngine * construct() {
			std::auto_ptr<ApplicationEngine> ptr(new ApplicationEngine());
			if (!ptr->init()) { return 0; }
			return ptr.release();
		}

		~ApplicationEngine();

		RenderingEngine * get_rendering_engine() const {
			return rendering_engine_;
		}

		void set_rendering_engine(RenderingEngine * rendering_engine) {
			std::swap(rendering_engine_, rendering_engine);
			delete rendering_engine;
		}

		void set_command_queue(CommandQueue * command_queue) {
			std::swap(command_queue_, command_queue);
			delete command_queue;
		}

		TouchHandler * get_touch_handler() const {
			return touch_handler_;
		}

		void set_touch_handler(TouchHandler * touch_handler) {
			std::swap(touch_handler_, touch_handler);
			delete touch_handler;
		}

		void render_frame();

	private:
		ApplicationEngine() :
			rendering_engine_(0), command_queue_(0), current_zoom_level_(1)
		{ };

		bool init();

		RenderingEngine * rendering_engine_;
		CommandQueue * command_queue_;
		TouchHandler * touch_handler_;

		float current_zoom_level_;
};
