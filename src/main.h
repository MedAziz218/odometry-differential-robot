#ifndef MAIN_PROG
#define MAIN_PROG

#include "SoftwareTimer.h"
#include "PIDController.h"
#include "MotorController.h"
void read_rightEncoder();
void read_leftEncoder();
void readEncoder(MotorController& mott);
#endif