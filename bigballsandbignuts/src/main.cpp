#include "main.h"

#define LEFT_FRONT_WHEEL_PORT 3
#define LEFT_BACK_WHEEL_PORT 1
#define RIGHT_FRONT_WHEEL_PORT 4
#define RIGHT_BACK_WHEEL_PORT 2
#define INTAKE_MOTOR_PORT 5
#define SHOOT_CLOSE_MOTOR_PORT 8
#define SHOOT_FAR_MOTOR_PORT 10

pros::Motor left_front_wheel(LEFT_FRONT_WHEEL_PORT, true); 
pros::Motor left_back_wheel(LEFT_BACK_WHEEL_PORT, true);
pros::Motor right_front_wheel(RIGHT_FRONT_WHEEL_PORT, true);
pros::Motor right_back_wheel(RIGHT_BACK_WHEEL_PORT, true);

pros::Motor intake_motor(INTAKE_MOTOR_PORT);
pros::Motor intake_motor_reverse(1, true);

pros::Motor shoot_close_motor(SHOOT_CLOSE_MOTOR_PORT);
pros::Motor shoot_far_motor(SHOOT_FAR_MOTOR_PORT, true);

pros::Controller master (pros::E_CONTROLLER_MASTER);

//drive task method
void drive(void* param) {
    int dtY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int dtX = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

    left_back_wheel.move((dtX + dtY));
    left_front_wheel.move((dtX + dtY));
    right_back_wheel.move((dtX - dtY));
    right_front_wheel.move((dtX - dtY));
}

//intake task methods
void intakeIn(void* param) {

    intake_motor_reverse.move(master.get_digital(DIGITAL_L1) * 127);
    /**
    int voltageAmount = -127 *(master.get_digital(DIGITAL_L1));
    intake_motor.move(voltageAmount);
    */
}

void intakeOut(void* param) {
    intake_motor.move(master.get_digital(DIGITAL_L2) * 127);

    /**
    int voltageAmount = -127 * master.get_digital(DIGITAL_L2);
    intake_motor.move(voltageAmount);
    */
}

void intakeController(void* param) {
    int y = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
    intake_motor.move(y);
}

//shoot motor task method
void shoot(void* param) {
    shoot_close_motor.move_velocity(200 * master.get_digital(DIGITAL_R2));
    shoot_far_motor.move_velocity(200 * master.get_digital(DIGITAL_R2));
}


void intake(void* param) {

//    intake_motor.move(master.get_digital(DIGITAL_L1) * 127);
  //  intake_motor.move(master.get_digital(DIGITAL_L2) * -127);


//working code
if (master.get_digital(DIGITAL_L1) == 1) {
    intake_motor.move(127 * master.get_digital(DIGITAL_L1));
}
if (master.get_digital(DIGITAL_L2) == 1) {
    intake_motor.move(-127 * master.get_digital(DIGITAL_L2));
}
if (master.get_digital(DIGITAL_R1) == 1) {
    intake_motor.move(0);
}


/**
//does not work, only goes and stops in one direction
if (master.get_digital(DIGITAL_L1) == 1) {
    intake_motor.move(127 * master.get_digital(DIGITAL_L1));
}

if (master.get_digital(DIGITAL_L1) == 0) {
        intake_motor.move(0);
    }
if (master.get_digital(DIGITAL_L2) == 1) {
    intake_motor.move(-127 * master.get_digital(DIGITAL_L2));
}

if (master.get_digital(DIGITAL_L2) == 0) {
        intake_motor.move(0);
    }
    */




    /**
    //bool runL1 = master.get_digital(DIGITAL_L1) == 1;
    while (master.get_digital(DIGITAL_L1)) {
            intake_motor.move(-127);
            pros::delay(1);
            
            /**
            if (!runL1) {
                intake_motor.move(0);
                runL1 = false;
            }
            
        }
    pros::delay(1);
    intake_motor.move(0);

    //bool runL2 = master.get_digital(DIGITAL_L2) == 1;
    while (master.get_digital(DIGITAL_L2)) {
            intake_motor.move(127);
            pros::delay(1);
            
            /**
            if (!runL2) {
                intake_motor.move(0);
                runL2 = false;
            }
            
        } 
    pros::delay(1);   
    intake_motor.move(0);
    */
}

/**
void leftBackDrive(void* param) {
    int dtY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int dtX = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

    left_back_wheel.move((dtX + dtY));
}

void leftFrontDrive(void* param) {
    int dtY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int dtX = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

    left_front_wheel.move((dtX + dtY));
}

void rightBackDrive(void* param) {
    int dtY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int dtX = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

    right_back_wheel.move((dtX - dtY));
}

void rightFrontDrive(void* param) {
    int dtY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int dtX = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

    right_front_wheel.move((dtX - dtY));
}
**/

void opcontrol() {
    while (true) {
        /**
        //drive code
        int dtY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int dtX = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

        left_back_wheel.move((dtX + dtY));
        left_front_wheel.move((dtX + dtY));
        right_back_wheel.move((dtX - dtY));
        right_front_wheel.move((dtX - dtY)); 
        **/


        /**
        //intake code
        while (master.get_digital(DIGITAL_L1) == 1) {
            intake_motor.move_velocity(200);
        }
        intake_motor.move_velocity(0);
        while (master.get_digital(DIGITAL_L2) == 1) {
            intake_motor.move_velocity(-200);
        }
        intake_motor.move_velocity(0);  
        **/
          
        pros::Task driveTask(drive);

        pros::Task intakeTask(intake);
        //pros::Task intakeInTask(intakeIn);
        //pros::Task intakeOutTask(intakeOut);
        //pros::Task intakeControllerTask(intakeController);

        //pros::Task leftBDrive(leftBackDrive);
        //pros::Task leftFDrive(leftFrontDrive);
        //pros::Task rightBDrive(rightBackDrive);
        //pros::Task rightFDrive(rightFrontDrive);

        pros::Task shootTask(shoot);

        pros::delay(2);
    }
}