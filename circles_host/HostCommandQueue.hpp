#pragma once

#include <shared/CommandQueue.hpp>
#include <circles_host/MachineThreadHost.hpp>
#include <memory>

class HostCommandQueue : public CommandQueue {
	public:
		static HostCommandQueue * construct() {
			std::auto_ptr<MachineThreadHost> ptr();
			if (!ptr->init()) { return 0; }
			return ptr.release();
		}

		void push(MachineCommand*);
		void process_responses();
		// CreateMachineCommand * create_command = new CreateMachineCommand("SineMachine", machine_id);
		
	private:
		HostCommandQueue() : machine_thread_host_(0) { }
		bool init();

		MachineThreadHost * machine_thread_host_;
};
