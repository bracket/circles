#include <arch/concurrency.hpp>
#include <circles_host/MachineThreadHost.hpp>
#include <portaudio.h>

#include <iostream>

void MachineThreadHost::thread_main() {
	MachineThread * machine_thread = MachineThread::construct();
	if (!machine_thread) { return; }
	while (!cmp_exchange_barrier(static_cast<MachineThread*>(0), machine_thread, &machine_thread_)) { ; }
}
