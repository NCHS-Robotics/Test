using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor LBdrive;
extern motor LFdrive;
extern motor RBdrive;
extern motor RFdrive;
extern motor IntakeMotor;
extern motor ShootClose;
extern motor ShootFar;
extern controller Controller1;
extern motor Endgame;
extern motor Lift;
extern inertial Inertial;
extern limit LimitSwitchFar;
extern limit LimitSwitchIntake;
extern line LineTrackerClose;
extern line LineTrackerFar;
extern bumper BumperRoller;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );