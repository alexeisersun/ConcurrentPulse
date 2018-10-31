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

    // each active sensor is set to 0 when the trigger signal is sent
    // and set to 1 when the echo arrives.
    // get the distances for all active sensors
    byte high_sig = 0;
    byte low_sig = 0;

    while (current_mask | ~mask != 0b11111111 && (micros() - pulse_time) <= CONPULSE_TIMEOUT)
    {
        // wait till a response is aquired, no matter where from
        while (CONPULSE_ECHO_PINR & mask == current_mask && (micros() - pulse_time) <= CONPULSE_TIMEOUT)
            ;
        byte pins = CONPULSE_ECHO_PINR;
        byte mask_changes = current_mask ^ pins;
        
        byte tmp_low_sig = low_sig;
        low_sig = ~pins & high_sig;
        
        high_sig |= mask_changes;
        current_mask |= mask_changes;
        
        byte mask_index = CONPULSE_NUM_SENSORS - 1;
        byte empty_bit = 0;
        byte low_sig_trigger = low_sig ^ tmp_low_sig;
        // write pulse time to buffer
        while (1)
        {
            if (!low_sig_trigger) break;
            empty_bit = low_sig_trigger & 0b00000001;
            low_sig_trigger >>= 1;
            if (!empty_bit) buffer[mask_index] = micros() - pulse_time;
            --mask_index;
        }
    }
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
