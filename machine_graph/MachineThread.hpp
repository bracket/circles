#pragma once

#include <boost/scoped_ptr.hpp>
#include <boost/utility.hpp>
#include <machine_graph/MachineGraph.hpp>
#include <memory>
#include <shared/AsynchronousQueue.hpp>
#include <shared/Ticker.hpp>

class MachineThread : boost::noncopyable {
	public:
		typedef AsynchronousQueue<BlockType*> BlockQueue;
		typedef AsynchronousQueue<MachineCommand*> AsyncCommandQueue;

		static MachineThread * construct() {
			std::auto_ptr<MachineThread> ptr(new MachineThread());
			if (!ptr->init()) { return 0; }
			return ptr.release();
		}

		bool loop();

		BlockQueue * get_block_queue() { return &block_queue_; }
		AsyncCommandQueue * get_command_queue() { return &command_queue_; }

		MachineGraph * get_machine_graph() const { return graph_.get(); }

	private:
		MachineThread() : sample_time_(0), last_rendered_(0) { }

		bool init();
		
		BlockQueue block_queue_;
		AsyncCommandQueue command_queue_;
		boost::scoped_ptr<MachineGraph> graph_;

		Ticker ticker_;
		int sample_time_, last_rendered_;
};
