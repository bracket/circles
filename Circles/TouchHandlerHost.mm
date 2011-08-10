#import <TouchHandlerHost.h>
#include <input/TouchHandler.hpp>

namespace {
	inline Vec2 transform_pt(Matrix<3, 3, float> const & T, CGPoint const & p) {
		Vec3 t = Vec3(p.x, p.y, 1.0f) * T;
		return Vec2(t.x() / t.z(), t.y() / t.z());
	}

	inline Vec2 transform_vec(Matrix<3, 3, float> const & T, CGPoint const & p) {
		Vec3 t = Vec3(p.x, p.y, 0.0f) * T;
		return Vec2(t.x() / t.z(), t.y() / t.z());
	}
}

@implementation TouchHandlerHost

- (void)setTouchHandler:(TouchHandler *)touch_handler {
  	touch_handler_ = touch_handler;
}

- (void) setTouchTransform:(Matrix<3, 3, float>)touch_transform {
  	touch_transform_ = touch_transform;
}

- (void)handlePinch:(UIPinchGestureRecognizer *)gestureRecognizer {
	UIGestureRecognizerState state = [gestureRecognizer state];
    if (state == UIGestureRecognizerStateBegan || state == UIGestureRecognizerStateChanged) {
		// do stuff
        [gestureRecognizer setScale:1];
	}
}

- (void)handleOneFingerPan:(UIPanGestureRecognizer *)gestureRecognizer {
	UIGestureRecognizerState state = [gestureRecognizer state];

	if (state == UIGestureRecognizerStateBegan) {
		CGPoint pt = [ gestureRecognizer locationInView:nil ];
		touch_handler_->handle_move_start(transform_pt(touch_transform_, pt));
	}
	else if (state == UIGestureRecognizerStateChanged) {
		CGPoint loc = [ gestureRecognizer locationInView:nil ];
		CGPoint delta = [ gestureRecognizer translationInView:nil ];
		touch_handler_->handle_move_move(
			transform_pt(touch_transform_, loc),
			transform_vec(touch_transform_, delta)
		);
	}
	else if (state == UIGestureRecognizerStateEnded) {
		CGPoint pt = [ gestureRecognizer locationInView:nil ];
		touch_handler_->handle_move_end(transform_pt(touch_transform_, pt));
	}
}
 
- (void)handleTwoFingerPan:(UIPanGestureRecognizer *)gestureRecognizer {
}

- (void)handleTap:(UITapGestureRecognizer *)gestureRecognizer {
}

@end
