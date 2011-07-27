#import "GLView.h"
#import <OpenGLES/ES2/gl.h>

#import <OpenGLES/EAGLDrawable.h>

namespace {
    void initialize_gl(EAGLContext * context, CAEAGLLayer * layer, int width, int height) {
		GLuint frame_buffer, render_buffer;
		glGenFramebuffers(1, &frame_buffer);
        glGenRenderbuffers(1, &render_buffer);

		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);

		[ context renderbufferStorage: GL_RENDERBUFFER fromDrawable: layer ];

		glFramebufferRenderbuffer(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_RENDERBUFFER, render_buffer
		);

		glViewport(0, 0, width, height);
    }
}

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

	initialize_gl(gl_context_, EAGL_layer, CGRectGetWidth(frame), CGRectGetHeight(frame));
	rendering_engine_ = RenderingEngine::construct();

	[self drawView:nil];

    return self;
}

- (void) drawView:(CADisplayLink *)displayLink {
  	rendering_engine_->render();
	[ gl_context_ presentRenderbuffer: GL_RENDERBUFFER ];
}

- (void)dealloc
{
    if ([ EAGLContext currentContext ] == gl_context_) {
		[ EAGLContext setCurrentContext: nil ];
	}

	[ gl_context_ release ];
    [ super dealloc ];
	delete rendering_engine_;
}

@end
