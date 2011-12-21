#pragma once

#include <machine_graph/commands/MachineCommand.hpp>
#include <shared/types.hpp>

class CommandQueue {
	public:
		CommandQueue() : client_id_(0) { }

		ClientID get_client_id() const { return client_id_; }

		virtual void push(MachineCommand * command) = 0;
		virtual void process_responses() = 0;
		virtual ~CommandQueue() { }

	protected:
		void set_client_id(ClientID id) { client_id_ = id; }

	private:
		ClientID client_id_;
};
