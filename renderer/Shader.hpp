#pragma once 

#include <boost/utility.hpp>
#include <memory>
#include <OpenGLES/ES2/gl.h>
#include <string>

class Shader : boost::noncopyable {
	public:
		enum Type {
			None           = 0,
			VertexShader   = 1,
			FragmentShader = 2
		};

		static Shader * construct(Type const & type, std::string const & source) {
			if (type == None) { return 0; }

			std::auto_ptr<Shader> ptr(new Shader(type, source));
			if (!ptr->init()) { return 0; }

			return ptr.release();
		}

		Type get_type() const { return type_; }
		std::string const & get_source() const { return source_; }
		GLuint get_name() const { return name_; }

		~Shader();

	private:
		Shader(Type const & type) : type_(type), name_(0) { }

		Shader(Type const & type, std::string const & source) :
			type_(type), source_(source), name_(0)
		{ }

		bool init();

        Type type_;
		std::string source_;
		GLuint name_;
};
