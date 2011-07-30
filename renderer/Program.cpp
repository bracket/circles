#include <OpenGLES/ES2/gl.h>
#include <renderer/Program.hpp>
#include <vector>

#include <math/MatrixOps.hpp>

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

	GLint projection_uniform = glGetUniformLocation(name_, "projection");

	Matrix<4, 4, float> projection_matrix = frustum(-1.6f, 1.6f, -2.4f, 2.4f, 1.0f, 10.0f);
	glUniformMatrix4fv(projection_uniform, 1, 0, projection_matrix.begin());

	Matrix<4, 4, float> rotation = identity_matrix<float>();

	GLint model_view_uniform = glGetUniformLocation(name_, "model_view");
	glUniformMatrix4fv(model_view_uniform, 1, 0, rotation.begin());
}

bool Program::bind_uniform(char const * name, float f) {
	GLint loc = glGetUniformLocation(name_, name);

	if (loc == -1) { return false; }

	glUniform1f(loc, f);

	return true;
}

bool Program::bind_uniform(char const * name, Matrix<4, 4, float> const & matrix) {
	GLint loc = glGetUniformLocation(name_, name);

	if (loc == -1) { return false; }

	glUniformMatrix4fv(loc, 1, 0, matrix.begin());

	return true;
}
