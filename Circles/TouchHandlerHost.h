#import <Foundation/Foundation.h>
#include <math/Matrix.hpp>

class TouchHandler;

@interface TouchHandlerHost : NSObject {
	@private
		TouchHandler * touch_handler_;
		Matrix<3, 3, float> touch_transform_;
}

- (void)setTouchHandler:(TouchHandler *)touch_handler;
- (void)setTouchTransform:(Matrix<3, 3, float>)touch_transform;

- (void)handlePinch:(UIPinchGestureRecognizer *)gestureRecognizer;
- (void)handleOneFingerPan:(UIPanGestureRecognizer *)gestureRecognizer;
- (void)handleTwoFingerPan:(UIPanGestureRecognizer *)gestureRecognizer;
- (void)handleTap:(UITapGestureRecognizer *)gestureRecognizer;

@end
