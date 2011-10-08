#include <machine_graph/commands/MachineGraphCommands.hpp>

CreateMachineCommand::CreateMachineCommand(std::string const & mt, MachineID mid) :
	MachineCommand(make_command_id(machine_graph_namespace, create_machine_command_id)),
	machine_type(mt), machine_id(mid)
{ }

LinkMachineCommand::LinkMachineCommand(MachineID in_id, MachineID out_id) :
	MachineCommand(make_command_id(machine_graph_namespace, link_machine_command_id)),
	input_id(in_id), output_id(out_id)
{ }
