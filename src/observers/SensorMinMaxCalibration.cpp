#include "SensorMinMaxCalibration.h"
#include "hardware/hardware.h"          // For Hardware class - necessary to retrieve the sensors.
#include "sensor/sensor.hpp"            // For Sensor class
#include "sensor/config/SensorConfig.h" // For SensorConfig class
#include "calibration/sensorcalibrationmanager.h"
#include "visitors/MinMaxPrinter.h" // For MinMaxPrinter class

#define MINMAXDURATION 15 // Duration for min/max calibration in seconds

SensorMinMaxCalibration::SensorMinMaxCalibration(SensorCalibrationManager *calmgr)
    : CalibrationManager(calmgr) {
    startCalibrationTime = millis();

    // Initialize the calibration process    // Initialize the calibration process
    Serial.print(F("Move the spacemouse for "));
    Serial.print(MINMAXDURATION);
    Serial.println(F(" sec."));
}

void SensorMinMaxCalibration::finish(Hardware *hardware) {
    bool warningsOccurred = false; // Flag to track if any warnings occurred during calibration

    MinMaxPrinter printer;

    // REVIEW - Should this loop be moved to hardware?
    for (uint8_t id = 0; id < MAX_SENSORS; id++) {

        Sensor *sensor = hardware->sensors[id]; // Pointer to the sensor
        if (sensor == nullptr) {
            continue; // Skip if the sensor is not available
        }

        SensorConfig *sensorcfg = sensor->getConfig(); // Get the sensor configuration to update
        sensorcfg->setMin(minValue[id]);               // Set the minimum value in the sensor configuration
        sensorcfg->setMax(maxValue[id]);               // Set the maximum value in the sensor configuration

        // Save the sensor configuration to EEPROM
        sensorcfg->persist(sensor->getId()); // Save the updated configuration to EEPROM

        sensor->accept(printer);    // Accept the printer visitor to print the sensor name
        sensorcfg->accept(printer); // Accept the printer visitor to print the sensor configuration values
    }

    CalibrationManager->deactivateMinMaxCalibration(warningsOccurred); // Finish the calibration process
}

void SensorMinMaxCalibration::update(Hardware *hardware) {
    // Finish the calibration process if the configured time has elapsed iterations are reached
    if (millis() - startCalibrationTime > (MINMAXDURATION * 1000)) {
        finish(hardware); // Finish the calibration process
        return;
    }

    for (uint8_t id = 0; id < MAX_SENSORS; id++) {
        Sensor *sensor = hardware->sensors[id]; // Pointer to the sensor
        if (sensor == nullptr) {
            continue; // Skip if the sensor is not available
        }

        // Update the minimum and maximum values
        const int centeredVal = sensor->getCenteredValue(); // Get the centered value from the sensor

        minValue[id] = (centeredVal < minValue[id]) ? centeredVal : minValue[id]; // Update the minimum value
        maxValue[id] = (centeredVal > maxValue[id]) ? centeredVal : maxValue[id]; // Update the maximum value
    }
}