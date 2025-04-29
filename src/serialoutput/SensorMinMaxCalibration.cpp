#include "SensorMinMaxCalibration.h"
#include "hardware/hardware.h"   // For Hardware class - necessary to retrieve the sensors.
#include "sensor/sensor.h"       // For Sensor class
#include "sensor/sensorconfig.h" // For SensorConfig class
#include "calibration/sensorcalibrationmanager.h"

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

    Serial.println(F("        Min |  Max | Range | Warning"));

    for (uint8_t id = 0; id < MAX_SENSORS; id++) {

        Sensor *sensor = hardware->sensors[id]; // Pointer to the sensor
        if (sensor == nullptr) {
            continue; // Skip if the sensor is not available
        }

        // Initialize the flags for min, max and working range warnings
        bool minWarning = false, maxWarning = false, rangeWarning = false;

        SensorConfig *sensorcfg = sensor->getConfig(); // Get the sensor configuration to update
#if 0
        const int min = sensorcfg->getMin(&minWarning);       // Get the minimum value from the sensor configuration
        const int max = sensorcfg->getMax(&maxWarning);       // Get the maximum value from the sensor configuration
#endif
        sensorcfg->setMin(minValue[id], &minWarning);         // Set the minimum value in the sensor configuration
        sensorcfg->setMax(maxValue[id], &maxWarning);         // Set the maximum value in the sensor configuration
        const int range = sensorcfg->getRange(&rangeWarning); // Calculate the working range from the sensor configuration

        // Print the value of the min, max and working range for each sensor
        Serial.print(sensor->getName());
        Serial.print(F(":  "));
        alignValue(minValue[id], 4);
        Serial.print(minValue[id]);
        Serial.print(F(" | "));
        alignValue(maxValue[id], 4);
        Serial.print(maxValue[id]);
        Serial.print(F(" | "));
        alignValue(range, 4);
        Serial.print(range);
        Serial.print(F("  | "));

        // TODO - Program more efficiently
        // Print the warning status for min, max and working range
        if (minWarning) {
            Serial.print(F("Min "));
        }
        warningsOccurred = warningsOccurred || minWarning; // Set the warning status if any of the conditions are met
        if (maxWarning) {
            if (warningsOccurred)
                Serial.print(F(", "));

            Serial.print(F("Max "));
        }
        warningsOccurred = warningsOccurred || maxWarning; // Set the warning status if any of the conditions are met

        if (rangeWarning) {
            if (warningsOccurred)
                Serial.print(F(", "));

            Serial.print(F("Range "));
        }
        warningsOccurred = warningsOccurred || rangeWarning; // Set the warning status if any of the conditions are met

        if (warningsOccurred) {
            Serial.print(F("small"));
        } else {
            Serial.print(F("ok"));
        }

        Serial.println();

        // Save the sensor configuration to EEPROM
        sensorcfg->saveSensorConfig(sensor->getId()); // Save the updated configuration to EEPROM
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

#if 0
        SensorConfig *sensorcfg = sensor->getConfig();
        sensorcfg->updateMin(centeredVal);
        sensorcfg->updateMax(centeredVal);
#endif
    }
}