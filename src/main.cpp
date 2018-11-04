#include <Arduino.h>
#include <ConcurrentPulse.h>

struct PulseData buffer[CONPULSE_NUM_SENSORS];
uint8_t sensor_mask = 0b00000001;
void setup() {
  Serial.begin(9600);
  setupDistanceSensors(sensor_mask);
}

void loop() {
  delay(1000);
  getDistances(&buffer, sensor_mask);
  
  for(int i = 0; i < CONPULSE_NUM_SENSORS; i++)
  {
    Serial.print(buffer[i].index);
    Serial.print(' ');
    Serial.println(buffer[i].distance);
  }
  
  Serial.println();
}