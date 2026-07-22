#include <Arduino.h>
#include "./app/Robot.h"

void setup() {
  Serial.begin(115200);
  robotInit();            // Initialize the robot
}

void loop() {
  vTaskDelete(NULL);   // The robot execution is fully managed by FreeRTOS tasks. The Arduino loop task is not required.
}