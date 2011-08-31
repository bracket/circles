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

- (void)setRenderingEngine:(RenderingEngine *)rendering_engine {
	app_engine_->set_rendering_engine(rendering_engine);
}

- (void)setCommandQueue:(CommandQueue *)command_queue {
	app_engine_->set_command_queue(command_queue);
}

- (void)dealloc {
	[super dealloc];
}

@end
