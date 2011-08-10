#pragma once

#include <boost/utility.hpp>
#include <math/Matrix.hpp>
#include <memory>
#include <OpenGLES/ES2/gl.h>
#include <renderer/Shader.hpp>

class Program : boost::noncopyable {
	public:
		~Program();

		static Program * construct(Shader * vertex, Shader * fragment) {
			std::auto_ptr<Program> ptr(new Program());
			if (!ptr->init(vertex, fragment)) { return 0; }
			return ptr.release();
		}

		void bind() const;

		Shader * get_vertex_shader() const { return vertex_shader_; }
		Shader * get_fragment_shader() const { return fragment_shader_; }
		GLuint get_name() const { return name_; }

		bool bind_uniform(char const * name, float f);
		bool bind_uniform(char const * name, Matrix<4, 4, float> const & matrix);

	private:
		Program() : vertex_shader_(0), fragment_shader_(0), name_(0) { }

		bool init(Shader * vertex_shader, Shader * fragment_shader);

		Shader * vertex_shader_, * fragment_shader_;
		GLuint name_;
};
