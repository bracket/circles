#include <machine_graph/SoundMachine.hpp>
#include <machine_graph/MachineGraph.hpp>

BlockType * SoundMachine::alloc_block() { return this->graph_->alloc_block(); }

void SoundMachine::free_block(BlockType * block) { this->graph_->free_block(block); }
