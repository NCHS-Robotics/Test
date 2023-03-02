// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LBdrive              motor         1               
// RBdrive              motor         2               
// IntakeMotor          motor         5               
// ShootClose           motor         6               
// ShootFar             motor         7               
// Controller1          controller                    
// Endgame              motor         20              
// LFdrive              motor         11              
// RFdrive              motor         4               
// Lift                 motor         8               
// Inertial             inertial      12              
// LimitSwitchFar       limit         A               
// LimitSwitchIntake    limit         B               
// EncoderC             encoder       C, D            
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
//initialize tasks
vex::task liftArmIntakeTask;
vex::task liftArmFarTask;
//vex::encoder

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


//lift arm up task
int liftArmIntake() {
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("lift intake thread");
  while(!(LimitSwitchIntake.pressing())) {
    Lift.spin(forward);
  }
  Lift.stop(hold);
  
  return 0;
}

//lift arm down task
int liftArmFar() {
  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print("lift arm thread");
  while(!(LimitSwitchFar.pressing())) {
    Lift.spin(reverse);
  }
  Lift.stop(hold);

  return 0;
}

//calculate voltage for shooter
int vCalc(int v) {
  while(EncoderC.velocity(rpm) <= 2500 || v >= 12) {
    v = v * 1.2;
  }
  return v;
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
  //Brain.Screen.clearScreen();
  double turnImportance = 0.5;

  //initialize sensors and motor speeds
  Lift.setPosition(0, degrees);
  Inertial.resetHeading();
  Inertial.resetRotation();
  Lift.setVelocity(100, percent);
  IntakeMotor.setVelocity(100, percent);

  //define tasks
  liftArmFarTask = vex::task(liftArmFar);
  liftArmFarTask.suspend();
  liftArmIntakeTask = vex::task(liftArmIntake);
  liftArmIntakeTask.suspend();

 
  // place driver control in this while loop
  while (true) {

    /*
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(1,40,"RPM:%f",ShootClose.velocity(vex::velocityUnits::rpm));
    Brain.Screen.printAt(1,80,"RPM:%f",ShootFar.velocity(vex::velocityUnits::rpm));
    Brain.Screen.printAt(1,120,"RPM:%f",IntakeMotor.velocity(vex::velocityUnits::rpm));\
    Brain.Screen.render();
    */
    Brain.Screen.setCursor(4, 1);
    Brain.Screen.print(EncoderC.velocity(rpm));
    Brain.Screen.setCursor(5, 1);
    Brain.Screen.print(ShootClose.velocity(rpm));
    Brain.Screen.setCursor(6, 1);
    Brain.Screen.print(ShootFar.velocity(rpm));
    double turnVal = Controller1.Axis4.position(percent);
    double forwardVal = Controller1.Axis3.position(percent);

    double turnVolts = -(turnVal) * 0.12;
    double forwardVolts = forwardVal * 0.12 * (1 - (std::abs(turnVolts)/12.0) * turnImportance);

    LFdrive.spin(forward, forwardVolts + turnVolts, voltageUnits::volt);
    LBdrive.spin(forward, forwardVolts + turnVolts, voltageUnits::volt);
    RFdrive.spin(forward, forwardVolts - turnVolts, voltageUnits::volt);
    RBdrive.spin(forward, forwardVolts - turnVolts, voltageUnits::volt);

    ControllerScreen.clearScreen();
    ControllerScreen.setCursor(0,0);
    ControllerScreen.print(Inertial.heading(degrees));
    
    if (Controller1.ButtonL2.pressing()){ //in
      IntakeMotor.spin(forward, 12.0 , voltageUnits::volt);
    }
    else if (Controller1.ButtonL1.pressing()){ //out
      IntakeMotor.spin(forward, -12.0 , voltageUnits::volt);
    }
    else{
      IntakeMotor.stop();
    }
    
    //user shoot that prevents voltage dropoff
    int r = 1800; //2100 --> 1800
    int rpmPrev = 1800;
    int temp = 1800;
    //changed v from int to double
    double v = 6.25; //6.25 --> 5.25
    //user shoot
    if (Controller1.ButtonR1.pressing()) {   
      temp = r;
      r = EncoderC.velocity(rpm);
      rpmPrev = temp;
      if(rpmPrev - r >= 100) {
        v = 10;
      }
      ShootClose.spin(forward, v, volt); //7
      ShootFar.spin(reverse, v, volt); //7
      v = 6.25;
      
    }
    else {
      ShootClose.stop(brake);
      ShootFar.stop(brake);
    }

    //moving the lift 
    //far
    if (Controller1.ButtonB.pressing()) {
      Lift.spin(reverse);
    } 
    else if (Controller1.ButtonY.pressing()) {
      liftArmFarTask.resume();
      if (liftArmFar() == 0) {
        liftArmFarTask.suspend();
      }
    }
    //intake
    else if (Controller1.ButtonX.pressing()){
      Lift.spin(forward);
    } 
    else if (Controller1.ButtonA.pressing()) {
      liftArmIntakeTask.resume();
      if (liftArmIntake() == 0) {
        liftArmIntakeTask.suspend();
      }
    }
    else {
      Lift.stop(hold);
    }
    

    //distance shoot
    if (Controller1.ButtonR2.pressing()) {
      ShootClose.spin(forward, 12, volt);
      ShootFar.spin(reverse, 12, volt);
    }

    //flywheel spins backwards to activate endgame
    if (Controller1.ButtonUp.pressing()) {
      ShootClose.spin(forward, -12, volt);
      ShootFar.spin(reverse, -12, volt);
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
