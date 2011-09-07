#include <algorithm>
#include <app_engine/ApplicationEngine.hpp>
#include <arch/common.hpp>
#include <math/Rectangle.hpp>
#include <renderer/Shader.hpp>
#include <shared/Ticker.hpp>

#include <iostream>

#include <math/MatrixOps.hpp>
#include <OpenGLES/ES2/gl.h>
#include <machines/templates/Template.hpp>
#include <renderer/Renderable.hpp>
#include <renderer/RenderingEngine.hpp>
#include <vector>

#include <shared/Ticker.hpp>

#include <machine_graph/commands/MachineGraphCommands.hpp>

boost::shared_ptr<ApplicationEngine> ApplicationEngine::app_engine_;

Template * create_test_template();

bool ApplicationEngine::init() {
	touch_handler_ = new TouchHandler(this);
	return true;
}

ApplicationEngine::~ApplicationEngine() {
	delete rendering_engine_;
	delete command_queue_;
	delete touch_handler_;
}

void ApplicationEngine::render_frame() {
	typedef MachineContainer::const_iterator iterator;

	for (iterator it = machines_.begin(); it != machines_.end(); ++it) {
		rendering_engine_->submit(it->second->get_renderable());
	}

	rendering_engine_->render();
}

void ApplicationEngine::processing_loop_step() {
	static Template * t = create_test_template();
	if (command_queue_) { command_queue_->process_responses(); }
}
