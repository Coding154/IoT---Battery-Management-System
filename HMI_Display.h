#ifndef HMI_DISPLAY_H
#define HMI_DISPLAY_H

#include <LiquidCrystal_I2C.h>
#include "FaultManager.h" // We need access to bmsData and currentState

// Initialize the LCD at I2C address 0x27 with 20 columns and 4 rows
LiquidCrystal_I2C lcd(0x27, 20, 4);

int currentScreen = 0;
int lastScreen = -1; // To track changes and avoid flickering

void setupHMI() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("BMS Booting Up...");
  delay(1000); // This is the ONLY time a delay is acceptable (during boot)
  lcd.clear();
}

void updateHMI() {
  // 1. FAULT PRIORITY OVERRIDE
  // If the system is failing, force the warning screen and stop normal rotation
  if (currentState == FAILSAFE || currentState == SHUTDOWN) {
    if (lastScreen != 99) { // 99 is our special fault screen ID
      lcd.clear();
      lastScreen = 99;
    }
    lcd.setCursor(0, 0);
    lcd.print("!!! CRITICAL FAULT !!!");
    lcd.setCursor(0, 1);
    if (currentState == SHUTDOWN) lcd.print("SENSOR DISCONNECTED ");
    else lcd.print("VOLTAGE OUT OF BOUND");
    lcd.setCursor(0, 3);
    lcd.print("SYSTEM HALTED       ");
    return; // Exit the function here so it doesn't draw the normal screens
  }

  // 2. NORMAL SCREEN ROTATION
  // If we just recovered from a fault, or naturally rotating, clear the screen once
  if (lastScreen == 99 || lastScreen != currentScreen) {
    lcd.clear();
    lastScreen = currentScreen;
  }

  // Draw Screen 0: Cells 1 & 2
  if (currentScreen == 0) {
    lcd.setCursor(0, 0); lcd.print("--- CELL VOLTAGES ---");
    lcd.setCursor(0, 1); lcd.print("Cell 1: "); lcd.print(bmsData.cellVoltages[0]); lcd.print("V");
    lcd.setCursor(0, 2); lcd.print("Cell 2: "); lcd.print(bmsData.cellVoltages[1]); lcd.print("V");
  } 
  // Draw Screen 1: Cells 3 & 4 + Imbalance
  else if (currentScreen == 1) {
    lcd.setCursor(0, 0); lcd.print("--- CELL VOLTAGES ---");
    lcd.setCursor(0, 1); lcd.print("Cell 3: "); lcd.print(bmsData.cellVoltages[2]); lcd.print("V");
    lcd.setCursor(0, 2); lcd.print("Cell 4: "); lcd.print(bmsData.cellVoltages[3]); lcd.print("V");
    lcd.setCursor(0, 3); lcd.print("Imb: "); lcd.print(bmsData.imbalance); lcd.print("V");
  }
  // Draw Screen 2: System Health
  else if (currentScreen == 2) {
    lcd.setCursor(0, 0); lcd.print("--- SYSTEM STATUS ---");
    lcd.setCursor(0, 1); lcd.print("Pack: "); lcd.print(bmsData.packVoltage); lcd.print("V");
    lcd.setCursor(0, 2); 
    lcd.print("Health: ");
    if (currentState == NORMAL) lcd.print("HEALTHY ");
    else if (currentState == DEGRADED) lcd.print("DEGRADED");
  }

  // Rotate to the next screen for the next time this function is called
  currentScreen++;
  if (currentScreen > 2) {
    currentScreen = 0; // Loop back to the first screen
  }
}

#endif