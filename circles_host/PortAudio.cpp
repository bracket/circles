#include <boost/range/algorithm.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/utility.hpp>
#include <circles_host/PortAudio.hpp>
#include <cmath>
#include <iostream>
#include <machine_graph/MachineThread.hpp>
#include <memory>
#include <portaudio.h>

namespace {
	const double two_pi = 8.0 * std::atan(1.0), hz = two_pi / 44.1e3;

	void log_pa_error(PaError err) {
		std::cerr << Pa_GetErrorText(err) << std::endl;
	}

	bool log_if_error(PaError err) {
		if (err == paNoError) { return false; }

		log_pa_error(err);
		return true;
	}

	bool start_or_free(PaStream * stream) {
		if (log_if_error(Pa_StartStream(stream))) { return false; }

		Pa_Terminate();
		return true;
	}
}

PortAudio::~PortAudio() {
	if (stream_) { 
		log_if_error(Pa_StopStream(stream_));
		log_if_error(Pa_CloseStream(stream_));
	}

	Pa_Terminate();
}

namespace {
	PaStreamParameters construct_stream_parameters(int device_index, int channel_count, bool input)
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

	int callback(
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
}

bool PortAudio::start_stream(int input_device_index, int output_device_index,
	PortAudioRenderer * renderer)
{
	PaStreamParameters in_params = construct_stream_parameters(input_device_index, 2, true),
		out_params = construct_stream_parameters(output_device_index, 2, false);

	PaStream * ptr;

	PaError err = Pa_OpenStream(
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
	if (!start_or_free(ptr)) { return false; }

	return true;
}

bool PortAudio::stream_is_active() const { return Pa_IsStreamActive(stream_); }

bool PortAudio::init() {
	if (log_if_error(Pa_Initialize())) { return false; }
	return true;
}

namespace {
	struct MachineThreadRenderer : PortAudioRenderer {
		typedef MachineThread::BlockQueue BlockQueue;

		MachineThreadRenderer(MachineThread * machine_thread)
			: machine_thread_(machine_thread) { }
		
		int operator () (void const * input,
			void * output,
			unsigned long frame_count,
			PaStreamCallbackTimeInfo const * time_info,
			PaStreamCallbackFlags flags)
		{
			float * left_channel = reinterpret_cast<float*>(output);
			MachineThread::BlockQueue * queue = machine_thread_->get_block_queue();

			static boost::optional<BlockType *> block;
			static BlockType::iterator it, end = it;

			unsigned int copied = 0;
			while (copied < frame_count) {
				if (it == end) {
					if (block) { delete *block; block = boost::none; }
					if (!queue->shift(block)) { break; }

					it = (*block)->channel_begin(0);
					end = (*block)->channel_end(0);
				}

				for (; it != end && copied < frame_count; ++copied) {
					*left_channel++ = *it;
					*left_channel++ = *it++;
				}
			}

			for (; copied < frame_count; ++copied) { *left_channel++ = 0; }

			return paContinue;
		}

		private:
			MachineThread * machine_thread_;
	};
}

PortAudioRenderer * construct_machine_thread_renderer(MachineThread * machine_thread) {
	return new MachineThreadRenderer(machine_thread);
}

/*
int weasel() {
	auto pa = PortAudio::construct();
	if (!pa) { return 1; }

	boost::scoped_ptr<PortAudioRenderer> renderer(new PlayRenderer());

	pa->start_stream(0, 2, renderer.get());
	while (pa->stream_is_active()) { Pa_Sleep(100); }

	return 0;
}
*/
