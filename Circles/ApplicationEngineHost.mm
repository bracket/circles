#import "ApplicationEngineHost.h"
#include <app_engine/ApplicationEngine.hpp>

@implementation ApplicationEngineHost

- (id)initWithView:(UIView *)view {
	self = [super init];
	if (!self) { return nil; }

	app_engine_ = ApplicationEngine::constuct();
	if (!app_engine_) { return nil; }

	app_engine_->set_rendering_engine([ view getRenderingEngine ]);

	return self;
}

- (ApplicationEngine *)getApplicationEngine {
	return app_engine_;
}

- (void)setRenderingEngine:(RenderingEngine *)rendering_engine {
	app_engine_->set_rendering_engine(rendering_engine);
}

- (void)dealloc {
	delete app_engine_;

	[super dealloc];
}

@end
