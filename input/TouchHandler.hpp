#pragma 

#include <input/Touchable.hpp>
#include <memory>
#include <set>
#include <math/Vec.hpp>

class ApplicationEngine;

class TouchHandler {
	// TODO: TouchableContainer should be a data structure that lets us quickly
	// find bounding boxes that intersect touch events, and lets us update
	// bounding boxes easily while Touchables are moving.
	typedef std::set<Touchable*> TouchableContainer;
	typedef TouchableContainer::iterator iterator;

	public:
		TouchHandler(ApplicationEngine * app_engine) :
			app_engine_(app_engine) { }

		~TouchHandler();

		bool insert_touchable(Touchable * touchable) {
			return touchables_.insert(touchable).second;
		}

		bool erase_touchable(Touchable * touchable) {
			iterator it = touchables_.find(touchable);
			if (it == touchables_.end()) { return false; }

			std::auto_ptr<Touchable> ptr(*it);
			touchables_.erase(it);
			return true;
		}

		void handle_move_start(Vec2 const & start);
		void handle_move_move(Vec2 const & loc, Vec2 const & );
		void handle_move_end(Vec2 const & end);

	private:
		ApplicationEngine * app_engine_;
		TouchableContainer touchables_;
};
