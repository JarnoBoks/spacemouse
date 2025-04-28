#include "SensorIdleCalibration.h"
#include "hardware/hardware.h" // For Hardware class - necessary to retrieve the sensors.
#include "sensor/sensor.h"     // For Sensor class
#include "calibration/sensorcalibrationmanager.h"

#define DEADZONEWARNING 10 // Define a threshold for dead zone warning
// NOTE - At the moment the dead zone warning threshold is non hardware type specific. This should be changed in the future.

SensorIdleCalibration::SensorIdleCalibration(SensorCalibrationManager *calmgr, const int numiterations)
    : requestedIterations(numiterations), processedIterations(0), CalibrationManager(calmgr) {
    startCalibrationTime = millis();
} // Start time for zeroing process - Send to debug output

void SensorIdleCalibration::start() {
    // Initialize the calibration process
    Serial.println(F("Starting calibration..."));

} // Start calibration process

void SensorIdleCalibration::finish(Hardware *hardware) {

    // Calculating average position by dividing the sum of all readings by the number of iterations
    for (uint8_t id = 0; id < MAX_SENSORS; id++) {

        // Calculate the dead zone for the sensor
        int sensorDZ = maxIdleValue[id] - minIdleValue[id];

        // Update the maximum dead zone seen for all sensors if necessary
        maxDeadZone = (sensorDZ > maxDeadZone) ? sensorDZ : maxDeadZone;

        // Update the idlePosition for each sensor (returns true if the idle position is in the predefined normal zone)
        // Secondary check: Check if the dead zone is above the warning threshold.
        Sensor *sensor = hardware->sensors[id]; // Pointer to the sensor
        warningsOccurred = warningsOccurred || !(sensor->setIdlePosition(sumReads[id] / processedIterations)) || (sensorDZ > DEADZONEWARNING);
    }

    Serial.println(F("Calibration finished!"));
    Serial.print(F("Took "));
    Serial.print(millis() - startCalibrationTime); // Print the time taken for calibration
    Serial.println(F(" ms. for "));
    Serial.print(processedIterations); // Print the number of processed iterations
    Serial.println(F(" iterations."));

    // TODO  - Notify the creator of this observer to let it be destroyed
    CalibrationManager->finishIdleCalibration(warningsOccurred); // Finish the calibration process

} // Finish calibration process

void SensorIdleCalibration::update(Hardware *hardware) {
    // Finish the calibration process if the requested iterations are reached
    if (processedIterations >= requestedIterations) {
        finish(hardware); // Finish the calibration process
        return;
    }

    for (uint8_t id = 0; id < MAX_SENSORS; id++) {
        Sensor *sensor = hardware->sensors[id]; // Pointer to the sensor
        if (sensor == nullptr) {
            continue; // Skip if the sensor is not available
        }

        // Update the mean value (= Idle position) for the sensor
        int _rawValue = sensor->getRawValue(); // Get the raw value from the sensor
        sumReads[id] += _rawValue;             // Add the raw read value to the sum of reads

        // Update the minimum and maximum values for dead zone evaluation
        minIdleValue[id] = (_rawValue < minIdleValue[id]) ? _rawValue : minIdleValue[id];
        maxIdleValue[id] = (_rawValue > maxIdleValue[id]) ? _rawValue : maxIdleValue[id];
    }

    processedIterations++; // Increment the number of processed iterations
}