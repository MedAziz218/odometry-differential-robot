#ifndef SOFTWARETIMER
#define SOFTWARETIMER

/**
 * @class SoftwareTimer
 * @brief A simple timer class for managing time-based events.
 * use
 */
class SoftwareTimer {
private:
    unsigned long END_OF_TIMER = 0; ///< End time of the timer in microseconds.
    unsigned long duration = 0;     ///< Duration of the timer in microseconds.
public:
    /**
     * @brief Constructor for the SoftwareTimer class.
     */
    SoftwareTimer();

    /**
     * @brief Start the timer with the given duration.
     * @param millis_dt The duration of the timer in milliseconds.
     */
    void start(int millis_dt );

    /**
     * @brief Start the timer with the given duration.
     * @param micros_dt The duration of the timer in microseconds.
     */
    void startMicros(int micros_dt );
    

    /**
     * @brief Check if the timer has finished.
     * @return True if the timer has finished; otherwise, false.
     */
    bool finished();

    /**
     * @brief Reset the timer, setting it to an initial state.
     */
    void reset();

    /**
     * @brief Restart the timer with the previous duration.
     */
    void restart();
};

#endif
