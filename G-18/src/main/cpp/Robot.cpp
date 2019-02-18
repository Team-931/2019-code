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
  m_chooser.AddOption(kAutoNameCustom, kAutoNameCustom);
  frc::SmartDashboard::PutData("Auto Modes", &m_chooser);

  frc::SmartDashboard::SetDefaultNumber ("Arm P coeff", armP);//to change and read in later
  //rightarm.ConfigSelectedFeedbackSensor (CTRE_MagEncoder_Absolute);//TO DO invert encoders and motors as necessary
  //cs::UsbCamera cam = frc::CameraServer::GetInstance()->StartAutomaticCapture();
  rightfront.SetNeutralMode(Coast);
  leftfront.SetNeutralMode(Coast);
  rightback.SetNeutralMode(Coast);
  leftback.SetNeutralMode(Coast);
  rightgripW.SetInverted(true);
  leftarm.SetNeutralMode(Brake);
  rightarm.SetNeutralMode(Brake);
  rightgripW.SetNeutralMode(Brake);
  leftgripW.SetNeutralMode(Brake);
  rightfangw.SetNeutralMode(Brake);
  leftfangw.SetNeutralMode(Brake);
  centerfang.SetNeutralMode(Brake);
  centerfang.SetInverted(false);
  left.SetInverted(true);
  right.SetInverted(true);
  rightfangw.SetInverted(true);
  anglearm.SetInputRange(farbackarm,farfrontarm);
  armEncoder.SetIndexSource(8);
  equalup.SetSafetyEnabled(false);
}

/**
 * This function is called every robot packet, no matter the mode. Use
 * this for items like diagnostics that you want ran during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {
  frc::SmartDashboard::PutNumber ("Left encoder", leftEncoder.Get());
  frc::SmartDashboard::PutNumber ("Right encoder", rightEncoder.Get());
  frc::SmartDashboard::PutNumber ("Pitch", navx.GetPitch());
  frc::SmartDashboard::PutNumber ("Roll", navx.GetRoll());
  frc::SmartDashboard::PutNumber ("Arm encoder", armEncoder.Get());
  frc::SmartDashboard::PutBoolean ("Pogo Limit", limitpogo.Get());
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
  m_autoSelected = m_chooser.GetSelected();
  // m_autoSelected = SmartDashboard::GetString("Auto Selector",
  //     kAutoNameDefault);
  std::cout << "Auto selected: " << m_autoSelected << std::endl;

  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } else {
    // Default Auto goes here
    anglearm.SetSetpoint(endgame);
    anglearm.Enable();
    driver.StopMotor();
  }
}

void Robot::AutonomousPeriodic() {
  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } else {
    // Default Auto goes here
  }
}

void Robot::TeleopInit() {
  armP = frc::SmartDashboard::GetNumber("Arm P coeff", armP);
  anglearm.SetP(armP);
  endgamephase = 0;
}
//invert fangs completly
void Robot::TeleopPeriodic() {
  if (endgamephase==0){
  robotcontrol();
  armcontrol();
  if (operatorstick.GetRawButton(1)&&operatorstick.GetRawButton(2)){
    endgameinit();
  }
 }
 else (endgameperiodic());
}

void Robot::TestPeriodic() {}

void Robot::robotcontrol() {
  fangs.Set (driverstick.GetRawButton(3)) ;
  if (driverstick.GetRawButton(9))
  arcadedrive=true;
  else if (driverstick.GetRawButton(10))
  arcadedrive=false;
  if (!driverstick.GetRawButton(1))
    if (arcadedrive) 
      driver.ArcadeDrive (-driverstick.GetRawAxis (1),driverstick.GetRawAxis (0));//TO DO reverse
    else 
      driver.TankDrive (-driverstick.GetRawAxis (1),-driverstick.GetRawAxis (3)); 
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
  if (operatorstick.GetRawButton(3)){
    centergriparm.Set(DoubleSolenoid::kReverse);
    gripers.Set(-1);
    //cargoarm=false;//TO DO reverse if needed
  }
  else{ 
    centergriparm.Set(DoubleSolenoid::kForward);

  
  if (operatorstick.GetRawButton(4)){
    gripers.Set(1);
    //cargoarm=true;
  }
  else{
    double speedgrip=operatorstick.GetRawAxis(0);
    if (speedgrip<0) 
     speedgrip*=.75;//setting speed for the griper wheels

    gripers.Set(-speedgrip);//To Do reverse if needed, and check speed
   }
  }
}
void Robot::armdegree(){
double armX=operatorstick.GetRawAxis(1);
if (std::abs(armX)>.1){
  anglearm.Disable();
  arms.Set (-armX*.2);//TO DO reverse if nessicary
}
else {
  if (!anglearm.IsEnabled()){
    anglearm.SetSetpoint(armEncoder.PIDGet());
    anglearm.Enable();
  }
  if (operatorstick.GetRawButton(5))//TO DO change buttons
    anglearm.SetSetpoint(farbackarm);//Add the angles
  if (operatorstick.GetRawButton(12))
    anglearm.SetSetpoint(farfrontarm);
  if (operatorstick.GetRawButton(11))
    anglearm.SetSetpoint(fronthatch);
  if (operatorstick.GetRawButton(6))
    anglearm.SetSetpoint(hatch);
  if (operatorstick.GetRawButton(9))
    anglearm.SetSetpoint(frontbay);
  if (operatorstick.GetRawButton(8))
    anglearm.SetSetpoint(shootbay);
  if (operatorstick.GetRawButton(7))
    anglearm.SetSetpoint(shootrocket);
  if (operatorstick.GetRawButton(10))
    anglearm.SetSetpoint(frontrocket);
  
}
}


#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
