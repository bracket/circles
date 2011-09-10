#include <input/TouchHandler.hpp>
#include <shared/erase_if.hpp>

#include <iostream>

namespace {
	template <bool (Touchable::*f)(Vec2 const &)>
	struct TouchHandlerWrapper {
		TouchHandlerWrapper(Vec2 const & loc) : loc_(loc) { }
		
		bool operator () (Touchable * touchable) const {
			Rectangle<float> const & rect = touchable->get_bounding_rect();
			if (!rect.contains(loc_)) { return false; }
			return !(touchable->*f)(loc_);
		}

		private:
			Vec2 loc_;
	};
}

void TouchHandler::handle_move_start(Vec2 const & start) {
	erase_if(touchables_, TouchHandlerWrapper<&Touchable::handle_move_start>(start));
}

void TouchHandler::handle_move_move(Vec2 const & loc) {
	erase_if(touchables_, TouchHandlerWrapper<&Touchable::handle_move_move>(loc));
}

void TouchHandler::handle_move_end(Vec2 const & end) {
	erase_if(touchables_, TouchHandlerWrapper<&Touchable::handle_move_end>(end));
}

void TouchHandler::handle_single_tap(Vec2 const & loc) {
	erase_if(touchables_, TouchHandlerWrapper<&Touchable::handle_single_tap>(loc));
}
