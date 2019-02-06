#include "Robot.h"

void Robot::endgameinit(){//solinoid, PID, arm
 endgame1=1;
 anglearm.SetSetpoint(endgame);
 anglearm.Enable();
 centertakeoff.Set(DoubleSolenoid::kForward);
}
void Robot::endgameperiodic(){//wheels, fangs, wheel fangs,  
 if (endgame1==1){
     fangs.Set(1);
     centerfang.Set(1);
     driver.TankDrive(1,1,false);
 }
}