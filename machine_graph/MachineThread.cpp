#include <machine_graph/MachineThread.hpp>
#include <machine_graph/MachineGraph.hpp>

bool MachineThread::init() {
	graph_.reset(MachineGraph::construct());

	return true;
}

bool MachineThread::loop() {
	if (!last_rendered_) { ticker_.reset(); }

	boost::optional<MachineCommand *> out;

	while (command_queue_.shift(out)) { graph_->dispatch_command(*out); } 
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
