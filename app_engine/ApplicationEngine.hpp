#pragma once

#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <input/TouchHandler.hpp>
#include <machines/MachineContainer.hpp>
#include <math/Vec.hpp>
#include <memory>
#include <renderer/RenderingEngine.hpp>
#include <set>
#include <shared/CommandQueue.hpp>

class ApplicationEngine {
	typedef std::set<Renderable *> RenderableContainer;

	public:
		static ApplicationEngine * get() { return app_engine_.get(); }

		static ApplicationEngine * construct() {
			if (app_engine_) { return app_engine_.get(); }

			std::auto_ptr<ApplicationEngine> ptr(new ApplicationEngine());
			if (!ptr->init()) { return 0; }
			app_engine_ = ptr;
			return app_engine_.get();
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

		void push_command(MachineCommand * command) { command_queue_->push(command); }

		void post_systems_init();
		void processing_loop_step();

		MachineID register_machine(Machine * machine) {
			register_touchable(machine->get_touchable());
			register_renderable(machine->get_renderable());

			return machines_.register_machine(machine);
		}

		bool delete_machine(MachineID id) {
			Machine * machine = machines_.find_machine(id);

			if (!machine) { return false; }

			erase_touchable(machine->get_touchable());
			erase_renderable(machine->get_renderable());

			return machines_.delete_machine(id);
		}

		void register_touchable(Touchable * touchable, bool is_owned_by_touch_handler=false) {
			if (!touchable) { return; }
			touch_handler_->insert_touchable(touchable, is_owned_by_touch_handler);
		}

		void erase_touchable(Touchable * touchable) {
			touch_handler_->erase_touchable(touchable);
		}

		void register_renderable(Renderable * renderable) {
			if (!renderable) { return; }
			renderables_.insert(renderable);
		}

		void erase_renderable(Renderable * renderable) {
			renderables_.erase(renderable);
		}

	private:
		static boost::shared_ptr<ApplicationEngine> app_engine_;

		ApplicationEngine() :
			rendering_engine_(0), command_queue_(0), current_zoom_level_(1)
		{ };

		bool init();

		RenderingEngine * rendering_engine_;
		CommandQueue * command_queue_;
		TouchHandler * touch_handler_;

		MachineContainer machines_;
		RenderableContainer renderables_;

		float current_zoom_level_;
};
