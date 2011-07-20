#include <machines/Machine.hpp>
#include <machines/MachineFactory.hpp>
#include <machines/MachineGraph.hpp>

class OutputMachine : public Machine {
	public:
		typedef Machine base;

		OutputMachine(MachineGraph * graph) : Machine(graph) { }

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
	Machine * constructor(MachineGraph * graph) { return new OutputMachine(graph); }

	struct registrar {
		registrar() {
			get_machine_factory().register_constructor("OutputMachine", constructor);
		};
	};

	registrar r;
}
