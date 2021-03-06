/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"

#include <iostream>

#include <frc/smartdashboard/SmartDashboard.h>

void Robot::RobotInit() {
  m_chooser.SetDefaultOption(kAutoNameDefault, kAutoNameDefault);
  m_chooser.AddOption(autoForward, autoForward);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);

  frc::SmartDashboard::SetDefaultNumber ("Arm P coeff", armP);//to change and read in later
  //rightarm.ConfigSelectedFeedbackSensor (CTRE_MagEncoder_Absolute);//TO DO invert encoders and motors as necessary
  
  outputCam.SetSource(frontCam);
  frc::SmartDashboard::PutBoolean("Rear view", false);
  //cs::UsbCamera cam = frc::CameraServer::GetInstance()->StartAutomaticCapture();
  rightfront.SetNeutralMode(Brake);
  rightfront.ConfigVoltageCompSaturation(12);
//  rightfront.EnableVoltageCompensation(true);

  leftfront.SetNeutralMode(Brake);
  leftfront.ConfigVoltageCompSaturation(12);
//  leftfront.EnableVoltageCompensation(true);

  rightback.SetNeutralMode(Brake);
  rightback.ConfigVoltageCompSaturation(12);
//  rightback.EnableVoltageCompensation(true);

  leftback.SetNeutralMode(Brake);
  leftback.ConfigVoltageCompSaturation(12);
//  leftback.EnableVoltageCompensation(true);

  rightgripW.SetInverted(true);
  leftarm.SetNeutralMode(Brake);
  rightarm.SetNeutralMode(Brake);
  rightgripW.SetNeutralMode(Brake);
  leftgripW.SetNeutralMode(Brake);
  rightfangw.SetNeutralMode(Brake);
  leftfangw.SetNeutralMode(Brake);

  centerfang1.SetNeutralMode(Brake);
  centerfang2.SetNeutralMode(Brake);
//  centerfang.ConfigVoltageCompSaturation(12);
  //centerfang.EnableVoltageCompensation(true);
  centerfang.SetInverted(false);

  left.SetInverted(true);
  right.SetInverted(true);
  rightfangw.SetInverted(true);
  anglearm.SetInputRange(farbackarm,farfrontarm);
  armEncoder.SetIndexSource(8);
  equalup.SetSafetyEnabled(false);
}

static int gripperDisplayBit = 0;
static constexpr int armButtonBase = 3;//TODO confirm this

/**
 * This function is called every robot packet, no matter the mode. Use
 * this for items like diagnostics that you want ran during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {
  if(driverstick.GetRawButtonPressed(5)) 
    if (outputCam.GetSource() == frontCam) {
      outputCam.SetSource(rearCam);
      frc::SmartDashboard::PutBoolean("Rear view", true);
    }
    else {
      outputCam.SetSource(frontCam);
      frc::SmartDashboard::PutBoolean("Rear view", false);
    }
  {//report arm position to operator console
    int buttonBit = 1<<(armButtonBase-1);
    int armPos = armEncoder.Get();
    int level = armPositions[0];
    const int tolerance = 2;
    for (int ix = 1; ix < armPositionCt; ++ix) {
      if (level + tolerance >= armPos) break;
      if ((level = armPositions[ix]) - tolerance > armPos) {
        buttonBit *= 3;
        break;
      }
      buttonBit <<= 1;
    }
    operatorstick.SetOutputs(buttonBit | gripperDisplayBit);
  }
  frc::SmartDashboard::PutNumber ("Left encoder", leftEncoder.Get());
  frc::SmartDashboard::PutNumber ("Right encoder", rightEncoder.Get());
  frc::SmartDashboard::PutNumber ("Yaw", (int) navx.GetYaw());
  frc::SmartDashboard::PutNumber ("Roll", (int) navx.GetRoll());
  frc::SmartDashboard::PutNumber ("Arm encoder", armEncoder.Get());
  frc::SmartDashboard::PutBoolean ("Pogo Limit", limitpogo.Get());
  frc::SmartDashboard::PutBoolean("Fang Limit", limitfang.Get());
  frc::SmartDashboard::PutBoolean ("Left linefollow", usethresshold(leftsensor));
  frc::SmartDashboard::PutBoolean ("Right linefollow", usethresshold(rightsensor));
}

/**
 * This autonomous (along with the chooser code above) shows how to select
 * between different autonomous modes using the dashboard. The sendable chooser
 * code works with the Java SmartDashboard. If you prefer the LabVIEW Dashboard,
 * remove all of the chooser code and uncomment the GetString line to get the
 * auto name from the text box below the Gyro.
 *
 * You can add additional auto modes by adding additional comparisons to the
 * if-else structure below with additional strings. If using the SendableChooser
 * make sure to add them to the chooser code above as well.
 */
void Robot::AutonomousInit() {
  
  armP = frc::SmartDashboard::GetNumber("Arm P coeff", armP);
  anglearm.SetP(armP);

  centertakeoff.Set(DoubleSolenoid::kForward);
  centertakeoff2.Set(DoubleSolenoid::kForward);

  centergriparm.Set(DoubleSolenoid::kForward);
  gripperDisplayBit = 2;

  m_autoSelected = m_chooser.GetSelected();
  // m_autoSelected = SmartDashboard::GetString("Auto Selector",
  //     kAutoNameDefault);
  std::cout << "Auto selected: " << m_autoSelected << std::endl;
  anglearm.SetSetpoint(endgame);
  anglearm.Enable();
// Up here applies to all Autos

  if (m_autoSelected == autoForward) {
    // Custom Auto goes here
    time.Reset();
    time.Start();
  } else {
    // Default Auto goes here

   // driver.StopMotor();
  }
}

