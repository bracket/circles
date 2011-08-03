#import "GLView.h"
#import <OpenGLES/ES2/gl.h>

#import <OpenGLES/EAGLDrawable.h>

#include <app_engine/ApplicationEngine.hpp>

@interface GLView ()
- (void)addGestureRecognizers;
@end

@implementation GLView

+ (Class) layerClass {
    return [ CAEAGLLayer class ];
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (!self) { return self; }
    
    CAEAGLLayer * EAGL_layer = (CAEAGLLayer*)super.layer;
    EAGL_layer.opaque = YES;
    
    gl_context_ = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!gl_context_ || ! [EAGLContext setCurrentContext:gl_context_]) {
        [self release];
        return nil;
    }

	[self addGestureRecognizers];

    return self;
}

- (void)allocateFramebufferStorage {
	// NOTE: We assume that the rendering engine creates and binds the render buffer.
	[ gl_context_ renderbufferStorage: GL_RENDERBUFFER fromDrawable: (CAEAGLLayer *)super.layer ];
}

- (void)setApplicationEngine:(ApplicationEngine *)app_engine {
	app_engine_ = app_engine;
}

- (void) drawView:(CADisplayLink *)displayLink {
	app_engine_->render_frame();
	[ gl_context_ presentRenderbuffer: GL_RENDERBUFFER ];
}

- (void)addGestureRecognizers {
	UIPinchGestureRecognizer *pinchGesture =
		[[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(zoomCanvas:)];
	[pinchGesture setDelegate:self];
	[self addGestureRecognizer:pinchGesture];
	[pinchGesture release];
}

- (void)zoomCanvas:(UIPinchGestureRecognizer *)gestureRecognizer {
	UIGestureRecognizerState state = [gestureRecognizer state];
    if (state == UIGestureRecognizerStateBegan || state == UIGestureRecognizerStateChanged) {

        app_engine_->zoom_canvas([gestureRecognizer scale]);

        [gestureRecognizer setScale:1];
    }
}

- (void)dealloc
{
    if ([ EAGLContext currentContext ] == gl_context_) {
		[ EAGLContext setCurrentContext: nil ];
	}

	[ gl_context_ release ];
    [ super dealloc ];
}

@end
