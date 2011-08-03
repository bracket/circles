#import <Foundation/Foundation.h>

class ApplicationEngine;

@interface TouchHandlerHost : NSObject {
	@private
		ApplicationEngine * app_engine_;
}

- (void)setApplicationEngine:(ApplicationEngine *)app_engine;
- (void)handlePinch:(UIPinchGestureRecognizer *)gestureRecognizer;
- (void)handleOneFingerPan:(UIPanGestureRecognizer *)gestureRecognizer;
- (void)handleTwoFingerPan:(UIPanGestureRecognizer *)gestureRecognizer;
- (void)handleTap:(UITapGestureRecognizer *)gestureRecognizer;

@end
