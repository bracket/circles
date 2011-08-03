#include <app_engine/ApplicationEngine.hpp>
#include <arch/common.hpp>
#include <renderer/Circle.hpp>
#include <renderer/Shader.hpp>
#include <shared/Ticker.hpp>

bool ApplicationEngine::init() {
	return true;
}

ApplicationEngine::~ApplicationEngine() {
	delete rendering_engine_;
}

namespace {
	Program * program = 0;

	Program * initialize_program() {
		std::string vertex_shader_source = get_file_contents("test", "vsh");
		std::auto_ptr<Shader> vertex_shader(Shader::construct(Shader::VertexShader, vertex_shader_source));
		if (!vertex_shader.get()) { return 0; }

		std::string fragment_shader_source = get_file_contents("test", "fsh");
		std::auto_ptr<Shader> fragment_shader(Shader::construct(Shader::FragmentShader, fragment_shader_source));
		if (!fragment_shader.get()) { return 0; }

		std::auto_ptr<Program> program(Program::construct(vertex_shader.get(), fragment_shader.get()));
		if (!program.get()) { return 0; }

		vertex_shader.release();
		fragment_shader.release();

		return program.release();
	}

	Circle * get_circle() {
		if (!program) { program = initialize_program(); }

		static Circle * circle = new Circle(program);

		return circle;
	}
}

void ApplicationEngine::render_frame() {
	Circle * circle = get_circle();
	rendering_engine_->submit(circle);

	rendering_engine_->render();
}
