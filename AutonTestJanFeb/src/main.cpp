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
// LimitSwitchFar       limit         A               
// LimitSwitchIntake    limit         B               
// LineTrackerClose     line          D               
// LineTrackerFar       line          E               
// BumperRoller         bumper        C               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

//The controller screen
vex::controller::lcd ControllerScreen = vex::controller::lcd();

//initialize motors
void init() {
  Lift.setVelocity(80, percent); //60 --> 69 --> 80
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
  Inertial.setHeading(355, degrees);

  while(Inertial.heading(degrees) >= (360 - headingVal) || (Inertial.heading(degrees) < 1)) {
    LFdrive.spin(forward);
    LBdrive.spin(forward);
    RFdrive.spin(reverse);
    RBdrive.spin(reverse);
  }
  stopAll(brake);
  wait(0.5, sec);
}

void turnRightInertial(int headingVal) {
  setDrivePercentage(20);
  Inertial.setHeading(5, degrees);

  while(Inertial.heading(degrees) <= headingVal || (Inertial.heading(degrees) > 359)) {
    LFdrive.spin(reverse);
    LBdrive.spin(reverse);
    RFdrive.spin(forward);
    RBdrive.spin(forward);
  }
  stopAll(brake);
  wait(0.5, sec);
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
  
  //init();

  //shoot 2 discs
  shootDiscs(8.45); //8 --> 8.25 --> 8.45
  wait(4.3, sec);
  IntakeMotor.spinFor(forward, 1000, degrees); //550
  shootDiscs(8.6); //nothing --> 8.45 --> 8.6
  wait(1.5, sec);
  IntakeMotor.spinFor(forward, 2500, degrees); //1500
  stopDiscs();

  //raise lift
  while(!(LimitSwitchIntake.pressing())) {
    Lift.spin(forward);
  }
  Lift.stop(brake);

  //roll roller
  turnLeftInertial(83); //86 --> 83
  IntakeMotor.spin(forward);
  /*
  while (!(BumperRoller.pressing())) {
    driveAll(reverse);
  }
  stopAll(brake);  
  */
  driveAllFor(reverse, 630);
  wait(0.3, sec);
  IntakeMotor.stop(brake);

  //pick up corner disc and roll roller
  driveAllFor(forward, 1180);
  turnRightInertial(85);
  IntakeMotor.spin(forward);
  driveAllFor(reverse, 1650);
  wait(0.3, sec);
  IntakeMotor.stop(brake);
  driveAllFor(forward, 650); //500 --> 550 --> 650
  
  //lower lift
  while(!(LimitSwitchFar.pressing())) {
    Lift.spin(reverse);
  }
  Lift.stop(brake);

  //shoot 1 disc
  turnLeftInertial(83); //82 --> 79 --> 82 --> 83
  driveAllFor(forward, 910); //810 --> 910

  IntakeMotor.spinFor(reverse, 550, degrees);

  shootDiscs(8.1); //7.7 --> 7.95 --> 8.1
  wait(4.5, sec);
  IntakeMotor.spinFor(forward, 2000, degrees); //1500
  stopDiscs();

  turnLeftInertial(135); //135 --> 132 --> 135

  //raise lift
  while(!(LimitSwitchIntake.pressing())) {
    Lift.spin(forward);
  }
  //Lift.spinFor(forward, 3, degrees);
  Lift.stop(brake);
  
  //pick up 3 discs in a row 
  /*
  IntakeMotor.spin(forward);
  setDrivePercentage(15);
  driveAllFor(reverse, 5000);
  IntakeMotor.stop();
  
  */

  setDrivePercentage(15);
  driveAll(reverse);
  wait(2, sec);
  IntakeMotor.spin(forward);
  wait(1.5, sec);
  IntakeMotor.stop(brake);
  wait(0.75, sec);
  IntakeMotor.spin(forward);
  wait(1.25, sec);
  IntakeMotor.stop(brake);
  wait(0.65, sec);
  IntakeMotor.spin(forward);
  wait(1.5, sec);
  IntakeMotor.stop();
  wait(1.2, sec);
  stopAll(brake);

  turnRightInertial(77);

  
  //shoot endgame
  ShootClose.spin(forward, -12, volt);
  ShootFar.spin(forward, -12, volt);
  wait(3, sec);

  //shoot 3 discs
  IntakeMotor.spinFor(reverse, 450, degrees);
  shootDiscs(6.75); //6.5
  wait(3, sec);
  IntakeMotor.spinFor(forward, 750, degrees);
  shootDiscs(6.55); //6.3
  wait(3, sec);
  IntakeMotor.spinFor(forward, 1000, degrees);
  wait(3, sec);
  IntakeMotor.spinFor(forward, 2500, degrees);
  stopDiscs();
}

void sec15Roller() {
  IntakeMotor.spin(forward);
  wait(0.3, sec);
  IntakeMotor.stop();
  driveAllFor(forward, 400);  
  turnLeftInertial(6);
  while(!(LimitSwitchFar.pressing())) {
    Lift.spin(reverse);
  }
  Lift.stop(brake);
  shootDiscs(8.5);
  wait(4.5, sec);
  IntakeMotor.spinFor(forward, 1000, degrees);
  wait(1, sec);
  IntakeMotor.spinFor(forward, 1000, degrees);
}

void sec15Roller2() {
  setDrivePercentage(35);
  driveAllFor(forward, 1500);
  turnLeftInertial(74);
  shootDiscs(8.5);
  wait(4.5, sec);
  IntakeMotor.spinFor(forward, 1000, degrees);
  wait(1, sec);
  IntakeMotor.spinFor(forward, 1000, degrees);
  stopDiscs();
  while(!(LimitSwitchIntake.pressing())) {
    Lift.spin(forward);
  }
  Lift.stop(brake);
  turnLeftInertial(6);
  IntakeMotor.spin(forward);
  driveAllFor(reverse, 500);
  wait(0.3, sec);
  IntakeMotor.stop();
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  Lift.setVelocity(80, percent); //60 --> 69 --> 80
  IntakeMotor.setVelocity(100, percent);
  ShootClose.setVelocity(100, percent);
  ShootFar.setVelocity(100, percent);


  //sec15Roller2();

  //resetLiftIntake();
  //sec15Roller();

  resetLiftFar();
  auton();
}
