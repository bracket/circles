#include <arch/common.hpp>
#include <cmath>
#include <OpenGLES/ES2/gl.h>
#include <math/MatrixOps.hpp>
#include <renderer/RenderingEngine.hpp>
#include <renderer/Shader.hpp>
#include <shared/Ticker.hpp>
#include <renderer/Circle.hpp>

bool RenderingEngine::init() {
	glViewport(0, 0, view_width_, view_height_);

	set_projection_matrix(perspective(
		1.0f, 10.0f,
		view_width_ / view_height_,
		static_cast<float>(2 * std::atan(1))
	));
	set_camera_matrix(identity_matrix<float>());

	return true;
}

namespace {
	Program * program = 0;

	Program * initialize_program() {
		std::string vertex_shader_source = get_file_contents("test", "vsh");
		std::auto_ptr<Shader> vertex_shader(Shader::construct(Shader::VertexShader, vertex_shader_source));
		if (!vertex_shader.get()) { return false; }

		std::string fragment_shader_source = get_file_contents("test", "fsh");
		std::auto_ptr<Shader> fragment_shader(Shader::construct(Shader::FragmentShader, fragment_shader_source));
		if (!fragment_shader.get()) { return false; }

		std::auto_ptr<Program> program(Program::construct(vertex_shader.get(), fragment_shader.get()));
		if (!program.get()) { return false; }

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

void RenderingEngine::render() {
	typedef RenderMap::iterator program_iterator;
	typedef RenderList::iterator iterator;

	Circle * circle = get_circle();
	submit(circle);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (program_iterator pit = render_map_.begin(); pit != render_map_.end(); ++pit) {
		pit->first->bind();
		pit->first->bind_uniform("projection", projection_matrix_);

		iterator it = pit->second.begin(), end = pit->second.end();
		for (; it != end; ++it) { (*it)->render(this); }
		pit->second.clear();
	}
}

void RenderingEngine::zoom_canvas(float scale) {
	current_zoom_level_ *= scale;

	if (current_zoom_level_ < 0.01f) { current_zoom_level_ = 0.01f; }
}
