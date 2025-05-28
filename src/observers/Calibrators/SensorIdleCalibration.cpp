#include "SensorIdleCalibration.hpp"
#include "sensor/sensors/Sensor.hpp" // For Sensor class
#include "sensor/calibration/SensorCalibrationManager.hpp"
#include <visitors/printers/SensorIdleCalibrationResultPrinter.hpp> // For IdlePositionPrinter class

#include <common/freeRAM.h>
#include <common/esp_print.h>

// REFACTOR - Warninglevel should be set in the SensorConfig class, not here
#define DEADZONEWARNING 10 // Define a threshold for dead zone warning
// NOTE - At the moment the dead zone warning threshold is non hardware type specific. This could be changed in the future.

/**
 * @brief Constructor for SensorIdleCalibration class
 * @param calmgr Pointer to the SensorCalibrationManager
 * @param numiterations Number of iterations for calibration
 */
SensorIdleCalibration::SensorIdleCalibration(SensorCalibrationManager *calmgr, const int numiterations)
    : m_requestedIterations(numiterations), m_CalibrationManager(calmgr) {

    for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
        m_sumReads[id] = 0;
        m_minIdleValue[id] = 1023;
        m_maxIdleValue[id] = 0;
    }

    _startCalibration(); // Call the initialize function to start the calibration process
}

/**
 * @brief Initialize the Idle calibration
 * @details This function is called to initialize the idle calibration process.
 */
void SensorIdleCalibration::_startCalibration() {
    m_startCalibrationTime = millis(); // Store the start time of the calibration process
    Serial.println(F("Starting calibration..."));
}

/**
 * @brief Finish the Idle calibration
 * @details This function is called to finish the idle calibration process.
 *          It calculates the average position for each sensor and prints the calibration results.
 *          It also checks for any warnings that occurred during the calibration process.
 * @param sensorCollection Pointer to the SensorCollection
 */
void SensorIdleCalibration::_finishCalibration(IObservable *sensorCollection) {

    SensorIdleCalibrationResultPrinter printer;

    // Calculating average position by dividing the sum of all readings by the number of iterations
    for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
        if (m_processedIterations == 0) {
            ESP_ERROR("Iterations = 0");
            continue; // Skip if no iterations were processed
        }

        // Calculate the idle position for the sensor
        const int idlePosition = m_sumReads[id] / m_processedIterations;

        // Calculate the deadzone for the sensor
        const int lowDZ = idlePosition - m_minIdleValue[id];          // Idle - lowest reading
        const int highDZ = m_maxIdleValue[id] - idlePosition;         // Highest reading - Idle
        const int sensorDZ = ((lowDZ > highDZ) ? lowDZ : highDZ) + 2; // Use the larger of the two deadzones and add failsafe value

        // Update the maximum deadzone seen for all the sensors if necessary
        m_maxDeadZone = (sensorDZ > m_maxDeadZone) ? sensorDZ : m_maxDeadZone;

        // Update the idlePosition for each sensor (returns true if the idle position is in the predefined normal zone)
        Sensor *sensor = static_cast<SensorCollection *>(sensorCollection)->getSensor(id); // Get the sensor from the collection
        const bool positionWarning = !(sensor->setIdlePosition(m_sumReads[id] / m_processedIterations));

        // Secondary check: Check if the deadzone is above the warning threshold.
        const bool m_warningsOccurred = m_warningsOccurred || positionWarning || (sensorDZ > DEADZONEWARNING);

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
}

/**
 * @brief Update the sensor calibration process
 * @param sensorCollection Pointer to the collection of sensors being observed
 * @details This function reads the raw values for each sensor and updates the sum of reads, minimum and maximum values.
 *          It also checks if the requested number of iterations has been reached and calls the finalizer.
 */
void SensorIdleCalibration::update(IObservable *sensorCollection) {
    // Finish the calibration process if the requested iterations are reached
    if (m_processedIterations >= m_requestedIterations) {
        _finishCalibration(sensorCollection); // Finish the calibration process
        return;
    }

    for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
        // Get the sensor by ID
        Sensor *sensor = static_cast<SensorCollection *>(sensorCollection)->getSensor(id);
        if (!sensor) {
            continue; // Skip if the sensor is not available
        }

        // Add the current read value to the total of readings for the sensor
        int _rawValue = sensor->getRawValue();
        m_sumReads[id] += _rawValue;

        // Update the minimum and maximum values for deadzone evaluation
        m_minIdleValue[id] = (_rawValue < m_minIdleValue[id]) ? _rawValue : m_minIdleValue[id];
        m_maxIdleValue[id] = (_rawValue > m_maxIdleValue[id]) ? _rawValue : m_maxIdleValue[id];
    }

    m_processedIterations++; // Increment the number of processed iterations
}

/**
 * The knob is in Idle position, so optimally the raw value should allways be around 512 and stable.
 * Practically this is not the case and the raw value won't be exactly in the middle of the range
 * and will fluctuate around the 'idle' value.
 *
 * Let's visualize this with an example:
 *
 * #    read value       sum of reads     min idle value     max idle value
 * 0    500              500               500                500
 * 1    520              1020              500                520
 * 2    510              1530              500                520
 * 3    530              2060              500                530
 *
 * The values are fluctuating between 500 and 530, so the average value is 510.
 * The minimum idle value is 500 and the maximum idle value is 530.
 * The deadzone is 30 (530 - 500).
 */
