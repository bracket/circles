#pragma once

#include <renderer/Program.hpp>

class Renderable {
	public:
		Renderable(Program * program)
			: program_(program) { }
		
		virtual ~Renderable() { }

		virtual void render() = 0;

		Program * get_program() const { return program_; }
	
	private:
		Program * program_;
};
