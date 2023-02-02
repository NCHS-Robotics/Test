/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\alexa                                            */
/*    Created:      Tue Jan 17 2023                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LBdrive              motor         1               
// LFdrive              motor         11              
// RBdrive              motor         2               
// RFdrive              motor         4               
// IntakeMotor          motor         5               
// ShootClose           motor         6               
// ShootFar             motor         7               
// Controller1          controller                    
// Endgame              motor         20              
// Lift                 motor         8               
// Inertial             inertial      12              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

//The controller screen
vex::controller::lcd ControllerScreen = vex::controller::lcd();

//initialize motors
void init() {
  IntakeMotor.setVelocity(100, percent);

  ShootClose.setVelocity(100, percent);
  ShootFar.setVelocity(100, percent);
}

//set drive percentage
void setDrivePercentage(int p) {
  LFdrive.setVelocity(p, percent);
  LBdrive.setVelocity(p, percent);
  RFdrive.setVelocity(p, percent);
  RBdrive.setVelocity(p, percent);
}

//spin all the motors in a given direction
void driveAll(vex::directionType dir) {
  LFdrive.spin(dir);
  LBdrive.spin(dir);
  RFdrive.spin(dir);
  RBdrive.spin(dir);
}

//driveAll with set degrees
void driveAllFor(vex::directionType dir, int degrees) {
  LFdrive.startRotateFor(dir, degrees, deg);
  LBdrive.startRotateFor(dir, degrees, deg);
  RFdrive.startRotateFor(dir, degrees, deg);
  RBdrive.spinFor(dir, degrees, deg);
}

//stop all the motors given a brakeType
void stopAll(vex::brakeType stopType) {
  LFdrive.stop(stopType);
  LBdrive.stop(stopType);
  RFdrive.stop(stopType);
  RBdrive.stop(stopType);
}

//spin all the motors given a direction and voltage
void spinVolts(directionType dir, int volts) {
  LFdrive.spin(dir, volts, volt);
  LBdrive.spin(dir, volts, volt);
  RFdrive.spin(dir, volts, volt);
  RBdrive.spin(dir, volts, volt);
}

//turn the robot
void turnLeft(int degrees) {
  LFdrive.startRotateFor(forward, degrees, deg);
  LBdrive.startRotateFor(forward, degrees, deg);
  RFdrive.startRotateFor(reverse, degrees, deg);
  RBdrive.spinFor(reverse, degrees, deg);
}

void turnRight(int degrees) {
  LFdrive.startRotateFor(reverse, degrees, deg);
  LBdrive.startRotateFor(reverse, degrees, deg);
  RFdrive.startRotateFor(forward, degrees, deg);
  RBdrive.spinFor(forward, degrees, deg);
}

//using inertial sensor
void turnLeftInertial(int headingVal) {
  setDrivePercentage(20);
  Inertial.setHeading(359.99, degrees);

  while(Inertial.heading(degrees) >= (360 - headingVal) || (Inertial.heading(degrees) < 1)) {
    ControllerScreen.clearScreen();
    ControllerScreen.setCursor(0,0);
    ControllerScreen.print(Inertial.heading(degrees));

    LFdrive.spin(forward);
    LBdrive.spin(forward);
    RFdrive.spin(reverse);
    RBdrive.spin(reverse);
  }
  stopAll(brake);
  ControllerScreen.clearScreen();
  ControllerScreen.setCursor(0,0);
  ControllerScreen.print("done turning");
  ControllerScreen.setCursor(5,0);
  ControllerScreen.print(Inertial.heading(degrees));
}

void turnRightInertial(int headingVal) {
  setDrivePercentage(20);
  Inertial.setHeading(0.1, degrees);

  while(Inertial.heading(degrees) <= headingVal || (Inertial.heading(degrees) > 359)) {
    ControllerScreen.clearScreen();
    ControllerScreen.setCursor(0,0);
    ControllerScreen.print(Inertial.heading(degrees));

    LFdrive.spin(reverse);
    LBdrive.spin(reverse);
    RFdrive.spin(forward);
    RBdrive.spin(forward);
  }
  stopAll(brake);
  ControllerScreen.clearScreen();
  ControllerScreen.setCursor(0,0);
  ControllerScreen.print("done turning");
  ControllerScreen.setCursor(5,0);
  ControllerScreen.print(Inertial.heading(degrees));
}

//shoot the discs
void shootDiscs(double volts) {
  ShootClose.spin(forward, volts, volt);
  ShootFar.spin(forward, volts, volt);
}

void shootDiscsFor(int deg) {
  ShootClose.spinFor(forward, deg, degrees);
  ShootFar.spinFor(forward, deg, degrees);
}

void stopDiscs() {
  IntakeMotor.stop(brake);
  ShootClose.stop(brake);
  ShootFar.stop(brake);
}



//PI Controller to move forward and back
void pi(int endValue) {  
  bool enableDrivePID = true;

  //double kP = 0.065; //dummy bot
  //main bot w 3 motors
  //double kP = 0.109; //no I variable
  double kP = 0.02; 
  double kI = 0.005;
  //double kD = 0.02; //breaks integral varibale

  //constants not used (set for dummy bot)
  
  //double kD = 0.2;

  //int pos = ShaftEncoder.position(degrees);
  int pos = (LFdrive.position(deg) + RFdrive.position(deg)) / 2;

  int desiredValue = pos + endValue;

  int error = 0; //SensorValue - DesiredValue : Position
  int prevError = 0; 
  int derivative; //error - prevError : Speed
  int totalError = 0; //totalError = totalError + error

  int countCheck = 0;

  while(enableDrivePID && countCheck < 5) {
    //pos = ShaftEncoder.position(degrees);

    
    int leftMotorPosition = LFdrive.position(degrees);
    int rightMotorPosition = RFdrive.position(degrees);
    int averagePosition = (leftMotorPosition + rightMotorPosition) / 2;
      
    
    //P
    error = desiredValue - averagePosition;

    //Speed - D
    derivative = error - prevError;

    //Integral - I
    
    if (error < 50 && error > -50) {
      totalError += error;
    }
    
    //totalError += error;

    double lateralMotorPower = (error * kP) + (totalError * kI);

    spinVolts(forward, lateralMotorPower);

    ControllerScreen.clearScreen();
    ControllerScreen.setCursor(0,0);
    ControllerScreen.print(error);
    ControllerScreen.setCursor(0,10);
    ControllerScreen.print(lateralMotorPower);

    prevError = error;

    if (error < 10 && error > -10) {
        countCheck++;
    }

    vex::task::sleep(20);
  }

  stopAll(brake);
}

void testInertial() {
  setDrivePercentage(5);

  Inertial.setHeading(0.01, degrees);

  ControllerScreen.clearScreen();
  ControllerScreen.print(Inertial.heading(degrees));
  //ControllerScreen.print(Inertial.heading(degrees) <= 90);

  for (int i = 0; i < 4; i++) {

    Inertial.setHeading(0, degrees);

    pi(500);

    while (Inertial.heading(degrees) <= 90 || Inertial.heading(degrees) > 355) {
      ControllerScreen.clearScreen();
      ControllerScreen.setCursor(0,0);
      ControllerScreen.print(Inertial.heading(degrees));
      /*
      LFdrive.spin(forward);
      LBdrive.spin(forward);
      RFdrive.spin(reverse);
      RBdrive.spin(reverse);  
      */
      
      /**/
      LFdrive.spin(reverse);
      LBdrive.spin(reverse);
      RFdrive.spin(forward);
      RBdrive.spin(forward);
    }
    

    stopAll(brake);
    ControllerScreen.clearScreen();
    ControllerScreen.print("Done Turning");
    ControllerScreen.setCursor(7,0);
    ControllerScreen.print(Inertial.heading(degrees));
  }
}

