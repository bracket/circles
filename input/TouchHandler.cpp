#include <input/TouchHandler.hpp>
#include <shared/erase_if.hpp>
#include <boost/range/algorithm.hpp>

#include <iostream>

struct TouchHandlerAccess {
	template <bool (Touchable::*f)(TouchHandler * handler, Vec2 const &)>
	struct TouchHandlerWrapper {
		TouchHandlerWrapper(TouchHandler * handler, Vec2 const & loc)
			: handler_(handler), loc_(loc) { }
		
		bool operator () (TouchHandler::TouchableInfo const & info) const {
			Touchable * touchable = info.touchable;
			Rectangle<float> const & rect = touchable->get_bounding_rect();
			if (!rect.contains(loc_)) { return false; }
			if ((touchable->*f)(handler_, loc_)) { return false; }

			if (info.is_owned_by_touch_handler) {
				handler_->touchables_to_delete_.push_back(info.touchable);
			}

			return true;
		}

		private:
			TouchHandler * handler_;
			Vec2 loc_;
	};
};

namespace {
	struct Deleter {
		template <class T>
		void operator () (T * t) const { delete t; }
	};
}

void TouchHandler::handle_rendezvous() {
	typedef RendezvousContainer::iterator iterator;
	iterator current = rendezvous_.begin(), it;

	for (; current < rendezvous_.end(); current = it) {
		for (it = current + 1; it < rendezvous_.end() && current->first == it->first; ++it) {
			current->second->handle_rendezvous(it->second);
			it->second->handle_rendezvous(current->second);
		}
	}

	rendezvous_.clear();

	boost::for_each(touchables_to_delete_, Deleter());
	touchables_to_delete_.clear();
}

void TouchHandler::handle_move_start(Vec2 const & start) {
	erase_if(touchables_, TouchHandlerAccess::TouchHandlerWrapper<&Touchable::handle_move_start>(this, start));
	handle_rendezvous();
}

void TouchHandler::handle_move_move(Vec2 const & loc) {
	erase_if(touchables_, TouchHandlerAccess::TouchHandlerWrapper<&Touchable::handle_move_move>(this, loc));
	handle_rendezvous();
}

void TouchHandler::handle_move_end(Vec2 const & end) {
	erase_if(touchables_, TouchHandlerAccess::TouchHandlerWrapper<&Touchable::handle_move_end>(this, end));
	handle_rendezvous();
}

void TouchHandler::handle_single_tap(Vec2 const & loc) {
	erase_if(touchables_, TouchHandlerAccess::TouchHandlerWrapper<&Touchable::handle_single_tap>(this, loc));
	handle_rendezvous();
}
