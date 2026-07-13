
#define BLYNK_PRINT Serial // <--- THIS TURNS ON THE LOGS


#ifndef TELEMETRY_H
#define TELEMETRY_H

// --- REPLACE WITH YOUR COPIED CREDENTIALS ---
#define BLYNK_TEMPLATE_ID "TMPL3owv0zBvh"
#define BLYNK_TEMPLATE_NAME "Battery Engine"
#define BLYNK_AUTH_TOKEN "5TKAKnoLDgDZ7ubmXb3vSmMvJXLwDVd0"
// --------------------------------------------

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "FaultManager.h" // We need access to bmsData and currentState

// Wokwi's simulated free WiFi network
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// Variables to track changes for our event-driven architecture
float last_sent_voltage = 0.0;
SystemState last_sent_state = NORMAL;

void setupTelemetry() {
  // Connect to the simulated WiFi and Blynk Cloud
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void executeTelemetry() {
  Blynk.run(); // Keeps the cloud connection alive

  // EVENT-DRIVEN RULE: 
  // Only transmit data if the pack voltage changes by > 0.05V, OR if the system state changes.
  if (abs(bmsData.packVoltage - last_sent_voltage) > 0.05 || currentState != last_sent_state) {
    
    // Send data to Blynk Virtual Pins (V1 through V6)
    Blynk.virtualWrite(V1, bmsData.cellVoltages[0]);
    Blynk.virtualWrite(V2, bmsData.cellVoltages[1]);
    Blynk.virtualWrite(V3, bmsData.cellVoltages[2]);
    Blynk.virtualWrite(V4, bmsData.cellVoltages[3]);
    Blynk.virtualWrite(V5, bmsData.packVoltage);
    Blynk.virtualWrite(V6, currentState); // Sends 0, 1, 2, or 3

    // Update our trackers
    last_sent_voltage = bmsData.packVoltage;
    last_sent_state = currentState;
    
    Serial.println("Cloud Sync: Anomaly/Change Detected. Data Transmitted.");
  }
}

#endif