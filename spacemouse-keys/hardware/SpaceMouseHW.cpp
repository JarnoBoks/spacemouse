#include <Arduino.h>
#include "SpaceMouseHW.h"
#include "config.h"

#define MINMAXDURATION 15  // The duration of the min-max calibration in seconds
#define DEADZONEWARNING 10 // A deadzone above the following value will be warned // TODO - Use the configured level for this

/**
 * Constructor / Destructor
 */

SpaceMouseHW_::SpaceMouseHW_(const int warnCpntMax, const int warnCpntMin, const int warnMMMax, const int warnMMMin, const char *axs[])
    : _axisNames{axs}, _pinList PINLIST, _invertList INVERTLIST, _maxVals MAXVALS, _minVals MINVALS,
      _warningCenterpointMin(warnCpntMin),
      _warningCenterpointMax(warnCpntMax),
      _warningMinMaxMinimum(warnMMMin),
      _warningMinMaxMaximum(warnMMMax) {}

SpaceMouseHW_::~SpaceMouseHW_() {}

bool SpaceMouseHW_::BusyZeroing(uint16_t numIterations, boolean debugFlag) {
    // Set up the zeroing datastructure, while initialising the constants.
    zeroing_t params;

    bool ret = _busyZeroing(&params, numIterations);

    // If in debugmode, call the print function of our parent and output the measured values per sensor.
    // The function will print an intro and outro too.
    if (debugFlag) {
        for (uint8_t i = 0; i < NUM_SENSORS; i++) {
            _printZeroedValue(&params, _axisNames[i], i);
        }
    }

    return ret;
}

/**
 *  @brief This function records the minimum and maximum movement of the joysticks/knob.
 *  After initialization, move the mouse for 15s and see the printed output.
 *  Replug/reset the mouse, to enable the semi-automatic calibration for a second time.
 */
void SpaceMouseHW_::CalcMinMax() {
    if (_minMaxCalcState == 0) {
        delay(2000);
        // Initialize the arrays
        for (uint8_t i = 0; i < NUM_SENSORS; i++) {
            _minVals[i] = 1023; // Set the min value to the maximum possible value
            _maxVals[i] = 0;    // Set the max value to the minimum possible value
        }
        _startMillis = millis(); // Record the current time
        _minMaxCalcState = 1;    // next State: measure!
        Serial.print(F("Please start moving the spacemouse around for "));
        Serial.print(MINMAXDURATION);
        Serial.println(F(" sec!"));

    } else if (_minMaxCalcState == 1) {
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
            Serial.println(F("\n\nStop moving the spacemouse. These are the results. Copy them in config.h"));
            _minMaxCalcState = 2;
        }
    } else if (_minMaxCalcState == 2) {
        Serial.print(F("#define MINVALS "));
        _printArray(_minVals, NUM_SENSORS);
        Serial.print(F("#define MAXVALS "));
        _printArray(_maxVals, NUM_SENSORS);

        // Calculate and print the working for each sensor (added for the HALL sensors, but this will function for the joysticks too)
        int workingRanges[NUM_SENSORS];
        int max = 0;
        int min = 0;
        for (uint8_t i = 0; i < NUM_SENSORS; i++) {
            workingRanges[i] = abs(_minVals[i]) + abs(_maxVals[i]);
            max = (abs(_maxVals[i]) > max) ? abs(_maxVals[i]) : max;
            min = (abs(_minVals[i]) > min) ? abs(_minVals[i]) : min;
        }
        Serial.print(F("Ranges are: "));
        _printArray(workingRanges, NUM_SENSORS);
        int centerRange = (max + (min * -1)) / 2;
        Serial.print(F("Center: "));
        Serial.println(centerRange);

        for (uint8_t i = 0; i < NUM_SENSORS; i++) {
            if (abs(_minVals[i]) < _warningMinMaxMinimum) {
                Serial.print(F("Warning: minValue["));
                Serial.print(i);
                Serial.print("] ");
                Serial.print(_axisNames[i]);
                Serial.print(F(" is small: "));
                Serial.println(_minVals[i]);
            }
            if (abs(_maxVals[i]) < _warningMinMaxMaximum) {
                Serial.print(F("Warning: maxValue["));
                Serial.print(i);
                Serial.print("] ");
                Serial.print(_axisNames[i]);
                Serial.print(F(" is small: "));
                Serial.println(_maxVals[i]);
            }
        }
        _minMaxCalcState = 3; // no further reporting
    }
}

