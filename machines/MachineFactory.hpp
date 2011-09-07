#pragma once

#include <machines/Machine.hpp>
#include <string>

class MachineFactory {
	public:
		typedef Machine * (*Constructor)();

	private:
		typedef std::map<std::string, Constructor> ConstructorMap;
		typedef ConstructorMap::iterator iterator;
		typedef ConstructorMap::const_iterator const_iterator;
	
	public:
		static MachineFactory & get() {
			static MachineFactory factory;
			return factory;
		}

		bool register_constructor(
			std::string const & name, Constructor constructor, bool force = false
		)
		{
			std::pair<iterator, bool> p = constructors_.insert(std::make_pair(name, constructor));
			if (p.second) { return true; }
			if (!force) { return false; }
			p.first->second = constructor;
			return false;
		}

		Machine * construct(std::string const & name) const {
			const_iterator it = constructors_.find(name);
			if (it == constructors_.end()) { return 0; }
			return (*it->second)();
		}

	private:
		ConstructorMap constructors_;
};
