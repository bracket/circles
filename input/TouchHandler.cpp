#include <input/TouchHandler.hpp>

#include <iostream>

namespace {
	template <void (Touchable::*f)(Vec2 const &), class Iterator>
	void apply_touch_event(Iterator begin, Iterator end, Vec2 const & loc) {
		for (; begin != end; ++begin) {
			Rectangle<float> const & rect = (*begin)->get_bounding_rect();
			if (rect.contains(loc)) { ((*begin)->*f)(loc); }
		}
	}
}

void TouchHandler::handle_move_start(Vec2 const & start) {
	apply_touch_event<&Touchable::handle_move_start>(touchables_.begin(), touchables_.end(), start);
}

void TouchHandler::handle_move_move(Vec2 const & loc) {
	apply_touch_event<&Touchable::handle_move_move>(touchables_.begin(), touchables_.end(), loc);
}

void TouchHandler::handle_move_end(Vec2 const & end) {
	apply_touch_event<&Touchable::handle_move_end>(touchables_.begin(), touchables_.end(), end);
}

void TouchHandler::handle_single_tap(Vec2 const & loc) {
	apply_touch_event<&Touchable::handle_single_tap>(touchables_.begin(), touchables_.end(), loc);
}
