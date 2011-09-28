#include <boost/range/algorithm.hpp>
#include <iostream>
#include <shared/EventLogger/EventLogger.hpp>
#include <iterator>

namespace {
	template <class OutIt>
	struct FlushMessage {
		FlushMessage(OutIt freed, EventLogFlags flags) :
			freed_(freed),
			level_(flags & EventLogLevelMask),
			channels_(flags & EventLogChannelMask)
		{ }

		void operator () (EventLogMessage * message) const {
			if ((message->flags & EventLogLevelMask) < level_) { return; }
			if (!(message->flags & EventLogChannelMask) & channels_) { return; }

			for (; message; message = message->next) {
				std::cout << message->message;
				*freed_++ = message;
			}
		}

		private:
			mutable OutIt freed_;
			unsigned int level_, channels_;
	};

	template <class OutIt>
	inline FlushMessage<OutIt> make_flusher(OutIt const & freed, EventLogFlags flags){
		return FlushMessage<OutIt>(freed,flags);
	}
}

void EventLogger::flush_messages() {
	boost::for_each(messages_, make_flusher(std::back_inserter(freed_), flags_));
	messages_.clear();
}
