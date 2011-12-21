#include <circles_host/HostCommandQueue.hpp>
#include <machine_graph/commands/MachineGraphCommands.hpp>

int main() {
	HostCommandQueue * command_queue = HostCommandQueue::construct();

	CreateMachineCommand * create_command = new CreateMachineCommand("SineMachine", 2);
	command_queue->push(create_command);

	usleep(500000);

	TargetID input_id;
	input_id.client_id = 0;
	input_id.machine_id = 2;

	TargetID output_id;
	output_id.client_id = 0;
	output_id.machine_id = 1;

	LinkMachineCommand * link_command = new LinkMachineCommand(input_id, output_id);
	command_queue->push(link_command);

	for (;;) { sleep(10); }

	return 0;
}
