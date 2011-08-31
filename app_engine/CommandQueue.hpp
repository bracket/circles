#pragma once

#include <machines/commands/MachineCommand.hpp>

class CommandQueue {
	public:
		virtual void push(MachineCommand * command) = 0;
		virtual void process_responses() = 0;
		virtual ~CommandQueue() { }
};
