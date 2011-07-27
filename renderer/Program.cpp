#include <OpenGLES/ES2/gl.h>
#include <renderer/Program.hpp>

Program::~Program() {
	if (name_) { glDeleteProgram(name_); }
	delete vertex_shader_;
	delete fragment_shader_;
}

bool Program::init(Shader * vertex, Shader * fragment) {
	if (!vertex) { return false; }
	if (!fragment) { return false; }

	name_ = glCreateProgram();
	if (!name_) { return false; }

	glAttachShader(name_, vertex->get_name());
	glAttachShader(name_, fragment->get_name());

	glLinkProgram(name_);

	GLint status;
	glGetProgramiv(name_, GL_LINK_STATUS, &status);
	
	if (!status) { return false; }

	vertex_shader_ = vertex;
	fragment_shader_ = fragment;

	return true;
}

void Program::bind() const {
	glUseProgram(name_);
}
