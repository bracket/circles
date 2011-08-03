#import <OpenGLES/EAGL.h>
#import <QuartzCore/CADisplayLink.h>
#import <QuartzCore/QuartzCore.h>
#import <renderer/RenderingEngine.hpp>
#import "TouchHandlerHost.h"
#import <UIKit/UIKit.h>

class ApplicationEngine;

@interface GLView : UIView <UIGestureRecognizerDelegate> {
    @private
        EAGLContext * gl_context_;
		ApplicationEngine * app_engine_;
}

+ (Class)layerClass;
- (void)drawView:(CADisplayLink *) displayLink;
- (void)setApplicationEngine:(ApplicationEngine *)app_engine;
- (void)allocateFramebufferStorage;
- (void)addGestureRecognizers:(TouchHandlerHost *)handler;

@end
