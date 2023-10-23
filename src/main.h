#ifndef MAIN_PROG
#define MAIN_PROG

#include "SoftwareTimer.h"
#include "PIDController.h"
#include "MotorController.h"

template <MotorController& mott>
void readEncoder();
void setLeftMotorTargetRPM(int rpm);
void setRightMotorTargetRPM(int rpm);
int CMtoPulses(float cm);
#endif