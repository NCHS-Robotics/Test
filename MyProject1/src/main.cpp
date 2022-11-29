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
// LFdrive              motor         1               
// LBdrive              motor         11              
// RFdrive              motor         10              
// RBdrive              motor         20              
// ShaftEncoder         encoder       A, B            
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

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
  double kP = .001;
  double kI = .00001;
  double kD = .00001; 

  int error = 0; //P
  int totalError = 0; //I
  int changeInError = 0; //D

  int prevError = 0;
  int desiredValue = ShaftEncoder.rotation(degrees) - endValue;
  int currentReading = ShaftEncoder.rotation(degrees);

  error = desiredValue - currentReading;

  while (error != 0) {
    currentReading = ShaftEncoder.rotation(degrees);
    error = desiredValue - currentReading;
    totalError += error;
    changeInError = prevError - error;

    double speedVolt = (error * kP) + (totalError * kI) + (changeInError * kD);

    ControllerScreen.clearScreen();
    ControllerScreen.print(speedVolt);

    spinVolts(forward, speedVolt);

    prevError = error;

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

    //PI doesn't work properly
    PID(1000);

    //arcTurn(2,5);  //left, right
    
  
}
