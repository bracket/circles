#include <app_engine/ApplicationEngine.hpp>
#include <machines/Machine.hpp>
#include <machines/MachineFactory.hpp>
#include <machine_graph/commands/MachineGraphCommands.hpp>

#include <math/intersection.hpp>
#include <Square.hpp>
#include <arch/common.hpp>
#include <iostream>

namespace {
	class OutputMachineRenderable;
	class OutputMachineTouchable;

	class OutputMachineTouchable : public Touchable {
		public:
			bool handle_move_end(Vec2 const & loc);

			void set_renderable(OutputMachineRenderable * renderable) { renderable_ = renderable; }

		private:
			OutputMachineRenderable * renderable_;
	};

	class OutputMachineRenderable : public Renderable {
		public:
			OutputMachineRenderable(Program * program) :
				Renderable(program),
				square_(program),
				touchable_(0)
			{ }

			void set_touchable(OutputMachineTouchable * touchable)
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
			OutputMachineTouchable * touchable_;
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

	bool OutputMachineTouchable::handle_move_end(Vec2 const & loc) {
		// make connection
		return true;
	}

	class OutputMachine : public Machine {
		public:
			OutputMachine(Renderable * renderable, Touchable * touchable) :
				Machine(renderable, touchable) { }
	};

	Machine * constructor() {
		static Program * program = initialize_program();

		OutputMachineRenderable * renderable = new OutputMachineRenderable(program);
		OutputMachineTouchable * touchable = new OutputMachineTouchable();

		renderable->set_touchable(touchable);
		touchable->set_renderable(renderable);

		return new OutputMachine(renderable, touchable);
	}

	struct registrar {
		registrar() {
			MachineFactory::get().register_constructor("OutputMachine", constructor);
		}
	};

	registrar r;
}
