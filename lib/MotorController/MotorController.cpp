#include "MotorController.h"
#include <Arduino.h>
#ifndef ENCODE_PULSE_PER_REV
    #define ENCODE_PULSE_PER_REV 226
    #define ENCODE_PULSE_TO_ANGLE_DEG 1.59292035398230088495575 // 360/226
    #define ENCODE_PULSE_TO_ANGLE_RAD 0.01390085244951235946222 // PI/226
#endif

// make sure to use set_PWM_channels function before this
void MotorController::setup(){
    pinMode(hbridgeA,OUTPUT); pinMode(hbridgeB,OUTPUT);
    pinMode(encoderPinA,INPUT);pinMode(encoderPinB,INPUT);
    ledcSetup(pwm_channel1, PWM_Freq, PWM_Res); ledcAttachPin(hbridgeA, pwm_channel1) ;
    ledcSetup(pwm_channel2, PWM_Freq, PWM_Res); ledcAttachPin(hbridgeB, pwm_channel2) ;
}
void MotorController::resetPulses(){
    noInterrupts(); pulses = 0; interrupts();
}
// returns the pulsesition in units of encoder pulses 
long int MotorController::getPulses(){
    long int pulses_returned=0; noInterrupts(); pulses_returned = pulses; interrupts();
    return pulses_returned;
}
bool MotorController::hasPulsesExceededTarget(){
        return pulses>targetPulses;

}
// returns the angle the motor has rotated in radians 
double MotorController::getAngleRad(){
    return getPulses()*ENCODE_PULSE_TO_ANGLE_RAD;
}
// returns the angle the motor has rotated in degrees 
double MotorController::getAngleDeg(){
    return getPulses()*ENCODE_PULSE_TO_ANGLE_DEG;
}
// returns the speed in units of encoder pulses per second
double MotorController::getSpeed(){ 
    double speed; unsigned long stop_indicator;
    noInterrupts(); 
    speed = 1/(last_elapsed_time/1.0e6) * movement_direction;
    stop_indicator = micros() - last_interrupt_time;
    interrupts();
    if (stop_indicator>ENC_STOP_TIME_THRESHOLD){ speed = 0; }
    return speed;
}
// returns the speed in units of Rounds per Minute
double MotorController::getSpeedRPM(){ 
    return getSpeed()*60.0/ENCODE_PULSE_PER_REV;
}
// set motor direction and voltage applied to the motor
void MotorController::setVoltage(int pwm_val){
    int direction = (pwm_val >=0) ? 1:-1;
    pwm_val = abs(pwm_val);
    if(direction == 1){
    ledcWrite(pwm_channel1,pwm_val);
    ledcWrite(pwm_channel2,LOW);
    }
    else if (direction == -1){
    ledcWrite(pwm_channel1,LOW);
    ledcWrite(pwm_channel2,pwm_val);
    }
    else {
    ledcWrite(pwm_channel1,LOW);
    ledcWrite(pwm_channel2,LOW);
    }
}
// set the pwm channels dedicated for the motor 
// (needed only in ESP32)
void MotorController::set_PWM_channels(int channel1,int channel2){
    pwm_channel1 = channel1; pwm_channel2 = channel2;
}
void MotorController::increment_pulses(){
    pulses ++; movement_direction = 1;
}
void MotorController::decrement_pulses(){
    pulses --; movement_direction = -1;
}

