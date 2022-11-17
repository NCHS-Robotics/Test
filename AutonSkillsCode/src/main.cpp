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

//drive laterally
void driveLateral(int deg) {
  primeFirst3(deg);
  RBdrive.spinFor(forward, deg, degrees);
}

//turn the robot
void turnRobot(int deg) {
  LFdrive.startRotateFor(forward, deg, degrees);
  LBdrive.startRotateFor(forward, deg, degrees);
  RFdrive.startRotateFor(reverse, deg, degrees);
  RBdrive.spinFor(reverse, deg, degrees);
}

//spin all drive motors
void spinAllDrive(int volts) {
  LFdrive.spin(forward, volts, volt);
  LBdrive.spin(forward, volts, volt);
  RFdrive.spin(forward, volts, volt);
  RBdrive.spin(forward, volts, volt);
}

//stop all drive motors
void stopAllDrive() {
  LFdrive.stop(brakeType::brake);
  LBdrive.stop(brakeType::brake);
  RFdrive.stop(brakeType::brake);
  RBdrive.stop(brakeType::brake);
}

//set motor velocity percentage
void setDrivePercentage(int percentage) {
  LFdrive.setVelocity(percentage, percent);
  LBdrive.setVelocity(percentage, percent);
  RFdrive.setVelocity(percentage, percent);
  RBdrive.setVelocity(percentage, percent);
}

void setMotorPercentage(int percentage) {
  setDrivePercentage(percentage);
  IntakeMotor.setVelocity(percentage, percent);
  ShootClose.setVelocity(percentage, percent);
  ShootFar.setVelocity(percentage, percent);
}

//shoot discs out of robot
//@param intakeDeg = degrees to spin intake
void shootDiscs(int intakeDeg) {
  ShootClose.spin(forward);
  ShootFar.spin(forward);
  wait (1, sec);
  IntakeMotor.spinFor(reverse, intakeDeg, deg);
  ShootClose.stop(brakeType::brake);
  ShootFar.stop(brakeType::brake);
  IntakeMotor.stop(brakeType::brake);
}

void auton() {
  //front: brain
  //back: intake
  //clockwise: negative
  //counter-clockwise: positive

  setMotorPercentage(50);
  setDrivePercentage(35);
  ShootClose.setVelocity(100, percent);
  ShootFar.setVelocity(100, percent);

  //shoot two preloads in blue hoop
  shootDiscs(720);
  
  //roll top rollers
  driveLateral(-3600);
  turnRobot(670);
  driveLateral(-380);
  IntakeMotor.spinFor(forward, -720, degrees);

  driveLateral(1200);
  turnRobot(-690);
  driveLateral(-1000);
  IntakeMotor.spinFor(forward, -720, degrees);

  //align with wall, pick to middle blue discs and shoot them
  driveLateral(720);
  turnRobot(750);
  driveLateral(1500);
  turnRobot(750);
  driveLateral(3000);

  IntakeMotor.setVelocity(100, percent);

  driveLateral(-720);
  turnRobot(200);
  IntakeMotor.spin(reverse);
  driveLateral(-2000);
  IntakeMotor.stop(brakeType::brake);
  turnRobot(-750);
  driveLateral(500);

  shootDiscs(1000);

  turnRobot(750);
  IntakeMotor.spin(reverse);
  driveLateral(-2000);
  IntakeMotor.stop(brakeType::brake);
  turnRobot(-1125);
  driveLateral(720);

  shootDiscs(1000);

  //roll bottom rollers
  setDrivePercentage(50);
  IntakeMotor.setVelocity(50, percent);

  driveLateral(-720);
  turnRobot(1125);
  driveLateral(720);
  turnRobot(750);
  driveLateral(-360);
  IntakeMotor.spinFor(forward, 720, degrees);

  driveLateral(720);
  turnRobot(750);
  driveLateral(-720);
  IntakeMotor.spinFor(forward, 720, degrees);

  //align with wall, pick up red middle discs and shoot them
  driveLateral(360);
  turnRobot(750);
  driveLateral(720);
  turnRobot(750);
  driveLateral(1200);

  setDrivePercentage(25);
  IntakeMotor.setVelocity(100, percent);

  driveLateral(-720);
  turnRobot(1125);
  IntakeMotor.spin(reverse);
  driveLateral(-2000);
  IntakeMotor.stop(brakeType::brake);
  turnRobot(-750);
  driveLateral(500);

  shootDiscs(1000);

  turnRobot(750);
  IntakeMotor.spin(reverse);
  driveLateral(-2000);
  IntakeMotor.stop(brakeType::brake);
  turnRobot(-1125);
  driveLateral(720);

  shootDiscs(1000);

  //endgame
  Endgame.spin(forward);
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

