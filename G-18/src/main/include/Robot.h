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
static constexpr double degreetick (double degreeangle){
  return (degreeangle-115)/360*1024;//IF want change back for the dashboard
  }
  
class Robot : public frc::TimedRobot {
  ::WPI_TalonSRX rightfront{1};
  ::WPI_TalonSRX rightback{2};
  frc::SpeedControllerGroup right {rightfront,rightback};
  ::WPI_TalonSRX leftfront{3};
  ::WPI_TalonSRX leftback{4};
  frc::SpeedControllerGroup left {leftfront,leftback};
  frc::DifferentialDrive driver {left,right};//TO DO check weather left or right needs to be inverted

  ::WPI_TalonSRX rightarm{5};
  ::WPI_TalonSRX leftarm{6};
  ::WPI_TalonSRX rightgripW{7};//W=wheel
  ::WPI_TalonSRX leftgripW{8};
  frc::SpeedControllerGroup gripers {rightgripW,leftgripW};
  frc::SpeedControllerGroup arms {rightarm,leftarm};

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

  int endgame1 {0};//phase 0 not endgame, phase 1 climb/start endgame,phase 2 stop main wheels, phase 3 bring up pogostick, phase 4 move onto plateform, phase 5 stop
  
 public:
  void endgameinit();
  void endgameperiodic();
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
  frc::PIDController anglearm{.1,0,0,&armEncoder,&arms};
  frc::SendableChooser<std::string> m_chooser;
  const std::string kAutoNameDefault = "Default";
  const std::string kAutoNameCustom = "My Auto";
  std::string m_autoSelected;
  ::AHRS navx {::SPI::kMXP};
  constexpr static double farbackarm=degreetick(200), farfrontarm=degreetick(-20), startingposition=0,
  endgame=degreetick(65), shootlow=degreetick(0), shoothigh=degreetick(60);
};
