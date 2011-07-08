#pragma once

#include <machines/Machine.hpp>
#include <memory>
#include <shared/AsynchronousQueue.hpp>

class MachineThread {
	public:
		typedef AsynchronousQueue<BlockType> BlockQueue;

		static MachineThread * construct() {
			std::auto_ptr<MachineThread> ptr(new MachineThread());
			if (!ptr->init()) { return 0; }
			return ptr.release();
		}

		bool loop();

		BlockQueue * get_block_queue() { return &block_queue_; }

	private:
		MachineThread() { }

		bool init() { return true; }

		BlockQueue block_queue_;
};
