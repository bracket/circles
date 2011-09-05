#include <machines/SoundMachine.hpp>
#include <machines/MachineFactory.hpp>
#include <machines/MachineGraph.hpp>

class SilentMachine : public SoundMachine {
	public:
		SilentMachine(MachineGraph * graph) : SoundMachine(graph) { }

		BlockType * render() {
			BlockType * out = this->alloc_block();

			for (int ch = 0; ch < out->channel_count(); ++ch) {
				BlockType::iterator it = out->channel_begin(ch),
					end = out->channel_end(ch);

				for (; it != end; ++it) { *it = 0.0f; }
			}

			return out;
		}
};

namespace {
	SoundMachine * constructor(MachineGraph * graph) { return new SilentMachine(graph); }

	struct registrar {
		registrar() {
			get_machine_factory().register_constructor("SilentMachine", constructor);
		}
	};

	registrar r;
}
