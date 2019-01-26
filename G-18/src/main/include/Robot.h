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
#include <frc/drive/DifferentialDrive.h>
#include <frc/DoubleSolenoid.h>
#include <AHRS.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/TimedRobot.h>
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/DigitalInput.h>
#include <frc/Encoder.h>
#include <cameraserver/CameraServer.h>

class Robot : public frc::TimedRobot {
  ::WPI_TalonSRX rightfront{1};
  ::WPI_TalonSRX rightback{2};
  frc::SpeedControllerGroup right {rightfront,rightback};
  ::WPI_TalonSRX leftfront{3};
  ::WPI_TalonSRX leftback{4};
  frc::SpeedControllerGroup left {leftfront,leftback};
  frc::DifferentialDrive driver {left,right};//TO DO check weather left or right needs to be inverted

  ::WPI_TalonSRX rightarm{5};//Magnetic encoder is attached to this
  ::WPI_TalonSRX leftarm{6};//TO Do find out which motor magnetic encoder is attached to
  ::WPI_TalonSRX rightgrip{9};
  ::WPI_TalonSRX leftgrip{10};
  frc::SpeedControllerGroup arms {rightarm,leftarm};
  frc::SpeedControllerGroup gripers {rightgrip,leftgrip};

  ::WPI_TalonSRX rightfang{7};
  ::WPI_TalonSRX leftfang{8};
  frc::SpeedControllerGroup fangs {rightfang,leftfang};
  frc::DoubleSolenoid rightwheelearm {0,4},
    leftwheelearm {1,5},
    righttakeoff {2,6},
    lefttakeoff {3,7};

  frc::DigitalInput limitright{4};
  frc::DigitalInput limitleft{5};
  frc::DigitalInput linesensor{6};
  frc::DigitalInput linesensor2{7};

  frc::Encoder leftEncoder{0,1};//,true}; //TO DO check if right on to reverse
  frc::Encoder rightEncoder{2,3};
 public:
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
