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
  //rightarm.ConfigSelectedFeedbackSensor (CTRE_MagEncoder_Absolute);//TO DO invert encoders and motors as necessary
  //cs::UsbCamera cam = frc::CameraServer::GetInstance()->StartAutomaticCapture();
  rightgripW.SetInverted(true);
  leftarm.SetNeutralMode(Brake);
  rightarm.SetNeutralMode(Brake);
  rightgripW.SetNeutralMode(Brake);
  leftgripW.SetNeutralMode(Brake);
  rightfangw.SetNeutralMode(Brake);
  leftfangw.SetNeutralMode(Brake);
  centerfang.SetNeutralMode(Brake);
  centerfang.SetInverted(true);
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
void Robot::RobotPeriodic() {}

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
  centertakeoff.Set(DoubleSolenoid::kForward);
  m_autoSelected = m_chooser.GetSelected();
  // m_autoSelected = SmartDashboard::GetString("Auto Selector",
  //     kAutoNameDefault);
  std::cout << "Auto selected: " << m_autoSelected << std::endl;

  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } else {
    // Default Auto goes here
  }
}

void Robot::AutonomousPeriodic() {
  if (m_autoSelected == kAutoNameCustom) {
    // Custom Auto goes here
  } else {
    // Default Auto goes here
  }
}

void Robot::TeleopInit() {}
//invert fangs completly
void Robot::TeleopPeriodic() {
  frc::SmartDashboard::PutNumber ("Left encoder", leftEncoder.Get());
  frc::SmartDashboard::PutNumber ("Right encoder", rightEncoder.Get());
  frc::SmartDashboard::PutNumber ("Pitch", navx.GetPitch());
  frc::SmartDashboard::PutNumber ("Roll", navx.GetRoll());
  if (endgamephase==0){
  robotcontrol();
  armcontrol();
<<<<<<< HEAD
  //Testing fangs:
  if (operatorstick.GetRawButton(2)) fangs.Set (1);
  else fangs.Set(0);
  if (operatorstick.GetRawButton(1)) centerfang.Set(1);
  else centerfang.Set(0);
  if (operatorstick.GetRawButton(6)&&driverstick.GetRawButton(8)){
=======
  if (operatorstick.GetRawButton(6)&&operatorstick.GetRawButton(5)){
>>>>>>> 04b1eedd840c179481d9dd8973da3e54fd71536e
    endgameinit();
  }
 }
 else (endgameperiodic());
}

void Robot::TestPeriodic() {}

void Robot::robotcontrol() {
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
  if (operatorstick.GetRawButton(7)){
    centergriparm.Set(DoubleSolenoid::kReverse);
    cargoarm=false;//TO DO reverse if needed
  }
  else 
    centergriparm.Set(DoubleSolenoid::kForward);
  
  if (operatorstick.GetRawButton(8)){
    cargoarm=true;
  }
  if (cargoarm)
    gripers.Set(-operatorstick.GetRawAxis(3));//To Do reverse if needed, and check speed
  else
    gripers.Set(operatorstick.GetRawAxis(3));
}
void Robot::armdegree(){
double armX=operatorstick.GetRawAxis(0);
if (std::abs(armX)>.1){
  anglearm.Disable();
  arms.Set (armX);//TO DO reverse if nessicary
}
else {
  if (!anglearm.IsEnabled()){
    anglearm.SetSetpoint(armEncoder.PIDGet());
    anglearm.Enable();
  }
  if (operatorstick.GetRawButton(9))//TO DO change buttons
    anglearm.SetSetpoint(farbackarm);//Add the angles
  if (operatorstick.GetRawButton(10))
    anglearm.SetSetpoint(farfrontarm);
  if (operatorstick.GetRawButton(11))
    anglearm.SetSetpoint(startingposition);
  if (operatorstick.GetRawButton(12))
    anglearm.SetSetpoint(shoothigh);
  if (operatorstick.GetRawButton(13))
    anglearm.SetSetpoint(shootlow);
  
}
}


#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
