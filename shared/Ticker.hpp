#pragma once

#include <sys/time.h>
#include <boost/utility.hpp>

class Ticker : boost::noncopyable {
	public:
		void reset() { gettimeofday(&last_, 0); }

		int tick() {
			gettimeofday(&next_, 0);
			int out = 1000000 * (next_.tv_sec - last_.tv_sec) + (next_.tv_usec - last_.tv_usec);
			last_ = next_;
			return out;
		}

	private:
		timeval last_, next_;
};
