#include <machine_graph/commands/MachineGraphCommands.hpp>

CreateMachineCommand::CreateMachineCommand(std::string const & mt) :
	MachineCommand(make_command_id(machine_graph_namespace, create_machine_command_id)),
	machine_type(mt)
{ }
