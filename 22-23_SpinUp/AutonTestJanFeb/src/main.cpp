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
// Inertial             inertial      10              
// LimitSwitchFar       limit         A               
// LimitSwitchIntake    limit         B               
// ShaftEncoderFlywheel encoder       C, D            
// ---- END VEXCODE CONFIGURED DEVICES ----
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\alexa                                            */
/*    Created:      Tue Jan 17 2023                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <sstream>

using namespace vex;

//The controller screen
vex::controller::lcd ControllerScreen = vex::controller::lcd();

//initialize shoot discs variables
bool enableShooter;
int r = 0;
double v = 0.0;

//initialize motors
void init() {
  Lift.setVelocity(100, percent);
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
void turnLeft(double volts) {
  LFdrive.spin(forward, volts, volt);
  LBdrive.spin(forward, volts, volt);
  RFdrive.spin(reverse, volts, volt);
  RBdrive.spin(reverse, volts, volt);
}

void turnRight(double volts) {
  LFdrive.spin(reverse, volts, volt);
  LBdrive.spin(reverse, volts, volt);
  RFdrive.spin(forward, volts, volt);
  RBdrive.spin(forward, volts, volt);
}

//turn the robot for degrees
void turnLeftFor(int degrees) {
  LFdrive.startRotateFor(forward, degrees, deg);
  LBdrive.startRotateFor(forward, degrees, deg);
  RFdrive.startRotateFor(reverse, degrees, deg);
  RBdrive.spinFor(reverse, degrees, deg);
}

void turnRightFor(int degrees) {
  LFdrive.startRotateFor(reverse, degrees, deg);
  LBdrive.startRotateFor(reverse, degrees, deg);
  RFdrive.startRotateFor(forward, degrees, deg);
  RBdrive.spinFor(forward, degrees, deg);
}

//using inertial sensor
void turnLeftInertial(int headingVal) {
  Inertial.setHeading(355, degrees);

  while(Inertial.heading(degrees) >= (360 - headingVal) || (Inertial.heading(degrees) < 1)) {
    LFdrive.spin(forward);
    LBdrive.spin(forward);
    RFdrive.spin(reverse);
    RBdrive.spin(reverse);
  }
  stopAll(brake);
  wait(0.1, sec);
}

void turnRightInertial(int headingVal) {
  Inertial.setHeading(5, degrees);

  while(Inertial.heading(degrees) <= headingVal || (Inertial.heading(degrees) > 359)) {
    LFdrive.spin(reverse);
    LBdrive.spin(reverse);
    RFdrive.spin(forward);
    RBdrive.spin(forward);
  }
  stopAll(brake);
  wait(0.1, sec);
}

//PID turning
void pidLeft(int target) {
  double kP = .02;
  double kI = .005;
  double kD = .0005;
  //int maxSpeed = 12; //12 for volts | 100 for percent
  //target = 360 - target;
  double speed;
  //int countCheck = 0;

  int error = 0;
  int prevError = 0;
  int derivative = 0;
  int totalError = 0;

  Inertial.setHeading(359, degrees);
  int checkVal = 360 - target;

  while (Inertial.heading(degrees) >= (360 - target)) {

    ControllerScreen.clearScreen();
    ControllerScreen.setCursor(0,0);
    ControllerScreen.print(Inertial.heading(degrees));
    ControllerScreen.setCursor(2,0);
    ControllerScreen.print(speed);

    prevError = error; 
    error = Inertial.heading(degrees) - target;
    derivative = error - prevError;
    totalError += error;

    speed = (error * kP) + (totalError * kI);// + (derivative * kD);

    turnLeft(speed);

    wait(20, msec);
  }
  stopAll(brake);
  ControllerScreen.clearScreen();
  ControllerScreen.setCursor(0,0);
  ControllerScreen.print(Inertial.heading(degrees));
  ControllerScreen.setCursor(2,0);
  ControllerScreen.print(checkVal);
}

void pidRight(int target) {
  double kP = .02;
  double kI = .005;
  double kD = .0005;
  //int maxSpeed = 12; //12 for volts | 100 for percent
  //target = 360 - target;
  double speed;
  //int countCheck = 0;

  int error = 0;
  int prevError = 0;
  int derivative = 0;
  int totalError = 0;

  Inertial.setHeading(1, degrees);
  int checkVal = target;

  while (Inertial.heading(degrees) <= (target)) {

    ControllerScreen.clearScreen();
    ControllerScreen.setCursor(0,0);
    ControllerScreen.print(Inertial.heading(degrees));
    ControllerScreen.setCursor(2,0);
    ControllerScreen.print(speed);

    prevError = error; 
    error = Inertial.heading(degrees) - target;
    derivative = error - prevError;
    totalError += error;

    speed = (error * kP) + (totalError * kI);// + (derivative * kD);

    turnRight(speed);

    wait(20, msec);
  }
  stopAll(brake);
  ControllerScreen.clearScreen();
  ControllerScreen.setCursor(0,0);
  ControllerScreen.print(Inertial.heading(degrees));
  ControllerScreen.setCursor(2,0);
  ControllerScreen.print(checkVal);
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

//stop the flywheel
void stopDiscs() {
  IntakeMotor.stop(brake);
  ShootClose.stop(brake);
  ShootFar.stop(brake);
  enableShooter = false;
}

//calculate voltage for shooter
int vCalc(int v) {
  while(ShaftEncoderFlywheel.velocity(rpm) <= 2500 || v >= 12) {
    v = v * 1.2;
  }
  return v;
}

//shooting that prevents voltage dropoff
int shootDiscs2() {
    int rpmPrev = r + 100;
    int temp = r + 100;
    double vTemp;
    while (true) {   
      temp = r;
      r = ShaftEncoderFlywheel.velocity(rpm);
      rpmPrev = temp;
      if(rpmPrev - r >= 100) {
        vTemp = v;
        //v = v * 1.15;
        v = 10;
      }
      ShootClose.spin(forward, v, volt); 
      ShootFar.spin(forward, v, volt); 
      v = vTemp;
    }

    return 0;
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

//reset the lift for shooting far
int resetLiftFar() {
  while (!(LimitSwitchFar.pressing())) {
    Lift.spin(reverse);
  }
  Lift.stop(brake);

  return 0;
}

//reset the lift for the intake
int resetLiftIntake() {
  while(!(LimitSwitchIntake.pressing())) {
    Lift.spin(forward);
  }
  Lift.stop(brake);

  return 0;
}

void skillsAuton() {
  setDrivePercentage(35);
  
  //initialize lift task
  task liftFarTask = task(resetLiftFar);
  liftFarTask.suspend();
  task liftIntakeTask = task(resetLiftIntake);
  liftIntakeTask.suspend();
  
  //resetLiftIntake();

  //roll first roller
  driveAll(reverse);
  wait(0.75, sec);
  stopAll(brake);
  IntakeMotor.spin(reverse);
  wait(0.3, sec);
  IntakeMotor.stop(brake);

  //pick up corner disc and roll second roller
  driveAllFor(forward, 1250);
  IntakeMotor.spinFor(forward, 300, degrees);
  turnRightInertial(91);
  IntakeMotor.spin(reverse);
  driveAll(reverse);
  wait(1.5, sec);
  stopAll(brake);
  wait(0.7, sec);
  IntakeMotor.stop(brake);
  driveAll(reverse);
  IntakeMotor.spin(reverse);
  wait(0.3, sec); //configure at comp
  stopAll(brake);
  IntakeMotor.stop(brake);

  /*
  //shoot all 3 discs
  driveAllFor(forward, 650);
  liftFarTask.resume();
  turnLeftInertial(84);
  IntakeMotor.spinFor(forward, 150, degrees); //75 --> 150
  driveAll(forward);
  wait(1.7, sec);
  Lift.spin(forward);
  wait(0.1, sec);
  Lift.stop(hold);
  stopAll(brake);

  //turnRightInertial(4);

  shootDiscs(10.5);
  wait(2.6, sec);
  IntakeMotor.spinFor(reverse, 500, degrees);
  wait(0.2, sec);
  IntakeMotor.spinFor(reverse, 650, degrees);
  wait(0.2, sec);
  IntakeMotor.spinFor(reverse, 950, degrees);
  stopDiscs();

  //turnLeftInertial(4);

  liftFarTask.suspend();
  liftIntakeTask.resume();

  //pick up 3 discs in a row and shoot them
  driveAllFor(reverse, 1350);
  turnLeftInertial(126);

  IntakeMotor.spin(reverse);
  driveAllFor(reverse, 4500);
  IntakeMotor.stop(brake);

  turnRightInertial(66);
  IntakeMotor.spinFor(forward, 100, degrees);
  shootDiscs(10.3);
  Lift.spin(reverse);
  wait(0.95, sec);
  Lift.stop(brake);
  wait(1, sec);
  IntakeMotor.spinFor(reverse, 2100, degrees);
  stopDiscs();

  //get 3 stack of discs
  liftIntakeTask.suspend();
  liftIntakeTask.resume();

  //added while loop bc task doesn't work???
  while(!(LimitSwitchIntake.pressing())) {
    Lift.spin(forward);
  }
  Lift.stop(brake);

  turnLeftInertial(65);
  IntakeMotor.spin(reverse);
  driveAllFor(reverse, 3150);
  IntakeMotor.stop(brake);
  driveAllFor(forward, 150);

  //shoot 3 stack of discs
  liftIntakeTask.suspend();
  liftFarTask.resume();
  turnRightInertial(31);
  IntakeMotor.spinFor(forward, 75, degrees);
  driveAll(forward);
  wait(1.7, sec);
  Lift.spin(forward);
  wait(0.1, sec);
  Lift.stop(hold);
  stopAll(brake);

  shootDiscs(8);
  wait(2.6, sec);
  IntakeMotor.spinFor(reverse, 500, degrees);
  wait(0.3, sec);
  IntakeMotor.spinFor(reverse, 650, degrees);
  wait(0.2, sec);
  IntakeMotor.spinFor(reverse, 950, degrees);
  stopDiscs();
  
  //go back to roller and spin roller
  liftFarTask.suspend();
  liftIntakeTask.resume();

  driveAll(reverse);
  wait(2.35, sec);
  stopAll(brake);
  
  turnLeftInertial(80);

  driveAll(reverse);
  wait(1, sec);
  stopAll(brake);
  IntakeMotor.spin(reverse);
  wait(0.3, sec);
  IntakeMotor.stop(brake);
  
  //pick up corner disc and spin other roller
  driveAllFor(forward, 1050);
  turnRightInertial(80);
  
  IntakeMotor.spin(reverse);
  driveAll(reverse);
  wait(1.5, sec);
  IntakeMotor.stop(brake);
  stopAll(brake);
  IntakeMotor.spin(reverse);
  wait(0.3, sec);
  IntakeMotor.stop(brake);
  stopAll(brake);

  //pick up 3 stray discs
  driveAllFor(forward, 500);
  IntakeMotor.spin(forward);
  turnRightInertial(81);
  driveAllFor(reverse, 1250);
  turnRightInertial(34);
  IntakeMotor.stop(brake);
  IntakeMotor.spin(reverse);
  driveAllFor(reverse, 4500);
  IntakeMotor.stop(brake);

  //shoot 3 discs
  turnRightInertial(65);
  IntakeMotor.spinFor(forward, 100, degrees);
  shootDiscs(10.25);
  Lift.spin(reverse);
  wait(0.95, sec);
  Lift.stop(brake);
  wait(1, sec);
  IntakeMotor.spinFor(reverse, 2100, degrees);
  stopDiscs();
  
  liftIntakeTask.suspend();
  liftFarTask.resume();

  //go back and endgame
  turnLeftInertial(65);
  setDrivePercentage(100);
  shootDiscs(-12);
  resetLiftIntake();
  driveAll(forward);
  wait(1.27, sec);
  stopAll(brake);
  turnRightInertial(6); 
}

void sec15Roller() {
  //setDrivePercentage(35);
  task liftFarTask = task(resetLiftFar);
  liftFarTask.suspend();
  task liftIntakeTask = task(resetLiftIntake);
  liftIntakeTask.suspend();

  //liftIntakeTask.resume();

  //spin the roller
  driveAll(reverse);
  wait(0.2, sec);
  stopAll(brake);
  IntakeMotor.spin(forward);
  wait(0.16, sec);
  IntakeMotor.stop(brake);

  //liftIntakeTask.suspend();
  liftFarTask.resume();
    
  //shoot the discs
  shootDiscs(12);
  driveAll(forward);
  wait(.35, sec);
  stopAll(brake);
  wait(0.15,sec);

  turnLeftInertial(7);
  wait(3.5, sec);
  IntakeMotor.spinFor(reverse, 500, degrees);
  wait(1.5, sec);
  IntakeMotor.spinFor(reverse, 950, degrees);
  stopDiscs();

  liftFarTask.suspend();
  liftIntakeTask.resume();

  //turn and get discs
  turnRightInertial(7);
  driveAll(reverse);
  wait(0.35, sec);
  stopAll(brake);
  turnLeftInertial(130);

  IntakeMotor.spin(reverse);
  wait(0.5, sec);
  driveAllFor(reverse, 3000);
  wait(.25, sec);
  IntakeMotor.stop(brake);
  */
}

