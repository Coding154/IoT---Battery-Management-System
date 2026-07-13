# IoT Battery Management System (BMS) 

## Overview
This project is a production-grade, event-driven Battery Management System (BMS) designed for a simulated 4-cell lithium battery pack. Built on an ESP32 microcontroller using a fully non-blocking architecture, the system monitors real-time voltages, manages fault states autonomously, and streams telemetry to an enterprise cloud dashboard.

##  Key Features & Modules
This integrated system successfully implements six core automotive/IoT engineering tasks:

1. Adaptive Multi-Cell Engine: Real-time ADC voltage mapping (2.5V - 4.2V) and dynamic imbalance calculations.
2. Safety Protection Kernel: Fully non-blocking (`millis()`-based) hardware control for relay cutoffs (LEDs) and pulsating audio alarms (Buzzer).
3. Intelligent Embedded HMI: Flicker-free I2C LCD rendering that rotates diagnostic screens and enforces fault-priority overrides during critical failures.
4. Fault-Tolerant Runtime: Autonomous state machine transitioning between `NORMAL`, `DEGRADED`, `FAILSAFE`, and `SHUTDOWN` states.
5. Intelligent Cloud Telemetry: Event-driven IoT syncing via Blynk, optimizing bandwidth by transmitting data only during state changes or anomalies.
6. Executive Dashboard: A live Blynk web dashboard featuring gauges, historical voltage charts, and real-time system health indicators.

##  Hardware Architecture (Wokwi Simulation)
* Microcontroller: ESP32 (Standard)
* Sensors: 4x Slide Potentiometers (Simulating Cell 1-4 Voltages on ADC1 pins)
* HMI: 20x4 I2C LCD Display
* Actuators: 2x LEDs (Simulating Main & Pre-charge Contactors)
* Alarms: 1x Piezo Buzzer

##  Software Architecture
To maintain clean, scalable, and modular code, the project is divided into distinct header files:
* `main.ino`: The main time-sliced execution loop running at 100ms, 250ms, 500ms, and 1000ms intervals.
* `BatteryEngine.h`: Math logic for converting raw ADC values to voltages and calculating pack imbalance.
* `FaultManager.h`: The diagnostic brain that assesses health and sets the system state.
* `SafetyKernel.h`: Physical hardware triggers for alarms and relay disconnects.
* `HMI_Display.h`: LCD screen routing and fault-screen overrides.
* `Telemetry.h`: Asynchronous Blynk cloud communication payload construction.

##  How to Run & Test
1. Open the https://wokwi.com/projects/469371232388349953 
2. Start the simulation. The system will boot and connect to the Wokwi-GUEST WiFi.
3. Observe the LCD rotating through normal operational screens.
4. Trigger a Fault: Slide any of the 4 potentiometers down to drop a cell voltage below 2.8V.
5. Observe the Reaction:
   * The LCD instantly snaps to a **CRITICAL FAULT** screen.
   * The Relay LEDs turn off (power cutoff).
   * The buzzer sounds a pulsating 1000Hz alarm.
   * The Serial Monitor logs a "Cloud Sync" anomaly.
   * The Blynk Cloud Dashboard updates the gauges and status LED in real-time.
  
##  IoT Dashboard Preview

Normal Operation State:
![Normal Dashboard](dashboard%201%20.png)

Critical Fault State (Simulated Cell Failure):
![Fault Dashboard](Dashboard%202.png)
