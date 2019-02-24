# include "Robot.h"

const double Robot::armPositions[] = 
    {farbackarm, hatch, shootrocket, shootbay, frontbay, frontrocket,
  fronthatch, farfrontarm};

const int Robot::armPositionCt = sizeof (Robot::armPositions) / sizeof (double);