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
  auto cam = frc::CameraServer::GetInstance()->StartAutomaticCapture();
  rightgripW.SetInverted(true);
  leftarm.SetNeutralMode(Brake);
  rightarm.SetNeutralMode(Brake);
  rightgripW.SetNeutralMode(Brake);
  leftgripW.SetNeutralMode(Brake);
  rightfangw.SetNeutralMode(Brake);
  leftfangw.SetNeutralMode(Brake);
  centerfang.SetNeutralMode(Brake);
  left.SetInverted(true);
  leftfangw.SetInverted(true);
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

void Robot::TeleopPeriodic() {
  robotcontrol();
  armcontrol();
}

void Robot::TestPeriodic() {}

void Robot::robotcontrol() {
  if (driverstick.GetRawButton(9))
  arcadedrive=true;
  else if (driverstick.GetRawButton(10))
  arcadedrive=false;
  if (arcadedrive) 
    driver.ArcadeDrive (-driverstick.GetRawAxis (1),driverstick.GetRawAxis (0));//TO DO revers
    else 
      driver.TankDrive (-driverstick.GetRawAxis (1),-driverstick.GetRawAxis (3)); 
  
}
void Robot::armcontrol(){
  arms.Set (operatorstick.GetRawAxis(0));//TO DO reverse if nessicary
  if (operatorstick.GetRawButton(7)){
    centergriparm.Set(DoubleSolenoid::kForward);
    cargoarm=false;//TO DO reverse if needed
  }
  else 
    centergriparm.Set(DoubleSolenoid::kReverse);
  
  if (operatorstick.GetRawButton(8)){
    cargoarm=true;
  }
  if (cargoarm)
    gripers.Set(operatorstick.GetRawAxis(3));//To Do reverse if needed, and check speed
  else
    gripers.Set(-operatorstick.GetRawAxis(3));
}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
