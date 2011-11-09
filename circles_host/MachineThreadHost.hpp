#pragma once

#include <machine_graph/MachineThread.hpp>
#include <memory>

class MachineThreadHost {
	public:
		MachineThreadHost() : machine_thread_(0) { }

		void thread_main();

	private:
		MachineThread * machine_thread_;
};
