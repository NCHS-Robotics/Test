#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor LeftFrontWheel = motor(PORT3, ratio6_1, true);
motor LeftBackWheel = motor(PORT11, ratio6_1, true);
motor RightFrontWheel = motor(PORT4, ratio6_1, false);
motor RightBackWheel = motor(PORT2, ratio6_1, false);
motor IntakeMotor = motor(PORT5, ratio18_1, false);
motor ShootClose = motor(PORT8, ratio18_1, false);
motor ShootFar = motor(PORT10, ratio18_1, true);

// VEXcode generated functions



/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}