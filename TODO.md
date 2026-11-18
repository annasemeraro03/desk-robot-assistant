# Project TODO – Desk Robot Assistant

This document collects the remaining implementation tasks, improvements and tests for the Desk Robot Assistant project.

Tasks can be checked off as they are completed.

---
## 0.5. Miglioramenti finali
- [ ] macchina a stati
- [ ] locomotion
- [ ] sincronizzazione calendario (USB)
- [ ] sincronizzazione meteo (USB)
- [ ] grafiche 

## 1. Finite State Machine (FSM)

### State management
- [ ] Review the use of `previousState`.
- [ ] Implement proper interruption context management.
- [ ] Preserve the original state when multiple interruptions occur.
- [ ] Ensure that the robot returns to the appropriate state after an interruption.
- [ ] Verify that invalid events are ignored.

### Focus session (Pomodoro)
- [ ] Verify the focus timer calculation and remaining time.
- [ ] Pause the timer when concentration is interrupted.
- [ ] Resume the timer using the previously saved remaining time.
- [ ] Prevent the timer from running during interruptions.
- [ ] Handle the case where the remaining time reaches zero during an interruption.
- [ ] Prevent a new focus session from accidentally overwriting an existing one.
- [ ] Document the alternative approach in which the timer continues running during interruptions (not enabled).

### Event priorities and transitions
- [ ] Ensure that shaking interrupts every FSM state and transitions to `ANGRY`.
- [ ] Verify the behavior of events received during `STARTUP` and `VICTORY`.
- [ ] Verify the behavior of petting in every relevant state.
- [ ] Ensure that `ANGRY` transitions to `CUDDLED` after petting.
- [ ] Verify all state transition durations:
  - [ ] `STARTUP` → `HAPPY`: 2 seconds.
  - [ ] `HAPPY` → `BORED`: 5 minutes without interaction.
  - [ ] `CUDDLED` → `HAPPY` or `FOCUSED`: 4 seconds.
  - [ ] `FOCUSED` → `VICTORY`: when the focus timer expires.
  - [ ] `VICTORY` → `HAPPY`: 4 seconds.

### Calendar and weather
- [ ] Verify calendar reminder handling from `HAPPY` and `FOCUSED`.
- [ ] Preserve the remaining focus time during calendar interruptions.
- [ ] Return to `FOCUSED` or `HAPPY` after a single tap, depending on whether a focus session is active.
- [ ] Allow triple tap to access weather information only from `HAPPY`.
- [ ] Return to `HAPPY` after a single tap on the weather screen.
- [ ] Prevent repeated calendar events from causing unwanted transitions.

### Testing
- [ ] Test all transitions defined in the project specification.
- [ ] Test interruptions during a focus session.
- [ ] Test multiple consecutive interruptions.
- [ ] Test repeated and closely spaced events.
- [ ] Verify timer behavior, including `millis()` rollover.

---

## 2. Battery Monitoring Removal

Battery level monitoring cannot be implemented because the project does not include a suitable battery monitoring component.

### Software changes
- [ ] Remove `MOOD_LOW_BATTERY` from `RobotState.h`.
- [ ] Remove `EVENT_POWER` and `PowerEvent` from `RobotEvents.h`.
- [ ] Remove battery driver initialization from `Robot.cpp`.
- [ ] Remove `PowerTask` creation from `Robot.cpp`, if it is only responsible for battery monitoring.
- [ ] Remove battery-related event handling from `MoodTask.cpp`.
- [ ] Remove battery-related OLED screens, icons and animations.
- [ ] Remove unused battery monitoring configuration parameters.
- [ ] Remove `BatteryDriver.cpp` and `BatteryDriver.h` if they are no longer used.
- [ ] Check for and remove any remaining references to battery monitoring.

---

## 3. OLED and Graphics

- [ ] Verify that all FSM states have a corresponding visual representation.
- [ ] Implement the startup screen displaying "Hello".
- [ ] Implement the happy, bored, cuddled, focused, angry and victory animations.
- [ ] Implement the calendar information screen.
- [ ] Implement the weather information screen.
- [ ] Verify display transitions and refresh behavior.

---

## 4. Locomotion

- [ ] Implement random movement in the `HAPPY` state.
- [ ] Stop the motors in states where movement is not required.
- [ ] Implement the rocking movement during `CUDDLED`.
- [ ] Verify motor direction and speed control.
- [ ] Test movement patterns and ensure safe operation on a desk.

---

## 5. Hardware and Sensors

- [ ] Verify TTP223 touch detection.
- [ ] Test single, double and triple tap recognition.
- [ ] Test long-press recognition for petting.
- [ ] Verify MPU6050 shake and lift detection.
- [ ] Test both DRV8833 motor drivers.
- [ ] Verify OLED communication over I2C.
- [ ] Test the complete power supply circuit.

---

## 6. Connectivity and External Services

- [ ] Verify Wi-Fi connectivity.
- [ ] Implement or complete PC communication.
- [ ] Verify focus session configuration from the PC interface.
- [ ] Implement calendar reminder reception.
- [ ] Implement weather information retrieval.
- [ ] Test communication failure and reconnection handling.

---

## 7. Integration and Final Testing

- [ ] Verify communication between FreeRTOS tasks.
- [ ] Check shared resource access and synchronization.
- [ ] Verify event queue behavior.
- [ ] Test the complete robot with all hardware components connected.
- [ ] Check memory usage and task stack sizes.
- [ ] Perform a complete functional test based on the project specification.
- [ ] Update the README with the final features and project architecture.