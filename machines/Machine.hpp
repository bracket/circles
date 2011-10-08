#pragma once

#include <renderer/Renderable.hpp>
#include <input/Touchable.hpp>
#include <shared/types.hpp>

class Machine {
	public:
		friend class MachineContainer;

		Renderable * get_renderable() const { return renderable_; }

		Touchable * get_touchable() const { return touchable_; }

		MachineID get_machine_id() const { return machine_id_; }

		Machine(Renderable * renderable, Touchable * touchable) :
			renderable_(renderable),
			touchable_(touchable)
		{ }

	private:
		void set_machine_id(MachineID id) { machine_id_ = id; }

		Renderable * renderable_;
		Touchable * touchable_;
		MachineID machine_id_;
};
