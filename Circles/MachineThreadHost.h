#import <Foundation/Foundation.h>

class MachineThread;

@interface MachineThreadHost : NSObject {
	MachineThread * machine_thread_;
}

- (void) threadMain;
- (MachineThread *)getMachineThread;

@end
