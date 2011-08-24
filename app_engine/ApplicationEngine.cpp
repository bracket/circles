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
	inline double wrap(double d) {
		double i;
		return modf(d, &i);
	}

	inline double clamp(double low, double high, double d) {
		return std::min(std::max(low, d), high);
	}

	class CircleRenderable;
	class CircleTouchable;

	// NOTE: Just a test touchable.  I think this pattern may be more general
	// though.  A Renderable will need to know the full interface of an
	// associated Touchable and vice versa.
	class CircleTouchable : public Touchable {
		public:
			void handle_move_move(Vec2 const & pos);
			void handle_single_tap(Vec2 const & pos);

            void set_renderable(CircleRenderable * renderable) 
				{ renderable_ = renderable; }
        
		private:
			CircleRenderable * renderable_;
	};

	class CircleRenderable : public Renderable {
		public:
			struct Vertex {
				GLfloat pos[3];
				GLfloat tex[2];

				Vec4 get_pos() const { return Vec4(pos[0], pos[1], pos[2], 1.0f); }
			};

			CircleRenderable(Program * program) :
				Renderable(program),
				touchable_(0),
				position_(0, 0, -4)
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

				ticker_.reset();
				phase_ = 0.0;
			}

			void set_touchable(CircleTouchable * touchable)
				{ touchable_ = touchable; }

			void set_position(Vec3 const & p) { position_ = p; }
			Vec3 const & get_position() const { return position_; }

			void render(RenderingEngine const * rendering_engine) {
				typedef std::vector<Vertex>::iterator iterator;

                double angle = atan2(position_.y(), position_.x());

				phase_ += (ticker_.tick() / 1e6) * 4 * angle;

				float T[] = {
					1,             0,             0,             0, 
					0,             1,             0,             0, 
					0,             0,             1,             0, 
					position_.x(), position_.y(), position_.z(), 1
				};

				Matrix<4, 4, float> M =
					rotate_around_z(static_cast<float>(phase_))
					* Matrix<4, 4, float>(T);
                
				if (touchable_) {
					Vec2 corners[] = {
						rendering_engine->project_to_device_independent(vertices_[0].get_pos() * M),
						rendering_engine->project_to_device_independent(vertices_[1].get_pos() * M),
						rendering_engine->project_to_device_independent(vertices_[2].get_pos() * M),
						rendering_engine->project_to_device_independent(vertices_[3].get_pos() * M)
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

				glVertexAttribPointer(tex_idx_, 2, GL_FLOAT, GL_FALSE,
					sizeof(Vertex), vertices_.front().tex);

				glEnableVertexAttribArray(pos_idx_);
				glEnableVertexAttribArray(tex_idx_);

				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				glDisableVertexAttribArray(pos_idx_);
				glDisableVertexAttribArray(tex_idx_);
			}

		private:
			std::vector<Vertex> vertices_;
			GLuint pos_idx_, tex_idx_;
			Vec3 position_;

			CircleTouchable * touchable_;

			Ticker ticker_;
			double phase_;
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

	void CircleTouchable::handle_move_move(Vec2 const & pos) {
		Vec3 pos3 = renderable_->get_position();
		pos3 = Vec3(-pos.x(), -pos.y(), 1.0f) * pos3.z();
        renderable_->set_position(pos3);
	}

	void CircleTouchable::handle_single_tap(Vec2 const & pos) {
		
	}
}

bool ApplicationEngine::init() {
	touch_handler_ = new TouchHandler(this);
	return true;
}

ApplicationEngine::~ApplicationEngine() {
	delete rendering_engine_;
	delete command_queue_;
	delete touch_handler_;
}

void ApplicationEngine::render_frame() {
	CircleData & data = get_circle_data(this);
	rendering_engine_->submit(data.renderable);

	rendering_engine_->render();
}
