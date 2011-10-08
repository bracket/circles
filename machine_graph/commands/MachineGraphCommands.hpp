#pragma once

#include <machine_graph/commands/MachineCommand.hpp>
#include <shared/types.hpp>
#include <string>

const unsigned short machine_graph_namespace = 0;
const unsigned short machine_namespace = 1;

const unsigned int create_machine_command_id = 1;
const unsigned int link_machine_command_id   = 2;

struct CreateMachineCommand : MachineCommand {
	CreateMachineCommand(std::string const & mt, MachineID machine_id);

	std::string machine_type;
	MachineID machine_id;
};

struct LinkMachineCommand : MachineCommand {
	LinkMachineCommand(MachineID in_id, MachineID out_id);
	
	MachineID input_id, output_id;
};
