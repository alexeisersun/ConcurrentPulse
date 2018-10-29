#include "ConcurrentPulse.h"

void bufferDistanceResults(byte mask)
{

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
