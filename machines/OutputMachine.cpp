#include <machines/SoundMachine.hpp>
#include <machines/MachineFactory.hpp>
#include <machines/MachineGraph.hpp>

class OutputMachine : public SoundMachine {
	public:
		typedef SoundMachine base;

		OutputMachine(MachineGraph * graph) : SoundMachine(graph) { }

		BlockType * render() {
			base::input_iterator it = this->input_begin(), end = this->input_end();
			if (it == end) { return 0; }

			BlockType * out = (*it++)->render();
			for (; it != end; ++it) {
				BlockType * agg = (*it)->render();
				if (!agg) { continue; }
				*out += *agg;
			}

			return out;
		}
};

namespace {
	SoundMachine * constructor(MachineGraph * graph) { return new OutputMachine(graph); }

	struct registrar {
		registrar() {
			get_machine_factory().register_constructor("OutputMachine", constructor);
		};
	};

	registrar r;
}
