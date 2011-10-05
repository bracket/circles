#pragma 

#include <algorithm>
#include <input/Touchable.hpp>
#include <memory>
#include <set>
#include <math/Vec.hpp>
#include <vector>

class ApplicationEngine;

class TouchHandler {
	struct TouchableInfo {
		Touchable * touchable;
		unsigned int counter;
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
			app_engine_(app_engine) { }

		bool insert_touchable(Touchable * touchable, bool is_owned_by_touch_handler) {
			TouchableInfo info = { touchable, 0, is_owned_by_touch_handler };

			if (touchables_.insert(info).second) { return true; }
			if (is_owned_by_touch_handler) { delete touchable; }

			return false;
		}

		bool erase_touchable(Touchable * touchable) {
			TouchableInfo info = { touchable, 0, false };
			iterator it = touchables_.find(info);
			if (it == touchables_.end()) { return false; }

			if (it->is_owned_by_touch_handler) { delete it->touchable; }
			touchables_.erase(it);
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

	private:
		ApplicationEngine * app_engine_;
		TouchableContainer touchables_;
		RendezvousContainer rendezvous_;
		std::vector<Touchable *> touchables_to_delete_;
};
