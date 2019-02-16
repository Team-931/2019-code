#include "Robot.h"//face down increases, face up decreases, the Roll

class LiftController:public frc::PIDController,frc::PIDSource{
frc::DifferentialDrive& difdrive;
::AHRS& ahrsnavx;
public:
LiftController(frc::DifferentialDrive&,::AHRS&);
virtual void PIDWrite(double pidoutput);
virtual double PIDGet();}
*liftcontroller=0;

void Robot::endgameinit(){//solinoid, PID, arm
 endgamephase=1;
 anglearm.SetSetpoint(endgame);
 anglearm.Enable();
 driver.SetSafetyEnabled(false);
 centertakeoff.Set(DoubleSolenoid::kReverse);
 leftfront.SetNeutralMode(Brake);
 leftback.SetNeutralMode(Brake);
 rightfront.SetNeutralMode(Coast);
 rightback.SetNeutralMode(Coast);
 liftcontroller=new LiftController(equalup,navx);
 liftcontroller->Enable();
}

LiftController::LiftController(frc::DifferentialDrive& difdrive_,::AHRS& ahrsnavx_)
:PIDController(.1,0,0,this,this),
difdrive(difdrive_), ahrsnavx(ahrsnavx_){
SetInputRange(-180,180);
SetContinuous(true);
SetSetpoint(-6);//how many degrees do we need when going up//check number//-6 means 6 degrees facing up
}
void LiftController::PIDWrite(double pidoutput){
difdrive.ArcadeDrive(1,pidoutput,false);//is the turn on the arcadedrive clockwise or conterclockwise
}
double LiftController::PIDGet(){//might need to make this oppsite
return ahrsnavx.GetRoll();//it could be GetRoll instead
}

void Robot::endgameperiodic(){//wheels, fangs, wheel fangs,  
 if (endgamephase==1){
     fangs.Set(1);
     //centerfang.Set(1);
     //left.Set(1);//reverse if needed, left wheels are what the power take off is conected to
     right.StopMotor();
 if (limitpogo.Get())//sync fangs and pogosticks
   {endgamephase=2;
   liftcontroller->Disable();
   time.Reset();
   time.Start();} 
    return;
 }
 if (endgamephase==2){
    fangs.Set(1);
    centerfang.StopMotor();
    driver.StopMotor();
 if (time.Get()>5)//TO DO change time
  {endgamephase=3;
  centertakeoff.Set(DoubleSolenoid::kForward);}
  return;
 }
 if (endgamephase==3){
     fangs.Set(1);
 if (limitpogo.Get())
    driver.StopMotor();
 else
    driver.ArcadeDrive(1,0);
 if (time.Get()>10)//TO DO change time
   endgamephase=4;
   return;
 }
 if (endgamephase==4)
   fangs.StopMotor();
   driver.StopMotor();
}