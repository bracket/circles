#include <input/TouchHandler.hpp>

#include <iostream>

TouchHandler::~TouchHandler() {
	for (iterator it = touchables_.begin(); it != touchables_.end(); ++it)
		{ delete *it; }
}

void TouchHandler::handle_move_start(Vec2 const & start) {
	std::cout << "start " << start << std::endl;
	for (iterator it = touchables_.begin(); it != touchables_.end(); ++it) {
		Rectangle<float> const & rect = (*it)->get_bounding_rect();
		std::cout << rect << std::endl;
		if (rect.contains(start)) { (*it)->handle_touch_start(start); }
	}
}

void TouchHandler::handle_move_move(Vec2 const & loc, Vec2 const & delta) {
	for (iterator it = touchables_.begin(); it != touchables_.end(); ++it) {
		Rectangle<float> const & rect = (*it)->get_bounding_rect();
		if (rect.contains(loc)) { (*it)->handle_touch_move(loc, delta); }
	}
}

void TouchHandler::handle_move_end(Vec2 const & end) {
	for (iterator it = touchables_.begin(); it != touchables_.end(); ++it) {
		Rectangle<float> const & rect = (*it)->get_bounding_rect();
		if (rect.contains(end)) { (*it)->handle_touch_end(end); }
	}
}
