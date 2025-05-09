#include "SensorIdleCalibration.h"
#include "hardware/hardware.h"       // For Hardware class - necessary to retrieve the sensors.
#include "sensor/sensors/Sensor.hpp" // For Sensor class
#include "calibration/sensorcalibrationmanager.h"
#include "visitors/IdlePositonPrinter.h" // For IdlePositionPrinter class

#define DEADZONEWARNING 10 // Define a threshold for dead zone warning
// NOTE - At the moment the dead zone warning threshold is non hardware type specific. This could be changed in the future.

// TODO - move texts to text.h

/**
 * @brief Constructor for SensorIdleCalibration class *
 * @param calmgr Pointer to the SensorCalibrationManager
 * @param numiterations Number of iterations for calibration
 */
SensorIdleCalibration::SensorIdleCalibration(SensorCalibrationManager *calmgr, const int numiterations)
    : requestedIterations(numiterations), processedIterations(0), startCalibrationTime(millis()), CalibrationManager(calmgr) {
    Serial.println(F("Starting calibration..."));

    for (uint8_t id = 0; id < MAX_SENSORS; id++) {
        sumReads[id] = 0;
        minIdleValue[id] = 1023;
        maxIdleValue[id] = 0;
    }
}

/**
 * @brief Finish the Idle calibration
 * @param hardware Pointer to the Hardware instance
 * @details This function calculates the average position by dividing the sum of all readings by the number of iterations.
 */
void SensorIdleCalibration::finish(Hardware *hardware) {

    IdlePositionPrinter printer;

    // Calculating average position by dividing the sum of all readings by the number of iterations
    for (uint8_t id = 0; id < MAX_SENSORS; id++) {

        // Calculate the dead zone for the sensor
        int sensorDZ = maxIdleValue[id] - minIdleValue[id];

        // Update the maximum dead zone seen for all sensors if necessary
        maxDeadZone = (sensorDZ > maxDeadZone) ? sensorDZ : maxDeadZone;

        // Update the idlePosition for each sensor (returns true if the idle position is in the predefined normal zone)
        // Secondary check: Check if the dead zone is above the warning threshold.
        Sensor *sensor = hardware->getSensor(id); // REFACTOR - Use sensorCollection instead of hardware
        bool positionWarning = !(sensor->setIdlePosition(sumReads[id] / processedIterations));
        warningsOccurred = warningsOccurred || positionWarning || (sensorDZ > DEADZONEWARNING);

        printer.setPrintParams(minIdleValue[id], maxIdleValue[id], sensorDZ); // Set the print parameters for the printer visitor
        sensor->accept(printer);                                              // Accept the printer visitor to print the information for this sensor
    }

    // Output the calibration process information
    Serial.println(F("Calibration finished!"));
    Serial.print(F("Took "));
    Serial.print(millis() - startCalibrationTime); // Print the time taken for calibration
    Serial.println(F(" ms for "));
    Serial.print(processedIterations); // Print the number of processed iterations
    Serial.println(F(" iterations."));

    // Notify the creator of this observer so it can be deleted.
    CalibrationManager->deactivateIdleCalibration(warningsOccurred); // Finish the calibration process

} // Finish calibration process

/**
 * @brief Update the sensor calibration process
 * @details This function is called to update the calibration process.
 *          It reads the raw values from the sensors and updates the sum of reads, minimum and maximum values.
 * @param hardware Pointer to the Hardware instance
 */
void SensorIdleCalibration::update(Hardware *hardware) {
    // Finish the calibration process if the requested iterations are reached
    if (processedIterations >= requestedIterations) {
        finish(hardware); // Finish the calibration process
        return;
    }

    for (uint8_t id = 0; id < MAX_SENSORS; id++) {
        // Get the sensor by ID
        Sensor *sensor = hardware->getSensor(id); // REFACTOR - Use sensorCollection instead of hardware
        if (sensor == nullptr) {
            continue; // Skip if the sensor is not available
        }

        // Update the mean value (= Idle position) for the sensor
        int _rawValue = sensor->getRawValue();
        sumReads[id] += _rawValue;

        // Update the minimum and maximum values for deadzone evaluation
        minIdleValue[id] = (_rawValue < minIdleValue[id]) ? _rawValue : minIdleValue[id];
        maxIdleValue[id] = (_rawValue > maxIdleValue[id]) ? _rawValue : maxIdleValue[id];
    }

    processedIterations++; // Increment the number of processed iterations
}