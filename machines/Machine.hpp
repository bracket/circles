#pragma once

#include <machines/Block.hpp>
#include <vector>

typedef Block<float, 1, 1024> BlockType;

class Machine {
	typedef std::vector<Machine*> InputContainer;

	public:
		virtual BlockType * render();
 
	private:
		InputContainer inputs_;
};
