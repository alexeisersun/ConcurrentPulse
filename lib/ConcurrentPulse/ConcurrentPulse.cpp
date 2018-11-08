#include "ConcurrentPulse.h"

void getDistances(struct PulseData (*buffer)[CONPULSE_NUM_SENSORS], uint8_t pin_mask)
{
    flushBuffer(buffer);

    // register for all incoming HIGH signals
    uint8_t high_sig = 0x0;

    // register for all incoming LOW signals
    uint8_t low_sig = 0x0;

    // buffer for values from echo pins.
    // the inactive sensors are neglected.
    uint8_t pin_buffer = CONPULSE_ECHO_PINR & pin_mask;

    uint32_t start_micros;
    start_micros = micros();

    // send a LOW pulse, then a HIGH, then a LOW
    CONPULSE_TRIG_PORTR &= ~pin_mask;
    delayMicroseconds(CONPULSE_TRIG_DURATION);
    CONPULSE_TRIG_PORTR |= pin_mask;
    delayMicroseconds(CONPULSE_ECHO_DURATION);
    CONPULSE_TRIG_PORTR &= ~pin_mask;

    // get the distances for all active sensors.
    // exit when all active sensors receive signals back or when timeout is exceeded.
    while ((low_sig & pin_mask) != pin_mask && (micros() - start_micros) <= CONPULSE_TIMEOUT)
    {
        // wait till a response is aquired, no matter where from
        while ((CONPULSE_ECHO_PINR & pin_mask) == pin_buffer && (micros() - start_micros) <= CONPULSE_TIMEOUT)
            ;
        uint8_t pins = CONPULSE_ECHO_PINR;
        uint8_t pin_changes = (pin_buffer ^ pins) & pin_mask;

        uint8_t tmp_low_sig = low_sig;
        uint8_t tmp_high_sig = high_sig;

        // register a LOW value only when it was already a HIGH one
        low_sig = (~pins & high_sig);

        // register a HIGH value when it arrives for the first time
        high_sig |= pin_changes;

        // check what values are new in `low_sig` and `high_sig`
        uint8_t low_sig_trigger = low_sig ^ tmp_low_sig;
        uint8_t high_sig_trigger = high_sig ^ tmp_high_sig;

        pin_buffer = pins;

        for (uint8_t i = 0; pin_changes; ++i)
        {
            // skip unnecessary computations when no change on the given pin
            if ((pin_changes & (1 << i)) == 0)
                continue;

            // register the start of the pulse
            if (high_sig_trigger & pin_changes & (1 << i))
            {
                (*buffer)[i] = {i, micros()};
            }

            // register the end of the pulse
            if (low_sig_trigger & pin_changes & (1 << i))
            {
                (*buffer)[i].distance = micros() - (*buffer)[i].distance;
            }

            // mark the current change as a read one
            pin_changes &= ~(1 << i);
        }
    }
}

void setupDistanceSensors(uint8_t pin_mask)
{
    // set trigger port to OUTPUT
    CONPULSE_TRIG_DDR |= pin_mask;

    // set echo port to INPUT
    CONPULSE_ECHO_DDR &= ~pin_mask;
}

void flushBuffer(struct PulseData (*buffer)[CONPULSE_NUM_SENSORS])
{
    for (int i = 0; i < CONPULSE_NUM_SENSORS; ++i)
    {
        (*buffer)[i] = {0, 0};
    }
}