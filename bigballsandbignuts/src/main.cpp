#include "main.h"

#define LEFT_FRONT_WHEEL_PORT 3
#define LEFT_BACK_WHEEL_PORT 11
#define RIGHT_FRONT_WHEEL_PORT 4
#define RIGHT_BACK_WHEEL_PORT 2
#define INTAKE_MOTOR_PORT 5

void opcontrol() {
    pros::Motor left_front_wheel(LEFT_FRONT_WHEEL_PORT, true); 
    pros::Motor left_back_wheel(LEFT_BACK_WHEEL_PORT, true);
    pros::Motor right_front_wheel(RIGHT_FRONT_WHEEL_PORT, true);
    pros::Motor right_back_wheel(RIGHT_BACK_WHEEL_PORT, true);

    pros::Motor intake_motor(INTAKE_MOTOR_PORT);

    pros::Controller master (pros::E_CONTROLLER_MASTER);

    while (true) {
        int dtY = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int dtX = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

        left_back_wheel.move((dtX + dtY));
        left_front_wheel.move((dtX + dtY));
        right_back_wheel.move((dtX - dtY));
        right_front_wheel.move((dtX - dtY));

        intake_motor.move_velocity(-200 * master.get_digital(DIGITAL_L1));
        intake_motor.move_velocity(200 * master.get_digital(DIGITAL_L2));
        

        /**
        while (master.get_digital(DIGITAL_L1) == 1) {
            intake_motor.move_velocity(-200);
        }
        intake_motor.move_velocity(0);
        

        while (master.get_digital(DIGITAL_L2) == 1) {
            intake_motor.move_velocity(200);
        }
        intake_motor.move_velocity(0);
        */

        pros::delay(2);
    }
}