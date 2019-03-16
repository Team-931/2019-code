#include "Robot.h"//face down increases, face up decreases, the Roll

class LiftController:public frc::PIDController,frc::PIDSource{
frc::DifferentialDrive& difdrive;
::AHRS& ahrsnavx;
public:
LiftController(frc::DifferentialDrive&,::AHRS&);
virtual void PIDWrite(double pidoutput);
virtual double PIDGet();
double speed;}
*liftcontroller=0;
static double pogospeed=1;
static double fangspeed=1;

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
 time.Reset();
 time.Start();
 //liftcontroller->Enable();
}

LiftController::LiftController(frc::DifferentialDrive& difdrive_,::AHRS& ahrsnavx_)
:PIDController(.1,0,0,this,this),
difdrive(difdrive_), ahrsnavx(ahrsnavx_),
speed (1) {
SetInputRange(-180,180);
SetContinuous(true);
SetSetpoint(-3);//how many degrees do we need when going up//check number//-6 means 6 degrees facing up
}
void LiftController::PIDWrite(double pidoutput){
difdrive.ArcadeDrive(speed,pidoutput,false);//is the turn on the arcadedrive clockwise or conterclockwise
}
double LiftController::PIDGet(){//might need to make this oppsite
return ahrsnavx.GetRoll();//it could be GetRoll instead
}
static constexpr double phase2speed = .25;
void Robot::endgameperiodic(){//wheels, fangs, wheel fangs,  
 if (endgamephase==1){
     fangs.Set(1);
     centerfang.Set(fangspeed);
     left.Set(pogospeed);//reverse if needed, left wheels are what the power take off is conected to
     right.StopMotor();
     
  if (limitpogo.Get())//sync fangs and pogosticks
   pogospeed=phase2speed;
   right.Set(-.2);
 //  liftcontroller->Disable();
   //liftcontroller->speed = phase2speed;
   //liftcontroller->SetSetpoint(2);//nose down
  if (limitfang.Get())
  fangspeed=phase2speed;
  
 
 //if (endgamephase==2){
    //fangs.Set(1);
     //centerfang.Set(-.80);
     //left.Set(phase2speed);
/*     centerfang.StopMotor();
    driver.StopMotor();
 */ 
    //right.Set(phase2speed);
    if (time.Get()>8.5)//TO DO change time
  {endgamephase=3;
  //liftcontroller->Disable();
  centertakeoff.Set(DoubleSolenoid::kForward);}
  return;
 }
 if (endgamephase==3){
     fangs.Set(1);
     centerfang.Set(.1);
 if (time.Get()>8.5)//TO DO change time
   endgamephase=4;
   return;
 }
 if (endgamephase==4){
   centerfang.Set(0);
    driver.ArcadeDrive(phase2speed,0,false);
  fangs.Set(0); //or 0
 if (time.Get()>9)//TO DO change time
   endgamephase= 0/* 5 */;
   return;
 }
 if (endgamephase==5)
   fangs.StopMotor();
   driver.StopMotor();
}