#pragma 

#include <algorithm>
#include <input/Touchable.hpp>
#include <memory>
#include <set>
#include <math/Vec.hpp>
#include <vector>

class ApplicationEngine;

class TouchHandler {
	// TODO: TouchableContainer should be a data structure that lets us quickly
	// find bounding boxes that intersect touch events, and lets us update
	// bounding boxes easily while Touchables are moving.
	typedef std::set<Touchable*> TouchableContainer;

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
		TouchHandler(ApplicationEngine * app_engine) :
			app_engine_(app_engine) { }

		bool insert_touchable(Touchable * touchable) {
			return touchables_.insert(touchable).second;
		}

		bool erase_touchable(Touchable * touchable) {
			iterator it = touchables_.find(touchable);
			if (it == touchables_.end()) { return false; }

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
};
