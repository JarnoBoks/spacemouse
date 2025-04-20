#include <Arduino.h>
#include <EEPROM.h>
#include "eepromStorage.h"
#include "SpaceMouseHW.h"
#include "config.h"
#include "text.h"

#define MINMAXDURATION 15  // The duration of the min-max calibration in seconds
#define DEADZONEWARNING 10 // A deadzone above the following value will be warned // TODO - Use the configured level for this

/**
 * Constructor / Destructor
 */
SpaceMouseHW_::SpaceMouseHW_(const int warnCpntMin, const int warnCpntMax, const int warnMMMin, const int warnMMMax, const int warnMMRange, const char *axs[])
    // SpaceMouseHW_::SpaceMouseHW_(const int warnCpntMin, const int warnCpntMax, const int warnMMMin, const int warnMMMax, const unsigned int warnMMRange, char *axs[])
    : _axisNames{axs},
      _pinList PINLIST,
      _invertList INVERTLIST,
      _maxVals MAXVALS,
      _minVals MINVALS,
      _warningCenterpointMin(warnCpntMin),
      _warningCenterpointMax(warnCpntMax),
      _warningMinMaxMinimum(warnMMMin),
      _warningMinMaxMaximum(warnMMMax),
      _warningMinMaxRange(warnMMRange) {

    if (!EEPROMStorage::isFirstRun()) {
        // If we are not in the first run, we can load the configuration from the EEPROM.

        // Read Deadzone from EEPROM, use the default (configured) value if the byte is not set.
        EEPROM.get(EEPROM_ADDRESS_DEADZONE, _deadzone);

        // Read min/max values from EEPROM, use the default (configured) values if the bytes are not set.
        EEPROM.get(EEPROM_ADDRESS_MINVALS, _minVals);
        EEPROM.get(EEPROM_ADDRESS_MAXVALS, _maxVals);
    } else {
        // Write the defaults to the EEPROM if this is the first run of the software.
        EEPROM.put(EEPROM_ADDRESS_DEADZONE, _deadzone); // Store the deadzone in the EEPROM
        EEPROM.put(EEPROM_ADDRESS_MINVALS, _minVals);   // Store the min values in the EEPROM
        EEPROM.put(EEPROM_ADDRESS_MAXVALS, _maxVals);   // Store the max values in the EEPROM
    }
}

/**
 * @brief Destructor for the SpaceMouseHW_ class.
 */
SpaceMouseHW_::~SpaceMouseHW_() {}

/**
 * @brief Measure the idle position of the spacemouse and store the results internally for future usage.
 * @param numIterations The number of iterations to take for the zeroing process. The more iterations, the more accurate the result.
 * @param serialOutput Flag to indicate if the results should be printed to the serial monitor.
 * @return the result of the zeroing process.
 * @retval true if no warnings occurred during the zeroing process.
 * @retval false if warnings occurred during the zeroing process.
 */
bool SpaceMouseHW_::BusyZeroing(uint16_t numIterations, boolean serialOutput) {
    // Set up the zeroing datastructure, while initialising the constants.
    zeroing_t params;

    bool ret = _busyZeroing(&params, numIterations);

    // If in debugmode, call the print function of our parent and output the measured values per sensor.
    // The function will print an intro and outro too.
    if (serialOutput) {
        for (uint8_t i = 0; i < NUM_SENSORS; i++) {
            _printZeroedValue(&params, _axisNames[i], i);
        }
    }

    return ret;
}

/**
 * @brief Starts the calibration process for the min and max values of the sensors.
 * @details The function will print the min and max values for each sensor to the serial monitor.
 * If the storeResults flag is set to true, the min and max values will be stored in EEPROM.
 * @param storeResults Flag to indicate if the results should be stored in EEPROM or only printed to the serial monitor.
 */
void SpaceMouseHW_::CalibrateMinMax(boolean storeResults) {
    _storeCalibrationResults = storeResults; // Set the flag to store the results in EEPROM

    // Check if the calibration process is already running. If not, start the calibration process.
    if (_minMaxCalcState == statemachineMinMaxCal_t::IDLE) {
        Serial.println(F("Calibrating Min & Max"));
        _minMaxCalcState = statemachineMinMaxCal_t::START; // start the calibration process
    }
}

/**
 *  @brief This function records the minimum and maximum movement of the joysticks/knob.
 *  After initialization, move the mouse for 15s and see the printed output.
 *  Replug/reset the mouse, to enable the semi-automatic calibration for a second time.
 *
 * @details The function will print the min and max values for each sensor to the serial monitor.
 * The min and max values are used to calculate the working range for each sensor.
 * The working range is the difference between the min and max values.
 * The function will also print a warning if the min or max values are below a certain threshold.
 * The threshold values are defined in the config.h file.
 */
