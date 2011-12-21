#include <Circles/MachineThreadHost.h>
#include <machine_graph/MachineThread.hpp>
#include <memory>
#include <shared/CommandQueue.hpp>
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
			command->set_client_id(get_client_id());

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
