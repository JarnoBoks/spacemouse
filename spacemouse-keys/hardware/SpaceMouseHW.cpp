#include <Arduino.h>
#include "SpaceMouseHW.h"
#include "config.h"

/**
 * Constructor
 */
SpaceMouseHW_::SpaceMouseHW_()
    : _pinList PINLIST, _invertList INVERTLIST, _maxVals MAXVALS, _minVals MINVALS {
    return;
}

/**
 *  @brief Function to read and store analogue voltages for each sensor/joystick axis.
 *  The function reads the pins that are setup in config.h and inverts the reading if necessary.
 *
 *  @param rawReads pointer to the array that will contain the read analog values
 */
void SpaceMouseHW_::readAllFromSensors() {

    for (int i = 0; i < NUM_SENSORS; i++) {
        if (_invertList[i] == 1) {
            // invert the reading
            _rawReads[i] = 1023 - analogRead(_pinList[i]);
        } else {
            _rawReads[i] = analogRead(_pinList[i]);
        }
    }
}

void SpaceMouseHW_::_printValue(const char *axisname, int val) {
    char debugOutputBuffer[20];

    sprintf(debugOutputBuffer, "%2.2s: %4d ", axisname, val);
    Serial.print(debugOutputBuffer);
}

void SpaceMouseHW_::_printRawRead(const char *axisname, int idx) {
    _printValue(axisname, _rawReads[idx]);
}

void SpaceMouseHW_::_printCentered(const char *axisname, int idx) {
    _printValue(axisname, centered[idx]);
}

/**
 *  @brief Calibrate (=zero) the space mouse. The function is blocking other functions of the spacemouse during zeroing.
 *
 *  @param centerPoints
 *  @param numIterations How many readings are taken to calculate the mean. Suggestion: 500 iterations, they take approx. 480ms.
 *  @param debugFlag With debugFlag = true, a suggestion for the dead zone is given on the serial interface to save to the config.h
 *  @return returns true, if no warnings occured. Warnings are given if the zero positions are very unlikely
 */
// A dead zone above the following value will be warned
#define DEADZONEWARNING 10 // TODO - Use the configured level for this

bool SpaceMouseHW_::busyZeroing(uint16_t numIterations, boolean debugFlag) {

    bool noWarningsOccured = true;

    // int act[NUM_SENSORS];       // actual value
    uint32_t mean[NUM_SENSORS]; // Array to count all values during the averaging
    int minValue[NUM_SENSORS];  // Array to store the minimum values
    int maxValue[NUM_SENSORS];  // Array to store the maximum values

    for (int i = 0; i < NUM_SENSORS; i++) {
        minValue[i] = 1023; // Set the min value to the maximum possible value
        maxValue[i] = 0;    // Set the max value to the minimum possible value
        mean[i] = 0;        // Initialize mean to zero
    }

    // Measure the duration of the zeroing prooces
    unsigned int long start, end;
    start = millis();

    uint16_t count; // Track the number of iterations

    for (count = 0; count < numIterations; count++) {
        readAllFromSensors();
        for (uint8_t i = 0; i < NUM_SENSORS; i++) {
            // Add to mean
            mean[i] = mean[i] + _rawReads[i];
            // Update the minimum and maximum values for dead zone evaluation
            if (_rawReads[i] < minValue[i]) {
                minValue[i] = _rawReads[i];
            }
            if (_rawReads[i] > maxValue[i]) {
                maxValue[i] = _rawReads[i];
            }
        }
    }

    int16_t deadZone[NUM_SENSORS]; // TODO
    int16_t maxDeadZone = 0;       // TODO

    // calculating average by dividing the mean by the number of iterations
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        centerPoints[i] = mean[i] / count;
        deadZone[i] = maxValue[i] - minValue[i];
        if (deadZone[i] > maxDeadZone) {
            // get maximum deadzone independent of the measured axis
            maxDeadZone = deadZone[i];
        }

#ifndef HALLEFFECT
// a centerpoint below or above those values will be warned (512 +/- 128)
#define CENTERPOINTWARNINGMIN 384
#define CENTERPOINTWARNINGMAX 640
#else
        // The centerpoint off the Hall effect mouse is not in the center of the ADC range, due to the hardware nature.
        // According to the height of the base plate, the centerpoint is shifted up or downwards.
        // a centerpoint below or above those values will be warned (512 +/- 128)
#define CENTERPOINTWARNINGMIN (720 - 128)
#define CENTERPOINTWARNINGMAX (720 + 128)
#endif

        if (deadZone[i] > DEADZONEWARNING || centerPoints[i] < CENTERPOINTWARNINGMIN || centerPoints[i] > CENTERPOINTWARNINGMAX) {
            noWarningsOccured = false;
        }
    } // for

    // Report everything, if with debugFlag
    if (debugFlag) {
        Serial.println(F("##  Min- Mean - Max -> Dead Zone"));
        for (int i = 0; i < 8; i++) {
            // FIXME Serial.print(axisNames[i]);
            Serial.print(" ");
            Serial.print(minValue[i]);
            Serial.print(" - ");
            Serial.print(centerPoints[i]);
            Serial.print(" - ");
            Serial.print(maxValue[i]);
            Serial.print(" -> ");
            Serial.print(deadZone[i]);
            Serial.print(" ");
            if (deadZone[i] > DEADZONEWARNING) {
                Serial.print(F(" Attention! Moved axis?"));
            }
            if (centerPoints[i] < CENTERPOINTWARNINGMIN || centerPoints[i] > CENTERPOINTWARNINGMAX) {
                Serial.print(F(" Attention! Axis in idle?"));
            }
            Serial.println("");
        }
        end = millis();
        Serial.println(F("Using mean as zero position..."));
        Serial.print(F("Suggestion for config.h: "));
        Serial.print(F("#define DEADZONE "));
        Serial.println(maxDeadZone);
        Serial.print(F("This took "));
        Serial.print(end - start);
        Serial.print(F(" ms for "));
        Serial.print(count);
        Serial.println(F(" iterations."));
    }
    return noWarningsOccured;
}
