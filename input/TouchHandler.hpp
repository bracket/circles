#pragma 

#include <algorithm>
#include <boost/range/algorithm.hpp>
#include <input/Touchable.hpp>
#include <math/Vec.hpp>
#include <memory>
#include <set>
#include <vector>

#include <iostream>

class ApplicationEngine;

class TouchHandler {
	struct TouchableInfo {
		Touchable * touchable;
		unsigned int event_counter;
		bool is_owned_by_touch_handler;
    };

	struct TouchableInfoCompare {
		template <class L, class R>
		bool operator() (L const & left, R const & right) const {
			return extract(left) < extract(right);
		}

		private:
			Touchable * extract(Touchable * touchable) const { return touchable; }

			Touchable * extract(TouchableInfo const & info) const { return info.touchable; }
	};

	typedef std::set<TouchableInfo, TouchableInfoCompare> TouchableContainer;

	typedef std::pair<Vec2, Touchable *> RendezvousPair;
	typedef std::vector<RendezvousPair> RendezvousContainer;

	typedef std::vector<Touchable*> ToDeleteContainer;
	typedef std::vector<TouchableInfo *> AlwaysNotifiedContainer;

	typedef TouchableContainer::iterator iterator;

	struct RendezvousCompare {
		template <class L, class R>
		bool operator () (L const & left, R const & right) {
			return cmp(extract(left), extract(right));
		}

		private:
			bool cmp(Vec2 const & left, Vec2 const & right) const {
				if (left.x() < right.x()) { return true; }
				if (right.x() < left.x()) { return false; }

				return left.y() < right.y();
			}

			Vec2 const & extract(Vec2 const & v) const { return v; }

			Vec2 const & extract(RendezvousPair const & p) const { return p.first; }
	};

	public:
		friend struct TouchHandlerAccess;

		TouchHandler(ApplicationEngine * app_engine) :
			app_engine_(app_engine),
			current_event_(0) { }

		bool insert_touchable(Touchable * touchable, bool is_owned_by_touch_handler) {
			TouchableInfo info = { touchable, current_event_, is_owned_by_touch_handler };

			if (touchables_.insert(info).second) { return true; }

			return false;
		}

		bool erase_touchable(Touchable * touchable) {
			TouchableInfo info = { touchable, 0, false };
			iterator it = touchables_.find(info);
			if (it == touchables_.end()) { return false; }

			if (it->is_owned_by_touch_handler) { delete it->touchable; }
			touchables_.erase(it);
			erase_always_notified(&*it);

			return true;
		}

		void insert_rendezvous(Vec2 const & pt, Touchable * touchable) {
			typedef RendezvousContainer::iterator iterator;

			iterator it = std::upper_bound(rendezvous_.begin(), rendezvous_.end(), pt, RendezvousCompare());
			rendezvous_.insert(it, RendezvousPair(pt, touchable));
		}

		void handle_rendezvous();

		void handle_move_start(Vec2 const & start);
		void handle_move_move(Vec2 const & loc);
		void handle_move_end(Vec2 const & end);
		void handle_single_tap(Vec2 const & loc);

		bool register_always_notified(Touchable * touchable) {
			TouchableInfo info  = { touchable, 0, false };
			iterator it = touchables_.find(info);
			if (it == touchables_.end()) { return false; }

			return insert_always_notified(&*it);
		}

		bool erase_always_notified(Touchable * touchable) {
			TouchableInfo info = { touchable, 0, false };
			iterator it = touchables_.find(info);
			if (it == touchables_.end()) { return false; }

			return erase_always_notified(&*it);
		}

	private:
		bool insert_always_notified(TouchableInfo const * info) {
			typedef AlwaysNotifiedContainer::iterator aiterator;

			std::pair<aiterator, aiterator> p = boost::equal_range(always_notified_, info);
			if (p.first != p.second) { return false; }

			always_notified_.insert(p.second, const_cast<TouchableInfo *>(info));
			return true;
		}

		bool erase_always_notified(TouchableInfo const * info) {
			typedef AlwaysNotifiedContainer::iterator aiterator;

			std::pair<aiterator, aiterator> p = boost::equal_range(always_notified_, info);
			if (p.first == p.second) { return false; }

			always_notified_.erase(p.first);
			return true;
		}

		ApplicationEngine * app_engine_;
		TouchableContainer touchables_;
		RendezvousContainer rendezvous_;
		ToDeleteContainer to_delete_;
		AlwaysNotifiedContainer always_notified_;

		unsigned int current_event_;
};
