#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor LFdrive = motor(PORT3, ratio18_1, false);
motor LBdrive = motor(PORT1, ratio18_1, false);
motor RFdrive = motor(PORT4, ratio18_1, true);
motor RBdrive = motor(PORT2, ratio18_1, true);
encoder ShaftEncoder = encoder(Brain.ThreeWirePort.A);
controller Controller1 = controller(primary);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}