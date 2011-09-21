#pragma once

#include <boost/scoped_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <machine_graph/Block.hpp>
#include <machine_graph/commands/MachineCommand.hpp>
#include <machine_graph/SoundMachine.hpp>
#include <machine_graph/SoundMachineFactory.hpp>
#include <string>
#include <utility>
#include <vector>

class MachineGraph {
	friend struct MachineGraphAccess;

	typedef boost::unordered_map<TargetID, SoundMachine*> MachineContainer;
	typedef CommandDispatchMap<SoundMachine> MachineDispatchMap;
	typedef CommandDispatchMap<MachineGraph> GraphDispatchMap;
	typedef boost::unordered_map<TargetID, MachineDispatchMap*> DispatchContainer;

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

		void dispatch_command(MachineCommand * command) {
			TargetID target_id = command->get_target_id();

			if (!target_id.machine_id) {
				graph_dispatch_.dispatch(this, command);
			}
			else {
				MachineContainer::iterator mit = machines_.find(target_id);
				if (mit == machines_.end()) { return; }

				DispatchContainer::iterator it = dispatchers_.find(target_id);
				if (it == dispatchers_.end()) { return; }
				it->second->dispatch(mit->second, command);
			}
		}

		bool add_machine(TargetID target_id, std::string const & machine_type, bool force = false) {
			std::auto_ptr<SoundMachine> ptr(SoundMachineFactory::get().construct(machine_type, this));
			if (!ptr.get()) { return false; }
			return add_machine(target_id, ptr, force);
		}

		SoundMachine * add_machine(TargetID target_id, std::auto_ptr<SoundMachine> machine, bool force = false) {
			typedef MachineContainer::iterator iterator;

            std::pair<iterator, bool> p = machines_.insert(std::make_pair(target_id, machine.get()));
			if (p.second) { return machine.release(); }
			if (!force) { return 0; }

			boost::scoped_ptr<SoundMachine> old(p.first->second);
			p.first->second = machine.release();
			return p.first->second;
		}

		bool link_machines(TargetID input_id, TargetID output_id) {
			typedef MachineContainer::iterator iterator;

			iterator in = machines_.find(input_id);
			if (in == machines_.end()) { return false; }

			iterator out = machines_.find(output_id);
			if (out == machines_.end()) { return false; }

			link_machines(in->second, out->second);

			return true;
		}

	private:
		// NOTE: SoundMachine ID 0 is invalid machine ID.  SoundMachine ID 1 is reserved
		// for the output machine.
		MachineGraph(int sample_rate) :
			output_(0),
			sample_rate_(sample_rate), current_machine_id_(1)
		{ }

		bool init();

		int next_id() { return ++current_machine_id_; }


		void link_machines(SoundMachine * input, SoundMachine * output) {
			output->push_input(input);
		}

		MachineContainer machines_;
		DispatchContainer dispatchers_;
		GraphDispatchMap graph_dispatch_;

		std::vector<BlockType*> free_blocks_;
		SoundMachine * output_;

		int sample_rate_, current_machine_id_;
};
