#include <algorithm>
#include <app_engine/ApplicationEngine.hpp>
#include <arch/common.hpp>
#include <math/Rectangle.hpp>
#include <renderer/Shader.hpp>
#include <shared/Ticker.hpp>

#include <iostream>

#include <math/MatrixOps.hpp>
#include <OpenGLES/ES2/gl.h>
#include <renderer/Renderable.hpp>
#include <renderer/RenderingEngine.hpp>
#include <vector>

#include <shared/Ticker.hpp>

// NOTE: This namespace contains some test Renderables and Touchables.  It
// should go away at some point.
namespace {
	inline double wrap(double f) {
		double i;
		return modf(f, &i);
	}

	class CircleRenderable;
	class CircleTouchable;

	// NOTE: Just a test touchable.  I think this pattern may be more general
	// though.  A Renderable will need to know the full interface of an
	// associated Touchable and vice versa.
	class CircleTouchable : public Touchable {
		public:
			void handle_touch_start(Vec2 const & start);

            void set_renderable(CircleRenderable * renderable) 
				{ renderable_ = renderable; }
        
		private:
			CircleRenderable * renderable_;
	};

	class CircleRenderable : public Renderable {
		public:
			struct Vertex {
				GLfloat pos[3];
				GLfloat color[4];

				Vec4 get_pos() const { return Vec4(pos[0], pos[1], pos[2], 1.0f); }
			};

			CircleRenderable(Program * program) :
				Renderable(program),
				touchable_(0)
			{
				Vertex vertices[] = {
					{ -0.5f, -0.5f, -4.0f, 1.0f, 1.0f, 1.0f, 1.0f },
					{ -0.5f,  0.5f, -4.0f, 1.0f, 0.0f, 0.0f, 0.0f },
					{  0.5f, -0.5f, -4.0f, 1.0f, 1.0f, 1.0f, 1.0f },
					{  0.5f,  0.5f, -4.0f, 1.0f, 1.0f, 1.0f, 1.0f },
				};

				Vertex * it = vertices, *end = vertices + sizeof(vertices) / sizeof(Vertex);
				for (; it != end; ++it) { vertices_.push_back(*it); }

				pos_idx_ = glGetAttribLocation(program->get_name(), "position");
				color_idx_ = glGetAttribLocation(program->get_name(), "color");

				time_ = 0.0f;
				ticker_.reset();
			}

			void set_touchable(CircleTouchable * touchable)
				{ touchable_ = touchable; }

			void render(RenderingEngine const * rendering_engine) {
				typedef std::vector<Vertex>::iterator iterator;

				time_ += ticker_.tick() / 1e6;

				//Matrix<4, 4, float> M = rotate_around_z(static_cast<float>(8 * std::atan(1) * time_));
				Matrix<4, 4, float> M = identity_matrix<float>();

				if (touchable_) {
					Vec2 corners[] = {
						rendering_engine->project_to_screen(M * vertices_[0].get_pos()),
						rendering_engine->project_to_screen(M * vertices_[1].get_pos()),
						rendering_engine->project_to_screen(M * vertices_[2].get_pos()),
						rendering_engine->project_to_screen(M * vertices_[3].get_pos())
					};

					Vec2 lower_left = corners[0], upper_right = corners[0];
					for (int i = 1; i < 4; ++i) {
						lower_left = min(lower_left, corners[i]);
						upper_right = max(upper_right, corners[i]);
					}

					touchable_->set_bounding_rect(Rectangle<float>(upper_right.y(), lower_left.x(),
						lower_left.y(), upper_right.x()));
				}

				get_program()->bind_uniform("model_view", rendering_engine->get_camera_matrix() * M);

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

			CircleTouchable * touchable_;
	};

	struct CircleData {
		CircleData(ApplicationEngine * app_engine) {
			program = initialize_program();
			renderable = new CircleRenderable(program);
			touchable = new CircleTouchable();

			renderable->set_touchable(touchable);
			touchable->set_renderable(renderable);

			app_engine->get_touch_handler()->insert_touchable(touchable);
		}

		Program * initialize_program() {
			std::string vertex_shader_source = get_file_contents("test", "vsh");
			std::auto_ptr<Shader> vertex_shader(Shader::construct(Shader::VertexShader, vertex_shader_source));
			if (!vertex_shader.get()) { return 0; }

			std::string fragment_shader_source = get_file_contents("test", "fsh");
			std::auto_ptr<Shader> fragment_shader(Shader::construct(Shader::FragmentShader, fragment_shader_source));
			if (!fragment_shader.get()) { return 0; }

			std::auto_ptr<Program> program(Program::construct(vertex_shader.get(), fragment_shader.get()));
			if (!program.get()) { return 0; }

			vertex_shader.release();
			fragment_shader.release();

			return program.release();
		}

		Program * program;
		CircleRenderable * renderable;
		CircleTouchable * touchable;
	};

	CircleData & get_circle_data(ApplicationEngine * app_engine) {
		static CircleData data(app_engine); 
		return data;
	}

	void CircleTouchable::handle_touch_start(Vec2 const & start) {
		std::cout << "circle touch start " << start << std::endl;
	}
}

bool ApplicationEngine::init() {
	touch_handler_ = new TouchHandler(this);
	return true;
}

ApplicationEngine::~ApplicationEngine() {
	delete rendering_engine_;
	delete touch_handler_;
}

void ApplicationEngine::render_frame() {
	CircleData & data = get_circle_data(this);
	rendering_engine_->submit(data.renderable);

	rendering_engine_->render();
}