void sec15Far() {
  //setDrivePercentage(35);

  task liftFarTask = task(resetLiftFar);
  liftFarTask.suspend();
  task liftIntakeTask = task(resetLiftIntake);
  liftIntakeTask.suspend();

  liftFarTask.resume();
  
  driveAllFor(forward, 1500);
  turnLeftInertial(74);
  shootDiscs(12);
  wait(3.75, sec);
  IntakeMotor.spinFor(reverse, 500, degrees);
  wait(1.35, sec);
  IntakeMotor.spinFor(reverse, 950, degrees);
  stopDiscs();

  liftFarTask.suspend();
  liftIntakeTask.resume();

  turnLeftInertial(6);
  IntakeMotor.spin(forward);
  driveAll(reverse);
  wait(0.55, sec);//calibrate timing at state
  IntakeMotor.stop();

  driveAllFor(forward, 500);
  turnRightInertial(122);
  resetLiftIntake();
  setDrivePercentage(60);
  IntakeMotor.spin(reverse);
  driveAllFor(reverse, 4400);
  IntakeMotor.stop();
}

int printRPM() {
  while (true) {
    ControllerScreen.clearScreen();
    ControllerScreen.setCursor(0,0);
    ControllerScreen.print(ShaftEncoderFlywheel.velocity(rpm));
  }
  return 0;
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  //calibrate inertial
  while(Inertial.isCalibrating()) {
    //ControllerScreen.print("calibrating");
  }
  //ControllerScreen.clearScreen();
  //ControllerScreen.setCursor(0,0);
  wait(1, sec);
  
  //"pre auton function" (need to also calibrate inertial sensor in function)
  Lift.setVelocity(100, percent);
  IntakeMotor.setVelocity(100, percent);
  ShootClose.setVelocity(100, percent);
  ShootFar.setVelocity(100, percent);
  setDrivePercentage(35);

  //sec15Roller();
  //sec15Far();
  skillsAuton(); 
}