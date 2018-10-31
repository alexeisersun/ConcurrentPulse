#include "ConcurrentPulse.h"

void getDistances(double buffer[], byte enabled_mask)
{
    double pulse_time;
    byte current_mask = 0;
    byte mask = 0b11111111 & enabled_mask;

    // send a LOW pulse, then a HIGH, then a LOW
    CONPULSE_TRIG_PORTR &= ~mask;
    delayMicroseconds(CONPULSE_TRIG_DURATION);
    CONPULSE_TRIG_PORTR |= mask;
    delayMicroseconds(CONPULSE_ECHO_DURATION);
    CONPULSE_TRIG_PORTR &= ~mask;

    // register when the pulse was sent
    pulse_time = micros();
}

void setupDistanceSensors()
{
    // set trigger port to OUTPUT
    CONPULSE_TRIG_DDR |= 0b11111111;

    // set echo port to INPUT
    CONPULSE_ECHO_DDR = 0b00000000;

    // flush old values for trigger and echo ports
    CONPULSE_TRIG_PORTR = 0b00000000;
    CONPULSE_ECHO_PORTR = 0b00000000;
}
