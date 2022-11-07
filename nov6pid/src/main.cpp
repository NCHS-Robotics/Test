#include "main.h"

//time: 23:50 
//2.0 video speed

//INITALIZE MOTORS BEFORE RUNNING

//LeftMotor
//RightMotor

#define LEFT_MOTOR_PORT 1
#define RIGHT_MOTOR_PORT 2


//settings - constants
double kP = 0.0;
double kI = 0.0;
double kD = 0.0;



int error; //SensorValue  - DesiredValue : Position (delta x)
int prevError = 0; //Position from a set time before (delay)
int derivate; //different between error and prevError, error - prevError = Speed (rate of change in error)
int totalError = 0; //totalError = totalError + error

int turnError;
int turnPrevError = 0;
int turnDerivative;
int turnTotalError = 0;

//autonomous settings
int desiredValue = 200; //want the motor to go 200 units
int turnDesiredValue = 0;

//variables modified for use
bool enableDrivePID = true;

//PID function
int drivePID() {
	while(enableDrivePID) {

		int leftMotorPosition = LeftMotor.get_position(); //absolute position in enconder units
		int rightMotorPosition = RightMotor.get_postiion();


		////////////////////////////////////
		//Lateral Movememtn PID
		////////////////////////////////////

		//get the average of both motors
		int averagePosition = (leftMotorPosition + rightMotorPosition) / 2;

		//Potential
		error = averagePosition - desiredValue;

		//Derivative
		derivate = error - prevError;

		//Velocity -> Position -> Absement
		//Integral
		totalError += error;


		///////////////////////////////////////
		//Turning movement PID
		///////////////////////////////////////

		int turnDifference = leftMotorPosition - rightMotorPosition;

		//Potential
		turnError = turnAveragePosition - turnDesiredValue;

		//Derivative
		turnDerivative = turnError - turnPrevError;

		//Integral
		turnTotalError += turnError;

		double turnMotorPower = (turnError * kP + turnDerivative * kD + turnTotalError * kI);






		double lateralMotorPower = (error * kP + derivate *kD + totalError * kI); // divided by 12.0 or tune PID variables

		LeftMotor.move(LEFT_MOTOR_PORT, );
		RightMotor.move(RIGHT_MOTOR_PORT, );


		//code
		prevError = error;
		turnPrevError = turnError;
		pros::delay(20);
	}
}