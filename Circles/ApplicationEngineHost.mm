#import "ApplicationEngineHost.h"
#import <app_engine/ApplicationEngine.hpp>

@implementation ApplicationEngineHost

- (id)init {
	self = [super init];
	if (!self) { return nil; }

	app_engine_ = ApplicationEngine::construct();
	if (!app_engine_) { return nil; }

	return self;
}

- (void)postSystemsInit {
	app_engine_->post_systems_init();
}

- (void)setRenderingEngine:(RenderingEngine *)rendering_engine {
	app_engine_->set_rendering_engine(rendering_engine);
}

- (void)setCommandQueue:(CommandQueue *)command_queue {
	app_engine_->set_command_queue(command_queue);
}

- (void)processingLoopStep:(NSTimer*)timer {
  	app_engine_->processing_loop_step();
}


- (void)dealloc {
	[super dealloc];
}

@end
