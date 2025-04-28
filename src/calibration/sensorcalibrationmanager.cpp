#include "sensorcalibrationmanager.h"
#include "config.h"                             // For hardware type
#include "hardware/hardware.h"                  // For Hardware class - necessary to retrieve the sensors.
#include "hardware/hardware_hall.h"             // For Hardware class - necessary to retrieve the sensors.
#include "hardware/hardware_joystick.h"         // For Hardware class - necessary to retrieve the sensors.
#include "serialoutput/SensorIdleCalibration.h" // For SensorIdleCalibration class

#define DEADZONEWARNING 10 // Define a threshold for dead zone warning
// NOTE - At the moment the dead zone warning threshold is non hardware type specific. This should be changed in the future.

SensorCalibrationManager *SensorCalibrationManager::instance = nullptr;

SensorCalibrationManager *SensorCalibrationManager::getInstance() {
    if (!instance) {
        instance = new SensorCalibrationManager();
    }
    return instance;
}

void SensorCalibrationManager::startIdleCalibration(const int iterations) {
    if (currentCalibration != nullptr) {
        Serial.println(F("Calibration already in progress!")); // Print a message if calibration is already in progress
        return;                                                // Delete the previous calibration observer if it exists
    }

    // Attach the idle calibration observer to the hardware
    currentCalibration = new SensorIdleCalibration(this, iterations); // Create a new instance of the idle calibration class
    Hardware *hardware = HW_TYPE::getInstance();                      // Get the hardware instance
    // FIXME - Test if the observer has been attached successfully
    hardware->attachObserver(currentCalibration); // Attach the idle calibration observer to the hardware
}

void SensorCalibrationManager::finishIdleCalibration(const bool warningsOccurred) {
    Hardware *hardware = HW_TYPE::getInstance();  // Get the hardware instance
    hardware->detachObserver(currentCalibration); // Detach the idle calibration observer from the hardware
    delete currentCalibration;                    // Delete the idle calibration observer
    currentCalibration = nullptr;                 // Set the pointer to null
}

void SensorCalibrationManager::calibrateMinMax(Sensor *sensors[], uint8_t count) {
}

void SensorCalibrationManager::calibrateDeadzone(Sensor *sensor, const uint8_t deadzone) {
#if 0
    SensorConfig *config = sensor->getConfig(); // Get the configuration of the sensor
    config->setDeadzone(deadzone);              // Set the dead zone for the sensor
    config->saveSensorConfig(sensor->getId());  // Save the updated configuration to EEPROM
#endif
}
