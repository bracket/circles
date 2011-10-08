#include <algorithm>
#include <app_engine/ApplicationEngine.hpp>
#include <arch/common.hpp>
#include <machines/MachineFactory.hpp>
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
	typedef RenderableContainer::const_iterator iterator;

	for (iterator it = renderables_.begin(); it != renderables_.end(); ++it)
		{ rendering_engine_->submit(*it); }

	rendering_engine_->render();
}

void ApplicationEngine::post_systems_init() {
	Machine * output_machine = MachineFactory::get().construct("OutputMachine");

	float T[] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		2.5, 3.0, 0.0
	};
	output_machine->get_renderable()->set_frame(T);
	register_touchable(output_machine->get_touchable());
	register_renderable(output_machine->get_renderable());

	std::cout << "output frame: " << output_machine->get_renderable()->get_frame() << std::endl;

	machines_.register_output_machine(output_machine);
}

void ApplicationEngine::processing_loop_step() {
	static Template * t = create_test_template();
	if (command_queue_) { command_queue_->process_responses(); }
}
