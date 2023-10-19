#define ENCODERCOUNT_PER_REV 226
#define ENCODERCOUNT_TO_ANGLE 1.59292035398230088495575 // 360/226
#define MY_PI 3.14159265358979323846264

#include "SoftwareTimer.h"
#include "PIDController.h"
#include "MotorController.h"
#include "main.h"

SoftwareTimer timer1 ;
PIDController rightMotorSpeedPID , leftMotorSpeedPID;
// encoderPinA is  /* YELLOW */
// encoderPinB is  /* GREEN */
MotorController rightMotor(26,25,35,34);
MotorController leftMotor (32,33,39,36);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  /*===========================* rightMotor setup *==================================*/
  rightMotor.set_PWM_channels(0,1);
  rightMotor.setup();
  rightMotor.setMotor(0);

  rightMotorSpeedPID.begin();
  rightMotorSpeedPID.tune(10,3,0);
  rightMotorSpeedPID.setpoint(110);
  rightMotorSpeedPID.limit(-150,150);
  /*===========================* leftMotor setup *==================================*/
  leftMotor.set_PWM_channels(2,3);
  leftMotor.setup();
  leftMotor.setMotor(0);

  leftMotorSpeedPID.begin();
  leftMotorSpeedPID.tune(10,3.2,0);
  leftMotorSpeedPID.setpoint(110);
  leftMotorSpeedPID.limit(-150,150);
  /*==========================================================================*/

  attachInterrupt(digitalPinToInterrupt(rightMotor.encoderPinA),read_rightEncoder,RISING);
  attachInterrupt(digitalPinToInterrupt(leftMotor.encoderPinA),read_leftEncoder,RISING);

  Serial.println("Setup Complete ...");
  delay(1000);
  Serial.println("Setup Complete ...");
}
int x=1;
void loop() {
  // put your main code here, to run repeatedly:
  if (timer1.finished()){
    timer1.start(500);
    int leftMotor_pos= leftMotor.getPos(); // angle in degrees
    int rightMotor_pos= rightMotor.getPos();
    Serial.print(leftMotor_pos);
    Serial.print(" , ");
    Serial.println(rightMotor_pos);
    
    if (x % 4 == 0) {
      rightMotor.setMotor(100);
      leftMotor.setMotor(0);
    } else if (x % 4 == 1) {
      rightMotor.setMotor(0);
      leftMotor.setMotor(0);
    }
     else if (x % 4 == 2) {
      rightMotor.setMotor(0);
      leftMotor.setMotor(100);
    }
    else if (x % 4 == 3) {
      rightMotor.setMotor(0);
      leftMotor.setMotor(0);
    }

    x ++;

  }



}

// void read_rightEncoder(){
//   int b = digitalRead(rightMotor.encoderPinB);
//   if (b>0) {rightMotor.increment_pos();}
//   else {rightMotor.decrement_pos();}
//   rightMotor.last_elapsed_time = micros() - rightMotor.last_interrupt_time;
//   rightMotor.last_interrupt_time = micros();
// }

// void read_leftEncoder(){
//   int b = digitalRead(leftMotor.encoderPinB);
//   if (b>0) {leftMotor.increment_pos();}
//   else {leftMotor.decrement_pos();}
//   leftMotor.last_elapsed_time = micros() - leftMotor.last_interrupt_time;
//   leftMotor.last_interrupt_time = micros();
// }

void read_rightEncoder(){
  readEncoder(rightMotor);
}
void read_leftEncoder(){
  readEncoder(leftMotor);
}
void readEncoder(MotorController& mott){
  int b = digitalRead(mott.encoderPinB);
  if (b>0) {mott.decrement_pos();}
  else {mott.increment_pos();}
  mott.last_elapsed_time = micros() - mott.last_interrupt_time;
  mott.last_interrupt_time = micros();
}