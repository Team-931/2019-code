/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

// This is a clarification of the Way

#pragma once

#include <string>
#include <ctre/phoenix.h>
#include <AHRS.h>
#include <frc/WPIlib.h>
#include <cameraserver/CameraServer.h>

class Robot : public frc::TimedRobot {
  ::WPI_TalonSRX rightfront{1};
  ::WPI_TalonSRX rightback{2};
  frc::SpeedControllerGroup right {rightfront,rightback};
  ::WPI_TalonSRX leftfront{3};
  ::WPI_TalonSRX leftback{4};
  frc::SpeedControllerGroup left {leftfront,leftback};
  frc::DifferentialDrive driver {left,right};//TO DO check weather left or right needs to be inverted

  ::WPI_TalonSRX arm{5};
  ::WPI_TalonSRX rightgripW{7};//W=wheel
  ::WPI_TalonSRX leftgripW{8};
  frc::SpeedControllerGroup gripers {rightgripW,leftgripW};

  ::WPI_TalonSRX rightfangw{9};
  ::WPI_TalonSRX leftfangw{10};
  ::WPI_TalonSRX centerfang{11};
  frc::SpeedControllerGroup fangs {rightfangw,leftfangw};
  frc::DoubleSolenoid centergriparm {0,1},
    centertakeoff {2,3};

  frc::DigitalInput limitright{4};
  frc::DigitalInput limitleft{5};
  frc::AnalogInput linesensor{0};
  frc::AnalogInput linesensor2{1};
  static inline bool usethresshold (frc::AnalogInput const&line)
  {return line.GetVoltage()>2;}

  frc::Encoder leftEncoder{0,1};//,true}; //TO DO check if right on to reverse
  frc::Encoder rightEncoder{2,3};
  frc::Encoder armEncoder{6,7};

//stating user interface items
  frc::Joystick driverstick{0};
  frc::Joystick operatorstick{1};
  //Climb either dashboard or press buttons

  bool arcadedrive {true};

  bool  cargoarm {true};
  
 public:
  void armcontrol();
  void robotcontrol();
  void RobotInit() override;
  void RobotPeriodic() override;
  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void TeleopInit() override;
  void TeleopPeriodic() override;
  void TestPeriodic() override;

 private:
  frc::SendableChooser<std::string> m_chooser;
  const std::string kAutoNameDefault = "Default";
  const std::string kAutoNameCustom = "My Auto";
  std::string m_autoSelected;
  ::AHRS navx {::SPI::kMXP};
};
