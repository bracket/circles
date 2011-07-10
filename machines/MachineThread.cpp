#include <machines/MachineFactory.hpp>
#include <machines/MachineThread.hpp>

bool MachineThread::init() {
	output_machine_.reset(get_machine_factory().construct("OutputMachine"));
	if (!output_machine_) { return false; }

	Machine * sine_machine = get_machine_factory().construct("SineMachine");
	if (!sine_machine) { return false; }

	output_machine_->push_input(sine_machine);

	return true;
}

bool MachineThread::loop() {
	if (!last_rendered_) { ticker_.reset(); }
	sample_time_ += ticker_.tick();

	const int sample_rate = 44100;
	int target_time = sample_time_ + sample_rate / 200;

	while (last_rendered_ < target_time) {
		BlockType * out = output_machine_->render();
		if (!out) { return true; }
		last_rendered_ += out->sample_count();
		block_queue_.push(out);
	}

	// TODO: We probably need to put the thread to sleep if we're getting
	// through this loop too quick
    return true;
}
