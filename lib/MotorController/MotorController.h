#ifndef MotorControllerLib
#define MotorControllerLib

// pulse width modulation constants
#define PWM_Res      8
#define PWM_Freq  1200

// if this amount of time passes without any encoder pulses 
// then the motor has stopped
#define ENC_STOP_TIME_THRESHOLD 10000 // 10000 micro second

class MotorController{
  public:
    // control data
    int hbridgeA, hbridgeB, encoderPinA, encoderPinB;
    int pwm_channel1= -1 , pwm_channel2= -1;
    // sensor data
    volatile unsigned long last_elapsed_time = 0;
    volatile unsigned long last_interrupt_time = 0;
    volatile long int pulses = 0;  
    volatile int  movement_direction = 0;
    int targetPulses = 0;

    /**
     * @brief Constructor for the MotorController class.
     * @param hbridgeA The H-bridge pin A.
     * @param hbridgeB The H-bridge pin B.
     * @param encoderPinA The encoder pin A.
     * @param encoderPinB The encoder pin B.
     * @param pulses The initial position (default: 0).
     */
    
    MotorController(const MotorController& ){};
    MotorController(int hbridgeA, int hbridgeB, int encoderPinA, int encoderPinB, int pulses = 0):

        hbridgeA(hbridgeA),hbridgeB(hbridgeB),
        encoderPinA(encoderPinA),encoderPinB(encoderPinB),
        pulses(pulses) {};
    
   
    /**
     * @brief Setup the motor controller.
     */
    void setup();

    /**
     * @brief Get the current position of the motor.
     * @return The current motor position.
     */
    long int getPulses();
    void resetPulses();
    double getAngleRad();
    double getAngleDeg(); 
    bool hasPulsesReachedTarget();
    /**
     * @brief Get the current speed of the motor.
     * @return The current motor speed.
     */
    double getSpeed();
    double getSpeedRPM();
    /**
     * @brief Set the motor PWM value to control the motor.
     * @param pwm_val The PWM value to set.
     */
    void setVoltage(int pwm_val);

    /**
     * @brief Set the PWM channels for motor control.
     * @param channel1 The first PWM channel.
     * @param channel2 The second PWM channel.
     */
    void set_PWM_channels(int channel1, int channel2);

    /**
     * @brief Increment the motor position.
     */
    void increment_pulses();

    /**
     * @brief Decrement the motor position.
     */
    void decrement_pulses();
};
#endif