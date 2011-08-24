#include <machines/MachineFactory.hpp>
#include <machines/MachineGraph.hpp>
#include <machines/commands/MachineCommand.hpp>
#include <machines/commands/MachineGraphCommands.hpp>
#include <memory>

#include <iostream>

const unsigned int create_machine_command_id = 1;

struct CreateMachineCommand : MachineCommand {
	CreateMachineCommand(std::string const & mt) :
		MachineCommand(make_command_id(machine_graph_namespace, create_machine_command_id)),
		machine_type(mt)
	{ }

	std::string machine_type;
};

struct MachineGraphAccess {
	static void handle_command(MachineGraph * graph, CreateMachineCommand * command) {
		int id = graph->add_machine(command->machine_type);
		std::cout << "the id is: " << id << "\n";
	}
};

namespace {
	template <class CommandType>
	void machine_graph_dispatcher(MachineGraph * graph, MachineCommand * command) {
		MachineGraphAccess::handle_command(graph, static_cast<CommandType*>(command));
	}
}

bool MachineGraph::init() {
    std::auto_ptr<Machine> out(get_machine_factory().construct("OutputMachine", this));

	if (!out.get()) { return false; }
	if (!add_machine_with_id(1, out.get())) { return false; }

	output_ = out.release();

	graph_dispatch_.register_dispatch(
		make_command_id(machine_graph_namespace, create_machine_command_id), 
		machine_graph_dispatcher<CreateMachineCommand>
	);

	this->dispatch_command(0, new CreateMachineCommand("SineMachine"));

	return true;
}

BlockType * MachineGraph::render() {
	return output_->render();
}

