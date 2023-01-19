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
// Inertial             inertial      16              
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

//shoot the discs
void shootDiscs(int wait1, int wait2) {
  ShootClose.spin(forward);
  ShootFar.spin(forward);
  wait(wait1, sec);
  IntakeMotor.spin(forward);
  wait(wait2, sec);
  IntakeMotor.stop(brake);
  ShootClose.stop(brake);
  ShootFar.stop(brake);
}

void shootDiscsFor(int deg) {
  ShootClose.spinFor(forward, deg, degrees);
  ShootFar.spinFor(forward, deg, degrees);
}

//stop all the motors given a brakeType
void stopAll(vex::brakeType stopType) {
  LFdrive.stop(stopType);
  LBdrive.stop(stopType);
  RFdrive.stop(stopType);
  RBdrive.stop(stopType);
}

//test the lift
void testLift() {
  Lift.setPosition(0, degrees);
  while(true) {
    ControllerScreen.clearScreen();
    ControllerScreen.print(Lift.position(degrees));
  }
}

//turning w initial sensor
void turnLeftInertial(int head) {
  setDrivePercentage(15);
  LFdrive.spin(forward);
  LBdrive.spin(forward);
  RFdrive.spin(reverse);
  RBdrive.spin(reverse);
  int stopValue = Inertial.rotation(degrees) + head;
  if ((Inertial.rotation(degrees) > stopValue - 5) && (Inertial.rotation(degrees) < stopValue + 5)) {
    ControllerScreen.print("Done");
    stopAll(brake);
  }

  /*
  if ((Inertial.heading(degrees) > head - 5) && (Inertial.heading(degrees) < head + 5)) {
    stopAll(brake);
  }
  */
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

  /*
  //resets motors
  bool resetDriveSensors = false;
  while (resetDriveSensors) {
    resetDriveSensors = false;

    LFdrive.setPosition(0, degrees);
    RFdrive.setPosition(0, degrees);
  }
  */

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

void auton() {
  
  init();

  //LIFT MOVES 1429 DEGREES TO SWITCH FROM LOW TO HIGH
  //need to shoot first then move lift up and go to roller
  //forward = down, reverse = up (lift)

  //shoot 2 discs
  shootDiscs(4, 3);

  //lift shooter
  Lift.spinFor(reverse, 1429, degrees);

  //roll roller
  turnLeft(600);
  IntakeMotor.spin(forward);
  driveAllFor(reverse, 250);
  wait(0.3, sec);
  driveAllFor(forward, 250);
  IntakeMotor.stop();

  //pick up corner disc
  turnRight(900);
  IntakeMotor.spin(forward);
  pi(500);
  IntakeMotor.stop();

  //roll roller
  turnLeft(900);
  IntakeMotor.spin(forward);
  driveAllFor(reverse, 250);
  wait(0.3, sec);
  driveAllFor(forward, 250);
  IntakeMotor.stop();

  //shoot 1 disc
  turnLeft(600);
  shootDiscs(4, 2);


  //pick up 3 discs in a row
  //shoot 3 discs
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
    ControllerScreen.print("done");

  //testLift();
  turnLeftInertial(90);

  //auton();


  //auton();

}
