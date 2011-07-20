#pragma once

#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <machines/Block.hpp>
#include <machines/Machine.hpp>
#include <machines/MachineFactory.hpp>
#include <string>
#include <utility>
#include <vector>

class MachineGraph {
	typedef boost::unordered_map<int, Machine*> MachineContainer;

	public:
		~MachineGraph() {
			typedef MachineContainer::iterator iterator;
			for (iterator it = machines_.begin(); it != machines_.end(); ++it)
				{ delete it->second; }
		}
		
		static MachineGraph * construct(int sample_rate = 44100) {
			std::auto_ptr<MachineGraph> ptr(new MachineGraph(sample_rate));
			if (!ptr->init()) { return 0; }
			return ptr.release();
		}
		
		int get_sample_rate() const { return sample_rate_; }

		BlockType * render();

		BlockType * alloc_block() {
			if (free_blocks_.empty()) { return new BlockType(); }
			BlockType * block = free_blocks_.back();
			free_blocks_.pop_back();
			return block;
		}

		void free_block(BlockType * block) { free_blocks_.push_back(block); }

		int add_machine(std::string const & machine_type) {
			std::auto_ptr<Machine> ptr(get_machine_factory().construct(machine_type, this));
			if (!ptr.get()) { return 0; }

			int id = next_id();
			if (!add_machine_with_id(id, ptr.get())) { return 0; }

			ptr.release();
			return id;
		}

		bool link_machines(int input_id, int output_id) {
			typedef MachineContainer::iterator iterator;

			iterator in = machines_.find(input_id);
			if (in == machines_.end()) { return false; }

			iterator out = machines_.find(output_id);
			if (out == machines_.end()) { return false; }

			link_machines(in->second, out->second);

			return true;
		}

	private:
		// NOTE: Machine ID 0 is invalid machine ID.  Machine ID 1 is reserved
		// for the output machine.
		MachineGraph(int sample_rate) :
			output_(0),
			sample_rate_(sample_rate), current_machine_id_(1)
		{ }

		bool init();

		int next_id() { return ++current_machine_id_; }

		bool add_machine_with_id(int id, Machine * machine, bool force = false) {
			typedef MachineContainer::iterator iterator;
            std::pair<iterator, bool> p = machines_.insert(std::make_pair(id, machine));
			if (p.second) { return true; }
			if (!force) { return false; }

			boost::scoped_ptr<Machine> old(p.first->second);
			p.first->second = machine;
			return true;
		}

		void link_machines(Machine * input, Machine * output) {
			output->push_input(input);
		}

		MachineContainer machines_;

		std::vector<BlockType*> free_blocks_;
		Machine * output_;

		int sample_rate_, current_machine_id_;
};
