#include <machine_graph/SoundMachineFactory.hpp>
#include <machine_graph/MachineGraph.hpp>
#include <machine_graph/commands/MachineCommand.hpp>
#include <machine_graph/commands/MachineGraphCommands.hpp>
#include <memory>

#include <iostream>

struct MachineGraphAccess {
	static void handle_command(MachineGraph * graph, CreateMachineCommand * command) {
		// graph->add_machine(constructed TargetID, command->machine_type);
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
    std::auto_ptr<SoundMachine> out(SoundMachineFactory::get().construct("OutputMachine", this));

	if (!out.get()) { return false; }
	TargetID out_id;
	out_id.machine_id = 1;
	
	output_ = add_machine(out_id, out);
	if (!output_) { return false; }
	
	graph_dispatch_.register_dispatch(
		make_command_id(machine_graph_namespace, create_machine_command_id), 
		&machine_graph_dispatcher<CreateMachineCommand>
	);

	return true;
}

BlockType * MachineGraph::render() {
	return output_->render();
}

