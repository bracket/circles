#include <machines/Machine.hpp>
#include <machines/MachineFactory.hpp>

class OutputMachine : public Machine {
	public:
		typedef Machine base;

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
	Machine * constructor() { return new OutputMachine(); }

	struct registrar {
		registrar() {
			get_machine_factory().register_constructor("OutputMachine", constructor);
		};
	};

	registrar r;
}
