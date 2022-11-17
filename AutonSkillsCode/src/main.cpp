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
// Gyro                 gyro          A               
// ---- END VEXCODE CONFIGURED DEVICES ---- 
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

vex::controller::lcd ControllerScreen = vex::controller::lcd();

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

//print onto controller screen in top left

void auton() {
  setMotorPercentage(50);
  ShootClose.setVelocity(100, percent);
  ShootFar.setVelocity(100, percent);

  //shoot two preloads in blue hoop
  ShootClose.spin(forward);
  ShootFar.spin(forward);
  wait (1, sec);
  IntakeMotor.spinFor(reverse, 720, deg);
  ShootClose.stop(brakeType::brake);
  ShootFar.stop(brakeType::brake);
  IntakeMotor.stop(brakeType::brake);

  wait(0.2, sec);
  
  //go to top left blue side roller and roll it 
  primeFirst3(-3750);
  RBdrive.spinFor(forward, -3750, degrees);

  LFdrive.startRotateFor(forward, 750, degrees);
  LBdrive.startRotateFor(forward, 750, degrees);
  RFdrive.startRotateFor(reverse, 750, degrees);
  RBdrive.spinFor(reverse, 750, degrees);

  primeFirst3(-230);
  RBdrive.spinFor(forward, -230, degrees);
  
  //roll top left red side roller
  primeFirst3(720);
  IntakeMotor.spinFor(forward, 720, degrees);

  LFdrive.startRotateFor(forward, -750, degrees);
  LBdrive.startRotateFor(forward, -750, degrees);
  RFdrive.startRotateFor(reverse, -750, degrees);
  RBdrive.spinFor(reverse, -750, degrees);

  primeFirst3(720);
  RBdrive.spinFor(forward, 720, degrees);

  primeFirst3(-230);
  RBdrive.spinFor(forward, -230, degrees);

  primeFirst3(-720);
  RBdrive.spinFor(forward, -720, degrees);

  //pick to top 2 middle blue discs 
  //shoot all 3 discs into red hoop
  //pick up bottom two middle red discs
  //shoot into red hoop
  //roll bottom right red side roller
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  /*
  ControllerScreen.clearScreen();
  ControllerScreen.print("Autonomous Started");
  */
  auton();

}

