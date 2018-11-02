#include "ConcurrentPulse.h"

void getDistances(double (* buffer)[CONPULSE_NUM_SENSORS], byte mask)
{
    flushBuffer(buffer);
    
    // send a LOW pulse, then a HIGH, then a LOW
    CONPULSE_TRIG_PORTR &= ~mask;
    delayMicroseconds(CONPULSE_TRIG_DURATION);
    CONPULSE_TRIG_PORTR |= mask;
    delayMicroseconds(CONPULSE_ECHO_DURATION);
    CONPULSE_TRIG_PORTR &= ~mask;

    // register when the pulse was sent
    double pulse_time;
    pulse_time = micros();
    
    // register for all incoming HIGH signals
    byte high_sig = 0;

    // register for all incoming LOW signals
    byte low_sig = 0;

    // buffer for values from echo pins.
    // the inactive sensors are neglected.
    byte pin_buffer = ~mask;

    // get the distances for all active sensors.
    // exit when all active sensors receive signals back or when timeout is exceeded.
    while ((low_sig & mask) != mask && (micros() - pulse_time) <= CONPULSE_TIMEOUT)
    {
        // wait till a response is aquired, no matter where from
        while ((CONPULSE_ECHO_PINR & mask) == pin_buffer && (micros() - pulse_time) <= CONPULSE_TIMEOUT)
            ;
        byte pins = CONPULSE_ECHO_PINR;
        byte pin_changes = (pin_buffer ^ pins) & mask;
        
        byte tmp_low_sig = low_sig;

        // register a LOW value only when it was already a HIGH one
        low_sig = (~pins & high_sig);

        // check what values are new in `low_sig`
        byte low_sig_trigger = low_sig ^ tmp_low_sig;

        // register a HIGH value when it arrives for the first time
        high_sig |= pin_changes;

        pin_buffer = pins;
        
        // write timestamps for each bit that changed in `low_sig`
        byte mask_index = 0;
        byte last_bit = 0;
        while (1)
        {
            if (!low_sig_trigger) break;
            last_bit = low_sig_trigger & 0b00000001;
            low_sig_trigger >>= 1;
            if (last_bit) (*buffer)[mask_index] = micros() - pulse_time;
            ++mask_index;
        }
    }
}

void setupDistanceSensors()
{
    // set trigger port to OUTPUT
    CONPULSE_TRIG_DDR |= 0b11111111;

    // set echo port to INPUT
    CONPULSE_ECHO_DDR = 0b00000000;
}

void flushBuffer(double (* buffer)[CONPULSE_NUM_SENSORS]){
    for (int i = 0; i < CONPULSE_NUM_SENSORS; ++i)
        (*buffer)[i] = -1;
}