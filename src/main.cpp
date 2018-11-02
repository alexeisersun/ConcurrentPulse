#include <Arduino.h>
#include <ConcurrentPulse.h>

double buffer[CONPULSE_NUM_SENSORS];

void setup() {
  Serial.begin(9600);
  setupDistanceSensors();
}

void loop() {
  delay(1000);
  getDistances(&buffer, 0b00000001);
  
  for(int i = 0; i < CONPULSE_NUM_SENSORS; i++)
  {
    Serial.println(buffer[i]);
  }
  
  Serial.println();
}