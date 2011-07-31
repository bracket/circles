#import <OpenGLES/EAGL.h>
#import <QuartzCore/CADisplayLink.h>
#import <QuartzCore/QuartzCore.h>
#import <renderer/RenderingEngine.hpp>
#import <UIKit/UIKit.h>

class ApplicationEngine;

@interface GLView : UIView <UIGestureRecognizerDelegate> {
    @private
        EAGLContext * gl_context_;
		RenderingEngine * rendering_engine_;
		ApplicationEngine * app_engine_;
}

+ (Class)layerClass;
- (void)drawView:(CADisplayLink *) displayLink;
- (RenderingEngine *)getRenderingEngine;
- (void)setApplicationEngine:(ApplicationEngine *)app_engine;

@end
