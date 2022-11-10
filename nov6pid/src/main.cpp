#include "main.h"

//time: 23:50 
//2.0 video speed

//INITALIZE MOTORS BEFORE RUNNING



#define LEFT_FRONT_MOTOR_PORT 3
#define LEFT_BACK_MOTOR_PORT 11
#define RIGHT_FRONT_MOTOR_PORT 4
#define RIGHT_BACK_MOTOR_PORT 2

pros::Motor LeftMotorFront (LEFT_FRONT_MOTOR_PORT, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor LeftMotorBack (LEFT_BACK_MOTOR_PORT, pros::E_MOTOR_GEAR_BLUE, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor RightMotorFront (RIGHT_FRONT_MOTOR_PORT, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor RightMotorBack (RIGHT_BACK_MOTOR_PORT, pros::E_MOTOR_GEAR_BLUE, true, pros::E_MOTOR_ENCODER_DEGREES);

//settings - constants
double kP = 0.1;
double kI = 0.1;
double kD = 0.1;

int error; //SensorValue  - DesiredValue : Position (delta x)
int prevError = 0; //Position from a set time before (delay)
int derivate; //different between error and prevError, error - prevError = Speed (rate of change in error)
int totalError = 0; //totalError = totalError + error

int turnError;
int turnPrevError = 0;
int turnDerivative;
int turnTotalError = 0;

//autonomous settings
int desiredValue = 1000; //want the motor to go x amount of units (Degrees)
int turnDesiredValue = 0;

//variables modified for use
bool enableDrivePID = true;
//resets the drive sensors
//bool resetDriveSensors = false;

//PID function
int drivePID() {
	while(enableDrivePID) {

		/**
		if (resetDriveSensors) {

		}
		*/

		int leftMotorPosition = LeftMotorFront.get_position(); //absolute position in enconder units (degrees bc set to degrees)
		int rightMotorPosition = RightMotorFront.get_position();


		////////////////////////////////////
		//Lateral Movement PID
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

		double lateralMotorPower = (error * kP + derivate *kD + totalError * kI); // divided by 12.0 or tune PID variables

		///////////////////////////////////////
		//Turning movement PID
		///////////////////////////////////////

		//get the difference in both motors
		int turnDifference = leftMotorPosition - rightMotorPosition;

		//Potential
		turnError = turnDifference - turnDesiredValue;

		//Derivative
		turnDerivative = turnError - turnPrevError;

		//Integral
		turnTotalError += turnError;

		double turnMotorPower = (turnError * kP + turnDerivative * kD + turnTotalError * kI);


		LeftMotorFront.move(lateralMotorPower);
		LeftMotorBack.move(lateralMotorPower);
		RightMotorFront.move(lateralMotorPower);
		RightMotorBack.move(lateralMotorPower);


		//code
		prevError = error;
		turnPrevError = turnError;
		pros::delay(20);
	}
}

void opcontrol() {
	while (true) {
		drivePID();
	}
}
