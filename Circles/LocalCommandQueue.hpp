#include <app_engine/CommandQueue.hpp>
#include <machines/MachineThread.hpp>
#include <Circles/MachineThreadHost.h>
#include <memory>
#include <vector>

class LocalCommandQueue : public CommandQueue {
	struct ResponseQueue {
		typedef std::vector<MachineCommand*> ContainerType;
		ContainerType ping_, pong_;

		void push(MachineCommand * command) { ping_.push_back(command); }
		void process();
	};

	public:
		static LocalCommandQueue * construct() {
			std::auto_ptr<LocalCommandQueue> ptr(new LocalCommandQueue());
			if (!ptr->init()) { return 0; }
			return ptr.release();
		}

		void push(MachineCommand * command) {
			response_queue_.push(command);
			async_queue_->push(command);
		}

		void process_responses() { response_queue_.process(); }

	private:
		LocalCommandQueue() { }

		bool init();

		MachineThread::AsyncCommandQueue * async_queue_;
		ResponseQueue response_queue_;
};
