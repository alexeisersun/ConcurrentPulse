#ifndef CONCURRENT_PULSE_H_
#define CONCURRENT_PULSE_H_

#include "Arduino.h"
#include "ConcurrentPulseSettings.h"

void setupDistanceSensors();
void bufferDistanceResults(byte mask);

#endif