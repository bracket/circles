#import <Foundation/Foundation.h>

class MachineThread;

@interface MachineThreadHost : NSObject {
	MachineThread * machine_thread_;
}

- (void) threadMain;

@end
