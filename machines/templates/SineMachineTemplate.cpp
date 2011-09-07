#include <machines/templates/Template.hpp>

namespace {
	class TemplateTouchable : public Touchable {
		void handle_move_start(Vec2 const & start) {
			// Create a touchable that'll make us a Sine machine obviously
		}
	};

	class TemplateMovingTouchable : public Touchable {
		void handle_move_move(Vec2 const & loc) {
			// move the touchable
		}
	};

	class TemplateRenderable : public Renderable {
	};

	class TemplateMovingRenderable : public Renderable {
	};
}

class SineMachineTemplate : public Template {
	public:
		SineMachineTemplate()

	private:
};
