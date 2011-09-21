#include <app_engine/ApplicationEngine.hpp>
#include <arch/common.hpp>
#include <machines/MachineFactory.hpp>
#include <machines/templates/Template.hpp>
#include <machine_graph/commands/MachineGraphCommands.hpp>
#include <math/intersection.hpp>
#include <Square.hpp>

#include <iostream>

// NOTE: This can be factored out to be very generic, and it should be.
// Virtually every single template is going to look this and it's a hell of a
// lot of code.

namespace {
	class SineTemplateTouchable;
	class SineTemplateRenderable;
	class SineTemplateMovingRenderable;
	class SineTemplateMovingTouchable;

	class SineTemplateTouchable : public Touchable {
		public: 
			bool handle_move_start(Vec2 const & start);

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

			void set_touchable(SineTemplateTouchable * touchable) { touchable_ = touchable; }

		private:
			Square square_;
			SineTemplateTouchable * touchable_;
	};

	class SineTemplateMovingTouchable : public Touchable {
		public:
			bool handle_move_move(Vec2 const & loc);
			bool handle_move_end(Vec2 const & end);

			void set_renderable(SineTemplateMovingRenderable * renderable)
				{ renderable_ = renderable; }

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

			void render(RenderingEngine const * rendering_engine);

			void set_touchable(Touchable * touchable) { touchable_ = touchable; }

		private:
			Square square_;
			Touchable * touchable_;
	};

	bool SineTemplateTouchable::handle_move_start(Vec2 const & start) {
		ApplicationEngine * app_engine = ApplicationEngine::get();

		// FIXME: Get smarter about my memory, damnit
		SineTemplateMovingRenderable * renderable = new SineTemplateMovingRenderable(renderable_->get_program());
		SineTemplateMovingTouchable * touchable = new SineTemplateMovingTouchable();

		renderable->set_frame(this->renderable_->get_frame());
		renderable->set_touchable(touchable);
		touchable->set_renderable(renderable);

		app_engine->register_renderable(renderable);
		app_engine->register_touchable(touchable);
        
        return true;
	}

	void SineTemplateRenderable::render(RenderingEngine const * rendering_engine) {
		square_.render(rendering_engine, get_frame());

		if (touchable_) {
			touchable_->set_bounding_rectangle(
				square_.get_bounding_rectangle(rendering_engine, get_frame())
			);
		}
	}

	void SineTemplateMovingRenderable::render(RenderingEngine const * rendering_engine) {
		square_.render(rendering_engine, get_frame());

		if (touchable_) {
			touchable_->set_bounding_rectangle(
				square_.get_bounding_rectangle(rendering_engine, get_frame())
			);
		}
	}

	bool SineTemplateMovingTouchable::handle_move_move(Vec2 const & loc) {
		RenderingEngine * rendering_engine = ApplicationEngine::get()->get_rendering_engine();
		Ray<3, float> ray = rendering_engine->unproject_device_independent(loc);

		boost::optional<Vec3> pos = intersection(ray, Plane<3, float>(Vec3(0, 0, 1), 0));
		if (!pos) { return false; }

        renderable_->set_position(*pos);
        return true;
    }

	bool SineTemplateMovingTouchable::handle_move_end(Vec2 const & end) {
		std::auto_ptr<SineTemplateMovingTouchable> touch_ptr(this);
        std::auto_ptr<SineTemplateMovingRenderable> render_ptr(renderable_);

		ApplicationEngine * app_engine = ApplicationEngine::get();

		// if in a place where we actually wanna create a machine
		Machine * machine = MachineFactory::get().construct("SineMachine");
		if (!machine) { return false; }
		
		machine->get_renderable()->set_frame(renderable_->get_frame());
		
		MachineID machine_id = app_engine->register_machine(machine);

		CreateMachineCommand * create_command = new CreateMachineCommand("SineMachine", machine_id);
		std::cout << "client side " << create_command << " " << create_command->get_target_id() << std::endl;
		app_engine->push_command(create_command);
		// fi

		app_engine->erase_renderable(renderable_);
        
        return false;
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
	Program * program = initialize_program();

	SineTemplateRenderable * renderable = new SineTemplateRenderable(program);
	SineTemplateTouchable * touchable = new SineTemplateTouchable();

	renderable->set_position(Vec3(0.0, 0.0, 0.0));
	renderable->set_touchable(touchable);
	touchable->set_renderable(renderable);

	app_engine->register_renderable(renderable);
	app_engine->register_touchable(touchable);

	return new Template(renderable, touchable);
}
