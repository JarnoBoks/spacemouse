#include "sensorcalibrationmanager.h"

#include "sensor/sensor.h" // For Sensor class
#include "sensor/sensorconfig.h"

#define DEADZONEWARNING 10 // Define a threshold for dead zone warning
// NOTE - At the moment the dead zone warning threshold is non hardware type specific. This should be changed in the future.

bool SensorCalibrationManager::calibrateIdle(Sensor *sensors[], uint8_t count, int iterations) {

    // FIXME - Debug output has to be generated!
    unsigned long startMillis = millis(); // Start time for zeroing process - Send to debug output

    bool WarningsOccurred = false;
    uint8_t maxDeadZone = 0;          // Maximum dead zone value
    uint32_t sumReads[count] = {0};   // Array to store sum of reads, necessaru for the average calculation, all items initialized to 0
    int minIdleValue[count] = {1023}; // Array to store minimum idle values for each sensor, all items set to maximum possible value
    int maxIdleValue[count] = {0};    // Array to store maximum idle values for each sensor, all items set to minimum possible value

    for (int c_iterations = 0; c_iterations < iterations; c_iterations++) {

        for (uint8_t i = 0; i < count; i++) {
            sensors[i]->readValue(); // Read the value from the sensor

            // Update the mean value (= Idle position) for the sensor
            int _rawValue = sensors[i]->getRawValue(); // Get the raw value from the sensor
            sumReads[i] += _rawValue;                  // Add the raw read value to the sum of reads

            // Update the minimum and maximum values for dead zone evaluation
            minIdleValue[i] = (_rawValue < minIdleValue[i]) ? _rawValue : minIdleValue[i];
            maxIdleValue[i] = (_rawValue > maxIdleValue[i]) ? _rawValue : maxIdleValue[i];
        }
    }

    // Calculating average position by dividing the sum of all readings by the number of iterations
    for (uint8_t i = 0; i < count; i++) {
        // Calculate the dead zone for the sensor
        int sensorDZ = maxIdleValue[i] - minIdleValue[i];

        // Update the maximum dead zone seen for all sensors if necessary
        maxDeadZone = (sensorDZ > maxDeadZone) ? sensorDZ : maxDeadZone;

        // Update the idlePosition for each sensor (returns true if the idle position is in the predefined normal zone)
        // Secondary check: Check if the dead zone is above the warning threshold.
        WarningsOccurred = WarningsOccurred || !(sensors[i]->setIdlePosition(sumReads[i] / iterations)) || (sensorDZ > DEADZONEWARNING);

    } // for

    return WarningsOccurred;
}

void SensorCalibrationManager::calibrateMinMax(Sensor *sensors[], uint8_t count) {
}

void SensorCalibrationManager::calibrateDeadzone(Sensor *sensor, const uint8_t deadzone) {
    SensorConfig *config = sensor->getConfig(); // Get the configuration of the sensor
    config->setDeadzone(deadzone);              // Set the dead zone for the sensor
    config->saveSensorConfig(sensor->getId());  // Save the updated configuration to EEPROM
}
