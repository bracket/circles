#import <Foundation/Foundation.h>
#import <app_engine/ApplicationEngine.hpp>
#import "GLView.h"
#import "app_engine/CommandQueue.hpp"

class RenderingEngine;
class MachineThread;

@interface ApplicationEngineHost : NSObject {
	@private
		ApplicationEngine * app_engine_;
}

- (void)setRenderingEngine:(RenderingEngine *)rendering_engine;
- (void)setCommandQueue:(CommandQueue *)command_queue;

- (void)processingLoopStep:(NSTimer*)timer;

@end
