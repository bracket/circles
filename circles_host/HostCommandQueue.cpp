#include <boost/thread.hpp>
#include <circles_host/HostCommandQueue.hpp>
#include <circles_host/MachineThreadHost.hpp>

namespace {
	struct ThreadStarter {
		ThreadStarter(MachineThreadHost * thread_host)
			: thread_host_(thread_host) { }

		void operator () () const { thread_host_->thread_main(); }

		private:
			MachineThreadHost * thread_host_;
	};
}


bool HostCommandQueue::init() {
	MachineThreadHost * machine_thread_host = new MachineThreadHost();
	ThreadStarter starter(machine_thread_host);

	boost::thread thread_host(starter);

	MachineThread * machine_thread = 0;
	while (!machine_thread) { machine_thread = machine_thread_host->get_machine_thread(); }

	async_queue_ = machine_thread->get_command_queue();

	return true;
}

void HostCommandQueue::ResponseQueue::process() {
	typedef ContainerType::iterator iterator;

	for (iterator it = ping_.begin(); it != ping_.end(); ++it) {
		if ((*it)->dispatch_callback()) {
			delete *it;
			continue;
		}

		pong_.push_back(*it);
	}

	ping_.swap(pong_);
	pong_.clear();
}
