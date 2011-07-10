#include <cmath>
#include <machines/Machine.hpp>
#include <machines/MachineFactory.hpp>

namespace {
    const double two_pi = 8.0 * std::atan(1.0);
    const int sample_rate = 44100;
}

class SineMachine : public Machine {
	public:
		SineMachine(double frequency = 440.0) :
			current_phase_(0.0),
			phase_delta_(frequency * two_pi / sample_rate)
		{ }

		BlockType * render() {
			BlockType * out = new BlockType();
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
	Machine * constructor() { return new SineMachine(); }

	struct registrar {
		registrar() {
			get_machine_factory().register_constructor("SineMachine", constructor);
		}
	};

	registrar r;
}
