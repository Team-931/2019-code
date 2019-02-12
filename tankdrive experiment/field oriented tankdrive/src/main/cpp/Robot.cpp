//TRAINER---TRAINER---TRAINER---TRAINER---TRAINER---TRAINER---TRAINER---TRAINER---
#include <frc/Joystick.h>
#include <frc/TimedRobot.h>
#include <frc/Timer.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/livewindow/LiveWindow.h>
#include <AHRS.h>
#include <ctre/Phoenix.h>
//#define usingtank
const double ramptime=0.25;
const double motorvoltage=11.5;//minimum accepted motor voltage for voltage
								//compensation

class Robot: public frc::TimedRobot {
	bool tankdrive=false;
private:
	::WPI_TalonSRX s_frontleft{2};
	::WPI_TalonSRX s_backleft{4};
	::WPI_TalonSRX s_frontright{3};
	::WPI_TalonSRX s_backright{8};

	frc::SpeedControllerGroup m_left{ s_frontleft,s_backleft};
	frc::SpeedControllerGroup m_right{ s_frontright, s_backright};
	frc::DifferentialDrive Robot { m_left,m_right };// robot drive system

	frc::Joystick stick { 0 };  // driver stick


	frc::Timer m_time;


public:
	Robot() {
		Robot.SetExpiration(0.1);
	}

	void RobotInit(){

	s_frontright.SetInverted(true);
	s_backright.SetInverted(true);
	s_frontleft.ConfigOpenloopRamp(ramptime,0);
	s_backleft.ConfigOpenloopRamp(ramptime,0);
	s_backright.ConfigOpenloopRamp(ramptime,0);
		//printf("Old code run\n");
	s_frontleft.ConfigVoltageCompSaturation(motorvoltage,0);
	s_backleft.ConfigVoltageCompSaturation(motorvoltage,0);
	s_backright.ConfigVoltageCompSaturation(motorvoltage,0);
		//printf("New code run\n");
::AHRS navX{::SPI::Port::kMXP};
	
	}

	






	void TeleopPeriodic() {
		double X = stick.GetX();
		double Y = stick.GetY();
    int Angle = NavX.GetAngle();
    ::SmartDashboard::PutNumber("angle", angle);
Robot.TankDrive(X,Y);
							}
};
START_ROBOT_CLASS(Robot)
