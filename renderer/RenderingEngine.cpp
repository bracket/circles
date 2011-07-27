#include <arch/common.hpp>
#include <cmath>
#include <OpenGLES/ES2/gl.h>
#include <renderer/RenderingEngine.hpp>
#include <renderer/Shader.hpp>
#include <shared/Ticker.hpp>

bool RenderingEngine::init() {
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

	return true;
}

void RenderingEngine::render() {
	typedef RenderMap::iterator program_iterator;
	typedef RenderList::iterator iterator;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (program_iterator pit = render_map_.begin(); pit != render_map_.end(); ++pit) {
		iterator it = pit->second.begin(), end = pit->second.end();
		for (; it != end; ++it) { (*it)->render(); }
		pit->second.clear();
	}
}