void SpaceMouseHW_::ProcessCalcMinMax() {
    if (_minMaxCalcState == statemachineMinMaxCal_t::START) {
        delay(1000);
        // Initialize the arrays
        for (uint8_t i = 0; i < NUM_SENSORS; i++) {
            _minVals[i] = 1023; // Set the min value to the maximum possible value
            _maxVals[i] = 0;    // Set the max value to the minimum possible value
        }
        _startMillis = millis();                               // Record the current time
        _minMaxCalcState = statemachineMinMaxCal_t::MEASURING; // next State: measure!
        Serial.print(F("Move the spacemouse for "));
        Serial.print(MINMAXDURATION);
        Serial.println(F(" sec."));

    } else if (_minMaxCalcState == statemachineMinMaxCal_t::MEASURING) {
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

        if (_storeCalibrationResults) {
            Serial.println(F("Stored values in EEPROM."));

#if 0
            // Store the min and max values in EEPROM if the storeResults flag is set to true
            for (uint8_t i = 0; i < NUM_SENSORS; i++) {
                EEPROM.put(EEPROM_ADDRESS_MINVALS + (i * sizeof(int)), _minVals[i]);
                EEPROM.put(EEPROM_ADDRESS_MAXVALS + (i * sizeof(int)), _maxVals[i]);
            }
#endif
            EEPROM.put(EEPROM_ADDRESS_MINVALS, _minVals); // Store the min values in the EEPROM
            EEPROM.put(EEPROM_ADDRESS_MAXVALS, _maxVals); // Store the max values in the EEPROM
        }

        _minMaxCalcState = statemachineMinMaxCal_t::IDLE; // no further reporting
    }
}

/**
 *  @brief Takes the centered joystick/sensor values, applies the deadzone and maps the centered values to +/- 350.
 */
#define TOTALSENSITIVITY 350
void SpaceMouseHW_::FilterAnalogReadOuts() {

    // Filter movement values. Set to zero if movement is below deadzone threshold.
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        if (abs(centered[i]) < _deadzone) {
            centered[i] = 0;
        } else if (centered[i] > _deadzone) {
            centered[i] = map(centered[i], _deadzone, _maxVals[i], 0, TOTALSENSITIVITY);
        } else { // if the value is smaller than -DEADZONE
            centered[i] = map(centered[i], _minVals[i], (-1 * _deadzone), -TOTALSENSITIVITY, 0);
        }
    } // for
}
#if 0
        if (centered[i] < _deadzone && centered[i] > -_deadzone) {
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
#endif

/**
 *  @brief Function to read analogue voltages for each sensor/joystick sensor.
 *  The function reads the pins that are configured in config.h and inverts each reading if configured as such.
 *  The read values are stored in the private data member _rawReads
 */
void SpaceMouseHW_::ReadAllFromSensors() {

    for (int i = 0; i < NUM_SENSORS; i++) {
        _rawReads[i] = analogRead(_pinList[i]); // Reset the raw reading for each sensor

        if (_invertList[i] == 1) {
            _rawReads[i] = 1023 - _rawReads[i];
        }

#if 0 // Compiled size optimization
        if (_invertList[i] == 1) {
            // invert the reading
            _rawReads[i] = 1023 - analogRead(_pinList[i]);
        } else {
            _rawReads[i] = analogRead(_pinList[i]);
        }
#endif
    }
}

/**
 * @brief Subtract center position of the joystick/knob from the measured position to determine movement.
 */
// TODO - this function could be integrated into the ReadAllFromSensors function to save some space.
void SpaceMouseHW_::CenterSensors() {
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        centered[i] = _rawReads[i] - _centerPoints[i];
    }
}

void SpaceMouseHW_::UpdateMinMax(const char *cmd, float value) {
    // The command should contain a string starting with a sign +/- indicating if
    // we update the min or max value. The remainder of the string should contain the axis name.

    // REVIEW - Changed to use strcmp for better readability and performance.
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        if (strcmp(cmd + 1, _axisNames[i]) == 0) { // Changed to strcmp for review
            // If the command matches the axis name, update the min or max value accordingly.
            if (cmd[0] == '+') {
                _maxVals[i] = value;
                EEPROM.put(EEPROM_ADDRESS_MAXVALS + (i * sizeof(int)), _maxVals[i]); // Store the max value in the EEPROM
            } else if (cmd[0] == '-') {
                _minVals[i] = value;
                EEPROM.put(EEPROM_ADDRESS_MINVALS + (i * sizeof(int)), _minVals[i]); // Store the min value in the EEPROM
            }
            return; // Exit the function after a matching axis name
        }
    }
}

/**
 * @brief Write the deadzone value to the EEPROM and set the private data member _deadzone.
 *        The deadzone value is used to filter out small movements of the joystick/knob.
 * @param value The deadzone value to be set, entered through the serial interface . The value is between 0 and 127.
 * @return 0 if the deadzone value is set successfully, -1 if the value is out of range.
 */
