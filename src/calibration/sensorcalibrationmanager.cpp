#include "sensorcalibrationmanager.h"
#include "config.h"                     // For hardware type
#include "hardware/hardware.h"          // For Hardware class - necessary to retrieve the sensors.
#include "hardware/hardware_hall.h"     // For Hardware class - necessary to retrieve the sensors.
#include "hardware/hardware_joystick.h" // For Hardware class - necessary to retrieve the sensors.

#include "observers/SensorIdleCalibration.h"   // For SensorIdleCalibration class
#include "observers/SensorMinMaxCalibration.h" // For SensorMinMaxCalibration class

#ifndef CF
#define CF(x) ((const __FlashStringHelper *)x)
#endif

// FIXME - Add deadzone warning in the output.
#define DEADZONEWARNING 10 // Define a threshold for dead zone warning
// NOTE - At the moment the dead zone warning threshold is non hardware type specific. This should be changed in the future.

// Text messages for calibration
static const char Error_AlreadyCalibrating[] PROGMEM = "Calibration already in progress!"; // Error message for already calibrating

SensorCalibrationManager *SensorCalibrationManager::instance = nullptr;

SensorCalibrationManager *SensorCalibrationManager::getInstance() {
    if (!instance) {
        instance = new SensorCalibrationManager();
    }
    return instance;
}

// -------------------- IDLE ---------------------------------------------

// TODO - We can save some code space - lot of duplicate code in the two functions
void SensorCalibrationManager::activateIdleCalibration(const int iterations) {
    if (currentCalibration != nullptr) {
        Serial.println(CF(Error_AlreadyCalibrating)); // Print a message if calibration is already in progress
        return;
    }

    // Attach the idle calibration observer to the hardware
    currentCalibration = new SensorIdleCalibration(this, iterations); // Create a new instance of the idle calibration class
    Hardware::getInstance()->attachObserver(currentCalibration);      // Attach the idle calibration observer to the hardware
}

void SensorCalibrationManager::deactivateIdleCalibration(const bool warningsOccurred) {
    Hardware::getInstance()->detachObserver(currentCalibration); // Detach the idle calibration observer from the hardware
    delete currentCalibration;                                   // Delete the idle calibration observer
    currentCalibration = nullptr;                                // Set the pointer to null
}

// -------------------- MINMAX -------------------------------------------

void SensorCalibrationManager::activateMinMaxCalibration() {
    if (currentCalibration != nullptr) {
        Serial.println(CF(Error_AlreadyCalibrating)); // Print a message if calibration is already in progress
        return;
    }

    // Attach the MinMax calibration observer to the hardware
    currentCalibration = new SensorMinMaxCalibration(this);      // Create a new instance of the idle calibration class
    Hardware::getInstance()->attachObserver(currentCalibration); // Attach the idle calibration observer to the hardware
}

void SensorCalibrationManager::deactivateMinMaxCalibration(const bool warningsOccurred) {
    Hardware::getInstance()->detachObserver(currentCalibration);
    delete currentCalibration;
    currentCalibration = nullptr;
}

#if 0
void SensorCalibrationManager::calibrateDeadzone(Sensor *sensor, const uint8_t deadzone) {

    SensorConfig *config = sensor->getConfig(); // Get the configuration of the sensor
    config->setDeadzone(deadzone);              // Set the dead zone for the sensor
    config->saveInEEPROM(sensor->getId());  // Save the updated configuration to EEPROM

}
#endif
