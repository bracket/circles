#include <app_engine/CommandQueue.hpp>
#include <machines/MachineThread.hpp>
#include <Circles/MachineThreadHost.h>
#include <memory>

class LocalCommandQueue : public CommandQueue {
	public:

		static LocalCommandQueue * construct() {
			std::auto_ptr<LocalCommandQueue> ptr(new LocalCommandQueue());
			if (!ptr->init()) { return 0; }
			return ptr.release();
		}

		void push(MachineCommand * command) { async_queue_->push(command); }

	private:
		LocalCommandQueue() { }

		bool init();

		MachineThread::AsyncCommandQueue * async_queue_;
};