int8_t SpaceMouseHW_::UpdateDeadzone(uint8_t value) {

    // Update the internal deadzone value
    _deadzone = value;

    // Store the value in the EEPROM. EEPROM.put() uses EEPROM.update and thus only writes data if the data has changed.
    EEPROM.put(EEPROM_ADDRESS_DEADZONE, _deadzone); // Store the value in the EEPROM.

    return 0; // Success
}

/**
 * @brief Write the deadzone value to the serial interface.
 *        This function is used to print the deadzone value to the serial interface.
 */
void SpaceMouseHW_::PrintDeadzone() {
    // Print the deadzone value to the serial interface
    Serial.print(F("Deadzone: "));
    Serial.println(_deadzone);
}

/**
 *  @brief Calibrate (=zero) the space mouse idle position. The function is blocking other functions of the spacemouse during zeroing.
 *
 *  @param numIterations How many readings are taken to calculate the mean. Suggestion: 500 iterations, they take approx. 480ms.
 *  @param debugFlag With debugFlag = true, a suggestion for the dead zone is given on the serial interface to save to the config.h
 *
 *  @return returns true, if no warnings occured. Warnings are given if the zero positions are very unlikely, ie if they are out of boundary of our hardware.
 */
bool SpaceMouseHW_::_busyZeroing(zeroing_t *params, uint16_t numIterations) {

    bool noWarningsOccurred = true;

    // Initialize the zeroing parameters in the data structure.
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

void SpaceMouseHW_::PrintMinMax() {

    Serial.println(F("        Min |  Max | Range | Warning"));

    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        int workingRange = abs(_minVals[i]) + abs(_maxVals[i]);

        // Print the value of the min, max and working range for each sensor
        Serial.print(_axisNames[i]);
        Serial.print(F(":  "));
        alignValue(_minVals[i], 4);
        Serial.print(_minVals[i]);
        Serial.print(F(" | "));
        alignValue(_maxVals[i], 4);
        Serial.print(_maxVals[i]);
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
        Serial.println(F("\n#####  Min - Mean -  Max -> Deadzone"));
    }

    // Print the axis name and the values to the serial interface
    // Using a formatted string: %4.4s  %4.4d - %4.4d - %4.4d -> %d
    Serial.print(axisname);
    Serial.print(F(": "));
    alignValue(params->minValue[i], 4);
    Serial.print(params->minValue[i]);
    Serial.print(F(" - "));
    alignValue(_centerPoints[i], 4);
    Serial.print(_centerPoints[i]);
    Serial.print(F(" - "));
    alignValue(params->maxValue[i], 4);
    Serial.print(params->maxValue[i]);
    Serial.print(F(" -> "));
    alignValue(params->deadZone[i], 4);
    Serial.print(params->deadZone[i]);
    Serial.print(F("   "));

    // Warn if the centrePoint is outside the normal deadzone (ie. the readings vary too much)
    if (params->deadZone[i] > DEADZONEWARNING) {
        Serial.print(F("!! Did axis move?"));
    }

    // Warn if the centerpoint is outside the normal centerpoint range (ie. the joystick is physically not centered)
    if (_centerPoints[i] < _warningCenterpointMin || _centerPoints[i] > _warningCenterpointMax) {
        Serial.print(F("!! Is axis in idle position?"));
    }
    Serial.println("");

    // Write the closure if processing the last sensor
    if (i == (NUM_SENSORS - 1)) {

        Serial.println(F("Using mean as centerpoint."));

        Serial.print(F("Calibration suggestion: 'DEADZONE "));
        Serial.print(params->maxDeadZone);
        Serial.println(F("'"));

        Serial.print(F("("));
        Serial.print((int)(millis() - _startMillis));
        Serial.print(F(" ms / "));
        Serial.print(params->count);
        Serial.print(F(" iterations)"));
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
 * @param index
 * @param value
 * @param alignmentWidth
 * @details The alignmentWidth is used to align the output of the values in the serial monitor.
 */
void SpaceMouseHW_::_printValue(const uint8_t index, const int value, const uint8_t alignmentWidth) {
    if (index > 0) {
        Serial.print(F(", "));
    }
    Serial.print(_axisNames[index]);
    Serial.print(F(":"));
    alignValue(value, alignmentWidth);
    Serial.print(value);
}

void SpaceMouseHW_::PrintRawReads() {
    // Report back 0-1023 raw ADC 10-bit values if enabled
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        _printValue(i, _rawReads[i]);
    }
}

void SpaceMouseHW_::PrintCentered() {
    // Report back values for sensor axis after centering and mapping
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        _printValue(i, centered[i], 2);
    }
}