void Robot::AutonomousPeriodic() {
  if (m_autoSelected == autoForward) {
   if (time.Get()<5)
    // Custom Auto goes here
    driver.ArcadeDrive(1,0,false);
    else
    driver.StopMotor();
  } else {
    // Default Auto goes here
    robotcontrol();
    armcontrol();
  }
}

void Robot::TeleopInit() {
  armP = frc::SmartDashboard::GetNumber("Arm P coeff", armP);
  anglearm.SetP(armP);
  endgamephase = 0;
}

void Robot::endgameTeleop() {
  { // driver control of fangs' forward motion
    int pov = endgamestick.GetPOV();
    if(pov == 0 && !limitfang.Get()) centerfang.Set (1);
    else if(pov == 180) centerfang.Set (-1);
    else centerfang.Set(0);
  }
  //centerfang.Set(-driverstick.GetRawButton(6));
  {//driver  control of pogosticks
    static bool pogodown= false;
    pogodown ^= endgamestick.GetRawButtonPressed(6);
    if (pogodown) {
      centertakeoff.Set(DoubleSolenoid::kReverse);
      centertakeoff2.Set(DoubleSolenoid::kReverse);
      fangs.Set (1);
    }
    else 
    { centertakeoff.Set(DoubleSolenoid::kForward);
      centertakeoff2.Set(DoubleSolenoid::kForward);
      fangs.Set (driverstick.GetRawButton(3)) ;
      }
  }
  
}

//invert fangs completly
void Robot::TeleopPeriodic() {
  if (endgamephase==0){
    endgameTeleop();
  robotcontrol();
  armcontrol();
  if (operatorstick.GetRawButton(11)/* &&operatorstick.GetRawButton(2) */){
    endgameinit();
  }
 }
 else (endgameperiodic());
}

void Robot::TestPeriodic() {}

void Robot::robotcontrol() {
  gripers.Set (-.05);
  if (driverstick.GetRawButtonPressed(7))
  arcadedrive=true;
  else if (driverstick.GetRawButtonPressed(8))
  arcadedrive=false;
  if (!driverstick.GetRawButton(1))
    if (arcadedrive) 
      if (outputCam.GetSource()==rearCam)
       driver.ArcadeDrive (driverstick.GetY(),driverstick.GetZ()/*driverstick.GetRawAxis (1)*.8,driverstick.GetRawAxis (2)*.8*/,true);//This is so the front changes with the camera
      else
      driver.ArcadeDrive (-driverstick.GetY(),driverstick.GetZ()/*-driverstick.GetRawAxis (1)/*.8,driverstick.GetRawAxis (2*.8)*/,true);//TO DO reverse
       
    else 
      if (outputCam.GetSource()==frontCam)
       driver.TankDrive (-driverstick.GetRawAxis (1)/*times .8*/*.8,-driverstick.GetRawAxis (3)/*times .8*/*.8,true); 
      else 
       driver.TankDrive (driverstick.GetRawAxis (3)/*times .8*/*.8,driverstick.GetRawAxis (1)/*times .8*/*.8,true); 
     else
    if (usethresshold(rightsensor)) 
     if (usethresshold(leftsensor))
      driver.TankDrive(.3,.3,false);
     else 
      driver.TankDrive(.3,.1,false);
    else
     if (usethresshold(leftsensor))
      driver.TankDrive(.1,.3,false);
    else 
      driver.TankDrive(0,0,false);

}
void Robot::armcontrol(){
  armdegree();
  if (operatorstick.GetRawButtonPressed(1)){
    centergriparm.Set(DoubleSolenoid::kReverse);//lost front wheels so just having open and close
    gripperDisplayBit = 1;
    //gripers.Set(-1);
    //cargoarm=false;//TO DO reverse if needed
  }
 
    //centergriparm.Set(DoubleSolenoid::kForward);

  
  if (operatorstick.GetRawButtonPressed(2)){
    centergriparm.Set(DoubleSolenoid::kForward);
    gripperDisplayBit = 2;
    //gripers.Set(1);
    //cargoarm=true;
  }
  else{
    double speedgrip=operatorstick.GetRawAxis(1);
    if (speedgrip<0) 
     speedgrip*=.75;//setting speed for the griper wheels

    gripers.Set(-speedgrip);//To Do reverse if needed, and check speed
   
  }
}
void Robot::armdegree(){
double armX=operatorstick.GetRawAxis(0);
if (std::abs(armX)>.1){
  anglearm.Disable();
  arms.Set (-armX*.2);//TO DO reverse if nessicary
}
else {
  if (!anglearm.IsEnabled()){
    anglearm.SetSetpoint(armEncoder.PIDGet());
    anglearm.Enable();
  }
  
  for (int ix = 0; ix < armPositionCt; ++ix)
    if(operatorstick.GetRawButton(ix+armButtonBase))// 3 is the first arm setting button now
      anglearm.SetSetpoint (armPositions[ix]);

  
}
}


#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
