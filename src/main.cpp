#include <Arduino.h>
#include <ConcurrentPulse.h>

struct PulseData buffer[CONPULSE_NUM_SENSORS];

void setup() {
  Serial.begin(9600);
  setupDistanceSensors();
}

void loop() {
  delay(1000);
  getDistances(&buffer, 0b00000001);
  
  for(int i = 0; i < CONPULSE_NUM_SENSORS; i++)
  {
    Serial.print(buffer[i].index);
    Serial.print(' ');
    Serial.println(buffer[i].distance);
  }
  
  Serial.println();
}