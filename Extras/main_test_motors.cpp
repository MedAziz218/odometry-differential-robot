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

  attachInterrupt(digitalPinToInterrupt(rightMotor.encoderPinA),readEncoder<rightMotor>,RISING);
  attachInterrupt(digitalPinToInterrupt(leftMotor.encoderPinA),readEncoder<leftMotor>,RISING);
  
  Serial.println("Setup Complete ...");
  delay(1000);
  Serial.println("Setup Complete ...");
}
int x=1;
void loop() {
  // put your main code here, to run repeatedly:
  if (timer1.finished()){
    timer1.start(0.5f);
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

template <MotorController& mott>
void readEncoder() {
   int b = digitalRead(mott.encoderPinB);
  if (b>0) {mott.decrement_pos();}
  else {mott.increment_pos();}
  mott.last_elapsed_time = micros() - mott.last_interrupt_time;
  mott.last_interrupt_time = micros();
}

