#pragma once

#include <circles_host/PortAudio.hpp>
#include <machine_graph/MachineThread.hpp>
#include <memory>

class MachineThreadHost {
	public:
		MachineThreadHost() : machine_thread_(0), port_audio_(0) { }

		void thread_main();

	private:
		MachineThread * machine_thread_;
		PortAudio * port_audio_;
};
