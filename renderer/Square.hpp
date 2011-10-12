#pragma once

#include <math/MatrixOps.hpp>
#include <math/Rectangle.hpp>
#include <math/Vec.hpp>
#include <OpenGLES/ES2/gl.h>
#include <renderer/Program.hpp>
#include <renderer/RenderingEngine.hpp>
#include <vector>

struct Square {
	struct Vertex {
		GLfloat pos[3];
		GLfloat tex[2];

		Vec4 get_pos() const { return Vec4(pos[0], pos[1], pos[2], 1.0f); }
	};

	Square(Program * program) :
		program_(program)
	{
		Vertex vertices[] = {
			{ -0.5f, -0.5f, 0.0f, -1.0f, -1.0f },
			{ -0.5f,  0.5f, 0.0f, -1.0f,  1.0f },
			{  0.5f, -0.5f, 0.0f,  1.0f, -1.0f },
			{  0.5f,  0.5f, 0.0f,  1.0f,  1.0f },
		};

		Vertex * it = vertices, *end = vertices + sizeof(vertices) / sizeof(Vertex);

		for (; it != end; ++it) { vertices_.push_back(*it); }

		pos_idx_ = glGetAttribLocation(program->get_name(), "position");
		tex_idx_ = glGetAttribLocation(program->get_name(), "tex");
	}

	void render(RenderingEngine const * engine, Renderable::FrameType const & f) {
		float F[] = {
			f[0][0], f[0][1], f[0][2], 0,
			f[1][0], f[1][1], f[1][2], 0,
			      0,       0,       1, 0,
			f[2][0], f[2][1], f[2][2], 1
		};
		Matrix<4, 4, float> M(F);
		
		program_->bind_uniform("model_view", engine->get_camera().get_camera_inverse() * M);

		glVertexAttribPointer(pos_idx_, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), vertices_.front().pos);

		glVertexAttribPointer(tex_idx_, 2, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), vertices_.front().tex);

		glEnableVertexAttribArray(pos_idx_);
		glEnableVertexAttribArray(tex_idx_);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisableVertexAttribArray(pos_idx_);
		glDisableVertexAttribArray(tex_idx_);
	}

	Rectangle<float> get_bounding_rectangle(
		RenderingEngine const * engine, Renderable::FrameType const & f) const
	{
		float F[] = {
			f[0][0], f[0][1], f[0][2], 0,
			f[1][0], f[1][1], f[1][2], 0,
			      0,       0,       1, 0,
			f[2][0], f[2][1], f[2][2], 1
		};
		Matrix<4, 4, float> M(F);

		Vec2 corners[] = {
			engine->get_camera().project_to_device_independent(vertices_[0].get_pos() * M),
			engine->get_camera().project_to_device_independent(vertices_[1].get_pos() * M),
			engine->get_camera().project_to_device_independent(vertices_[2].get_pos() * M),
			engine->get_camera().project_to_device_independent(vertices_[3].get_pos() * M)
		};

		return ::get_bounding_rectangle(corners);
	}

	private:
		Program * program_;
		std::vector<Vertex> vertices_;
		GLuint pos_idx_, tex_idx_;
};