/**
 *  @brief Takes the centered joystick/sensor values, applies a deadzone and maps the values to +/- 350.
 *  @param centered pointer to array with 8 centered analog values
 */
#define TOTALSENSITIVITY 350
void SpaceMouseHW_::FilterAnalogReadOuts() {

    // Filter movement values. Set to zero if movement is below deadzone threshold.
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        if (centered[i] < DEADZONE && centered[i] > -DEADZONE) {
            centered[i] = 0;
        } else {
            if (centered[i] < 0) { // if the value is smaller 0 ...
                // ... map the value from the [min,-DEADZONE] to [-350,0]
                centered[i] = map(centered[i], _minVals[i], -DEADZONE, -TOTALSENSITIVITY, 0);
            } else { // if the value is > 0 ...
                // ... map the values from the [DEADZONE,max] to [0,+350]
                centered[i] = map(centered[i], DEADZONE, _maxVals[i], 0, TOTALSENSITIVITY);
            }
        }
    }
}

/**
 *  @brief Function to read and store analogue voltages for each sensor/joystick axis.
 *  The function reads the pins that are setup in config.h and inverts the reading if necessary.
 *  The read values are stored in the private data member _rawReads
 */
void SpaceMouseHW_::ReadAllFromSensors() {

    for (int i = 0; i < NUM_SENSORS; i++) {
        if (_invertList[i] == 1) {
            // invert the reading
            _rawReads[i] = 1023 - analogRead(_pinList[i]);
        } else {
            _rawReads[i] = analogRead(_pinList[i]);
        }
    }
}

/**
 * @brief Subtract center position of the joystick/knob from the measured position to determine movement.
 */
void SpaceMouseHW_::CenterSensors() {
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        centered[i] = _rawReads[i] - _centerPoints[i];
    }
}

/**
 *  @brief Calibrate (=zero) the space mouse. The function is blocking other functions of the spacemouse during zeroing.
 *
 *  @param numIterations How many readings are taken to calculate the mean. Suggestion: 500 iterations, they take approx. 480ms.
 *  @param debugFlag With debugFlag = true, a suggestion for the dead zone is given on the serial interface to save to the config.h
 *
 *  @return returns true, if no warnings occured. Warnings are given if the zero positions are very unlikely, ie if they are out of boundary of our hardware.
 */
bool SpaceMouseHW_::_busyZeroing(zeroing_t *params, uint16_t numIterations) {

    bool noWarningsOccurred = true;

    // Initialize the zeroing parameters in the structure.
    for (int i = 0; i < NUM_SENSORS; i++) {
        params->minValue[i] = 1023; // Set the min value to the maximum possible value
        params->maxValue[i] = 0;    // Set the max value to the minimum possible value
        params->mean[i] = 0;        // Initialize mean to zero
        params->deadZone[i] = 0;
    }
    params->maxDeadZone = 0;
    params->count = 0;

    // Measure the duration of the zeroing process
    _startMillis = millis();

    for (params->count = 0; params->count < numIterations; params->count++) {
        ReadAllFromSensors();
        for (uint8_t i = 0; i < NUM_SENSORS; i++) {
            // Add to mean
            params->mean[i] = params->mean[i] + _rawReads[i];
            // Update the minimum and maximum values for dead zone evaluation
            if (_rawReads[i] < params->minValue[i]) {
                params->minValue[i] = _rawReads[i];
            }
            if (_rawReads[i] > params->maxValue[i]) {
                params->maxValue[i] = _rawReads[i];
            }
        }
    }

    // Calculating average by dividing the mean by the number of iterations
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        // Update the centerPoint for each sensor
        _centerPoints[i] = params->mean[i] / params->count;

        // TODO - comments
        params->deadZone[i] = params->maxValue[i] - params->minValue[i];
        if (params->deadZone[i] > params->maxDeadZone) {
            // get maximum deadzone independent of the measured axis
            params->maxDeadZone = params->deadZone[i];
        }

        // Test if the measured value is within our boundaries. The boundaries are different for every hardware type and thus are set
        // by the child and included in the zeroing params struct.
        if (params->deadZone[i] > DEADZONEWARNING || _centerPoints[i] < _warningCenterpointMin || _centerPoints[i] > _warningCenterpointMin) {
            noWarningsOccurred = false;
        }
    } // for

    return noWarningsOccurred;
}

