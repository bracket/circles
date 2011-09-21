#pragma once

#include <machine_graph/commands/MachineCommand.hpp>
#include <shared/types.hpp>
#include <string>

const unsigned short machine_graph_namespace = 0;
const unsigned short machine_namespace = 1;

const unsigned int create_machine_command_id = 1;

struct CreateMachineCommand : MachineCommand {
	CreateMachineCommand(std::string const & mt, MachineID machine_id);

	std::string machine_type;
	MachineID machine_id;
};
