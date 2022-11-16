// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LFdrive              motor         3               
// LBdrive              motor         1               
// RFdrive              motor         4               
// RBdrive              motor         2               
// IntakeMotor          motor         5               
// ShootClose           motor         8               
// ShootFar             motor         10              
// Inertial             inertial      11              
// Endgame              motor         19              
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Pushkar Betsur                                   */
/*    Created:      Mon Nov 14 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

//prime first 3 motors
void primeFirst3(int deg) {
  LFdrive.startRotateFor(forward, deg, degrees);
  LBdrive.startRotateFor(forward, deg, degrees);
  RFdrive.startRotateFor(forward, deg, degrees);
}

//spin all motors
void spinAll() {
  LFdrive.spin(forward, 5, volt);
  LBdrive.spin(forward, 5, volt);
  RFdrive.spin(forward, 5, volt);
  RBdrive.spin(forward, 5, volt);
}

//stop all motors
void stopAll() {
  LFdrive.stop(brakeType::brake);
  LBdrive.stop(brakeType::brake);
  RFdrive.stop(brakeType::brake);
  RBdrive.stop(brakeType::brake);
}

//set motor velocity percentage
void setMotorPercentage(int percentage) {
  LFdrive.setVelocity(percentage, percent);
  LBdrive.setVelocity(percentage, percent);
  RFdrive.setVelocity(percentage, percent);
  RBdrive.setVelocity(percentage, percent);
  IntakeMotor.setVelocity(percentage, percent);
  ShootClose.setVelocity(percentage, percent);
  ShootFar.setVelocity(percentage, percent);
}

void autonomous() {
  setMotorPercentage(100);
  IntakeMotor.setVelocity(100, percent);

  //shoot two preloads in blue hoop

  //IntakeMotor.spin(forward, ///)
  
  //go to top left blue side roller and roll it 
  //pick up disc and roll top left red side roller
  //pick to top 2 middle blue discs 
  //shoot all 3 discs into red hoop
  //pick up bottom two middle red discs
  //shoot into red hoop
  //roll bottom right red side roller
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
}

