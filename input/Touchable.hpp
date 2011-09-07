#pragma once

#include <math/Rectangle.hpp>
#include <math/Vec.hpp>

class Touchable {
	public:
		virtual ~Touchable() { }

		Touchable(Rectangle<float> const & bounding_rect) :
			bounding_rect_(bounding_rect) { }

		Rectangle<float> const & get_bounding_rect() { return bounding_rect_; }

		virtual void handle_move_start(Vec2 const & start) { }
		virtual void handle_move_move(Vec2 const & loc) { }
		virtual void handle_move_end(Vec2 const & end) { }
		virtual void handle_single_tap(Vec2 const & loc) { }
	
		void set_bounding_rect(Rectangle<float> const & rect)
			{ bounding_rect_ = rect; }

	protected:
		Touchable() { }

	private:
		Rectangle<float> bounding_rect_;
};
