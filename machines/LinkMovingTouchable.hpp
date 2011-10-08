#pragma once

#include <app_engine/ApplicationEngine.hpp>
#include <input/Touchable.hpp>
#include <math/Ray.hpp>
#include <renderer/Renderable.hpp>
#include <renderer/RenderingEngine.hpp>
#include <Square.hpp>
#include <math/intersection.hpp>

#include <iostream>

class LinkMovingTouchable;
class LinkMovingRenderable;

class LinkMovingTouchable : public Touchable {
	public:
		LinkMovingTouchable(Machine * machine) :
			start_machine_(machine)
		{ }
        
        ~LinkMovingTouchable();

		bool handle_move_move(TouchHandler *, Vec2 const & loc);
		bool handle_move_end(TouchHandler *, Vec2 const & loc);

		void set_renderable(LinkMovingRenderable * renderable) { renderable_ = renderable; }

		Machine * get_start_machine() const { return start_machine_; }

		TouchableTypeID get_touchable_type_id() const { return Touchable::TypeIDLinkMovingTouchable; }

	private:
		Machine * start_machine_;
		LinkMovingRenderable * renderable_;
};

class LinkMovingRenderable : public Renderable {
	public:
		LinkMovingRenderable(Program * program) :
			Renderable(program),
			square_(program),
			touchable_(0)
		{ }

		void set_touchable(LinkMovingTouchable * touchable) { touchable_ = touchable; }

		void render(RenderingEngine const * rendering_engine) {
			square_.render(rendering_engine, get_frame());

			if (touchable_) {
				touchable_->set_bounding_rectangle(
					square_.get_bounding_rectangle(rendering_engine, get_frame())
				);
			}
		}

	private:
		Square square_;
		LinkMovingTouchable * touchable_;
};

inline LinkMovingTouchable::~LinkMovingTouchable() {
	ApplicationEngine::get()->erase_renderable(renderable_);
	delete renderable_;
}


inline bool LinkMovingTouchable::handle_move_move(TouchHandler *, Vec2 const & loc) {
	RenderingEngine * rendering_engine = ApplicationEngine::get()->get_rendering_engine();
	Ray<3, float> ray = rendering_engine->unproject_device_independent(loc);

	boost::optional<Vec3> pos = intersection(ray, Plane<3, float>(Vec3(0, 0, 1), 0));
	if (!pos) { return false; }

	renderable_->set_position(*pos);
	return true;
}

inline bool LinkMovingTouchable::handle_move_end(TouchHandler * handler, Vec2 const & loc) {
	handler->insert_rendezvous(loc, this);
	return false;
}
