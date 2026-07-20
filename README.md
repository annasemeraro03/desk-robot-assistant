# Desk Robot Assistant

An interactive desktop robotic assistant based on the Seeed Studio XIAO ESP32-S3, integrating sensors, actuators, and interactive features.

## Overview

DeskRobotAssistant is an embedded robotics project that aims to develop a compact desktop robot capable of interacting with users through hardware components and real-time control.

The system is built around the **Seeed Studio XIAO ESP32-S3** microcontroller and combines different electronic components to manage sensors, actuators, and user interaction features.

## Features

- Real-time embedded control
- Sensor integration

## Hardware

Main components:

- **Microcontroller:**
  - Seeed Studio XIAO ESP32-S3

- **Motors and Motor Control:**
  - 4x N20 Gear Motors
  - 2x DRV8833 Motor Driver

- **Sensors:**
  - TTP223 Touch Sensor
  - MPU6050 Gyroscope and Accelerometer

- **Display:**
  - 1.3" OLED Display (SH1106)

- **Power Management:**
  - TP4056 LiPo Battery Charging Module
  - 3.7V LiPo Battery 1000mAh
  - MT3608 Booster Module
  - 4mm Slide Switch

- **Other Components:**
  - Jumper Wires

## Software

The firmware is developed using:

- PlatformIO
- Arduino framework
- C/C++

## Project Structure