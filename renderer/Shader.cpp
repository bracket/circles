#include <renderer/Shader.hpp>
#include <OpenGLES/ES2/gl.h>
#include <shared/EventLogger.hpp>

namespace {
	inline GLenum shader_type_to_gl_enum(Shader::Type const & type) {
		switch (type) {
			case Shader::VertexShader: return GL_VERTEX_SHADER;
			case Shader::FragmentShader: return GL_FRAGMENT_SHADER;
			default: return GL_INVALID_ENUM;
		};
	}
}

Shader::~Shader() {
	if (name_) { glDeleteShader(name_); }
}

bool Shader::init() {
	GLenum shader_type = shader_type_to_gl_enum(get_type());
	if (shader_type == GL_INVALID_ENUM) { return false; }

	name_ = glCreateShader(shader_type);
	if (!name_) { return false; }

	char const * src = source_.c_str();
	glShaderSource(name_, 1, &src, 0);

	glCompileShader(name_);

	GLint status;
	glGetShaderiv(name_, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLint length = 0;
		glGetShaderiv(name_, GL_INFO_LOG_LENGTH, &length);

		std::vector<char> log(length);

		GLsizei written = 0;
		glGetShaderInfoLog(name_, length, &written, &*log.begin());

		log_message(EventLogWarn, EventLogGL, log);

		return false;
	}

	return true;
}
