#include <machines/MachineFactory.hpp>
#include <machines/MachineGraph.hpp>
#include <machines/commands/MachineCommand.hpp>
#include <machines/commands/MachineGraphCommands.hpp>
#include <memory>

#include <iostream>

struct MachineGraphAccess {
	static void handle_command(MachineGraph * graph, CreateMachineCommand * command) {
		std::cout << "handle_command command = " << command
			<< " response = " << command->get_response() << std::endl;

		command->set_response(success_response);
	}
};

namespace {
	template <class CommandType>
	void machine_graph_dispatcher(MachineGraph * graph, MachineCommand * command) {
		MachineGraphAccess::handle_command(graph, static_cast<CommandType*>(command));
	}
}

bool MachineGraph::init() {
    std::auto_ptr<SoundMachine> out(get_machine_factory().construct("OutputMachine", this));

	if (!out.get()) { return false; }
	if (!add_machine_with_id(1, out.get())) { return false; }

	output_ = out.release();

	graph_dispatch_.register_dispatch(
		make_command_id(machine_graph_namespace, create_machine_command_id), 
		&machine_graph_dispatcher<CreateMachineCommand>
	);

	return true;
}

BlockType * MachineGraph::render() {
	return output_->render();
}

