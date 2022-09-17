#include "main.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"

//maps left and right joysticks to left and right controllers
std::int32_t pros::Controller::get_analog(pros::controller_analog_e_t channel); 

//set the motors
std::int32_t motor_move(const std::int8_t voltage); //set the motors 

//map buttons to use arm with controller buttons not joystick
std::int32_t pros::Controller::get_digital(pros::controller_digital_e_t button);

//use velocity-controlled movement functions to keep moving at a constant speed
std::int32_t pros::Motor::move_velocity(const std::int32_t velocity); 

//define "variables" for motor ports
#define LEFT_WHEELS_PORT 1
#define RIGHT_WHEELS_PORT 10
#define ARM_PORT 8
#define CLAW_PORT 3

#define LEFT_BUMPER_PORT 'a'
#define RIGHT_BUMPER_PORT 'b'
#define ARM_LIMIT_SWITCH_PORT 'h'

void opcontrol() {
    pros::Motor left_wheels(LEFT_WHEELS_PORT);
    pros::Motor right_wheels (RIGHT_WHEELS_PORT, true); //true reverses the motor
    pros::Motor arm(ARM_PORT, MOTOR_GEARSET_36); //gearset_36 for red motor gearset
    pros::Motor claw(CLAW_PORT, MOTOR_GEARSET_36);

    pros::ADIDigitalIn left_bumper (LEFT_BUMPER_PORT);
    pros::ADIDigitalIn right_bumper (RIGHT_BUMPER_PORT);
    pros::ADIDigitalIn arm_limit (ARM_LIMIT_SWITCH_PORT);

    pros::Controller master (CONTROLLER_MASTER);

    while (true) {
        int power = master.get_analog(ANALOG_LEFT_Y);
        int turn = master.get_analog(ANALOG_RIGHT_X);
        //voltages for motors   
        int left = power + turn;
        int right = power - turn;

        if (left_bumper.get_value() || right_bumper.get_value()) {
            if (left < 0) {
                left = 0;
            }
            if (right < 0) {
                right = 0;
            }
        }
        
        left_wheels.move(left);
        right_wheels.move(right);

        //checks which right trigger is pressed and moves the motor in that direction
        if (master.get_digital(DIGITAL_R1)) {
            arm.move_velocity(100); //100 rpm bc its a 100 rpm motor
        }
        else if (master.get_digital(DIGITAL_R2) && !arm_limit.get_value()) {
            arm.move_velocity(-100); //negative bc right motor
        }
        else {
            arm.move_velocity(0);
        }

        if (master.get_digital(DIGITAL_L1)) {
            claw.move_velocity(100);
        }
        else if (master.get_digital(DIGITAL_L1)) {
            claw.move_velocity(-100);
        }
        else {
            claw.move_velocity(0);
        }

        //delay task
        pros::delay(2);
    }

    #define MOTOR_MAX_SPEED 100 //36 gearset
    void autonomous() {
        pros::Motor left_wheels (LEFT_WHEELS_PORT);
        pros::Motor right_wheels (RIGHT_WHEELS_PORT, true);

        right_wheels.move_relative(1000, MOTOR_MAX_SPEED);
        left_wheels.move_relative(1000, MOTOR_MAX_SPEED);
    }
}


