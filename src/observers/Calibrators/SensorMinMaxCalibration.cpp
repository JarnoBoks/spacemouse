#include "SensorMinMaxCalibration.hpp"

#include <sensor/SensorCollection.hpp>
// #include <sensor/sensors/Sensor.hpp>
// #include <sensor/config/SensorConfig.hpp>
#include <sensor/calibration/SensorCalibrationManagerMinMax.hpp>

#include <visitors/SensorUpdateMinMaxVisitor.hpp>
#include <visitors/printers/SensorConfigMinMaxPrinter.hpp>

#define MINMAXDURATION 15 // Duration for min/max calibration in seconds

SensorMinMaxCalibration::SensorMinMaxCalibration(SensorCalibrationManager *calmgr)
    : m_CalibrationManager(calmgr) {
    for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
        m_minValue[id] = 1023;  // Initialize minimum values to maximum possible value (1023)
        m_maxValue[id] = -1023; // Initialize maximum values to minimum possible value (-1023)
    }

    _startCalibration(); // Call the initialize function to start the calibration process
}

void SensorMinMaxCalibration::_startCalibration() {
    // Initialize the calibration process
    m_startCalibrationTime = millis();
    Serial.print(F("Move the spacemouse for "));
    Serial.print(MINMAXDURATION);
    Serial.println(F(" sec."));
}

void SensorMinMaxCalibration::_finishCalibration(IObservable *Observable) {
    bool warningsOccurred = false; // Flag to track if any warnings occurred during calibration

    SensorCollection *sensorCollection = static_cast<SensorCollection *>(Observable);
    if (!sensorCollection) {
        ESP_ERROR("Sensor collection is null");
        return;
    }

    // Store the minimum and maximum values in the sensor configurations of each sensor
    SensorUpdateMinMaxVisitor visitor(m_minValue, m_maxValue, true);
    sensorCollection->accept(visitor);

    SensorConfigMinMaxPrinter printer;
    sensorCollection->accept(printer);

#if 0 // REMOVE - This is not needed anymore, the Printer visitor does this
    // REVIEW - Should this loop be moved to the sensorcollection?
    for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {

        Sensor *sensor = static_cast<SensorCollection *>(sensorCollection)->getSensor(id);
        if (!sensor) {
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
#endif
    m_CalibrationManager->deactivate(warningsOccurred); // Finish the calibration process
}

void SensorMinMaxCalibration::update(IObservable *sensorCollection) {

    // Finish the calibration process if the configured time has elapsed iterations are reached
    if (millis() - m_startCalibrationTime > (MINMAXDURATION * 1000)) {
        _finishCalibration(sensorCollection); // Finish the calibration process
        return;
    }

    for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
        Sensor *sensor = static_cast<SensorCollection *>(sensorCollection)->getSensor(id); // Get the sensor from the collection
        if (sensor == nullptr) {
            continue; // Skip if the sensor is not available
        }

        // Update the minimum and maximum values
        const int centeredVal = sensor->getCntValue(); // Get the centered value from the sensor

        m_minValue[id] = (centeredVal < m_minValue[id]) ? centeredVal : m_minValue[id]; // Update the minimum value
        m_maxValue[id] = (centeredVal > m_maxValue[id]) ? centeredVal : m_maxValue[id]; // Update the maximum value
    }
}