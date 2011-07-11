#include <machines/MachineGraph.hpp>
#include <machines/MachineFactory.hpp>
#include <memory>

bool MachineGraph::init() {
    std::auto_ptr<Machine> out(get_machine_factory().construct("OutputMachine"));

	if (!out.get()) { return false; }
	if (!add_machine_with_id(1, out.get())) { return false; }

	output_ = out.release();

	return true;
}
