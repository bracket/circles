#pragma once

#include <input/Touchable.hpp>
#include <renderer/Renderable.hpp>
#include <Square.hpp>

class ApplicationEngine;
class RenderingEngine;
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

		void render(RenderingEngine const * rendering_engine);

	private:
		Square square_;
		LinkMovingTouchable * touchable_;
};

