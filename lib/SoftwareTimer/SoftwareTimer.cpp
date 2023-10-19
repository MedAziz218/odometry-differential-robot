#include <Arduino.h>
#include "SoftwareTimer.h"
// Constructor
SoftwareTimer::SoftwareTimer() : END_OF_TIMER(0), duration(0) {}

// Start the timer with the given duration in milliseconds
void SoftwareTimer::start(unsigned long millis_dt) {
    duration = millis_dt;
    END_OF_TIMER = micros() + millis_dt * 1000;
}

// Check if the timer has finished
bool SoftwareTimer::finished() {
  return END_OF_TIMER <= micros() ; 
}

// Reset the timer
void SoftwareTimer::reset() {
    END_OF_TIMER = 0;
}

// Restart the timer with the previous duration
void SoftwareTimer::restart() {
    start(duration);
}
