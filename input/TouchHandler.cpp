#include <input/TouchHandler.hpp>
#include <shared/erase_if.hpp>
#include <boost/range/algorithm.hpp>

#include <iostream>

struct TouchHandlerAccess {
	template <bool (Touchable::*f)(TouchHandler * handler, Vec2 const &)>
	struct TouchHandlerWrapper {
		TouchHandlerWrapper(TouchHandler * handler, Vec2 const & loc, bool force)
			: handler_(handler), loc_(loc), force_(force) { }

		bool operator () (TouchHandler::TouchableInfo const * info) const {
			return (*this)(const_cast<TouchHandler::TouchableInfo &>(*info));
		}

		bool operator () (TouchHandler::TouchableInfo const & info) const {
			return (*this)(const_cast<TouchHandler::TouchableInfo &>(info));
		}
		
		bool operator () (TouchHandler::TouchableInfo & info) const {
			// Don't do anything stupid in here to the touchable pointer!!
			if (handler_->current_event_ == info.event_counter) { return false; }
			info.event_counter = handler_->current_event_;

			Touchable * touchable = info.touchable;
			Rectangle<float> const & rect = touchable->get_bounding_rect();
			if (!rect.contains(loc_) && !force_) { return false; }
			if ((touchable->*f)(handler_, loc_)) { return false; }

			if (info.is_owned_by_touch_handler)
				{ handler_->to_delete_.push_back(info.touchable); }

			handler_->erase_always_notified(&info);

			return true;
		}

		private:
			TouchHandler * handler_;
			Vec2 loc_;
			bool force_;
	};

	void delete_pending(TouchHandler * handler) {
		boost::for_each(handler->to_delete_, Deleter());
		handler->to_delete_.clear();
	}
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
}

void TouchHandler::handle_move_start(Vec2 const & start) {
	++current_event_;
	boost::for_each(always_notified_, TouchHandlerAccess::TouchHandlerWrapper<&Touchable::handle_move_start>(this, start, true));
	erase_if(touchables_, TouchHandlerAccess::TouchHandlerWrapper<&Touchable::handle_move_start>(this, start, false));
	handle_rendezvous();
	TouchHandlerAccess::delete_pending(this);
}

void TouchHandler::handle_move_move(Vec2 const & loc) {
	++current_event_;
	boost::for_each(always_notified_, TouchHandlerAccess::TouchHandlerWrapper<&Touchable::handle_move_move>(this, loc, true));
	erase_if(touchables_, TouchHandlerAccess::TouchHandlerWrapper<&Touchable::handle_move_move>(this, loc, false));
	handle_rendezvous();
	TouchHandlerAccess::delete_pending(this);
}

void TouchHandler::handle_move_end(Vec2 const & end) {
	++current_event_;
	boost::for_each(always_notified_, TouchHandlerAccess::TouchHandlerWrapper<&Touchable::handle_move_end>(this, end, true));
	erase_if(touchables_, TouchHandlerAccess::TouchHandlerWrapper<&Touchable::handle_move_end>(this, end, false));
	handle_rendezvous();
	TouchHandlerAccess::delete_pending(this);
}

void TouchHandler::handle_single_tap(Vec2 const & loc) {
	++current_event_;
	boost::for_each(always_notified_, TouchHandlerAccess::TouchHandlerWrapper<&Touchable::handle_single_tap>(this, loc, true));
	erase_if(touchables_, TouchHandlerAccess::TouchHandlerWrapper<&Touchable::handle_single_tap>(this, loc, false));
	handle_rendezvous();
	TouchHandlerAccess::delete_pending(this);
}
