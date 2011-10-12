#include <Square.hpp>
#include <renderer/Program.hpp>
#include <renderer/RenderingEngine.hpp>

Square::Square(Program * program) :
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

void Square::render(RenderingEngine const * engine, Renderable::FrameType const & f) {
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
