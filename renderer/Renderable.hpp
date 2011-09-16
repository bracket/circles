#pragma once

#include <renderer/Program.hpp>
#include <math/Matrix.hpp>
#include <math/MatrixOps.hpp>

class RenderingEngine;

class Renderable {
	public:
		typedef Matrix<3, 3, float> FrameType;

		virtual ~Renderable() { }

		virtual void render(RenderingEngine const * rendering_engine) = 0;

		Program * get_program() const { return program_; }

		void set_frame(FrameType const & frame) { frame_ = frame; }
		FrameType const & get_frame() const { return frame_; }

		void set_position(Vec3 const & pos) { frame_.set_row(2, pos); }

		void set_position(Vec2 const & pos) { frame_.set_row(2, Vec3(pos.x(), pos.y(), 0.0f)); }

	protected:
		Renderable(Program * program)
			: program_(program), frame_(identity_matrix3<float>()) { }

	private:
		Program * program_;
		FrameType frame_;
};
