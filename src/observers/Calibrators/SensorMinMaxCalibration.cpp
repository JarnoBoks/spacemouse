#include "SensorMinMaxCalibration.h"

#include "sensor/SensorCollection.hpp"                           // For SensorCollection class
#include "sensor/sensors/Sensor.hpp"                             // For Sensor class
#include "sensor/config/SensorConfig.h"                          // For SensorConfig class
#include "Sensor/calibration/SensorCalibrationManagerMinMax.hpp" // For SensorCalibrationManager class

#include "visitors/MinMaxPrinter.h" // For MinMaxPrinter class

#define MINMAXDURATION 15 // Duration for min/max calibration in seconds

SensorMinMaxCalibration::SensorMinMaxCalibration(SensorCalibrationManager *calmgr)
    : m_CalibrationManager(calmgr) {
    m_startCalibrationTime = millis();

    // Initialize the calibration process    // Initialize the calibration process
    Serial.print(F("Move the spacemouse for "));
    Serial.print(MINMAXDURATION);
    Serial.println(F(" sec."));
}

void SensorMinMaxCalibration::finish(SensorCollection *sensorCollection) {
    bool warningsOccurred = false; // Flag to track if any warnings occurred during calibration

    MinMaxPrinter printer;

    // REVIEW - Should this loop be moved to hardware?
    for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {

        Sensor *sensor = sensorCollection->getSensor(id);
        if (sensor == nullptr) {
            continue; // Skip if the sensor is not available
        }

        SensorConfig *sensorcfg = sensor->getConfig(); // Get the sensor configuration to update
        sensorcfg->setMin(m_minValue[id]);             // Set the minimum value in the sensor configuration
        sensorcfg->setMax(m_maxValue[id]);             // Set the maximum value in the sensor configuration

        // Save the sensor configuration to EEPROM
        sensorcfg->persist(sensor->getId()); // Save the updated configuration to EEPROM

        sensor->accept(printer);    // Accept the printer visitor to print the sensor name
        sensorcfg->accept(printer); // Accept the printer visitor to print the sensor configuration values
    }

    m_CalibrationManager->deactivate(warningsOccurred); // Finish the calibration process
}

void SensorMinMaxCalibration::update(SensorCollection *sensorCollection) {
    // Finish the calibration process if the configured time has elapsed iterations are reached
    if (millis() - m_startCalibrationTime > (MINMAXDURATION * 1000)) {
        finish(sensorCollection); // Finish the calibration process
        return;
    }

    for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
        Sensor *sensor = sensorCollection->getSensor(id);
        if (sensor == nullptr) {
            continue; // Skip if the sensor is not available
        }

        // Update the minimum and maximum values
        const int centeredVal = sensor->getCntValue(); // Get the centered value from the sensor

        m_minValue[id] = (centeredVal < m_minValue[id]) ? centeredVal : m_minValue[id]; // Update the minimum value
        m_maxValue[id] = (centeredVal > m_maxValue[id]) ? centeredVal : m_maxValue[id]; // Update the maximum value
    }
}