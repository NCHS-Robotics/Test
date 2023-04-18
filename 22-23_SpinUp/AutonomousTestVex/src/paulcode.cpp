Brain.Screen.clearScreen();
  double turnImportance = 0.5;
  // place driver control in this while loop
  while (true) {

    Brain.Screen.clearScreen();
    Brain.Screen.printAt(1,40,"RPM:%f",Shootaclose.velocity(vex::velocityUnits::rpm));
    Brain.Screen.printAt(1,80,"RPM:%f",Shootafar.velocity(vex::velocityUnits::rpm));
    Brain.Screen.printAt(1,120,"RPM:%f",Suck.velocity(vex::velocityUnits::rpm));

    Brain.Screen.render();
    
    double turnVal = Controller1.Axis4.position(percent);
    double forwardVal = Controller1.Axis3.position(percent);

    double turnVolts = -(turnVal) * 0.12;
    double forwardVolts = forwardVal * 0.12 * (1 - (std::abs(turnVolts)/12.0) * turnImportance);

    LFdrive.spin(forward, forwardVolts - turnVolts, voltageUnits::volt);
    LBdrive.spin(forward, forwardVolts - turnVolts, voltageUnits::volt);
    RFdrive.spin(forward, forwardVolts + turnVolts, voltageUnits::volt);
    RBdrive.spin(forward, forwardVolts + turnVolts, voltageUnits::volt);

    //double suckVal = Controller1.ButtonL1.pressing();
    //double suckVolts = suckVal * 0.12;
    
    if (Controller1.ButtonL1.pressing()){
      Suck.spin(forward, 12.0 , voltageUnits::volt);
    }
    else if (Controller1.ButtonL2.pressing()){
      Suck.spin(forward, -12.0 , voltageUnits::volt);
    }
    else{
      Suck.stop();
    }
    
    /*void Shoot() {
      Shootabig.spin(reverse, 12.0 , voltageUnits::volt);
    }
    void Nothing() {
      Shootabig.spin(forward, 0 , voltageUnits::volt);
    }

    Controller1.ButtonR1.pressed(Shoot);
    Controller1.ButtonR2.pressed(Nothing);
    */
    if (Controller1.ButtonR1.pressing()){
      Shootaclose.spin(forward, 12.0 , voltageUnits::volt);
      Shootafar.spin(reverse, 12.0, voltageUnits::volt);
    }
    else{
      Shootaclose.stop();
      Shootafar.stop();
    }
    //Suck.spin(forward, suckVolts , voltageUnits::volt);
    //Suck.spin(reverse, suckVolts , voltageUnits::volt);

    if (Controller1.ButtonX.pressing()) {
      Endgame.spin(reverse, 12.0, voltageUnits::volt);
    }
    else if(Controller1.ButtonB.pressing()) {
      Endgame.spin(forward, 12.0, voltageUnits::volt);
    }
    else{
      Endgame.stop(brakeType::hold);
    }