#include "SensorIdleCalibration.h"
#include "sensor/sensors/Sensor.hpp" // For Sensor class
#include "sensor/calibration/SensorCalibrationManager.hpp"
#include <printervisitors/IdlePositionPrinter.h> // For IdlePositionPrinter class

#include "common/freeRAM.h"

#define DEADZONEWARNING 10 // Define a threshold for dead zone warning
// NOTE - At the moment the dead zone warning threshold is non hardware type specific. This could be changed in the future.

/**
 * @brief Constructor for SensorIdleCalibration class *
 * @param calmgr Pointer to the SensorCalibrationManager
 * @param numiterations Number of iterations for calibration
 */
SensorIdleCalibration::SensorIdleCalibration(SensorCalibrationManager *calmgr, const int numiterations)
    : m_requestedIterations(numiterations), m_CalibrationManager(calmgr) {

    m_startCalibrationTime = millis(); // Store the start time of the calibration process
    Serial.println(F("Starting calibration..."));

    for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
        m_sumReads[id] = 0;
        m_minIdleValue[id] = 1023;
        m_maxIdleValue[id] = 0;
    }
}

/**
 * @brief Finish the Idle calibration
 * @param hardware Pointer to the Hardware instance
 * @details This function calculates the average position by dividing the sum of all readings by the number of iterations.
 */
void SensorIdleCalibration::finish(IObservable *sensorCollection) {

    IdlePositionPrinter printer;

    // Calculating average position by dividing the sum of all readings by the number of iterations
    for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {

        // Calculate the dead zone for the sensor
        int sensorDZ = m_maxIdleValue[id] - m_minIdleValue[id];

        // Update the maximum dead zone seen for all sensors if necessary
        m_maxDeadZone = (sensorDZ > m_maxDeadZone) ? sensorDZ : m_maxDeadZone;

        // Update the idlePosition for each sensor (returns true if the idle position is in the predefined normal zone)
        // Secondary check: Check if the dead zone is above the warning threshold.
        Sensor *sensor = static_cast<SensorCollection *>(sensorCollection)->getSensor(id); // Get the sensor from the collection
        bool positionWarning = !(sensor->setIdlePosition(m_sumReads[id] / m_processedIterations));
        m_warningsOccurred = m_warningsOccurred || positionWarning || (sensorDZ > DEADZONEWARNING);

        printer.setPrintParams(m_minIdleValue[id], m_maxIdleValue[id], sensorDZ); // Set the print parameters for the printer visitor
        sensor->accept(printer);                                                  // Accept the printer visitor to print the information for this sensor
    }

    // Output the calibration process information
    Serial.println(F("Calibration finished!"));
    Serial.print(F("Took "));
    Serial.print(millis() - m_startCalibrationTime); // Print the time taken for calibration
    Serial.println(F(" ms for "));
    Serial.print(m_processedIterations); // Print the number of processed iterations
    Serial.println(F(" iterations."));

    // Notify the creator of this observer so it can be deleted.
    m_CalibrationManager->deactivate(m_warningsOccurred); // Finish the calibration process

} // Finish calibration process

/**
 * @brief Update the sensor calibration process
 * @details This function is called to update the calibration process.
 *          It reads the raw values from the sensors and updates the sum of reads, minimum and maximum values.
 * @param hardware Pointer to the Hardware instance
 */
void SensorIdleCalibration::update(IObservable *sensorCollection) {
    // Finish the calibration process if the requested iterations are reached
    if (m_processedIterations >= m_requestedIterations) {
        finish(sensorCollection); // Finish the calibration process
        return;
    }

    for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
        // Get the sensor by ID
        Sensor *sensor = static_cast<SensorCollection *>(sensorCollection)->getSensor(id);
        if (sensor == nullptr) {
            continue; // Skip if the sensor is not available
        }

        // Update the mean value (= Idle position) for the sensor
        int _rawValue = sensor->getRawValue();
        m_sumReads[id] += _rawValue;

        // Update the minimum and maximum values for deadzone evaluation
        m_minIdleValue[id] = (_rawValue < m_minIdleValue[id]) ? _rawValue : m_minIdleValue[id];
        m_maxIdleValue[id] = (_rawValue > m_maxIdleValue[id]) ? _rawValue : m_maxIdleValue[id];
    }

    m_processedIterations++; // Increment the number of processed iterations
}