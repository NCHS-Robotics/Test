#include "vex.h"

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LBdrive              motor         2               
// LFdrive              motor         1               
// RBdrive              motor         7               
// RFdrive              motor         8               
// GyroG                gyro          G               
// ---- END VEXCODE CONFIGURED DEVICES ----

using namespace vex;

// A global instance of competition
competition Competition;

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  Brain.Screen.print("CALIBRATING GYRO");
}

void driveTurn(int degrees, int speed) {
  LBdrive.resetRotation();
  LFdrive.resetRotation();
  RBdrive.resetRotation();
  RFdrive.resetRotation();
  LBdrive.startRotateTo(degrees, deg, speed, rpm);
  LFdrive.startRotateTo(degrees, deg, speed, rpm);
  RBdrive.startRotateTo(-(degrees), deg, speed, rpm);
  RFdrive.rotateTo(-(degrees), deg, speed, rpm);
}

void drive(int degrees, int speed) {
  LBdrive.resetRotation();
  LFdrive.resetRotation();
  RBdrive.resetRotation();
  RFdrive.resetRotation();
  LBdrive.startRotateTo(degrees, deg, speed, rpm);
  LFdrive.startRotateTo(degrees, deg, speed, rpm);
  RBdrive.startRotateTo(degrees, deg, speed, rpm);
  RFdrive.rotateTo(degrees, deg, speed, rpm);
}

void stopMotors() {
  LBdrive.stop();
  LFdrive.stop();
  RBdrive.stop();
  RFdrive.stop();
}

void GyroPrint() {
  Brain.Screen.clearScreen();
  Brain.Screen.printAt(9, 13, "Gyro: %f", GyroG.value(rotationUnits::deg));
}

/*
Turn Until Gyro Method
turns the robot until the given gyroLimit is reached
positive - clockwise
negative - counterclockwise
*/
void TurnUntilGyro(double gyroLimit) {
  //checks for positive or negative direction
  bool clockwise;
  if (gyroLimit >= 0) {
    clockwise = true;
  }
  else {
    clockwise = false;
  }
  
  //turns the robot clockwise if positive 
  if (clockwise == true) {
    gyroLimit = GyroG.value(rotationUnits::deg) + gyroLimit;
    //MAKE SURE TO TURN PROPERLY IF GYROLIMIT > 360
    while(GyroG.value(rotationUnits::deg) <= gyroLimit) {
      GyroPrint();
      driveTurn(10, 100);
    }
    GyroPrint();
    Brain.Screen.print("Stop");
  }
  //turns the robot counter clockwise if negative
  else {
    double gyroInitial = GyroG.value(rotationUnits::deg);
    gyroLimit = gyroInitial + gyroLimit;
    double gyroValue = GyroG.value(rotationUnits::deg);
    //MAKE SURE TO TURN PROPERLY IF GYROVALUE < 0
    while(gyroValue >= gyroLimit) {
      GyroPrint();
      driveTurn(-10, 100);
      gyroValue = GyroG.value(rotationUnits::deg);
    }
    GyroPrint();
    Brain.Screen.print("Stop");
  }
}

void autonomous(void) {
  task::sleep(1500);

  drive(360, 55);
  TurnUntilGyro(90);
  drive(360, 55);
  TurnUntilGyro(-90);
  drive(360, 55);

}

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  //Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  //auto
  autonomous();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
