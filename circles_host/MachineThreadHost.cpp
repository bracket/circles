#include <arch/concurrency.hpp>
#include <circles_host/MachineThreadHost.hpp>
#include <unistd.h>

#include <iostream>

PortAudioRenderer * construct_machine_thread_renderer(MachineThread * machine_thread);

void MachineThreadHost::thread_main() {
	typedef MachineThread::AsyncCommandQueue AsyncCommandQueue;

	MachineThread * machine_thread = MachineThread::construct();
	if (!machine_thread) { return; }
	while (!cmp_exchange_barrier(static_cast<MachineThread*>(0), machine_thread, &machine_thread_)) { ; }

	PortAudio * port_audio = PortAudio::construct();
	if (!port_audio) { return; }

	std::swap(port_audio, port_audio_);
	if (port_audio) { delete port_audio; }

	PortAudioRenderer * renderer = construct_machine_thread_renderer(machine_thread);
	port_audio_->start_stream(0, 2, renderer);

	MachineGraph * graph = machine_thread->get_machine_graph();

	TargetID input_id;
	input_id.client_id = 1;
	input_id.machine_id = 2;

	TargetID output_id;
	output_id.client_id = 0;
	output_id.machine_id = 1;

	graph->add_machine(input_id, "SineMachine");
	graph->link_machines(input_id, output_id);

	for (;;) {
		machine_thread_->loop();
		usleep(5000);
	}
}
