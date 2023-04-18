/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\alexa                                            */
/*    Created:      Wed Nov 16 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Gyro                 gyro          D               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "robot-config.h"

using namespace vex;

vex::controller::lcd ControllerScreen = vex::controller::lcd();

int main() {
  task::sleep(1500);

  while (1) {
    Brain.Screen.clearScreen();
    Brain.Screen.setFont(fontType::mono40);
    Brain.Screen.printAt(20, 80, "Gyro");
    Brain.Screen.printAt(20, 120, "Reading = %f", Gyro.rotation(degrees));

    ControllerScreen.setCursor(0,0);
    ControllerScreen.print(Gyro.rotation(degrees));
    ControllerScreen.clearScreen();
  }

  task::sleep(10);
}