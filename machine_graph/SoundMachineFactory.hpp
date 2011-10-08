#pragma once

#include <map>
#include <string>
#include <utility>

class SoundMachine;
class MachineGraph;

class SoundMachineFactory {
	public:
		typedef SoundMachine * (*MachineConstructor)(MachineGraph *);

	private:
		typedef std::map<std::string, MachineConstructor> ConstructorMap;
		typedef ConstructorMap::iterator iterator;
		typedef ConstructorMap::const_iterator const_iterator;
 
	public:
		static SoundMachineFactory & get() {
			static SoundMachineFactory factory;
			return factory;
		}

		bool register_constructor(
			std::string const & name, MachineConstructor constructor, bool force = false
		)
		{
			std::pair<iterator, bool> p = constructors_.insert(std::make_pair(name, constructor));
			if (p.second) { return true; }
			if (!force) { return false; }
			p.first->second = constructor;
			return false;
		}

		SoundMachine * construct(std::string const & name, MachineGraph * graph) const {
			const_iterator it = constructors_.find(name);
			if (it == constructors_.end()) { return 0; }
			return (*it->second)(graph);
		}
		
	private:
		ConstructorMap constructors_;
};
