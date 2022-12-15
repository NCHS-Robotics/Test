/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\alexa                                            */
/*    Created:      Wed Nov 23 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LFdrive              motor         3               
// LBdrive              motor         1               
// RFdrive              motor         4               
// RBdrive              motor         2               
// ShaftEncoder         encoder       A, B            
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;
using namespace std;

vex::controller::lcd ControllerScreen = vex::controller::lcd();

//spin all the motors in a given direction
void driveAll(vex::directionType dir) {
  LFdrive.spin(dir);
  LBdrive.spin(dir);
  RFdrive.spin(dir);
  RBdrive.spin(dir);
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

//turn the robot in an arc path instead of the robot twisting in place
void arcTurn(int leftVolts, int rightVolts) {
  LFdrive.spin(forward, leftVolts, volt);
  LBdrive.spin(forward, leftVolts, volt);
  RFdrive.spin(forward, rightVolts, volt);
  RBdrive.spin(forward, rightVolts, volt);
}

//accelerate the robot by spinning at increasing speeds
//@param speed in volts
void accelerate(int speed) {
  for (int i = 0; i <= speed; i++) {
    spinVolts(forward, i);
  }
}

//implement two encoders for each side?
//^^only if using driveAll

//PID algorithm
//driving forward
void PID(int endValue) {
  double kP = 0.2;
  double kI = 0.14150;
  double kD = 0.02544; 

  int error = 0; //P
  int integral = 0; //total error
  int derivative = 0; //change in error

  int prevError = 0;
  int desiredValue = ShaftEncoder.rotation(degrees) + endValue; 
  int currentReading = ShaftEncoder.rotation(degrees);

  error = desiredValue - currentReading;

  while (error != 0) {
    currentReading = ShaftEncoder.rotation(degrees);
    
    error = desiredValue - currentReading;

    integral += error;

    /**
    if (error < 100 || error > -100) {
      integral += error;
    }
    else {
      integral = 0;
    }
    */

    derivative = error - prevError; 

    double speedVolt = (error * kP);// + (integral * kI) - (derivative * kD) ;

    ControllerScreen.clearScreen();
    ControllerScreen.setCursor(0,0);
    ControllerScreen.print(error);
    ControllerScreen.setCursor(0,10);
    ControllerScreen.print(speedVolt);


    spinVolts(forward, speedVolt);

    prevError = error;

  }
  stopAll(brakeType::brake);
}



void PID2(int endValue) {

  
  bool enableDrivePID = true;

  double kP = 0.015;
  double kI = 0.00012;
  double kD = 0.00075;

  int pos = ShaftEncoder.position(degrees);

  int desiredValue = pos + endValue;

  int error = 0; //SensorValue - DesiredValue : Position
  int prevError = 0; 
  int derivative; //error - prevError : Speed
  int totalError = 0; //totalError = totalError + error

  //resets motors
  bool resetDriveSensors = false;
  while (resetDriveSensors) {
    resetDriveSensors = false;

    LFdrive.setPosition(0, degrees);
    RFdrive.setPosition(0, degrees);
  }

  while(enableDrivePID) {
    
    /*
    if (error < 5 && error > -5) {
      wait(20, msec);
      if (error < 5 || error > -5) {
        enableDrivePID = false;
        break;
      }
    }
    */

    pos = ShaftEncoder.position(degrees);

    /**
    int leftMotorPosition = LFdrive.position(degrees);
    int rightMotorPosition = RFdrive.position(degrees);
    int averagePosition = (leftMotorPosition + rightMotorPosition)/2;
    */   
    
    //P
    error = desiredValue - pos;

    //Speed - D
    derivative = error - prevError;

    //Integral - I
    if (error < 133) {
      totalError += error;
    }

    double lateralMotorPower = (error * kP) + (derivative * kD) + (totalError * kI);

    spinVolts(forward, (-1 * lateralMotorPower));

    ControllerScreen.clearScreen();
    ControllerScreen.setCursor(0,0);
    ControllerScreen.print(error);
    ControllerScreen.setCursor(0,10);
    ControllerScreen.print(lateralMotorPower);

    prevError = error;
    vex::task::sleep(20);
  }
}


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

    /*
    double startValue = ShaftEncoder.rotation(degrees);
    while(ShaftEncoder.rotation(degrees) > startValue - 360) {
      driveAll(forward);
    }
    stopAll(brake);
    */

    /*  
    startValue = ShaftEncoder.rotation(degrees);
    while(ShaftEncoder.rotation(degrees) < startValue + 360) {
      driveAll(reverse);
    }
    stopAll(brake);
    */

    //PID doesn't work properly
    //PID(500);

    PID2(500);

    //controlla();

    //arcTurn(2,5);  //left, right
    
  
}