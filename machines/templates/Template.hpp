#pragma once

#include <renderer/Renderable.hpp>
#include <input/Touchable.hpp>
#include <boost/utility.hpp>

class Template : boost::noncopyable {
	public:
		Renderable * get_renderable() const { return renderable_; }
		Touchable * get_touchable() const { return touchable_; }
		
		virtual ~Template() {
			delete touchable_;
			delete renderable_;
		}

	protected:
		Template(Renderable * renderable, Touchable * touchable) :
			renderable_(renderable),
			touchable_(touchable)
		{ }

	private:
		Renderable * renderable_;
		Touchable * touchable_;
};
