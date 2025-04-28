#include "SensorMinMaxCalibration.h"
#include "hardware/hardware.h" // For Hardware class - necessary to retrieve the sensors.
#include "sensor/sensor.h"     // For Sensor class
#include "calibration/sensorcalibrationmanager.h"

#define MINMAXDURATION 15 // Duration for min/max calibration in seconds

SensorMinMaxCalibration::SensorMinMaxCalibration(SensorCalibrationManager *calmgr, const int numiterations)
    : CalibrationManager(calmgr) {
    startCalibrationTime = millis();
} // Start time for zeroing process - Send to debug output

void SensorMinMaxCalibration::start() {
    // Initialize the calibration process
    Serial.print(F("Move the spacemouse for "));
    Serial.print(MINMAXDURATION);
    Serial.println(F(" sec."));

    if (_minMaxCalcState == statemachineMinMaxCal_t::MEASURING) {
        if (millis() - _startMillis < (MINMAXDURATION * 1000)) {
            for (uint8_t i = 0; i < NUM_SENSORS; i++) {
                // Update the minimum and maximum values
                if (centered[i] < _minVals[i]) {
                    _minVals[i] = centered[i];
                }
                if (centered[i] > _maxVals[i]) {
                    _maxVals[i] = centered[i];
                }
            }
        } else {
            // 15s are over. Go to the next state and report via console.
            Serial.println(F("\n\nFinished.\nResults: "));
            _minMaxCalcState = statemachineMinMaxCal_t::RESULTS; // next State: results
        }
    } else if (_minMaxCalcState == statemachineMinMaxCal_t::RESULTS) {

        PrintMinMax(); // Print the min and max values to the serial monitor

        _minMaxCalcState = statemachineMinMaxCal_t::IDLE; // no further reporting
    }

} // Start calibration process

void SensorMinMaxCalibration::finish(Hardware *hardware) {

    Serial.println(F("        Min |  Max | Range | Warning"));

    for (uint8_t id = 0; id < MAX_SENSORS; id++) {

        Sensor *sensor = hardware->sensors[id]; // Pointer to the sensor
        if (sensor == nullptr) {
            continue; // Skip if the sensor is not available
        }

        const int workingRange = abs(minValue[id]) + abs(maxValue[id]);

        // Print the value of the min, max and working range for each sensor
        Serial.print(sensor->getName());
        Serial.print(F(":  "));
        alignValue(minValue[id], 4);
        Serial.print(minValue[id]);
        Serial.print(F(" | "));
        alignValue(maxValue[id], 4); // Updated to use maxValue[id]
        Serial.print(maxValue[id]);  // Updated to use maxValue[id]
        Serial.print(F(" | "));
        alignValue(workingRange, 4);
        Serial.print(workingRange);
        Serial.print(F("  | "));

        // Check if the min or max values are below the warning threshold
        bool isWarning = false;
        bool isFirst = true;

        if (abs(_minVals[i]) < _warningMinMaxMinimum) {
            Serial.print(F("Min"));
            isWarning = true;
            isFirst = false;
        }
        if (abs(_maxVals[i]) < _warningMinMaxMaximum) {
            if (!isFirst) {
                Serial.print(F(", "));
            }
            Serial.print(F("Max"));
            isWarning = true;
            isFirst = false;
        }
        if (workingRange < _warningMinMaxRange) {
            if (!isFirst) {
                Serial.print(F(", "));
            }
            Serial.print(F("Range"));
            isWarning = true;
        }
        if (isWarning) {
            Serial.print(F(" small"));
        } else {
            Serial.print(F("ok"));
        }

        Serial.println();
    }
}

/*     // Calculating average position by dividing the sum of all readings by the number of iterations
    for (uint8_t id = 0; id < MAX_SENSORS; id++) {

        // Calculate the dead zone for the sensor
        int sensorDZ = maxIdleValue[id] - minIdleValue[id];

        // Update the maximum dead zone seen for all sensors if necessary
        maxDeadZone = (sensorDZ > maxDeadZone) ? sensorDZ : maxDeadZone;

        // Update the idlePosition for each sensor (returns true if the idle position is in the predefined normal zone)
        // Secondary check: Check if the dead zone is above the warning threshold.
        Sensor *sensor = hardware->sensors[id]; // Pointer to the sensor
        /* warningsOccurred = warningsOccurred || !(sensor->setIdlePosition(sumReads[id] / processedIterations)) || (sensorDZ > DEADZONEWARNING); */
//}

// Serial.println(F("Calibration finished!"));

// TODO  - Notify the creator of this observer to let it be destroyed
// CalibrationManager->finishIdleCalibration(warningsOccurred); // Finish the calibration process

//} // Finish calibration process

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