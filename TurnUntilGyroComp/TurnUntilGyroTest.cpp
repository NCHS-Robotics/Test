#include "vex.h"

int main(){

  task::sleep(1500);
  LBdrive.spin(directionType::fwd, 50, velocityUnits::rpm);
  LFdrive.spin(directionType::fwd, 50, velocityUnits::rpm);
  RBdrive.spin(directionType::rev, 50, velocityUnits::rpm);
  RFdrive.spin(directionType::rev, 50, velocityUnits::rpm);

  while(GyroG.value(rotationUnits::deg) <= 93) {
    Brain.Screen.clearScreen();
    Brain.Screen.setFont(fontType::mono40);
    Brain.Screen.printAt(20, 80, "Gyro");
    Brain.Screen.printAt(20, 120, "Reading = %f", GyroG.value(rotationUnits::deg));

    task::sleep(10);

    LBdrive.stop();
    LFdrive.stop();
    RBdrive.stop();
    RFdrive.stop();

  }
}