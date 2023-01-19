using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor LBdrive;
extern motor RBdrive;
extern motor IntakeMotor;
extern motor ShootClose;
extern motor ShootFar;
extern controller Controller1;
extern motor Endgame;
extern motor LFdrive;
extern motor RFdrive;
extern motor Lift;
extern inertial Inertial;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );