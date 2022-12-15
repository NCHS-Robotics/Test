// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftDrive            motor         1               
// RightDrive           motor         2               
// IntakeMotor          motor         5               
// ShootClose           motor         8               
// ShootFar             motor         10              
// Controller1          controller                    
// Inertial             inertial      11              
// Endgame              motor         19              
// Gyro                 gyro          A               
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <cmath>

using namespace vex;
using namespace std;

// A global instance of competition
competition Competition;

vex::controller::lcd ControllerScreen = vex::controller::lcd();

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*
//prime first 3 motors
void primeFirst(int deg) {
  
  LFdrive.startRotateFor(forward, deg, degrees);
  LBdrive.startRotateFor(forward, deg, degrees);
  RFdrive.startRotateFor(forward, deg, degrees);
  
}
*/

//drive laterally
void driveLateral(int deg) {
  //primeFirst3(deg);
  LeftDrive.startRotateFor(forward, deg, degrees);
  RightDrive.spinFor(forward, deg, degrees);
}

//turn the robot
void turnRobot(int deg) {
  LeftDrive.startRotateFor(forward, deg, degrees);
  RightDrive.spinFor(reverse, deg, degrees);
}

//spin all drive motors
void spinAllDriveForward() {
  LeftDrive.spin(forward);
  RightDrive.spin(forward);
}

void spinAllDriveReverse() {
  LeftDrive.spin(reverse);
  RightDrive.spin(reverse);
}

//stop all drive motors
void stopAllDrive() {
  LeftDrive.stop(brakeType::brake);
  RightDrive.stop(brakeType::brake);
}

//set motor velocity percentage
void setDrivePercentage(int percentage) {
  LeftDrive.setVelocity(percentage, percent);
  RightDrive.setVelocity(percentage, percent);
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
  wait (2, sec);
  IntakeMotor.spinFor(reverse, intakeDeg, deg);
  ShootClose.stop(brakeType::brake);
  ShootFar.stop(brakeType::brake);
  IntakeMotor.stop(brakeType::brake);
}

//calibrate gyro
void resetGyro() {
  Gyro.resetAngle();
  Gyro.resetHeading();
  Gyro.resetRotation();
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

  Brain.Screen.clearScreen();
  double turnImportance = 0.5;
  // place driver control in this while loop
  while (true) {

    Brain.Screen.clearScreen();
    Brain.Screen.printAt(1,40,"RPM:%f",ShootClose.velocity(vex::velocityUnits::rpm));
    Brain.Screen.printAt(1,80,"RPM:%f",ShootFar.velocity(vex::velocityUnits::rpm));
    Brain.Screen.printAt(1,120,"RPM:%f",IntakeMotor.velocity(vex::velocityUnits::rpm));\
    Brain.Screen.printAt(1,160, "Heading Angle:%f",Gyro.value(deg));
    Brain.Screen.render();
    
    double turnVal = Controller1.Axis4.position(percent);
    double forwardVal = Controller1.Axis3.position(percent);

    double turnVolts = -(turnVal) * 0.12;
    double forwardVolts = forwardVal * 0.12 * (1 - (std::abs(turnVolts)/12.0) * turnImportance);

    LeftDrive.spin(forward, forwardVolts - turnVolts, voltageUnits::volt);
    RightDrive.spin(forward, forwardVolts - turnVolts, voltageUnits::volt);

    /*
    LFdrive.spin(forward, forwardVolts + turnVolts, voltageUnits::volt);
    LBdrive.spin(forward, forwardVolts + turnVolts, voltageUnits::volt);
    RFdrive.spin(forward, forwardVolts - turnVolts, voltageUnits::volt);
    RBdrive.spin(forward, forwardVolts - turnVolts, voltageUnits::volt);
    */

    //double suckVal = Controller1.ButtonL1.pressing();
    //double suckVolts = suckVal * 0.12;
    
    if (Controller1.ButtonL2.pressing()){
      IntakeMotor.spin(forward, 12.0 , voltageUnits::volt);
    }
    else if (Controller1.ButtonL1.pressing()){
      IntakeMotor.spin(forward, -12.0 , voltageUnits::volt);
    }
    else{
      IntakeMotor.stop();
    }
    
    
    /*void Shoot() {
      Shootabig.spin(reverse, 12.0 , voltageUnits::volt);
    }
    void Nothing() {
      Shootabig.spin(forward, 0 , voltageUnits::volt);
    }

    Controller1.ButtonR1.pressed(Shoot);
    Controller1.ButtonR2.pressed(Nothing);
    */

    //regular shoot
    /*
    if (Controller1.ButtonR1.pressing()){
      ShootClose.spin(forward, 12.0 , voltageUnits::volt);
      ShootFar.spin(forward, 12.0, voltageUnits::volt);
    }
    else if (Controller1.ButtonR2.pressing()) {
      ShootClose.spin(reverse, 12.0, volt);
      ShootFar.spin(reverse, 12.0, volt);
    }
    else{
      ShootClose.stop();
      ShootFar.stop();
    }
    */
    
    

    //coast at 210 rpm when holding shoot button
    /**/
    //R1 = close shoot
    if (Controller1.ButtonR1.pressing()){
      ShootClose.setVelocity(50, percent);
      ShootFar.setVelocity(50, percent);
      ShootClose.spin(forward);
      ShootFar.spin(forward);
    }
    //R2 = far shoot
    else if (Controller1.ButtonR2.pressing()) {
      ShootClose.setVelocity(100, percent);
      ShootFar.setVelocity(100, percent);
      ShootClose.spin(forward);
      ShootFar.spin(forward);
    }
    else{
      ShootClose.stop();
      ShootFar.stop();
    }
    /**/
    

    //Suck.spin(forward, suckVolts , voltageUnits::volt);
    //Suck.spin(reverse, suckVolts , voltageUnits::volt);

    
    if (Controller1.ButtonUp.pressing()){ 
      Endgame.spin(forward, 12.0, voltageUnits::volt);
    }
    else if(Controller1.ButtonDown.pressing()) {
      Endgame.spin(reverse, 12.0, voltageUnits::volt);
    }
    else{
      Endgame.stop(brakeType::hold);
    }
    

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}