void resetLiftFar() {
  while (!(LimitSwitchFar.pressing())) {
    Lift.spin(reverse);
  }
  Lift.stop(brake);
}

void resetLiftIntake() {
  while(!(LimitSwitchIntake.pressing())) {
    Lift.spin(forward);
  }
  Lift.spinFor(forward, 3, degrees);
  Lift.stop(brake);
}

void auton() {
  
  init();

  //shoot 2 discs
  shootDiscs(7.8);
  wait(4.3, sec);
  IntakeMotor.spinFor(forward, 1000, degrees); //550
  wait(1.5, sec);
  IntakeMotor.spinFor(forward, 2500, degrees); //1500
  stopDiscs();

  //raise lift
  while(!(LimitSwitchIntake.pressing())) {
    Lift.spin(forward);
  }
  Lift.spinFor(forward, 3, degrees);
  Lift.stop(brake);

  //roll roller
  turnLeftInertial(82);
  driveAllFor(reverse, 630);
  IntakeMotor.spin(forward);
  wait(0.3, sec);
  IntakeMotor.stop(brake);

  //pick up corner disc and roll roller
  driveAllFor(forward, 1100);
  turnRightInertial(80);
  IntakeMotor.spin(forward);
  driveAllFor(reverse, 1420);
  wait(0.3, sec);
  IntakeMotor.stop(brake);

  //roll roller
  IntakeMotor.spinFor(forward, 200, degrees);
  driveAllFor(forward, 630);
  
  //lower lift
  while(!(LimitSwitchFar.pressing())) {
    Lift.spin(reverse);
  }
  Lift.stop(brake);

  //shoot 1 disc
  turnLeftInertial(78);
  driveAllFor(forward, 900);
  shootDiscs(8);
  wait(4.5, sec);
  IntakeMotor.spinFor(forward, 2000, degrees); //1500
  stopDiscs();

  //raise lift
  while(!(LimitSwitchIntake.pressing())) {
    Lift.spin(forward);
  }
  Lift.spinFor(forward, 3, degrees);
  Lift.stop(brake);

  //pick up 3 discs in a row
  turnLeftInertial(135);
  IntakeMotor.spin(forward);
  setDrivePercentage(15);
  driveAllFor(reverse, 5000);
  IntakeMotor.stop();

  //shoot 3 discs
  turnRightInertial(80);
  shootDiscs(6);
  wait(3, sec);
  IntakeMotor.spinFor(forward, 500, degrees);
  wait(3, sec);
  IntakeMotor.spinFor(forward, 500, degrees);
  wait(3, sec);
  IntakeMotor.spinFor(forward, 500, degrees);
  stopDiscs();

  //pick up pile of discs
  //shoot all 3 discs
  //roll roller
  //pick up corner disc
  //get roller
  //shoot 1 disc
  //pick up pile of discs
  //shoot 3 discs
  //pick up 3 discs in a line
  //shoot 3 discs
  //go to corner
  //endgame
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  while(Inertial.isCalibrating()) {
    ControllerScreen.clearScreen();
    ControllerScreen.setCursor(0,0);
    ControllerScreen.print("calibrating");
  }
  ControllerScreen.clearScreen();
    ControllerScreen.setCursor(0,0);
    ControllerScreen.print("done calibrating");

  resetLiftFar();
  auton();

  //resetLiftFar();
  //resetLiftIntake();
}
