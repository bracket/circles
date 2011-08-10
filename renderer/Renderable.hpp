#pragma once

#include <renderer/Program.hpp>
#include <math/Rectangle.hpp>

class RenderingEngine;

class Renderable {
	public:
		virtual ~Renderable() { }

		virtual void render(RenderingEngine const * rendering_engine) = 0;

		Program * get_program() const { return program_; }

	protected:
		Renderable(Program * program)
			: program_(program) { }

	private:
		Program * program_;
};
