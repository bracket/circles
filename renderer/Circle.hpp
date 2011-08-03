#include <math/MatrixOps.hpp>
#include <OpenGLES/ES2/gl.h>
#include <renderer/Renderable.hpp>
#include <renderer/RenderingEngine.hpp>
#include <vector>

#include <shared/Ticker.hpp>

inline double wrap(double f) {
	double i;
	return modf(f, &i);
}

class Circle : public Renderable {
	public:
		struct Vertex {
			GLfloat pos[3];
			GLfloat color[4];
		};

		Circle(Program * program) : Renderable(program) {
			Vertex vertices[] = {
				{ -0.5f, -0.5f, -4.0f, 1.0f, 0.0f, 0.0f, 1.0f },
				{ -0.5f,  0.5f, -4.0f, 1.0f, 0.0f, 0.0f, 1.0f },
				{  0.5f, -0.5f, -4.0f, 1.0f, 0.0f, 0.0f, 1.0f },
				{  0.5f,  0.5f, -4.0f, 1.0f, 0.0f, 0.0f, 1.0f },
			};

			Vertex * it = vertices, *end = vertices + sizeof(vertices) / sizeof(Vertex);
			for (; it != end; ++it) {
				vertices_.push_back(*it);
			}

			pos_idx_ = glGetAttribLocation(program->get_name(), "position");
			color_idx_ = glGetAttribLocation(program->get_name(), "color");

			time_ = 0.0f;
			ticker_.reset();
		}

		void render(RenderingEngine const * rendering_engine) {
			typedef std::vector<Vertex>::iterator iterator;

			time_ += ticker_.tick() / 1e6;

			get_program()->bind_uniform("model_view",
				rendering_engine->get_camera_matrix() * rotate_around_z(8 * std::atan(4) * time_)
			);

			for (iterator it = vertices_.begin(); it != vertices_.end(); ++it) {
				it->color[0] = (GLfloat)wrap(time_);
				it->color[1] = (GLfloat)wrap(2.0 * time_);
				it->color[2] = (GLfloat)wrap(3.0 * time_);
				it->color[3] = 1.0f;
			}

			glVertexAttribPointer(pos_idx_, 3, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), vertices_.front().pos);

			glVertexAttribPointer(color_idx_, 4, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), vertices_.front().color);

			glEnableVertexAttribArray(pos_idx_);
			glEnableVertexAttribArray(color_idx_);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glDisableVertexAttribArray(pos_idx_);
			glDisableVertexAttribArray(color_idx_);
		}

	private:
		std::vector<Vertex> vertices_;
		GLuint pos_idx_, color_idx_;
		Ticker ticker_;
		double time_;
};
