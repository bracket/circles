#import <OpenGLES/EAGL.h>
#import <QuartzCore/CADisplayLink.h>
#import <QuartzCore/QuartzCore.h>
#import <renderer/RenderingEngine.hpp>
#import <UIKit/UIKit.h>

@interface GLView : UIView <UIGestureRecognizerDelegate> {
    @private
        EAGLContext * gl_context_;
		RenderingEngine * rendering_engine_;
}

+ (Class)layerClass;
- (void)drawView:(CADisplayLink *) displayLink;

@end
