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
		machine_thread = [ machine_thread_host getMachineThread ];
	}

	async_queue_ = machine_thread->get_command_queue();

	return true;
}

void LocalCommandQueue::ResponseQueue::process() {
	typedef ContainerType::iterator iterator;

	for (iterator it = ping_.begin(); it != ping_.end(); ++it) {
		if ((*it)->dispatch_callback()) { delete *it; continue; }
		pong_.push_back(*it);
	}

	ping_.swap(pong_);
	pong_.clear();
}
