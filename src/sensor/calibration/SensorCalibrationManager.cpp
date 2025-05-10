#include "SensorCalibrationManager.hpp"
#include "sensor/SensorCollection.hpp"
#include "observers/IObserver.hpp"

#include <Arduino.h> // For PROGMEM

#ifndef CF
#define CF(x) ((const __FlashStringHelper *)x)
#endif

// FIXME - Add deadzone warning in the output.
#define DEADZONEWARNING 10 // Define a threshold for dead zone warning
// NOTE - At the moment the dead zone warning threshold is non hardware type specific. This should be changed in the future.

// Text messages for calibration
static const char Error_AlreadyCalibrating[] PROGMEM = "Calibration already in progress!"; // Error message for already calibrating

void SensorCalibrationManager::activate() {
    if (currentCalibration != nullptr) {
        Serial.println(CF(Error_AlreadyCalibrating));
        return;
    }
}

void SensorCalibrationManager::deactivate(const bool warningsOccurred) {
    m_SensorCollection->detachObserver(currentCalibration);
    delete currentCalibration;
}

#if 0 // REVIEW - What to do with this code?
void SensorCalibrationManager::calibrateDeadzone(Sensor *sensor, const uint8_t deadzone) {

    SensorConfig *config = sensor->getConfig(); // Get the configuration of the sensor
    config->setDeadzone(deadzone);              // Set the dead zone for the sensor
    config->saveInEEPROM(sensor->getId());  // Save the updated configuration to EEPROM

}
#endif
