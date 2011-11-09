#include <boost/range/algorithm.hpp>
#include <boost/utility.hpp>
#include <cmath>
#include <memory>
#include <portaudio.h>
#include <iostream>
#include <boost/smart_ptr/scoped_ptr.hpp>

const double two_pi = 8.0 * std::atan(1.0), hz = two_pi / 44.1e3;

void log_pa_error(PaError err) {
	std::cerr << Pa_GetErrorText(err) << std::endl;
}

bool log_if_error(PaError err) {
	if (err == paNoError) { return false; }

	log_pa_error(err);
	return true;
}

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

		~PortAudio() {
			if (stream_) { 
				log_if_error(Pa_StopStream(stream_));
				log_if_error(Pa_CloseStream(stream_));
			}

			Pa_Terminate();
		}

		bool start_stream(int input_device_index, int output_device_index,
			PortAudioRenderer * renderer)
		{

			PaStreamParameters in_params = construct_stream_parameters(input_device_index, 2, true),
				out_params = construct_stream_parameters(output_device_index, 2, false);

			PaStream * ptr;

			auto err = Pa_OpenStream(
				&ptr,
				&in_params,
				&out_params,
				44100,
				paFramesPerBufferUnspecified,
				paNoFlag,
				&callback,
				renderer
			);

			if (log_if_error(err)) { return false; }

			stream_pointer s(ptr);

			if (log_if_error(Pa_StartStream(s.get()))) { return false; }

			stream_.swap(s);
			return true;
		}

		bool stream_is_active() { return Pa_IsStreamActive(stream_.get()); }

	private:
		PortAudio() { }

		bool init() {
			if (log_if_error(Pa_Initialize())) { return false; }
			return true;
		}

		PaStreamParameters construct_stream_parameters(int device_index, int channel_count, bool input) const
		{
			PaStreamParameters out;
			memset(&out, 0, sizeof(out));

			out.device = device_index;
			out.channelCount = channel_count;
			out.sampleFormat = paFloat32;

			PaDeviceInfo const * info = Pa_GetDeviceInfo(device_index);
			out.suggestedLatency = input ? info->defaultLowInputLatency : info->defaultLowOutputLatency;

			return out;
		}

		static int callback(
			void const * input, void * output, unsigned long frame_count,
			PaStreamCallbackTimeInfo const * time_info,
			PaStreamCallbackFlags flags,
			void * renderer
		)
		{
			return (*static_cast<PortAudioRenderer*>(renderer))(
				input, output, frame_count, time_info, flags
			);
		}

		PaStream * stream_;
};

struct PlayRenderer : PortAudioRenderer {
	PlayRenderer() : current_time_(0), max_time_(5 * 44100) { load(); }

	~PlayRenderer() { delete [] samples_; }

	int operator () (void const * input,
		void * output,
		unsigned long frame_count,
		PaStreamCallbackTimeInfo const * time_info,
		PaStreamCallbackFlags flags)
	{
		float * out = reinterpret_cast<float*>(output);
		int next_time = current_time_ + frame_count,
			end_time = (std::min)(next_time, max_time_),
			delta = end_time - current_time_;

		memcpy(out, samples_ + 2 * current_time_, delta * sizeof(float[2]));

		current_time_ = end_time;
		return current_time_ == max_time_ ? paComplete : paContinue;
	}

	private:
		void load() {
			samples_ = new float[2 * max_time_];

			FILE * fd = fopen("out.dat", "rb");
			fread(samples_, 1, sizeof(float[2]) * max_time_, fd);
			fclose(fd);
		}

		float * samples_;
		int current_time_, max_time_;
};

int weasel() {
	auto pa = PortAudio::construct();
	if (!pa) { return 1; }

	boost::scoped_ptr<PortAudioRenderer> renderer(new PlayRenderer());

	pa->start_stream(0, 2, renderer.get());
	while (pa->stream_is_active()) { Pa_Sleep(100); }

	return 0;
}
