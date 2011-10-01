#include <app_engine/ApplicationEngine.hpp>
#include <machines/Machine.hpp>
#include <machines/MachineFactory.hpp>
#include <machine_graph/commands/MachineGraphCommands.hpp>

#include <math/intersection.hpp>
#include <Square.hpp>
#include <arch/common.hpp>
#include <iostream>

namespace {
	class SineMachine;
	class SineMachineRenderable;
	class SineMachineTouchable;

	class LinkMovingTouchable;
	class LinkMovingRenderable;

	class LinkMovingTouchable : public Touchable {
		public:
			LinkMovingTouchable(Machine * machine) :
				start_machine_(machine) { }

			bool handle_move_move(TouchHandler *, Vec2 const & loc);
			bool handle_move_end(TouchHandler *, Vec2 const & loc);

			void set_renderable(LinkMovingRenderable * renderable) { renderable_ = renderable; }

		private:
			Machine * start_machine_;
			LinkMovingRenderable * renderable_;
	};

	class LinkMovingRenderable : public Renderable {
		public:
			LinkMovingRenderable(Program * program) :
				Renderable(program),
				square_(program),
				touchable_(0)
			{ }

			void set_touchable(LinkMovingTouchable * touchable) { touchable_ = touchable; }

			void render(RenderingEngine const * rendering_engine) {
				square_.render(rendering_engine, get_frame());

				if (touchable_) {
					touchable_->set_bounding_rectangle(
						square_.get_bounding_rectangle(rendering_engine, get_frame())
					);
				}
			}

		private:
			Square square_;
			LinkMovingTouchable * touchable_;
	};

	bool LinkMovingTouchable::handle_move_move(TouchHandler *, Vec2 const & loc) {
		RenderingEngine * rendering_engine = ApplicationEngine::get()->get_rendering_engine();
		Ray<3, float> ray = rendering_engine->unproject_device_independent(loc);

		boost::optional<Vec3> pos = intersection(ray, Plane<3, float>(Vec3(0, 0, 1), 0));
		if (!pos) { return false; }

		renderable_->set_position(*pos);
        return true;
	}

	bool LinkMovingTouchable::handle_move_end(TouchHandler *, Vec2 const & loc) {
		return true;
	}

	class SineMachineTouchable : public Touchable {
		public:
			bool handle_move_start(TouchHandler *, Vec2 const & loc);
			bool handle_single_tap(TouchHandler *, Vec2 const & loc);

			void set_machine(SineMachine * machine) { machine_ = machine; }

            void set_renderable(SineMachineRenderable * renderable) 
				{ renderable_ = renderable; }
        
		private:
			SineMachine * machine_;
			SineMachineRenderable * renderable_;
	};

	class SineMachineRenderable : public Renderable {
		public:
			SineMachineRenderable(Program * program) :
				Renderable(program),
				square_(program),
				touchable_(0)
			{ }

			void set_touchable(SineMachineTouchable * touchable)
				{ touchable_ = touchable; }

			void render(RenderingEngine const * rendering_engine) {
				square_.render(rendering_engine, get_frame());

				if (touchable_) {
					touchable_->set_bounding_rectangle(
						square_.get_bounding_rectangle(rendering_engine, get_frame())
					);
				}
			}

		private:
			Square square_;
			SineMachineTouchable * touchable_;
	};

	class SineMachine : public Machine {
		public:
			SineMachine(SineMachineRenderable * renderable, SineMachineTouchable * touchable) : 
				Machine(renderable, touchable)
			{
				touchable->set_machine(this);
			}
	};

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

	bool SineMachineTouchable::handle_move_start(TouchHandler *, Vec2 const & loc) {
		ApplicationEngine * app_engine = ApplicationEngine::get();

		LinkMovingRenderable * link_renderable = new LinkMovingRenderable(renderable_->get_program());
		LinkMovingTouchable * link_touchable = new LinkMovingTouchable(static_cast<Machine*>(machine_));

		link_renderable->set_frame(this->renderable_->get_frame());
		link_renderable->set_touchable(link_touchable);
		link_touchable->set_renderable(link_renderable);

		app_engine->register_renderable(link_renderable);
		app_engine->register_touchable(link_touchable);

		return true;
	}

	void circle_tap_callback(MachineCommand * command, CommandResponse * response) {
		std::cout << "tappa tappa tappa command:" << command
			<< " response: " << response << std::endl;
	}

	bool SineMachineTouchable::handle_single_tap(TouchHandler *, Vec2 const & pos) {
//		ApplicationEngine * app_engine = ApplicationEngine::get();

		// CreateMachineCommand * command = new CreateMachineCommand("SineMachine");
		// command->set_callback(circle_tap_callback);

		// app_engine->push_command(command);
        return true;
	}

	Machine * constructor() {
		static Program * program = initialize_program();

		SineMachineRenderable * renderable = new SineMachineRenderable(program);
		SineMachineTouchable * touchable = new SineMachineTouchable();


		renderable->set_touchable(touchable);
		touchable->set_renderable(renderable);

		return new SineMachine(renderable, touchable);
	}

	struct registrar {
		registrar() {
			MachineFactory::get().register_constructor("SineMachine", constructor);
		}
	};

	registrar r;
}
