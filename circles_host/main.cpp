#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <circles_host/MachineThreadHost.hpp>

namespace {
	struct ThreadStarter {
		ThreadStarter(MachineThreadHost * thread_host)
			: thread_host_(thread_host) { }

		void operator () () const {
			thread_host_->thread_main();
		}

		private:
			MachineThreadHost * thread_host_;
	};
}

int main() {
	boost::scoped_ptr<MachineThreadHost> machine_thread_host(new MachineThreadHost());
	boost::thread host_thread(ThreadStarter(machine_thread_host.get()));

	host_thread.join();

	return 0;
}
