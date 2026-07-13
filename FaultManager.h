#ifndef FAULT_MANAGER_H
#define FAULT_MANAGER_H

#include "BatteryEngine.h" // We need access to the bmsData

// Define our four system states
enum SystemState { NORMAL, DEGRADED, FAILSAFE, SHUTDOWN };
SystemState currentState = NORMAL;

void evaluateSystemHealth() {
  // Always default to NORMAL at the start of each check
  currentState = NORMAL;

  // 1. Check for SHUTDOWN (Disconnected sensor / 0V)
  if (bmsData.minVoltage < 0.1) {
    currentState = SHUTDOWN;
    return; // Halt further checks
  }

  // 2. Check for FAILSAFE (Critical under-voltage or over-voltage)
  // Let's say a cell drops below 2.8V or spikes above 4.25V
  if (bmsData.minVoltage < 2.8 || bmsData.maxVoltage > 4.25) {
    currentState = FAILSAFE;
    return; 
  }

  // 3. Check for DEGRADED (Minor Imbalance)
  // If the difference between the highest and lowest cell is > 0.2V
  if (bmsData.imbalance > 0.2) {
    currentState = DEGRADED;
  }
}

#endif