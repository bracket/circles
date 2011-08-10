#import "GLView.h"
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/EAGLDrawable.h>

#include <app_engine/ApplicationEngine.hpp>
#include <math/Matrix.hpp>

@implementation GLView

+ (Class) layerClass {
    return [ CAEAGLLayer class ];
}

namespace {
	Matrix<3, 3, float> make_touch_transform(float view_width, float view_height) {
		float m[] = {
			1, 0,           0,
			0, -1,          0,
			0, view_height, 1
		};

		return Matrix<3,3,float>(m);
	}
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

	touch_handler_host_ = [[ TouchHandlerHost alloc ] init ];
    [ touch_handler_host_
        setTouchTransform: make_touch_transform(CGRectGetWidth(frame), CGRectGetHeight(frame)) ];
	[ self addGestureRecognizers:touch_handler_host_ ];

    return self;
}

- (void)allocateFramebufferStorage {
	// NOTE: We assume that the rendering engine creates and binds the render buffer.
	[ gl_context_ renderbufferStorage: GL_RENDERBUFFER fromDrawable: (CAEAGLLayer *)super.layer ];
}

- (void)setApplicationEngine:(ApplicationEngine *)app_engine {
	app_engine_ = app_engine;
	[ touch_handler_host_ setTouchHandler: app_engine->get_touch_handler() ];
}

- (void) drawView:(CADisplayLink *)displayLink {
	app_engine_->render_frame();
	[ gl_context_ presentRenderbuffer: GL_RENDERBUFFER ];
}

- (void)addGestureRecognizers:(TouchHandlerHost *)handler {
	UIPinchGestureRecognizer * pinchGesture = [[ UIPinchGestureRecognizer alloc ]
		initWithTarget:handler action:@selector(handlePinch:) ];
	[ self addGestureRecognizer:pinchGesture ];
	[ pinchGesture release ];

	UIPanGestureRecognizer * oneFingerPan = [[ UIPanGestureRecognizer alloc ]
		initWithTarget:handler action:@selector(handleOneFingerPan:) ];
	oneFingerPan.minimumNumberOfTouches = 1;
	oneFingerPan.maximumNumberOfTouches = 1;
	[ self addGestureRecognizer:oneFingerPan ];
	[ oneFingerPan release ];

	UIPanGestureRecognizer * twoFingerPan = [[ UIPanGestureRecognizer alloc ]
		initWithTarget:handler action:@selector(handleTwoFingerPan:) ];
	twoFingerPan.minimumNumberOfTouches = 2;
	twoFingerPan.maximumNumberOfTouches = 2;
	[ self addGestureRecognizer:twoFingerPan ];
	[ twoFingerPan release ];

	UITapGestureRecognizer * tap = [[ UITapGestureRecognizer alloc ]
		initWithTarget:handler action:@selector(handleTap:) ];
	[ self addGestureRecognizer:tap ];
	[ tap release ];
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
