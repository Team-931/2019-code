#include <frc/WPIlib.h>
#include "Robot.h"

class autocontrol {frc::PIDController compass;
public:
autocontrol (frc::DifferentialDrive&,::AHRS&,frc::Encoder&,frc::Encoder&);

}