#include <Circles/LocalCommandQueue.hpp>

bool LocalCommandQueue::init() {
	MachineThreadHost * machine_thread_host = [[ MachineThreadHost alloc ] init ];

	if (!machine_thread_host) { return false; }

	NSThread * thread = [
		[ NSThread alloc ]
			initWithTarget:machine_thread_host
			selector:@selector(threadMain) object:nil
	];

	[ machine_thread_host release ];
	
	[ thread start ];

	MachineThread * machine_thread = 0;
	while (!machine_thread) {
	// FIXME:
	// does this guarantee that machine_thread is correctly initialized to the right thing?
	// NO! we probably need a memory barrier in MachineThreadHost threadMain.
		machine_thread = [ machine_thread_host getMachineThread ];
	}

	async_queue_ = machine_thread->get_command_queue();

	return true;
}
