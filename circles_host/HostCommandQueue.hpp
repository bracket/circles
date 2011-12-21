#pragma once

#include <circles_host/MachineThreadHost.hpp>
#include <memory>
#include <shared/CommandQueue.hpp>
#include <vector>

class HostCommandQueue : public CommandQueue {
	struct ResponseQueue {
		typedef std::vector<MachineCommand*> ContainerType;
		ContainerType ping_, pong_;

		void push(MachineCommand * command) { ping_.push_back(command); }
		void process();
	};

	public:
		static HostCommandQueue * construct() {
			std::auto_ptr<HostCommandQueue> ptr(new HostCommandQueue());
			if (!ptr->init()) { return 0; }
			return ptr.release();
		}

		void push(MachineCommand * command) {
			response_queue_.push(command);
			async_queue_->push(command);
		}

		void process_responses() { response_queue_.process(); }
		
	private:
		HostCommandQueue() : async_queue_(0) { }

		bool init();

		MachineThread::AsyncCommandQueue * async_queue_;
		ResponseQueue response_queue_;
};
