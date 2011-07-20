#include <machines/Machine.hpp>
#include <machines/MachineGraph.hpp>

BlockType * Machine::alloc_block() { return this->graph_->alloc_block(); }

void Machine::free_block(BlockType * block) { this->graph_->free_block(block); }
