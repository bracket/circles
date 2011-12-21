#pragma once

#include <boost/utility.hpp>
#include <memory>
#include <portaudio.h>

struct PortAudioRenderer {
	virtual int operator () (
		void const * input,
		void * output,
		unsigned long frame_count,
		PaStreamCallbackTimeInfo const * time_info,
		PaStreamCallbackFlags flags) = 0;
	
	virtual ~PortAudioRenderer() { }
};

struct PortAudio : boost::noncopyable {
	public:
		static inline PortAudio * construct() {
			std::auto_ptr<PortAudio> ptr(new PortAudio());
			if (!ptr->init()) { return 0; }
			return ptr.release();
		}

		~PortAudio();

		bool start_stream(int input_device_index, int output_device_index,
			PortAudioRenderer * renderer);

		bool stream_is_active() const;

	private:
		PortAudio() { }

		bool init();

		PaStream * stream_;
};
