#include <cmath>
#include <machines/SoundMachine.hpp>
#include <machines/MachineFactory.hpp>

namespace {
    const double two_pi = 8.0 * std::atan(1.0);
    const int sample_rate = 44100;
}

class SineMachine : public SoundMachine {
	public:
		SineMachine(MachineGraph * graph, double frequency = 440.0) :
			SoundMachine(graph),
			current_phase_(0.0),
			phase_delta_(frequency * two_pi / sample_rate)
		{ }

		BlockType * render() {
			BlockType * out = this->alloc_block();
            BlockType::iterator end = out->channel_end(0);

			for (BlockType::iterator it = out->channel_begin(0); it != end; ++it) {
				*it = std::sin(current_phase_);
				current_phase_ += phase_delta_;
			}

			current_phase_ -= two_pi * static_cast<int>(current_phase_ / two_pi);

			for (int ch = 1; ch < out->channel_count(); ++ch) {
				out->assign_channel(ch, out->channel_begin(0), out->channel_end(0));
			}

			return out;
		}
	
	private:
		double current_phase_, phase_delta_;
};

namespace {
	SoundMachine * constructor(MachineGraph * graph) { return new SineMachine(graph); }

	struct registrar {
		registrar() {
			get_machine_factory().register_constructor("SineMachine", constructor);
		}
	};

	registrar r;
}
