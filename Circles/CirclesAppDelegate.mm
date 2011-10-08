#import "app_engine/ApplicationEngine.hpp"
#import "Circles/LocalCommandQueue.hpp"
#import "CirclesAppDelegate.h"
#import "GLView.h"
#import "MachineThreadHost.h"
#import <QuartzCore/CADisplayLink.h>
#import <UIKit/UIKit.h>

namespace {
	GLView * construct_view() {
		CGRect screenBounds = [[UIScreen mainScreen] bounds];

		UIWindow * window = [[ UIWindow alloc ] initWithFrame:screenBounds ];
		GLView * view = [[ GLView alloc ] initWithFrame: screenBounds ];

		[ window addSubview: view ];
		[ window makeKeyAndVisible ];

		return view;
	}
}

@implementation CirclesAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	app_engine_host_ = [[ ApplicationEngineHost alloc ] init ];

	view_ = construct_view();

	RenderingEngine * rendering_engine = RenderingEngine::construct(
		CGRectGetWidth(view_.frame), CGRectGetHeight(view_.frame)
	);

	[ app_engine_host_ setRenderingEngine:rendering_engine];

	[ view_ allocateFramebufferStorage ];

	[ view_ setApplicationEngine:ApplicationEngine::get() ];

	CommandQueue * command_queue = LocalCommandQueue::construct();
	[ app_engine_host_ setCommandQueue:command_queue ];

	[ app_engine_host_ postSystemsInit ];

	NSTimer * processing_timer = [ NSTimer
		scheduledTimerWithTimeInterval:1.0e-3
		target:app_engine_host_
		selector:@selector(processingLoopStep:)
		userInfo:nil
		repeats:YES
	];

	[ [ NSRunLoop currentRunLoop ] addTimer:processing_timer forMode:NSDefaultRunLoopMode];

    CADisplayLink * display_link = [
		CADisplayLink displayLinkWithTarget:view_ 
			selector:@selector(drawView:) 
	];

	[ display_link addToRunLoop:[ NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
     */
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
     */
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    /*
     Called when the application is about to terminate.
     Save data if appropriate.
     See also applicationDidEnterBackground:.
     */
}

- (void)dealloc
{
	[app_engine_host_ release];
    [view_ release];
    [super dealloc];
}

@end
