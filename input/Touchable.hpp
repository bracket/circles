#pragma once

#include <math/Rectangle.hpp>
#include <math/Vec.hpp>

class Touchable {
	public:
		virtual ~Touchable() { }

		Touchable(Rectangle<float> const & bounding_rect) :
			bounding_rect_(bounding_rect) { }

		Rectangle<float> const & get_bounding_rect() { return bounding_rect_; }

		virtual void handle_touch_start(Vec2 const & start) { }
	 	virtual void handle_touch_move(Vec2 const & loc, Vec2 const & delta) { }
		virtual void handle_touch_end(Vec2 const & end) { }
	
    void set_bounding_rect(Rectangle<float> const & rect)
			{ bounding_rect_ = rect; }

	protected:
		Touchable() { }

	private:
		Rectangle<float> bounding_rect_;
};
