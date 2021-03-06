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
  cs::UsbCamera frontCam{"Front Cam", 0}, rearCam{"Rear Cam", 1};
  cs::MjpegServer outputCam {frc::CameraServer::GetInstance()->AddSwitchedCamera("Output Cam")};

  ::WPI_TalonSRX rightfront{1};
  ::WPI_TalonSRX rightback{2};
  frc::SpeedControllerGroup right {rightfront,rightback};
  ::WPI_TalonSRX leftfront{3};
  ::WPI_TalonSRX leftback{4};
  frc::SpeedControllerGroup left {leftfront,leftback};
  frc::DifferentialDrive driver {left,right};//TO DO check weather left or right needs to be inverted

  ::WPI_TalonSRX rightarm{5};//top
  ::WPI_TalonSRX leftarm{6};//bottom
  ::WPI_TalonSRX rightgripW{7};//W=wheel
  ::WPI_TalonSRX leftgripW{8};
  frc::SpeedControllerGroup gripers {rightgripW,leftgripW};
  frc::SpeedControllerGroup arms {rightarm,leftarm};

  ::WPI_TalonSRX rightfangw{9};
  ::WPI_TalonSRX leftfangw{10};
  ::WPI_TalonSRX centerfang1{11};
  ::WPI_TalonSRX centerfang2{12};
  frc::SpeedControllerGroup centerfang{centerfang1,centerfang2};
  frc::SpeedControllerGroup fangs {rightfangw,leftfangw};
  frc::DifferentialDrive equalup {left,centerfang};
  frc::DoubleSolenoid centergriparm {4,5},
    centertakeoff {6,7},centertakeoff2 {1,0};
  frc::DigitalInput limitpogo{4};
  frc::DigitalInput limitfang{5};
  frc::AnalogInput leftsensor{0};
  frc::AnalogInput rightsensor{1};
  static inline bool usethresshold (frc::AnalogInput const&line)
  {return line.GetVoltage()>2;}

  frc::Encoder leftEncoder{0,1};//2048 ticks per rotation 6in*3.14 per rotation
  frc::Encoder rightEncoder{2,3,true}; //reverse
  frc::Encoder armEncoder{6,7, true};  //reverse

//stating user interface items
  frc::Joystick driverstick{0};
  frc::Joystick operatorstick{1};
  frc::Joystick endgamestick{2};

  //Climb either dashboard or press buttons

  bool arcadedrive {false};

  //bool  cargoarm {true};

  int endgamephase {0};//phase 0 not endgame, phase 1 climb/start endgame,phase 2 stop main wheels, phase 3 bring up pogostick, phase 4 move onto plateform, phase 5 stop
  
 public:
  void Autocenterbay();
  void endgameinit();
  void endgameperiodic();
  void armcontrol();
  void armdegree();
  void robotcontrol();
  void RobotInit() override;
  void RobotPeriodic() override;
  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void TeleopInit() override;
  void TeleopPeriodic() override;
  void TestPeriodic() override;
  void endgameTeleop();

 private:
  double armP = .02;
  frc::PIDController anglearm{armP,0,0,&armEncoder,&arms};
  frc::SendableChooser<std::string> m_chooser;
  const std::string kAutoNameDefault = "Default";
  const std::string autoForward = "off platform";
  std::string m_autoSelected;
  ::AHRS navx {::SPI::kMXP};
  constexpr static double farbackarm=(-70), farfrontarm=(70), startingposition=0,
  endgame=(19), shootrocket=(-42), shootbay=(-30), vertical=(0), hatch=(-53),
  fronthatch=(53), frontrocket=(44), frontbay=(31);
  const static double armPositions[];
  const static int armPositionCt;// = sizeof (Robot::armPositions) / sizeof (double);

  //add extra angles
  frc::Timer time;
};
