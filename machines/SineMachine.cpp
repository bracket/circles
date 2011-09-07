#include <app_engine/ApplicationEngine.hpp>
#include <machines/Machine.hpp>
#include <machines/MachineFactory.hpp>

#include <Square.hpp>


namespace {
	class SineMachineRenderable;
	class SineMachineTouchable;

	class SineMachineTouchable : public Touchable {
		public:
			void handle_move_move(Vec2 const & pos);
			void handle_single_tap(Vec2 const & pos);

            void set_renderable(SineMachineRenderable * renderable) 
				{ renderable_ = renderable; }
        
		private:
			SineMachineRenderable * renderable_;
	};

	class SineMachineRenderable : public Renderable {
		public:
			SineMachineRenderable(Program * program) :
				Renderable(program),
				square_(program),
				touchable_(0)
			{
			}

			void set_touchable(SineMachineTouchable * touchable)
				{ touchable_ = touchable; }

			void set_position(Vec3 const & p) { square_.set_position(p); }
			Vec3 const & get_position() const { return square_.get_position(); }

			void render(RenderingEngine const * rendering_engine) {
				square_.render(rendering_engine);

				if (touchable_) {
					touchable_->set_bounding_rectangle(
						square_.get_bounding_rectangle(rendering_engine)
					);
				}
			}

		private:
			Square square_;
			SineMachineTouchable * touchable_;
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

	void SineMachineTouchable::handle_move_move(Vec2 const & pos) {
		Vec3 pos3 = renderable_->get_position();
		pos3 = Vec3(-pos.x(), -pos.y(), 1.0f) * pos3.z();
        renderable_->set_position(pos3);
	}

	void circle_tap_callback(MachineCommand * command, CommandResponse * response) {
		std::cout << "tappa tappa tappa command:" << command
			<< " response: " << response << std::endl;
	}

	class SineMachine : public Machine {
		public:
			SineMachine(Renderable * renderable, Touchable * touchable) : 
				Machine(renderable, touchable) { }
	};

	void SineMachineTouchable::handle_single_tap(Vec2 const & pos) {
		ApplicationEngine * app_engine = ApplicationEngine::get();

		CreateMachineCommand * command = new CreateMachineCommand("SineMachine");
		command->set_callback(circle_tap_callback);

		app_engine->push_command(command);
	}

	Machine * constructor() {
		static Program * program = initialize_program();

		Renderable * renderable = new SineMachineRenderable(program);
		Touchable * touchable = new SineMachineTouchable();

		renderable->set_touchable(touchable);
		touchable->set_renderable(renderable);

		return new SineMachine(renderable, touchable);
	}

	struct registrar {
		registrar() {
			get().register_constructor("SineMachine", constructor);
		}
	};

	registrar r;
}
