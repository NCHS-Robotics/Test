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
  Lift.setVelocity(100, percent); //60 --> 69 --> 80 --> 100
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

void turnR(int target) {
  bool i = Inertial.heading(degrees);


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
  //initialize lift task
  task liftFarTask = task(resetLiftFar);
  liftFarTask.suspend();
  task liftIntakeTask = task(resetLiftIntake);
  liftIntakeTask.suspend();
  

  //roll first roller
  driveAll(reverse);
  IntakeMotor.spin(reverse);
  wait(1, sec);
  IntakeMotor.stop(brake);
  stopAll(brake);

  //pick up corner disc and roll second roller
  driveAllFor(forward, 1250);
  turnRightInertial(90);
  IntakeMotor.spin(reverse);
  driveAll(reverse);
  wait(2, sec);
  IntakeMotor.stop(brake);
  stopAll(brake);

  //shoot all 3 discs
  driveAllFor(forward, 650);
  liftFarTask.resume();
  turnLeftInertial(83);
  IntakeMotor.spinFor(forward, 100, degrees);
  driveAll(forward);
  wait(1.7, sec);
  Lift.spin(forward);
  wait(0.1, sec);
  Lift.stop(hold);
  stopAll(brake);

  shootDiscs(12);
  wait(2.4, sec);
  IntakeMotor.spinFor(reverse, 500, degrees);
 
  IntakeMotor.spinFor(reverse, 650, degrees);
  
  IntakeMotor.spinFor(reverse, 950, degrees);
  stopDiscs();

  //shoot 2 discs
  /**
  shootDiscs(12);
  wait(3, sec);
  IntakeMotor.spinFor(reverse, 500, degrees); 
  wait(1, sec);
  IntakeMotor.spinFor(reverse, 500, degrees);
  stopDiscs();
  
  //raise lift
  liftIntakeTask.resume();

  //roll roller
  turnLeftInertial(83); 
  IntakeMotor.spin(forward);
  driveAll(reverse);
  wait(1.1, sec);
  IntakeMotor.stop(brake);
  
  //pick up corner disc and roll roller
  driveAllFor(forward, 1180);
  turnRightInertial(85);
  IntakeMotor.spin(forward);
  driveAllFor(reverse, 1650);
  wait(0.3, sec);
  IntakeMotor.stop(brake);
  driveAllFor(forward, 650); 
  
  
  //lower lift
  liftFarTask.resume();

  //shoot 1 disc
  turnLeftInertial(83); 
  driveAllFor(forward, 910);

  IntakeMotor.spinFor(reverse, 550, degrees);
  
  liftFarTask.suspend();

  shootDiscs(8.1); 
  wait(4.5, sec);
  IntakeMotor.spinFor(forward, 2000, degrees); //1500
  stopDiscs();

  liftIntakeTask.resume();
  

  //spit out disc and align for row
  IntakeMotor.spin(forward);  
  turnLeftInertial(135); 

  //pick up discs and align for shooting discs
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
  IntakeMotor.spinFor(reverse, 450, degrees);

  
  //shoot endgame
  ShootClose.spin(forward, -12, volt);
  ShootFar.spin(forward, -12, volt);
  wait(4, sec);
  

  //shoot 3 discs
  shootDiscs(6.75); 
  wait(3, sec);
  IntakeMotor.spinFor(forward, 750, degrees);
  shootDiscs(6.55);
  wait(3, sec);
  IntakeMotor.spinFor(forward, 1000, degrees);
  wait(3, sec);
  IntakeMotor.spinFor(forward, 2500, degrees);
  stopDiscs();

  **/
}

void sec15Roller() {
  //setDrivePercentage(35);

  task liftFarTask = task(resetLiftFar);
  liftFarTask.suspend();
  task liftIntakeTask = task(resetLiftIntake);
  liftIntakeTask.suspend();
    
  ControllerScreen.print("shooting");
  shootDiscs(12);
  driveAll(forward);
  wait(.35, sec);
  stopAll(brake);
  wait(0.15,sec);


  turnLeftInertial(9);
  wait(3.5, sec);
  IntakeMotor.spinFor(reverse, 500, degrees);
  wait(1.5, sec);
  IntakeMotor.spinFor(reverse, 950, degrees);
  stopDiscs();

  ControllerScreen.setCursor(1,0);
  ControllerScreen.print("roller-movingback");
  liftIntakeTask.resume();
  
  turnRightInertial(9);
  IntakeMotor.spin(forward);
  driveAll(reverse); //changed
  wait(1.25, sec);
  IntakeMotor.stop(brake);
  stopAll(brake);

  ControllerScreen.clearScreen();
  ControllerScreen.setCursor(1,0);
  ControllerScreen.print("roller-done");
  driveAllFor(forward, 525);
  //pidLeft(79.5);
  turnLeftInertial(130);

  IntakeMotor.spin(reverse);
  wait(0.5, sec);

  driveAllFor(reverse, 3000);
  wait(.25, sec);
   
  IntakeMotor.stop(brake);
  
  turnRightInertial(100);
}

void sec15Far() {
  //setDrivePercentage(35);

  task liftFarTask = task(resetLiftFar);
  liftFarTask.suspend();
  task liftIntakeTask = task(resetLiftIntake);
  liftIntakeTask.suspend();
  
  driveAllFor(forward, 1500);
  turnLeftInertial(74);
  shootDiscs(12);
  wait(4, sec);
  IntakeMotor.spinFor(reverse, 500, degrees);
  wait(1.35, sec);
  IntakeMotor.spinFor(reverse, 950, degrees);
  stopDiscs();

  liftIntakeTask.resume();

  turnLeftInertial(6);
  IntakeMotor.spin(forward);
  driveAll(reverse);
  wait(0.75, sec);
  IntakeMotor.stop();

  driveAllFor(forward, 500);
  turnRightInertial(120);
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
    ControllerScreen.print("calibrating");
  }
  ControllerScreen.clearScreen();
  ControllerScreen.setCursor(0,0);
  wait(1, sec);
  
  //"pre auton function" (need to also calibrate inertial sensor in function)
  Lift.setVelocity(100, percent); //60 --> 69 --> 80 --> 100
  IntakeMotor.setVelocity(100, percent);
  ShootClose.setVelocity(100, percent);
  ShootFar.setVelocity(100, percent);
  setDrivePercentage(35);

  //ready 
  //sec15Roller();
  //sec15Far(); //shooter off?? test again tmrw (3/6)

  //needs work
  skillsAuton();
  
}
