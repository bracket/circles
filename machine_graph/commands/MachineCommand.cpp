#include <machine_graph/commands/MachineCommand.hpp>

CommandResponse * const success_response = 
	reinterpret_cast<CommandResponse*>(
		reinterpret_cast<unsigned int>(new CommandResponse()) | 0x00000001
	);
