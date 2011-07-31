#import <Foundation/Foundation.h>
#import <app_engine/ApplicationEngine.hpp>

class RenderingEngine;
class MachineThread;

@interface ApplicationEngineHost : NSObject {
	@private
		ApplicationEngine * app_engine_;
}

- (void)setRenderingEngine:(RenderingEngine *)rendering_engine;
- (ApplicationEngine *)getApplicationEngine;

@end
