#pragma once

#include <renderer/Renderable.hpp>
#include <input/Touchable.hpp>
#include <boost/utility.hpp>

class Template : boost::noncopyable {
	public:
		Template(Renderable * renderable, Touchable * touchable) :
			renderable_(renderable),
			touchable_(touchable)
		{ }
		
		~Template() {
			delete touchable_;
			delete renderable_;
		}

		Renderable * get_renderable() const { return renderable_; }
		Touchable * get_touchable() const { return touchable_; }

	private:
		Renderable * renderable_;
		Touchable * touchable_;
};
