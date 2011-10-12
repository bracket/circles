#include <app_engine/ApplicationEngine.hpp>
#include <machines/LinkMovingTouchable.hpp>
#include <math/intersection.hpp>
#include <math/Ray.hpp>
#include <renderer/RenderingEngine.hpp>

LinkMovingTouchable::~LinkMovingTouchable() {
	ApplicationEngine::get()->erase_renderable(renderable_);
	delete renderable_;
}


bool LinkMovingTouchable::handle_move_move(TouchHandler *, Vec2 const & loc) {
	RenderingEngine * rendering_engine = ApplicationEngine::get()->get_rendering_engine();
	Ray<3, float> ray = rendering_engine->get_camera().unproject_device_independent(loc);

	boost::optional<Vec3> pos = intersection(ray, Plane<3, float>(Vec3(0, 0, 1), 0));
	if (!pos) { return false; }

	renderable_->set_position(*pos);
	return true;
}

bool LinkMovingTouchable::handle_move_end(TouchHandler * handler, Vec2 const & loc) {
	handler->insert_rendezvous(loc, this);
	return false;
}

void LinkMovingRenderable::render(RenderingEngine const * rendering_engine) {
	square_.render(rendering_engine, get_frame());

	if (touchable_) {
		touchable_->set_bounding_rectangle(
			square_.get_bounding_rectangle(rendering_engine, get_frame())
		);
	}
}
