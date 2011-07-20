#include <machines/MachineThread.hpp>
#include <machines/MachineGraph.hpp>

bool MachineThread::init() {
	graph_.reset(MachineGraph::construct());

	int silent = graph_->add_machine("SilentMachine");
	if (!silent) { return false; }

	if (!graph_->link_machines(silent, 1)) { return false; }

	//int sine = graph_->add_machine("SineMachine");
	//if (!sine)  { return false; }

	//if (!graph_->link_machines(sine, 1)) { return false; }

	return true;
}

bool MachineThread::loop() {
	if (!last_rendered_) { ticker_.reset(); }
	sample_time_ += ticker_.tick();

	int target_time = sample_time_ + graph_->get_sample_rate() / 200;

	while (last_rendered_ < target_time) {
		BlockType * out = graph_->render();
		if (!out) { return true; }
		last_rendered_ += out->sample_count();
		block_queue_.push(out);
	}

	// TODO: We probably need to put the thread to sleep if we're getting
	// through this loop too quick
    return true;
}
