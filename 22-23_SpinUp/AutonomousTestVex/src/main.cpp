/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\alexa                                            */
/*    Created:      Thu Nov 10 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftFrontWheel       motor         3               
// LeftBackWheel        motor         11              
// RightFrontWheel      motor         4               
// RightBackWheel       motor         2               
// IntakeMotor          motor         5               
// ShootClose           motor         8               
// ShootFar             motor         10              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

/*
#define LeftFrontWheel 3 //blue, reversed
#define LeftBackWheel 11 //blue, reversed
#define RightFrontWheel 4 //blue
#define RightBackWheel 2 //blue
#define IntakeMotor 5 //green
#define ShootClose 8 //green
#define ShootFar 10 //green, reversed
*/

using namespace vex;

//drive all the wheels at once either forward or backward
bool lateralDrive(int deg) {
  bool result = false;
  bool result1 = LeftFrontWheel.spinToPosition(deg, degrees);
  bool result2 = LeftBackWheel.spinToPosition(deg, degrees);
  bool result3 = RightFrontWheel.spinToPosition(deg, degrees);
  bool result4= RightBackWheel.spinToPosition(deg, degrees);
  if (result1 && result2 && result3 && result4) {
    result = true;
  }
  return result;
}

//Autonomous code
void autonomous() {
  //move forward slightly to spin roller
  bool lateralDriveResult = false;
  while (!lateralDriveResult) {
    lateralDriveResult = lateralDrive(360);
  }

  lateralDriveResult = false;
  while(!lateralDriveResult) {
    //move roller spinner motor x amount of degrees
  }

  lateralDriveResult = false;
  while (!lateralDriveResult) {
    lateralDriveResult = lateralDrive(-360);
  }

  //lign up to miss shots and score discs in the +1 box underneath goal
  //shoot discs
  //(if time) try and get discs in predicted locations
}

/**
void userControl() {
    
}
**/

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  autonomous();
  //userControl();
  
}
