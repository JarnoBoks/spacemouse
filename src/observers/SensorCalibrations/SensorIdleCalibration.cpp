#include "SensorIdleCalibration.hpp"
#include "sensor/sensors/Sensor.hpp" // For Sensor class

#include <visitors/printers/SensorIdleCalibrationResultPrinter.hpp> // For IdlePositionPrinter class

#include <sensor/calibrator/states/CalibratorStateBase.hpp> // For CalibratorStateBase class

#include <common/freeRAM.h>
#include <common/esp_print.h>

/**
 * Example:
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

SensorIdleCalibration::SensorIdleCalibration(ICalibratorState *calibratorState) {

    m_CalibratorState = calibratorState; // Set the calibrator state

    for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
        m_sumReads[id] = 0;
        m_minIdleValue[id] = 4095; // Initialize minimum idle value to maximum possible value (4095 is max for ESP, 1023 is max for Arduino).
        m_maxIdleValue[id] = 0;    // Initialize maximum idle value to minimum possible value
    }
}

/**
 * @brief   Finish the Idle calibration
 * @details This function is called to finish the idle calibration process.
 *          It calculates the average position for each sensor and prints the calibration results.
 *          It also checks for any warnings that occurred during the calibration process.
 * @param sensorCollection Pointer to the SensorCollection
 */
void SensorIdleCalibration::_finishCalibration(IObservable *sensorCollection) {

    SensorIdleCalibrationResultPrinter printer;

    // Calculating average position by dividing the sum of all readings by the number of iterations
    for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {

        // Calculate the idle position for the sensor
        const int idlePosition = m_sumReads[id] / m_processedIterations; // Average value of the sensor readings

        // Calculate the deadzone for the sensor
        const uint8_t lowDZ = idlePosition - m_minIdleValue[id];      // Idle - lowest reading (fe. 721 - 719 )
        const uint8_t highDZ = m_maxIdleValue[id] - idlePosition;     // Highest reading - Idle (fe 726 - 721)
        const uint8_t sensorDZ = ((lowDZ > highDZ) ? lowDZ : highDZ); // Use the larger of the two deadzones

        // Update the maximum deadzone seen for all the sensors
        m_maxDeadZone = (sensorDZ > m_maxDeadZone) ? sensorDZ : m_maxDeadZone;

        // Update the idlePosition for the sensor (returns true if the idle position is in the predefined normal zone)
        Sensor *sensor = static_cast<SensorCollection *>(sensorCollection)->getSensor(id); // Get the sensor from the collection

        const bool positionWarning = !(sensor->setIdlePosition(m_sumReads[id] / m_processedIterations));
        const bool deadzoneWarning = !sensor->setDeadzone(sensorDZ); // Set the deadzone for the sensor

        // Secondary check: Check if the deadzone is above the warning threshold.
        const bool m_warningsOccurred = m_warningsOccurred || positionWarning || deadzoneWarning;

        printer.setPrintParams(m_minIdleValue[id], m_maxIdleValue[id]); // Set the print parameters for the printer visitor
        sensor->accept(printer);                                        // Accept the printer visitor to print the information for this sensor
    }
}

/**
 * @brief   Update the Sensor Idle calibration
 * @param   sensorCollection Pointer to the collection of sensors being observed
 * @details This function reads the raw values for each sensor and updates the sum of reads, minimum and maximum values.
 *          It also checks if the requested number of iterations has been reached and calls the finalizer.
 */
void SensorIdleCalibration::update(IObservable *sensorCollection) {

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

    // Notify the calibration manager that an update has been processed
    m_CalibratorState->update();
}
