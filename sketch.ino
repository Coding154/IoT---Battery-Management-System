#include "BatteryEngine.h"
#include "FaultManager.h"
#include "SafetyKernel.h"
#include "HMI_Display.h" 
#include "Telemetry.h" 


unsigned long previousMillis100 = 0;
unsigned long previousMillis250 = 0; 
unsigned long previousMillis500 = 0; // Timer for Cloud Telemetry
unsigned long previousMillis1000 = 0; 

void setup() {
  Serial.begin(115200);
  
  pinMode(CELL1_PIN, INPUT);
  pinMode(CELL2_PIN, INPUT);
  pinMode(CELL3_PIN, INPUT);
  pinMode(CELL4_PIN, INPUT);

 
  setupSafetyKernel();
  setupHMI(); 
  setupTelemetry(); 
}

void loop() {
  unsigned long currentMillis = millis();

  // Every 100ms: Read sensors and calculate metrics
  if (currentMillis - previousMillis100 >= 100) {
    previousMillis100 = currentMillis;
    readBatterySensors();
  }

  // Every 250ms: Check faults & run safety kernel
  if (currentMillis - previousMillis250 >= 250) {
    previousMillis250 = currentMillis;
    evaluateSystemHealth(); 
    executeSafetyKernel();  
  }

  // Every 500ms: Check if we need to send data to the cloud
  if (currentMillis - previousMillis500 >= 500) {
    previousMillis500 = currentMillis;
    executeTelemetry(); 
  }

  // Every 1000ms: Update the LCD HMI
  if (currentMillis - previousMillis1000 >= 1000) {
    previousMillis1000 = currentMillis;
    updateHMI(); 
  }
}