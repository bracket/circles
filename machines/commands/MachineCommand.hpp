#pragma once

#include <algorithm>
#include <boost/utility.hpp>
#include <vector>

union CommandID {
	struct {
		unsigned short name;
		unsigned short space;
	};

	unsigned int id;
};

inline CommandID make_command_id(unsigned short space, unsigned short name) {
	CommandID id = { name, space };
	return id;
}

class MachineCommand : boost::noncopyable {
	public:
		virtual ~MachineCommand() { }

        CommandID get_command_id() const { return command_id_; }

	protected:
		explicit MachineCommand(CommandID command_id)
			: command_id_(command_id) { }

	private:
		CommandID command_id_;
};

template <class TargetType>
struct CommandDispatchMap {
	typedef void (*CommandDispatcher)(TargetType *, MachineCommand*);
	typedef std::pair<CommandID, CommandDispatcher> DispatchPair;
	typedef std::vector<DispatchPair> DispatchVector;

	typedef typename DispatchVector::iterator iterator;
	typedef typename DispatchVector::const_iterator const_iterator;

	void register_dispatch(CommandID id, CommandDispatcher dispatcher) {
		typedef std::pair<iterator, iterator> range_type;

		range_type r = std::equal_range(dispatchers_.begin(), dispatchers_.end(), id, cmp_);
		dispatchers_.insert(r.second, DispatchPair(id, dispatcher));
	}

	void dispatch(TargetType * target, MachineCommand * command) const {
		typedef std::pair<const_iterator, const_iterator> range_type;

		range_type r = std::equal_range(dispatchers_.begin(), dispatchers_.end(), *command, cmp_);
		for (; r.first != r.second; ++r.first) { (*r.first->second)(target, command); }
	}

	private:
		struct Cmp {
			template <class L, class R>
			bool operator () (L const & left, R const & right) const {
				return this->extract(left) < this->extract(right);
			}

			unsigned int extract(unsigned int const & i) const { return i; }

			unsigned int extract(MachineCommand const & cmd) const { return cmd.get_command_id().id; }

            unsigned int extract(CommandID const & id) const { return id.id; }

			template <class F, class S>
			unsigned int extract(std::pair<F, S> const & p) const { return this->extract(p.first); }
		};

		DispatchVector dispatchers_;
		Cmp cmp_;
};

template <class TargetType, class CommandType>
void machine_command_dispatcher(TargetType * target, MachineCommand * command) {
	return static_cast<TargetType*>(target)->handle_command(static_cast<CommandType*>(command));
}
