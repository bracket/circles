#pragma once

#include <renderer/Renderable.hpp>
#include <touchable/Touchable.hpp>

class Template {
	public:
		Renderable * get_renderable() const { return renderable_; }
		Touchable * get_touchable() const { return touchable_; }

	protected:
		Template(Renderable * renderable, Touchable * touchable) :
			renderable_(renderable),
			touchable_(touchable)
		{ }

	private:
		Renderable * renderable_;
		Touchable * touchable_;
};
