#pragma once

#include <renderer/Renderable.hpp>
#include <input/Touchable.hpp>

class Machine {
	public:
		Renderable * get_renderable() const { return renderable_; }

		Touchable * get_touchable() const { return touchable_; }

		int get_target_machine_id() const { return target_machine_id_; }

	protected:
		Machine(Renderable * renderable, Touchable * touchable, int target_machine_id) :
			renderable_(renderable),
			touchable_(touchable), 
			target_machine_id_(target_machine_id)
		{ }

	private:
		Renderable * renderable_;
		Touchable * touchable_;
		int target_machine_id_;
};