/**
 * @brief Outputs the zeroing result for an axis to the serial interface.
 *        If processing the first axis, the header is written to the interface too.
 *        If processing the last axis, the footer is written to the interface too.
 *
 * @param params
 * @param axisname
 * @param i
 */
void SpaceMouseHW_::_printZeroedValue(zeroing_t *params, const char *axisname, int i) {
    // Write the header if processing the first sensor
    if (i == 0) {
        Serial.println(F("##  Min- Mean - Max -> Dead Zone"));
    }
    Serial.print(axisname);
    Serial.print(" ");
    Serial.print(params->minValue[i]);
    Serial.print(" - ");
    Serial.print(_centerPoints[i]);
    Serial.print(" - ");
    Serial.print(params->maxValue[i]);
    Serial.print(" -> ");
    Serial.print(params->deadZone[i]);
    Serial.print(" ");

    // Warn if the centrePoint is outside the normal deadzone (ie. the readings vary too much)
    if (params->deadZone[i] > DEADZONEWARNING) {
        Serial.print(F(" Attention! Did axis move?"));
    }

    // Warn if the centerpoint is outside the normal centerpoint range (ie. the joystick is physically not centered)
    if (_centerPoints[i] < _warningCenterpointMin || _centerPoints[i] > _warningCenterpointMax) {
        Serial.print(F(" Attention! Is axis in idle position?"));
    }
    Serial.println("");

    // Write the closure if processing the last sensor
    if (i == NUM_SENSORS) {
        Serial.println(F("Using mean as zero position..."));
        Serial.print(F("Suggestion for config.h: "));
        Serial.print(F("#define DEADZONE "));
        Serial.println(params->maxDeadZone);
        Serial.print(F("This took "));
        Serial.print(millis() - _startMillis);
        Serial.print(F(" ms for "));
        Serial.print(params->count);
        Serial.println(F(" iterations."));
    }
}

/**
 *  @brief Prints an array to the Serial, in order to copy the output again to C-Code. Example output: {-519, -521, -512, -2, -519, -482, -508, -1}
 *  @param arr array to print
 *  @param size size of the array
 */
void SpaceMouseHW_::_printArray(int arr[], int size) {
    Serial.print("{");
    for (int i = 0; i < size; i++) {
        Serial.print(arr[i]);
        if (i < size - 1) {
            Serial.print(", ");
        }
    }
    Serial.print("}");

    Serial.print(DEBUG_LINE_END);
}

/**
 * @brief Output the axisname and the value to the serial interface.
 * @param axisname
 * @param val
 */
void SpaceMouseHW_::_printValue(const char *axisname, int val) {
    char debugOutputBuffer[20];

    sprintf(debugOutputBuffer, "%4.4s:%4d, ", axisname, val);
    Serial.print(debugOutputBuffer);
}

void SpaceMouseHW_::PrintRawReads() {
    // Report back 0-1023 raw ADC 10-bit values if enabled
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        _printValue(_axisNames[i], _rawReads[i]);
    }
}

void SpaceMouseHW_::PrintCentered() {
    // Report back values for sensor axis after centering and mapping
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        _printValue(_axisNames[i], centered[i]);
    }
}
