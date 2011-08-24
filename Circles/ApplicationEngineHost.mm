#import "ApplicationEngineHost.h"
#include <app_engine/ApplicationEngine.hpp>

@implementation ApplicationEngineHost

- (id)init {
	self = [super init];
	if (!self) { return nil; }

	app_engine_ = ApplicationEngine::construct();
	if (!app_engine_) { return nil; }

	return self;
}

- (ApplicationEngine *)getApplicationEngine {
	return app_engine_;
}

- (void)setRenderingEngine:(RenderingEngine *)rendering_engine {
	app_engine_->set_rendering_engine(rendering_engine);
}

- (void)setCommandQueue:(CommandQueue *)command_queue {
	app_engine_->set_command_queue(command_queue);
}

- (void)dealloc {
	delete app_engine_;

	[super dealloc];
}

@end
