#ifndef CONCURRENT_PULSE_H_
#define CONCURRENT_PULSE_H_

#include "Arduino.h"
#include "ConcurrentPulseSettings.h"

struct PulseData {
    uint8_t index;
    uint32_t distance;
};

void setupDistanceSensors();
void getDistances(struct PulseData (*buffer)[CONPULSE_NUM_SENSORS], byte mask);
void flushBuffer(struct PulseData (* buffer)[CONPULSE_NUM_SENSORS]);

#endif