#pragma once

#include <algorithm>
#include <boost/utility.hpp>
#include <vector>
#include <shared/types.hpp>

inline CommandID make_command_id(unsigned short space, unsigned short name) {
	CommandID id = {{ name, space }};
	return id;
}

struct CommandResponse {
	virtual ~CommandResponse() { }
	virtual std::string response_string() { return "Success"; }
	virtual int response_type() { return 0; }
};

extern CommandResponse * const success_response;

class MachineCommand : boost::noncopyable {
	public:
		typedef void (*ResponseCallback)(MachineCommand *, CommandResponse *);

		virtual ~MachineCommand() {
			if (reinterpret_cast<unsigned int>(response_) & 0x00000001) { return; }
			delete response_;
		}

		void set_target_id(TargetID target_id) { target_id_ = target_id; }
		TargetID get_target_id() const { return target_id_; }

		void set_client_id(ClientID id) { target_id_.client_id = id; }
		ClientID get_client_id() { return target_id_.client_id; }

		void set_machine_id(MachineID id) { target_id_.machine_id = id; }
		MachineID get_machine_id() { return target_id_.machine_id; }

		void set_callback(ResponseCallback callback) { callback_ = callback; }

        CommandID get_command_id() const { return command_id_; }

		bool dispatch_callback() {
			if (!response_) { return false; }
			if (!callback_) { return true; }
			(*callback_)(this, response_);

			return true;
		}

		void set_response(CommandResponse * response) { response_ = response; }
		CommandResponse * get_response() const { return response_; }

	protected:
		explicit MachineCommand(CommandID command_id) :
			target_id_(0), command_id_(command_id),
			response_(0), callback_(0)
		{ }

	private:
		TargetID target_id_;
		CommandID command_id_;
		CommandResponse * response_;
		ResponseCallback callback_;
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
