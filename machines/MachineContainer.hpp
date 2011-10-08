#pragma once

#include <map>
#include <machines/Machine.hpp>

class MachineContainer {
	typedef std::map<MachineID, Machine *> MachineMap;
	typedef std::map<Machine *, MachineID> ReverseMap;

	public:
		typedef MachineMap::const_iterator const_iterator;

		static const MachineID OUTPUT_MACHINE_ID = 1;

		MachineContainer() : last_machine_id_(OUTPUT_MACHINE_ID) { }

		~MachineContainer() {
			typedef MachineMap::iterator iterator;

			iterator it = machine_map_.begin(),
				end = machine_map_.end();

			for (; it != end; ++it) { delete it->second; }
		}

		MachineID register_machine(Machine * machine) {
			typedef ReverseMap::iterator r_iterator;
			r_iterator rit = reverse_map_.find(machine);

			if (rit != reverse_map_.end()) { return rit->second; }

			MachineID id = next_id();

			return register_machine_with_id(id, machine);
		}

		MachineID register_output_machine(Machine * machine) {
			return register_machine_with_id(OUTPUT_MACHINE_ID, machine);
		}

		bool delete_machine(MachineID id) {
			typedef MachineMap::iterator m_iterator;

			m_iterator mit = machine_map_.find(id);
			if (mit == machine_map_.end()) { return false; }

			Machine * machine = mit->second;

			reverse_map_.erase(machine);
			machine_map_.erase(mit);

			delete machine;

			return true;
		}

		Machine * find_machine(MachineID id) const {
			const_iterator it = machine_map_.find(id);

			if (it == end()) { return 0; }

			return it->second;
		}

		const_iterator begin() const { return machine_map_.begin(); };
		const_iterator end() const { return machine_map_.end(); };

	private:
		int next_id() { return ++last_machine_id_; }

		MachineID register_machine_with_id(MachineID id, Machine * machine) {
			typedef MachineMap::iterator iterator;
			typedef std::pair<iterator, bool> pair_type;

			std::auto_ptr<Machine> ptr(machine);

			pair_type p = machine_map_.insert(std::make_pair(id, machine));
			if (!p.second) {
				std::auto_ptr<Machine> old(p.first->second);
				p.first->second = ptr.release();
				reverse_map_.erase(machine);
			}
			reverse_map_.insert(std::make_pair(machine, id));

			machine->set_machine_id(id);

			return id;
		}


		MachineMap machine_map_;
		ReverseMap reverse_map_;
		int last_machine_id_;
};
