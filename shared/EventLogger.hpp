#pragma once

#include <arch/concurrency.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/range.hpp>
#include <cstring>
#include <vector>

// NOTE: First two bits are the log level, rest of the bits are channels

enum EventLogFlags {
	EventLogFlagNone     = 0x0000,
	EventLogDebug        = 0x0001,
	EventLogWarn         = 0x0002,
	EventLogCritical     = 0x0003,
	EventLogLevelMask    = 0x0003,
	EventLogGeneral      = 0x0004,
	EventLogGL           = 0x0008,
	EventLogSoundMachine = 0x0010,
	EventLogChannelMask  = 0xFFFC,
	EventLogMax          = 0xFFFF,
};

struct EventLogMessage {
	typedef boost::posix_time::ptime ptime;

	EventLogMessage() : flags(EventLogFlagNone), next(0) { message[0] = 0; }

	void reset() { timestamp = ptime(); flags = EventLogFlagNone; next = 0; message[0] = 0; }

	ptime timestamp;
	EventLogFlags flags;
	char message[128];
	EventLogMessage * next;
};

class EventLogger {
	public:
		EventLogger() :
			head_(new EventLogMessage()), tail_(head_), 
			freed_head_(new EventLogMessage()), freed_tail_(freed_head_),
			flags_(static_cast<EventLogFlags>(EventLogDebug | EventLogChannelMask))
		{ }

		static EventLogger & get() {
			static EventLogger logger;
			return logger;
		}

		void push_message(EventLogMessage * message) { push_queue(message, &head_, &tail_); }

		EventLogMessage * alloc_message() {
			EventLogMessage * m = shift_freed();
			if (m) { m->reset(); return m; }
			return new EventLogMessage();
		}

		void flush_messages();

		void set_level(EventLogFlags flags)
			{ flags_ = static_cast<EventLogFlags>((flags_  & ~EventLogLevelMask) | (flags & EventLogLevelMask)); }

		int get_level() const { return flags_ & EventLogLevelMask; }

		void set_channels(EventLogFlags flags)
			{ flags_ = static_cast<EventLogFlags>((flags_ & ~EventLogChannelMask) | (flags & EventLogChannelMask)); }

		EventLogFlags get_channels() const { return static_cast<EventLogFlags>(flags_ & EventLogChannelMask); }

		bool check_flags(EventLogFlags flags) const {
			if (!(flags & EventLogChannelMask & flags_)) { return false; }
			if ((flags & EventLogLevelMask) < get_level()) { return false; }
			return true;
		}

	private:
		EventLogMessage * shift_freed() {
			EventLogMessage * m = shift_queue<3>(&freed_head_, &freed_tail_);
			if (!m) { return 0; }

			m->reset();
			return m;
		}

		void push_queue(EventLogMessage * message, EventLogMessage ** head, EventLogMessage ** tail) {
			bool done = false;
			EventLogMessage * last = 0;

			while (!done) {
				last = *tail;
				if (last->next) { cmp_exchange(last, get_tail(last), tail); }
				else { done = cmp_exchange(static_cast<EventLogMessage*>(0), message, &last->next); }
			}

			cmp_exchange(last, get_tail(last), tail);
		}

		template <int max_retries>
		EventLogMessage * shift_queue(EventLogMessage ** head, EventLogMessage ** tail) {
			if (*head == *tail) { return 0; }

			for (int i = 0; i < max_retries; ++i) {
				EventLogMessage * message = *head, * next = message->next;
				if (!next) { return 0; }
				if (cmp_exchange(message, next, head)) { return message; }
			}

			return 0;
		}

		EventLogMessage * get_tail(EventLogMessage * message) const {
			for (; message->next; message = message->next) { }
			return message;
		}

		EventLogMessage * head_, * tail_;
		EventLogMessage * freed_head_, * freed_tail_;

		EventLogFlags flags_;
};

template <class RanIt>
inline EventLogMessage * make_message(EventLogFlags flags, RanIt begin, RanIt end) {
    using namespace boost::posix_time;
    
	EventLogger & logger = EventLogger::get();
	EventLogMessage * message = logger.alloc_message(),
		* current = message;

	ptime timestamp; // = now()

	message->flags = flags;
	message->timestamp = timestamp;

	for (RanIt next = begin; begin < end; begin = next) {
		current->timestamp = timestamp;
		current->flags = flags;

		next += std::min(end - next, 127);
		*std::copy(begin, end, message->message) = 0;

		if (next != end) {
			current->next = logger.alloc_message();
			current = current->next;
		}
	}

	return message;
}

template <class Range>
inline void log_message(EventLogFlags level, EventLogFlags channel, Range const & range) {
	EventLogger & logger = EventLogger::get();
	if (level < logger.get_level()) { return; }
	if (!(channel & logger.get_channels())) { return; }
	logger.push_message(make_message(static_cast<EventLogFlags>(level | channel), boost::begin(range), boost::end(range)));
}

inline void log_message(EventLogFlags level, EventLogFlags channel, char const * const & str) {
	log_message(level, channel, std::make_pair(str, str + std::strlen(str)));
}
