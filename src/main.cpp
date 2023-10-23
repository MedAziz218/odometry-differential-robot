#define ENCODE_PULSE_PER_REV 226
#define ENCODE_PULSE_TO_ANGLE_DEG 1.59292035398230088495575 // 360/226
#define ENCODE_PULSE_TO_ANGLE_RAD 0.01390085244951235946222 // PI/226
#define WHEEL_DIAMETER 65.00 // Wheel diameter in millimeters
// #define MY_PI 3.14159265358979323846264

#include "SoftwareTimer.h"
#include "PIDController.h"
#include "MotorController.h"
#include "main.h"

SoftwareTimer timer1,timer2,timer3;
PIDController rightMotorSpeedPID, leftMotorSpeedPID;
// encoderPinA is  /* YELLOW */
// encoderPinB is  /* GREEN */
MotorController rightMotor(26, 25, 35, 34);
MotorController leftMotor(32, 33, 39, 36);
double l,r;
int rightMotorTargetRPM = 0;
int leftMotorTargetRPM = 0;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  /*===========================* rightMotor setup *==================================*/
  rightMotor.set_PWM_channels(0, 1);
  rightMotor.setup();
  rightMotor.setVoltage(0);

  rightMotorSpeedPID.begin();
  rightMotorSpeedPID.tune(10, 3, 0);
  rightMotorSpeedPID.setpoint(0);
  rightMotorSpeedPID.limit(-150, 150);
  /*===========================* leftMotor setup *==================================*/
  leftMotor.set_PWM_channels(2, 3);
  leftMotor.setup();
  leftMotor.setVoltage(0);

  leftMotorSpeedPID.begin();
  leftMotorSpeedPID.tune(10, 3.2, 0);
  leftMotorSpeedPID.setpoint(0);
  leftMotorSpeedPID.limit(-150, 150);
  /*==========================================================================*/

  attachInterrupt(digitalPinToInterrupt(rightMotor.encoderPinA), readEncoder<rightMotor>, RISING);
  attachInterrupt(digitalPinToInterrupt(leftMotor.encoderPinA), readEncoder<leftMotor>, RISING);

  setLeftMotorTargetRPM(leftMotorTargetRPM);
  setRightMotorTargetRPM(rightMotorTargetRPM);

  delay(1000);
  Serial.println("Setup Complete ...");
}
int program_counter = 0;
String recievedMsg = "";
void loop()
{
  while (Serial.available()){
    recievedMsg += (char)Serial.read();
  }
  if (recievedMsg.length()){
    Serial.println("MSG: "+recievedMsg);
    recievedMsg = "";
  }
  // put your main code here, to run repeatedly:
  if (timer1.finished())
  {
    timer1.start(1000);
    int leftMotor_pos = leftMotor.getPulses(); // angle in degrees
    int rightMotor_pos = rightMotor.getPulses();
    // Serial.print(">Pos: ");Serial.print(leftMotor_pos);Serial.print(" , ");Serial.println(rightMotor_pos);
    
    double leftMotor_rpm = leftMotor.getSpeedRPM();
    double rightMotor_rpm = rightMotor.getSpeedRPM();
    // Serial.print(">RPM: ");Serial.print(leftMotor_rpm);Serial.print(" , ");Serial.println(rightMotor_rpm);

    
  }
  //TODO: remove the speedPID controllers
  //TODO: instead of controlling the speed of the motors , make the position tracking work
  //TODO: and control the motors based on that
  //TODO: if the position tracking is good then it the speed of the motors doesn't matter at all 
  //TODO: conclusion REMOVE THE SPEEDPID and ADD THE POSITION TRACKING 
  //TEST: try to make the robot go from (0,0) to (50,0) in cm 
  
  //TODO: add bluetooth serial to control the robot and get serial output messages
  if (program_counter==0){
    setLeftMotorTargetRPM(120);
    setRightMotorTargetRPM(120);
    leftMotor.targetPulses = leftMotor.getPulses()+CMtoPulses(50);
    rightMotor.targetPulses = rightMotor.getPulses()+CMtoPulses(50);
    program_counter=1;
    // Serial.println("here");
    timer3.start(5000);
  }else if (program_counter==1 && leftMotor.hasPulsesExceededTarget() && rightMotor.hasPulsesExceededTarget()){
    setLeftMotorTargetRPM(0);
    setRightMotorTargetRPM(0);
    program_counter = 2;
    Serial.println("here1");

  }
  
  
  
  if (timer2.finished()){
    timer2.start( 5 ); // control the motors voltage every 1 milli second
    // control the motor voltage with the speedPID to get the desired RPM for each motor
     l = leftMotorSpeedPID.compute(leftMotor.getSpeedRPM() );//,GRAPH,VERBOSE) ;
     r =rightMotorSpeedPID.compute(rightMotor.getSpeedRPM() );//,GRAPH,VERBOSE);
    // Serial.print(">PWM: ");Serial.print(l);Serial.print(" , ");Serial.print(r);Serial.println(" , ");
    Serial.print(">speed: ");Serial.print(leftMotor.getSpeedRPM());Serial.print(" , ");Serial.print(rightMotor.getSpeedRPM());Serial.println(" , "); 
    // Serial.print(">bool: ");Serial.print(leftMotor.hasPulsesExceededTarget());Serial.print(" , ");Serial.print(rightMotor.hasPulsesExceededTarget());Serial.println(" , "); 
    // Serial.print(">target: ");Serial.print(leftMotor.targetPulses);Serial.print(" , ");Serial.print(rightMotor.targetPulses);Serial.println(" , "); 
    // Serial.print(">pulses: ");Serial.print(leftMotor.getPulses());Serial.print(" , ");Serial.print(rightMotor.getPulses());Serial.print(" , "); Serial.println(rightMotor.movement_direction);
    
    leftMotor.setVoltage(l);
    rightMotor.setVoltage(r);
  }else {
    leftMotor.setVoltage(l);
    rightMotor.setVoltage(r);
  }
}

void setLeftMotorTargetRPM(int rpm){
  // setTargetSpeed
  leftMotorTargetRPM = rpm;
  leftMotorSpeedPID.setpoint(leftMotorTargetRPM);
}
void setRightMotorTargetRPM(int rpm){
  // setTargetSpeed
  rightMotorTargetRPM = rpm;
  rightMotorSpeedPID.setpoint(rightMotorTargetRPM);
}


void MoveForward(int pulses, int mRPM) {
  leftMotor.targetPulses = leftMotor.getPulses() + pulses;
  rightMotor.targetPulses = rightMotor.getPulses() + pulses;
  
  setLeftMotorTargetRPM(mRPM);
  setRightMotorTargetRPM(mRPM);

  if (leftMotor.getPulses() < leftMotor.targetPulses){
    setLeftMotorTargetRPM(0);
  }
  if (rightMotor.getPulses() < rightMotor.targetPulses ){
    setRightMotorTargetRPM(0);
  } 
}
int CMtoPulses(float cm){
  float circumference = (WHEEL_DIAMETER * PI) / 10;
  float revolutions = cm/circumference ;
  return (int) revolutions*ENCODE_PULSE_PER_REV;
}


template <MotorController &mott>
void readEncoder()
{
  int b = digitalRead(mott.encoderPinB);
  if (b > 0)
  {
    mott.decrement_pulses();
  }
  else
  {
    mott.increment_pulses();
  }
  mott.last_elapsed_time = micros() - mott.last_interrupt_time;
  mott.last_interrupt_time = micros();
}
