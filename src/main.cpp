#include <Arduino.h>
#include "./app/Robot.h"

void setup() {
  Serial.begin(9600);
  Serial.println("SERIAL TEST OK");
  robotInit();            // Initialize the robot
}

void loop() {
  Serial.println("BOARD RUNNING");
  vTaskDelete(NULL);   // The robot execution is fully managed by FreeRTOS tasks. The Arduino loop task is not required.
}