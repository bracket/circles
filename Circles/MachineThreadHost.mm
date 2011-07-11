#import <AudioToolbox/Audiotoolbox.h>
#import <AudioUnit/AudioUnit.h>
#import <CoreFoundation/CFRunLoop.h> 
#import <MachineThreadHost.h>
#include <cmath>
#include <cstring>
#include <iostream>
#include <machines/MachineThread.hpp>
#include <vector>


/*! @file
MachineThreadHost is the OS specific class responsible for instantiating the
machines/MachineThread class and starting the separate thread for sound
rendering.  More accurately, its threadMain function is called in the
AppDelegate to spawn the new thread.  It's also responsible for starting audio
processing on the host OS and copying the output from the MachineThread's
buffers to the host OS's output audio buffers, performing conversions as
necessary.
*/

// TODO: This is kind of an ugly macro.  We probably need a slightly better error
// reporting convention so we can collect errors out of GL too.
#define RETURN_IF_ERR(s) if (log_os_status(s, __LINE__)) { return false; }

namespace {
	bool log_os_status(OSStatus error_code, int line) {
		if (error_code == noErr) { return false; }
        NSError *error = [NSError errorWithDomain:NSOSStatusErrorDomain
                                             code:error_code
                                         userInfo:nil];
        NSLog(@"Error at line %i: %@", line, [error description]);
		return true;
	}

	//! @brief Callback used by the RunLoop to ask MachineThread to prerender audio output.
	void fire_machine_thread_loop(CFRunLoopTimerRef timer, void * ptr) {
        if (!ptr) { return; }
        static_cast<MachineThread*>(ptr)->loop();
	} 

	// NOTE: iOS documentation says it uses 8.24 fixed point integer format for
	// sound output.  My ears tell me it's actually 16.16.
	template <class T>
	inline long int samplize(T const & t) {
		return static_cast<long int>(t * static_cast<T>(1 << 15));
	}

	//! @brief Callback used by the iOS AudioUnit system to copy the MachineThread's buffers to output.
	OSStatus machine_thread_host_au_callback(
		void * inRefCon,
		AudioUnitRenderActionFlags * flags,
		AudioTimeStamp const * timestamp,
		UInt32 bus_number,
		UInt32 frame_count,
		AudioBufferList * data
	)
	{
		MachineThread * machine_thread = static_cast<MachineThread*>(inRefCon);
		MachineThread::BlockQueue * queue = machine_thread->get_block_queue();
		AudioUnitSampleType * left_channel = static_cast<AudioUnitSampleType*>(data->mBuffers[0].mData);

		static boost::optional<BlockType *> out;
		static BlockType::iterator it, end = it;

		UInt32 copied = 0;
		while (copied < frame_count) {
			if (it == end) {
				if (out) { delete *out; out = boost::none; } // NOTE: We want to recycle this.
				if (!queue->shift(out)) { break; }
				it = (*out)->channel_begin(0);
				end = (*out)->channel_end(0);
			}
			
			for (; it != end && copied < frame_count; ++copied)
				{ *left_channel++ = samplize(*it++); }
		}

		return 0;
	}

	AudioComponentDescription make_output_unit_description() {
		AudioComponentDescription out;
		std::memset(&out, 0, sizeof(out));
		out.componentType = kAudioUnitType_Output;
		out.componentSubType = kAudioUnitSubType_RemoteIO;
		out.componentManufacturer = kAudioUnitManufacturer_Apple;

		return out;
	}

	bool start_audio_unit_processing(MachineThread * machine_thread) {
		OSStatus result;

		AUGraph graph;
		result = NewAUGraph(&graph);
		RETURN_IF_ERR(result);

		AudioComponentDescription out_desc = make_output_unit_description();

		AUNode node;
		result = AUGraphAddNode(graph, &out_desc, &node);
		RETURN_IF_ERR(result);

		result = AUGraphOpen(graph);
		RETURN_IF_ERR(result);

		AudioUnit unit;
		result = AUGraphNodeInfo(graph, node, 0, &unit);
		RETURN_IF_ERR(result);

		const double sample_rate = 44100.0;

		// NOTE: I don't think we actually have control over this property.  I
		// think we have to query it from the AudioSession, but I don't quite
		// know how to do that yet.  In either case there's apparently
		// performance implication for not matching the hardware's sample rate,
		// so we should do it anyway.
		result = AudioUnitSetProperty(
			unit,
			kAudioUnitProperty_SampleRate,
			kAudioUnitScope_Input, 0,
			&sample_rate, sizeof(sample_rate)
		);
		RETURN_IF_ERR(result);

		AURenderCallbackStruct callback;
		std::memset(&callback, 0, sizeof(callback));
		callback.inputProc = &machine_thread_host_au_callback;
		callback.inputProcRefCon = machine_thread;

		result = AudioUnitSetProperty(
			unit,
			kAudioUnitProperty_SetRenderCallback,
			kAudioUnitScope_Input, 0,
			&callback, sizeof(callback)
		);
		RETURN_IF_ERR(result);

		result = AUGraphInitialize(graph);
		RETURN_IF_ERR(result);

		result = AUGraphStart(graph);
		RETURN_IF_ERR(result);

		return true;
	}
}

@implementation MachineThreadHost

-(void) threadMain {
	bool done = false;

	// NOTE: This should really be a property and done in the main thread,
	// since it's probably going to need a reference to the MachineThread.
	MachineThread * machine_thread = MachineThread::construct();
	if (!machine_thread) { return; }

	CFRunLoopTimerContext context;
	std::memset(&context, 0, sizeof(context));
	context.version = 0;
	context.info = machine_thread;

	// NOTE: We try and have the MachineThread render about .005 seconds ahead
	// of real-time.  This may need to be tuned, or dynamically handled by the
	// MachineThread instead.
	CFRunLoopTimerRef timer = CFRunLoopTimerCreate(
		kCFAllocatorDefault,
		CFAbsoluteTimeGetCurrent(),
		.005,
		0, 0,
		&fire_machine_thread_loop,
		&context
	);

	CFRunLoopAddTimer(CFRunLoopGetCurrent(), timer, kCFRunLoopDefaultMode);

	start_audio_unit_processing(machine_thread);

	while (!done) {
		SInt32 result = CFRunLoopRunInMode(kCFRunLoopDefaultMode, .5, YES);
		done = (result == kCFRunLoopRunStopped || result == kCFRunLoopRunFinished);
	}

	delete machine_thread;
}


@end
