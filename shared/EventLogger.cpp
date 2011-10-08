#include <boost/range/algorithm.hpp>
#include <iostream>
#include <shared/EventLogger.hpp>
#include <iterator>

void EventLogger::flush_messages() {
	unsigned int level = flags_ & EventLogLevelMask,
		channels = flags_ & EventLogChannelMask;

	while(EventLogMessage * message = shift_queue<1>(&head_, &tail_)) {
		if ((message->flags & EventLogLevelMask) < level) { continue; }
		if (!(message->flags & EventLogChannelMask) & channels) { continue; }

		// TODO: Format nicely, annotate what level and channels this message is coming across, and
		// output a legitimate timestamp.
		std::cout << message->message;
	}
}
