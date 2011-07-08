#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <QuartzCore/QuartzCore.h>


@interface GLView : UIView {
    @private
        EAGLContext * gl_context_;
        float time_stamp_;
}

+ (Class)layerClass;
- (void)drawView:(CADisplayLink *) displayLink;

@end
