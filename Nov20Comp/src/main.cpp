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
// Controller1          controller                    
// Inertial             inertial      11              
// Endgame              motor         19              
// Gyro                 gyro          A               
// ---- END VEXCODE CONFIGURED DEVICES ----
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

//prime first 3 motors
void primeFirst3(int deg) {
  LFdrive.startRotateFor(forward, deg, degrees);
  LBdrive.startRotateFor(forward, deg, degrees);
  RFdrive.startRotateFor(forward, deg, degrees);
}

void spinAll() {
  LFdrive.spin(forward, 5, volt);
  LBdrive.spin(forward, 5, volt);
  RFdrive.spin(forward, 5, volt);
  RBdrive.spin(forward, 5, volt);
}

void stopAll() {
  LFdrive.stop(brakeType::brake);
  LBdrive.stop(brakeType::brake);
  RFdrive.stop(brakeType::brake);
  RBdrive.stop(brakeType::brake);
}

void setMotorPercentage(int percentage) {
  LFdrive.setVelocity(percentage, percent);
  LBdrive.setVelocity(percentage, percent);
  RFdrive.setVelocity(percentage, percent);
  RBdrive.setVelocity(percentage, percent);
  IntakeMotor.setVelocity(percentage, percent);
  ShootClose.setVelocity(percentage, percent);
  ShootFar.setVelocity(percentage, percent);
}

//calibrate gyro
void resetGyro() {
  Gyro.resetAngle();
  Gyro.resetHeading();
  Gyro.resetRotation();
}
void autonomous(void) {

  setMotorPercentage(50);
  ShootClose.setVelocity(100, percent);
  ShootFar.setVelocity(100, percent);
  //resetGyro();
  //wait(1, sec);

  primeFirst3(-330);
  RBdrive.spinFor(reverse, 330, deg);

  IntakeMotor.spin(forward);
  wait(1, sec);
  IntakeMotor.stop(brakeType::brake);

  primeFirst3(360);
  RBdrive.spinFor(forward, 360, deg);

  /*
  double heading = Gyro.heading();

  while (Gyro.heading() <= (heading + 270)) {
    LFdrive.spin(forward);
    LBdrive.spin(forward);
    RFdrive.spin(reverse);
    RBdrive.spin(reverse);
  }
  LFdrive.stop(brakeType::brake);
  LBdrive.stop(brakeType::brake);
  RFdrive.stop(brakeType::brake);
  RBdrive.stop(brakeType::brake);
  */

  
  LFdrive.startRotateFor(reverse, 750, degrees);
  LBdrive.startRotateFor(reverse, 750, degrees);
  RFdrive.startRotateFor(forward, 750, degrees);  
  RBdrive.spinFor(forward, 750, degrees);
  

  primeFirst3(1700);
  RBdrive.spinFor(forward, 1700, degrees);

  ShootClose.spin(forward);
  ShootFar.spin(forward);
  wait(3, sec);
  IntakeMotor.spin(reverse);
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

    LFdrive.spin(forward, forwardVolts + turnVolts, voltageUnits::volt);
    LBdrive.spin(forward, forwardVolts + turnVolts, voltageUnits::volt);
    RFdrive.spin(forward, forwardVolts - turnVolts, voltageUnits::volt);
    RBdrive.spin(forward, forwardVolts - turnVolts, voltageUnits::volt);

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
    
    if (Controller1.ButtonR1.pressing()){
      ShootClose.spin(forward, 12.0 , voltageUnits::volt);
      ShootFar.spin(forward, 12.0, voltageUnits::volt);
    }
    else{
      ShootClose.stop();
      ShootFar.stop();
    }
    
    

    //coast at 210 rpm when holding shoot button
    /*
    if (Controller1.ButtonR1.pressing()) {
      ShootClose.spin(forward, 12.0, volt);
      ShootFar.spin(forward, 12.0, volt);
      double velo = ShootClose.velocity(rpm);
      if (velo >= 210) {
        ShootClose.stop(brakeType::coast);
        ShootFar.stop(brakeType::coast);
      } else {
        ShootClose.spin(forward, 12.0, volt);
        ShootFar.spin(reverse, 12.0, volt);
      }
    } 
    else {
      ShootClose.stop();
      ShootFar.stop();
    }
    */
    

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
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}

