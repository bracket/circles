#import "TouchHandlerHost.h"

@implementation TouchHandlerHost

- (void)setApplicationEngine:(ApplicationEngine *)app_engine {
	app_engine_ = app_engine;
}

- (void)handlePinch:(UIPinchGestureRecognizer *)gestureRecognizer {
	UIGestureRecognizerState state = [gestureRecognizer state];
    if (state == UIGestureRecognizerStateBegan || state == UIGestureRecognizerStateChanged) {
		// do stuff
        [gestureRecognizer setScale:1];
	}
}

- (void)handleOneFingerPan:(UIPanGestureRecognizer *)gestureRecognizer {
}

- (void)handleTwoFingerPan:(UIPanGestureRecognizer *)gestureRecognizer {
}

- (void)handleTap:(UITapGestureRecognizer *)gestureRecognizer {
}

@end
