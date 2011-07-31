#import <UIKit/UIKit.h>
#import "ApplicationEngineHost.h"
#import "GLView.h"

@interface CirclesAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow * window_;
    GLView * view_;
	ApplicationEngineHost * app_engine_host_;
}

@end
