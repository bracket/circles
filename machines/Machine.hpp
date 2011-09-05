#pragma once

#include <algorithm>
#include <machines/Block.hpp>
#include <vector>

class MachineGraph;

typedef Block<float, 1024, 1> BlockType;

class SoundMachine {
	typedef std::vector<SoundMachine*> InputContainer;

	public:
		typedef InputContainer::iterator input_iterator;
		typedef InputContainer::const_iterator const_input_iterator;

		virtual BlockType * render() = 0;

		virtual ~SoundMachine() { }

		SoundMachine(MachineGraph * graph) : graph_(graph) { }

		input_iterator input_begin() { return inputs_.begin(); }
		const_input_iterator input_begin() const { return inputs_.begin(); }

		input_iterator input_end() { return inputs_.end(); }
		const_input_iterator input_end() const { return inputs_.end(); }

		void push_input(SoundMachine * machine) { inputs_.push_back(machine); }

		bool remove_input(SoundMachine * machine) {
			input_iterator it = std::find(inputs_.begin(), inputs_.end(), machine);
			if (it  == inputs_.end()) { return false; }
			inputs_.erase(it);
			return true;
		}

	protected:
        BlockType * alloc_block();
        void free_block(BlockType * block);

	private:
		InputContainer inputs_;
		MachineGraph * graph_;
};
