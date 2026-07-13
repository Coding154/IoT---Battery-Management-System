#ifndef SAFETY_KERNEL_H
#define SAFETY_KERNEL_H

#include "FaultManager.h" // We need access to the currentState

// Define the output pins
const int RELAY1_PIN = 2;  // Red LED
const int RELAY2_PIN = 4;  // Green LED
const int BUZZER_PIN = 15;

// Variables for the non-blocking buzzer
unsigned long previousBuzzerMillis = 0;
bool buzzerState = false;

void setupSafetyKernel() {
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Start with power flowing normally (Relays closed/ON)
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
}

void executeSafetyKernel() {
  if (currentState == NORMAL || currentState == DEGRADED) {
    // Safe states: Relays stay closed, silence the buzzer
    digitalWrite(RELAY1_PIN, HIGH);
    digitalWrite(RELAY2_PIN, HIGH);
    noTone(BUZZER_PIN); // Turn off any active tones
  } 
  else if (currentState == FAILSAFE || currentState == SHUTDOWN) {
    // Critical states: Open relays to cut the power!
    digitalWrite(RELAY1_PIN, LOW);
    digitalWrite(RELAY2_PIN, LOW);
    
    // Non-blocking pulsating audio alarm
    unsigned long currentMillis = millis();
    if (currentMillis - previousBuzzerMillis >= 500) {
      previousBuzzerMillis = currentMillis;
      buzzerState = !buzzerState; // Toggle the state
      
      // Play or stop the tone based on the toggled state
      if (buzzerState) {
        tone(BUZZER_PIN, 1000); // Play a 1000 Hz beep
      } else {
        noTone(BUZZER_PIN);     // Silence the beep
      }
    }
  }
}

#endif