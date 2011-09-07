#include <machines/templates/Template.hpp>
#include <app_engine/ApplicationEngine.hpp>
#include <Square.hpp>


namespace {
	class SineTemplateMovingRenderable;
	class SineTemplateMovingTouchable;
	class SineTemplateRenderable;

	class SineTemplateTouchable : public Touchable {
		public: 
			void handle_move_start(Vec2 const & start);


			void set_renderable(SineTemplateRenderable * renderable) { renderable_ = renderable; }

		private:
			SineTemplateRenderable * renderable_;
	};

	class SineTemplateRenderable : public Renderable {
		public:
			SineTemplateRenderable(Program * program) :
				Renderable(program),
				square_(program),
				touchable_(0)
			{ }

			void render(RenderingEngine const * rendering_engine);

			void set_touchable(SineTemplateMovingTouchable * touchable) { touchable_ = touchable; }

			void set_position(Vec2 const & loc) { square_.set_position(loc); }
			
			void get_position() { square_.get_position(); }

		private:
			Square square_;
			SineTemplateMovingTouchable * touchable_;
	};

	class SineTemplateMovingTouchable : public Touchable {
		public:
			void handle_move_move(Vec2 const & loc) {
				renderable->set_position(loc);
			}

			void handle_move_end(Vec2 const & loc) {
				std::auto_ptr<SineTemplateMovingTouchable> ptr(this);
				ApplicationEngine * app_engine = ApplicationEngine::get();

				// if in a place where we actually wanna create a machine
				Machine * machine = MachineFactory::construct("SineMachine");
				if (machine) { app_engine->register_machine(machine); }
				// fi

				app_engine->erase_touchable(this);
				app_engine->erase_renderable(renderable_);
				delete renderable_;
			}

			void set_renderable(SineTemplateMovingRenderable * renderable) {
				renderable_ = renderable;
			}

		private:
			SineTemplateMovingRenderable * renderable_;
	};

	class SineTemplateMovingRenderable : public Renderable {
		public:
			SineTemplateMovingRenderable(Program * program) :
				Renderable(program),
				square_(program),
				touchable_(0)
			{ }

			void render(RenderingEngine * rendering_engine) {
				square_.render(rendering_engine);

				if (touchable_) {
					touchable_->set_bounding_rectangle(
						square_.get_bounding_rectangle(rendering_engine)
					);
				}
			}

			void set_touchable(Touchable * touchable) { touchable_ = touchable; }

		private:
			Square square_;
			Touchable * touchable_;
	};

	void SineTemplateTouchable::handle_move_start(Vec2 const & start) {
		ApplicationEngine * app_engine = ApplicationEngine::get();

	// FIXME: Get smarter about my memory, damnit
		SineTemplateMovingRenderable * renderable = new SineTemplateMovingRenderable();
		SineTemplateMovingTouchable * touchable = new SineTemplateMovingTouchable();

		renderable->set_position(this->renderable_->get_position());
		renderable->set_touchable(touchable);
		touchable->set_renderable(renderable);

		app_engine->register_renderable(renderable);
		app_engine->register_touchable(touchable);
	}

	void SineTemplateMovingRenderable::render(RenderingEngine const * rendering_engine) {
		square_.render(rendering_engine);

		if (touchable_) {
			touchable_->set_bounding_rectangle(
				square_.get_bounding_rectangle(rendering_engine)
			);
		}
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
}

Template * create_test_template() {
	ApplicationEngine * app_engine = ApplicationEngine::get();

	SineTemplateRenderable * renderable = new SineTemplateRenderable();
	SineTemplateTouchable * touchable = new SineTemplateTouchable();

	renderable->set_position(Vec3(0.0, 0.0, -4.0));
	renderable->set_touchable(touchable);
	touchable->set_renderable(renderable);

	app_engine->register_renderable(renderable);
	app_engine->register_touchable(touchable);

	return new Template(renderable, touchable);
}
