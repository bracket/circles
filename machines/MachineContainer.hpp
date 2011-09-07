#pragma once

#include <map>
#include <machines/Machine.hpp>

class MachineContainer {
	typedef std::map<MachineID, Machine *> MachineMap;
	typedef std::map<Machine *, MachineID> ReverseMap;

	public:
		typedef MachineMap::const_iterator const_iterator;

		MachineContainer() : last_machine_id_(0) { }

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

			machine_map_.insert(std::make_pair(id, machine));
			reverse_map_.insert(std::make_pair(machine, id));

			return id;
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

		MachineMap machine_map_;
		ReverseMap reverse_map_;
		int last_machine_id_;
};
