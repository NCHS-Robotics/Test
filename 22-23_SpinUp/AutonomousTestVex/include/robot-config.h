using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor LeftFrontWheel;
extern motor LeftBackWheel;
extern motor RightFrontWheel;
extern motor RightBackWheel;
extern motor IntakeMotor;
extern motor ShootClose;
extern motor ShootFar;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );