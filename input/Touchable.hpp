#pragma once

#include <boost/utility.hpp>
#include <math/Rectangle.hpp>
#include <math/Vec.hpp>

class TouchHandler;

class Touchable : boost::noncopyable {
	public:
		enum TouchableTypeID {
			TypeIDNone = 0,
			TypeIDLinkMovingTouchable,
		};

		virtual ~Touchable() { }

		Rectangle<float> const & get_bounding_rect() { return bounding_rect_; }

		// NOTE: TouchHandlers should return true to indicate that the
		// Touchable should remain in the container after being handled.

		virtual bool handle_move_start(TouchHandler * handler, Vec2 const & start) { return true; }
		virtual bool handle_move_move(TouchHandler * handler, Vec2 const & loc) { return true; }
		virtual bool handle_move_end(TouchHandler * handler, Vec2 const & end) { return true; }
		virtual bool handle_single_tap(TouchHandler * handler, Vec2 const & loc) { return true; }
	
		void set_bounding_rectangle(Rectangle<float> const & rect)
			{ bounding_rect_ = rect; }

		virtual void handle_rendezvous(Touchable * touchable) { }

		virtual TouchableTypeID get_touchable_type_id() const { return TypeIDNone; }

	protected:
		Touchable() { }

	private:
		Rectangle<float> bounding_rect_;
};
