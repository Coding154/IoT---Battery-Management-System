#ifndef BATTERY_ENGINE_H
#define BATTERY_ENGINE_H

// Define the ADC pins we wired up
const int CELL1_PIN = 32;
const int CELL2_PIN = 33;
const int CELL3_PIN = 34;
const int CELL4_PIN = 35;

// This container holds all our live battery data
struct BatteryData {
  float cellVoltages[4];
  float packVoltage;
  float packAverage;
  float maxVoltage;
  float minVoltage;
  float imbalance;
  int strongestCell;
  int weakestCell;
};

// Create a global instance of this data that other files can read
BatteryData bmsData;

void readBatterySensors() {
  // 1. Read raw ADC values (0 to 4095)
  int rawADC[4];
  rawADC[0] = analogRead(CELL1_PIN);
  rawADC[1] = analogRead(CELL2_PIN);
  rawADC[2] = analogRead(CELL3_PIN);
  rawADC[3] = analogRead(CELL4_PIN);

  // Reset pack voltage to 0 before calculating
  bmsData.packVoltage = 0.0;
  bmsData.maxVoltage = 0.0;
  bmsData.minVoltage = 5.0; // Start high so real voltages overwrite it

  // 2. Convert ADC to simulated Voltage (let's say 2.5V to 4.2V per cell)
  for (int i = 0; i < 4; i++) {
    // Map the 0-4095 range to a 2.5 - 4.2 float range
    bmsData.cellVoltages[i] = 2.5 + ((float)rawADC[i] / 4095.0) * (4.2 - 2.5);
    
    // Add to total pack voltage
    bmsData.packVoltage += bmsData.cellVoltages[i];

    // 3. Find Strongest (Max) and Weakest (Min) Cells
    if (bmsData.cellVoltages[i] > bmsData.maxVoltage) {
      bmsData.maxVoltage = bmsData.cellVoltages[i];
      bmsData.strongestCell = i + 1;
    }
    if (bmsData.cellVoltages[i] < bmsData.minVoltage) {
      bmsData.minVoltage = bmsData.cellVoltages[i];
      bmsData.weakestCell = i + 1;
    }
  }

  // 4. Calculate Pack Averages and Imbalance
  bmsData.packAverage = bmsData.packVoltage / 4.0;
  bmsData.imbalance = bmsData.maxVoltage - bmsData.minVoltage; // [cite: 41]
}

#endif