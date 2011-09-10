#pragma once

#include <math/Rectangle.hpp>
#include <math/Vec.hpp>

class Touchable {
	public:
		virtual ~Touchable() { }

		Rectangle<float> const & get_bounding_rect() { return bounding_rect_; }

		// NOTE: TouchHandlers should return true to indicate that the
		// Touchable should remain in the container after being handled.

		virtual bool handle_move_start(Vec2 const & start) { return true; }
		virtual bool handle_move_move(Vec2 const & loc) { return true; }
		virtual bool handle_move_end(Vec2 const & end) { return true; }
		virtual bool handle_single_tap(Vec2 const & loc) { return true; }
	
		void set_bounding_rectangle(Rectangle<float> const & rect)
			{ bounding_rect_ = rect; }

	protected:
		Touchable() { }

	private:
		Rectangle<float> bounding_rect_;
};
