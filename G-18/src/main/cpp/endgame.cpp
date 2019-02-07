#include "Robot.h"

void Robot::endgameinit(){//solinoid, PID, arm
 endgamephase=1;
 anglearm.SetSetpoint(endgame);
 anglearm.Enable();
 centertakeoff.Set(DoubleSolenoid::kForward);
}
void Robot::endgameperiodic(){//wheels, fangs, wheel fangs,  
 if (endgamephase==1){
     fangs.Set(1);
     centerfang.Set(1);
     left.Set(1);//reverse if needed, left wheels are what the power take off is conected to
     right.StopMotor();
 if (limitpogo.Get())//sync fangs and pogosticks
   endgamephase=2; 
    return;
 }
 if (endgamephase==2){
    fangs.Set(1);
    centerfang.StopMotor();
    driver.StopMotor();
 if (end2())//fill in
  {endgamephase=3;
  centertakeoff.Set(DoubleSolenoid::kReverse);}
  return;
 }
 if (endgamephase==3){
     fangs.Set(1);
 if (limitpogo.Get())
    driver.StopMotor();
 else
    driver.ArcadeDrive(1,0);
 }
}