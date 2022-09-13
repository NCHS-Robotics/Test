#include "main.h"

#define LEFT_WHEELS_PORT 1
#define RIGHT_WHEELS_PORT 10
#define MOTOR_MAX_SPEED 100


void run() {
  pros::Motor left_wheels(LEFT_WHEELS_PORT);
  pros::Motor right_wheels(RIGHT_WHEELS_PORT, true); //true reverses the motor

  right_wheels.move_relative(1000, MOTOR_MAX_SPEED);
  left_wheels.move_relative(1000, MOTOR_MAX_SPEED);


}
