#include "AVRCommandHandler.hpp"

// Collections
#include "commandhandler/CollectionCarrier/CollectionCarrier.hpp"
#include <knob/KnobAxisCollection.hpp>
#include "sensor/SensorCollection.hpp"

// Sensor Calibration
#include <sensor/calibrator/Calibrator.hpp>
#include "sensor/calibrator/states/CalibratorStateIdle.hpp"
#include "sensor/calibrator/states/CalibratorStateMinMax.hpp"

// Axes
#include <knob/axis/KnobAxis.hpp>
#include <knob/axis/config/KnobAxisConfig.hpp>
#include <knob/axis/config/KnobAxisDirectionConfig.hpp>

// Sensors
#include "sensor/config/SensorConfig.hpp"

// Kinematics
#include <kinematics/Kinematics.hpp>
#include <kinematics/config/kinematicsconfig.hpp>

// Visitors
#include <visitors/printers/AxisConfigPrinter.hpp>
#include <visitors/printers/SwitchYZPrinter.hpp>
#include <visitors/printers/ExclusiveModePrinter.hpp>
#include <visitors/printers/SensorNamePrinter.hpp>
#include <visitors/printers/SensorConfigMinMaxPrinter.hpp>
#include <visitors/printers/SensorIdleDeadzonePrinter.hpp>

// Observers
#include "observers/DebugOutput/DebugOutputSensorsRaw.hpp"
#include "observers/DebugOutput/DebugOutputSensorsCentered.hpp"
#include "observers/DebugOutput/DebugOutputSensorsFiltered.hpp"
#include "observers/DebugOutput/DebugOutputAxesModified.hpp" // Implementation of the ODebugOutputAxes class
#include "observers/DebugOutput/DebugOutputAxesSensitivity.hpp"
#include "observers/DebugOutput/DebugOutputLoopFrequency.hpp"

// Helpers & Common
#include <common/esp_print.h>
#include <Arduino.h> // For Serial

// Maximum size of the input buffer
#define MAX_INPUT_SIZE 48

// Input commands that can be handled
static const char CMD_IDLE_P[] PROGMEM = "IDLE";
static const char CMD_MINMAX_P[] PROGMEM = "MINMAX";
static const char CMD_DEBUG_P[] PROGMEM = "DEBUG";
static const char CMD_SENS_P[] PROGMEM = "SENS";
static const char CMD_GATE_P[] PROGMEM = "GATE";
static const char CMD_MODFUNC_P[] PROGMEM = "MODFUNC";
static const char CMD_INVERT_P[] PROGMEM = "INVERT";
static const char CMD_SWITCHXY_P[] PROGMEM = "SWITCHXY";
static const char CMD_EXCL_P[] PROGMEM = "EXCL"; // Note: Old versions of the SpaceMouse used "EXLC" as command, but this is now deprecated.

/**
 * @brief Handles the input received from the serial monitor.
 * @details Parses the input string and executes the corresponding command.
 * @param input The input string received from the serial monitor.
 * @param inputsize The size of the input string.
 * @param bytesRead The number of bytes read from the input.
 */
void AVRCommandHandler::handleInput(char input[], const uint8_t inputsize, const int8_t bytesRead) {
    if (bytesRead == 0) {
        return; // No input received, exit the function
    }

    // Retrieve the command name and parameters from the input
    // Assuming input is a text like "command param1 param2"

    char *wordPtr = NULL;  // Pointer to the current word buffer
    char *words[3] = {};   // Array of word pointers
    uint8_t wordCount = 0; // The number of words/items that are in the input string.

    wordPtr = strtok(input, " ");                                // Tokenize the input string by using spaces as the delimiter.
    while (wordPtr != NULL && wordPtr - input < inputsize - 1) { // cast to char to prevent signed/unsigned comparison. Will fit easily due to the buffer size of 64.
        words[wordCount++] = wordPtr;                            // Store the token in the words array and increment wordCount
        if (wordCount >= 3) {
            break; // Stop if we have processed three words
        }
        wordPtr = strtok(NULL, " "); // Get the next token
    }

    // DEVNOTE The ESP32 version delegates the command to the registered commands for execution, the AVR version does that on its own.

    if (strcmp_P(words[0], CMD_IDLE_P) == 0) {
        DetachCurrentObservers(); // Detach the previous observer if it exists
        // Handle IDLE command
        executeIdle(words[1], words[2], --wordCount); // Execute the command with the retrieved parameters (wordCount decremented, while the first word is the command name)
    } else if (strcmp_P(words[0], CMD_MINMAX_P) == 0) {
        DetachCurrentObservers(); // Detach the previous observer if it exists
        // Handle MINMAX command
        executeMinMax(words[1], words[2], --wordCount);
    } else if (strcmp_P(words[0], CMD_DEBUG_P) == 0) {
        DetachCurrentObservers(); // Detach the previous observer if it exists
        // Handle DEBUG command
        executeDebug(words[1], words[2], --wordCount);
    } else if (strcmp_P(words[0], CMD_SENS_P) == 0) {
        DetachCurrentObservers(); // Detach the previous observer if it exists
        // Handle SENS command
        executeSens(words[1], words[2], --wordCount);
    } else if (strcmp_P(words[0], CMD_GATE_P) == 0) {
        DetachCurrentObservers(); // Detach the previous observer if it exists
        // Handle GATE command
        executeGate(words[1], words[2], --wordCount);
    } else if (strcmp_P(words[0], CMD_MODFUNC_P) == 0) {
        DetachCurrentObservers(); // Detach the previous observer if it exists
        // Handle MODFUNC command
        executeModFunc(words[1], words[2], --wordCount);
    } else if (strcmp_P(words[0], CMD_INVERT_P) == 0) {
        DetachCurrentObservers(); // Detach the previous observer if it exists
        // Handle INVERT command
        executeInvert(words[1], words[2], --wordCount);
    } else if (strcmp_P(words[0], CMD_SWITCHXY_P) == 0) {
        DetachCurrentObservers(); // Detach the previous observer if it exists
        // Handle SWITCHXY command
        executeSwitchXY(words[1], words[2], --wordCount);
    } else if (strcmp_P(words[0], CMD_EXCL_P) == 0) {
        DetachCurrentObservers(); // Detach the previous observer if it exists
        // Handle EXLC command
        executeExlc(words[1], words[2], --wordCount);
    } else {
        Serial.println(F("Unknown Command"));
        return; // Unknown command, exit the function
    }
}

/**
 * @brief   Parses the input received from the serial monitor in to a buffer and calls the handleInput function to process the input.
 * @details Reads the input from the serial monitor and stores it in a buffer.
 *          The input is terminated by a newline character or when the buffer is full.
 */
void AVRCommandHandler::parseSerialMonitorInput() {
    uint8_t bytesRead = 0;
    char inputBuffer[MAX_INPUT_SIZE];            // Buffer to store the input command
    memset(inputBuffer, 0, sizeof(inputBuffer)); // Clear the buffer

    // Read the input into the buffer until a newline character or buffer limit
    while (Serial.available() > 0 && bytesRead < sizeof(inputBuffer) - 1) {
        //  Read a character from the serial input
        char c = Serial.read();
        if (c == '\n') {
            break; // Stop reading at newline
        }
        if (c == '\r') {
            continue; // Ignore carriage return
        }
        inputBuffer[bytesRead++] = toupper(c);
    }

    inputBuffer[bytesRead] = '\0'; // Null-terminate the string

    handleInput(inputBuffer, sizeof(inputBuffer), bytesRead); // Call the handleInput function to process the input
}

#define PRM_MIN_IT 500  // Minimum number of iterations for idle calibration
#define PRM_MAX_IT 5000 // Maximum number of iterations for idle calibration
void AVRCommandHandler::executeIdle(const char *param1, const char *param2, const uint8_t paramCount) {

    if (!getCollectionCarrier()) {
        ESP_ERROR("No collection identifier");
        return;
    }

    SensorCollection *sensorCollection = getCollectionCarrier()->getSensorCollection();
    if (!sensorCollection) {
        ESP_ERROR("No sensor collection");
        return;
    }

    if (paramCount == 0) {
        // No params provided, show the current idle positions and deadzone values of the sensors
        SensorIdleDeadzonePrinter printer;
        sensorCollection->accept(printer); // Accept the printer visitor to print the information for this sensor
        return;
    }

    if (paramCount == 1) {
        long requestedIterations = 0;
        if (!convertWordNumber(param1, (long *)&requestedIterations)) {
            ESP_WARN("Param not number");
            return;
        }

        requestedIterations = (requestedIterations < PRM_MIN_IT) ? PRM_MIN_IT : requestedIterations; // Ensure minimum iterations
        requestedIterations = (requestedIterations > PRM_MAX_IT) ? PRM_MAX_IT : requestedIterations; // Ensure maximum iterations

        Calibrator *calibrator = sensorCollection->getCalibrator(); // Get the calibrator instance from the sensor collection
        RETURN_E_IF_NULL(calibrator, "No calibrator found in sensor collection");

        if (!calibrator->start(new CalibratorStateIdle(requestedIterations))) {
            ESP_WARN("Failed to start idle calibration");
            return; // Failed to start the calibration, exit the function
        }
        return;
    }

    if (paramCount == 2) {
        // Not used - Ignore commands that have two parameters
        ESP_WARN("Too many parameters for IDLE command");
        return;
    }
}

void AVRCommandHandler::executeMinMax(const char *param1, const char *param2, const uint8_t paramCount) {
    // Implementation for MINMAX command
    if (!getCollectionCarrier()) {
        ESP_ERROR("Carrier N/A");
        return;
    }
    if (!(getCollectionCarrier()->getSensorCollection())) {
        ESP_ERROR("No sensor collection available");
        return; // No sensor collection available, exit the function
    }
    SensorCollection *sensorCollection = getCollectionCarrier()->getSensorCollection();

    if (paramCount == 0) {
        // No params provided, show config
        ESP_PRINT(F("MinMaxCommand::execute: Show config"));

        SensorConfigMinMaxPrinter MinMaxPrinter;
        SensorNamePrinter NamePrinter;

        for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
            Sensor *sensor = sensorCollection->getSensor(id); // Pointer to the sensor
            if (sensor == nullptr) {
                continue; // Skip if the sensor is not available
            }

            sensor->accept(NamePrinter);                // Let the sensor accept the Printer visitor to print the sensor name
            sensor->getConfig()->accept(MinMaxPrinter); // Let the sensorconfig accept the Printer visitor to print the sensor configuration values
        }
        return;
    }

    if (paramCount == 1) {
        bool requestedPersistence = false;
        if (!convertWordBool(param1, &requestedPersistence)) {
            ESP_WARN("Param not boolean");
            return;
        }

        Calibrator *calibrator = sensorCollection->getCalibrator(); // Get the calibrator instance from the sensor collection
        RETURN_E_IF_NULL(calibrator, "No calibrator found in sensor collection");

        if (!calibrator->start(new CalibratorStateMinMax(requestedPersistence))) {
            ESP_WARN("Failed to start minmax calibration");
            return; // Failed to start the calibration, exit the function
        }
    }
    if (paramCount == 2) {
        // Command received: MINMAX <+|-><sensorname> <value>

        // Get the value that has to be set
        long requestedValue = 0; // Default value for the second word
        if (!convertWordNumber(param2, (long *)&requestedValue)) {
            ESP_WARN("Param not float");
            return; // Second parameter is not a number
        }

        // Get the direction (+ is maximum, - is minimum)
        char direction = param1[0]; // Get the first character of the first parameter

        // Get the sensor from the sensorname (fe. HES0 = 1, HES1 = 2, etc.)
        char *reqSensorName = (char *)param1 + 1;                    // Get the sensor name (skip the first character)
        Sensor *sensor = sensorCollection->getSensor(reqSensorName); // Get the sensor by its name

        if (sensor == nullptr) {
            ESP_ERROR("Sensor not found");
            return; // Sensor not found, exit the function
        }

        if (direction == '+') {
            // Set the maximum value for the sensor
            ESP_INFO("Set max for sensor ");
            sensor->getConfig()->setMax(requestedValue);

        } else if (direction == '-') {
            // Set the minimum value for the sensor
            ESP_INFO("Set min for sensor ");
            sensor->getConfig()->setMin(requestedValue);

        } else {
            ESP_WARN("Unknown command");
            return; // Invalid direction, exit the function
        }

        ESP_INFO2("Set minmax for sensor ", param1);
        ESP_INFO2("to ", requestedValue);

        // Store the value in the EEPROM
        sensor->getConfig()->persist();
        ESP_INFO("Store minmax for sensor");
    }
}

void AVRCommandHandler::executeDebug(const char *param1, const char *param2, const uint8_t paramCount) {

    // Implementation for DEBUG command
    if (paramCount == 0 || paramCount >= 2) {
        return;
    }

    // Command is called with one or two parameters. We assume the requested level is set in the first parameter.
    // The second parameter is not used in this implementation, but it can be used for future extensions.

    long requestedLevel = 0; // Default value for the second word
    if (!convertWordNumber(param1, (long *)&requestedLevel)) {
        return; // First parameter is not a number
    }

    switch (requestedLevel) {
    case -1:
        DebugParamOff(); // Set the state to off
        break;
    case 0:
        DebugParamOff(); // Set the state to off
        break;
    case 1:
        DebugParamSensorInformationRaw();
        break;
    case 2:
        DebugParamSensorInformationCentered();
        break;
    case 3:
        // Filtered sensors
        DebugParamSensorInformationFiltered();
        break;
    case 4:
        // Translation and rotation values - without modifier function, inversion, YZ switching nor Exclusivemode applied
        DebugParamAxisInformation();
        break;
    case 5:
        // Centered values (2nd debug) and translation & rotation values side by side for direct reference. Modifier function and inversion are applied. Any configured YZ switching or Exclusivemode is not applied.
        DebugParamSensorAxisInformation();
        break;
    case 6:
        // Debug 5 and the key state after applying the kill-key functionality
        DebugParamSensorAxisKeysInformation();
        break;
    case 7:
        // Centered values, translation & rotation, keystate (with kill switch) and exclusive mode applied
        // TODO setState(new DebugParamSensorInformationCentered(this));
        break;
    case 8:
        // Report the frequency of the loop()
        DebugParamLoopFrequency();
        break;
    case 9:
        // Report the bits and bytes send as button codes
        // TODO setState(new DS_ButtonCodesInformation(this));
        break;
    case 10:
        // Report details about the encoder wheel, if ROTARY_AXIS > 0 or ROTARY_KEYS>0
        // TODO setState(new DS_EncoderWheelInformation(this));
        break;
    default:
        // Handle unknown debug level
        break;
    }
}

void AVRCommandHandler::executeSens(const char *param1, const char *param2, const uint8_t paramCount) {

    // Call the base class execute function to handle common functionality
    float requestedValue = executeAxis(param1, param2, paramCount);
    if (requestedValue < 0) {
        // No update of the configuration parameters possible or needed.
        return;
    }

    bool touched = false; // Flag to indicate if the sensitivity was set
    for (uint8_t i = 0; i < NUM_AX_DIRCFG; i++) {
        if (m_knobAxisDirectionConfig[i]) {
            m_knobAxisDirectionConfig[i]->setSensitivity(requestedValue);
            touched |= true;
        }
    }

    if (touched) {
        m_knobAxis->getConfig()->persist(); // Store the value in non-volatile memory
    }
}

void AVRCommandHandler::executeGate(const char *param1, const char *param2, const uint8_t paramCount) {
    // Call the base class execute function to handle common functionality
    float requestedValue = executeAxis(param1, param2, paramCount);
    if (requestedValue < 0) {
        // No update of the configuration parameters possible or needed.
        return;
    }

    bool touched = false; // Flag to indicate if the sensitivity was set
    for (uint8_t i = 0; i < NUM_AX_DIRCFG; i++) {
        if (m_knobAxisDirectionConfig[i]) {
            m_knobAxisDirectionConfig[i]->setGate(requestedValue);
            touched |= true;
        }
    }

    if (touched) {
        m_knobAxis->getConfig()->persist(); // Store the value in non-volatile memory
    }
}

void AVRCommandHandler::executeModFunc(const char *param1, const char *param2, const uint8_t paramCount) {
    // Call the base class execute function to handle common functionality
    float requestedValue = executeAxis(param1, param2, paramCount);
    ModFunc_t mF = static_cast<ModFunc_t>(requestedValue); // Cast the requested value to ModFunc_t

    bool touched = false; // Flag to indicate if the sensitivity was set
    for (uint8_t i = 0; i < NUM_AX_DIRCFG; i++) {
        if (m_knobAxisDirectionConfig[i]) {
            m_knobAxisDirectionConfig[i]->setModFuncType(mF);
            touched |= true;
        }
    }

    if (touched) {
        m_knobAxis->getConfig()->persist(); // Store the value in non-volatile memory
    }
}

void AVRCommandHandler::executeInvert(const char *param1, const char *param2, const uint8_t paramCount) {
    // Implementation for INVERT command
    ESP_PRINT(F("InvertCommand executed"));

    // Call the base class execute function to handle common functionality
    float requestedValue = executeAxis(param1, param2, paramCount);

    // Check if the axis has a valid AxisDirectionConfig
    if (!m_knobAxisDirectionConfig) {
        ESP_PRINT(F("InvertCommand::execute: No direction config available"));
        return; // No direction config available, exit the function
    }

    KnobAxisConfig *cfgKnobAxis = m_knobAxis->getConfig(); // Get the axis configuration instance
    cfgKnobAxis->inversion = requestedValue;               // Set the inversion value to the requested value
    cfgKnobAxis->persist();                                // Store the value in non-volatile memory
}

void AVRCommandHandler::executeSwitchXY(const char *param1, const char *param2, const uint8_t paramCount) {
    // Implementation for SWITCHXY command
    ESP_DBG(F("SwitchYZCommand executed"));

    if (paramCount == 0) {
        // No parameters provided, handle accordingly
        SwitchYZPrinter printer;
        Kinematics *kinematics = m_CollectionCarrier->getKinematics();
        kinematics->getConfig()->accept(printer); // Accept the printer visitor to print the YZ switch configuration
        return;
    }

    if (paramCount == 1) {
        // One parameter provided, handle accordingly

        long requestedLevel = 0; // Default value for the second word
        if (!convertWordNumber(param1, (long *)&requestedLevel)) {
            return; // First parameter is not a number
        }

        KinematicsConfig *config = m_CollectionCarrier->getKinematics()->getConfig(); // Get the kinematics configuration instance
        // TODO - Check for Null pointer (on ESP)
        config->setSwitchYZ(requestedLevel);
        config->persist();

        return;
    }

    if (paramCount == 2) {
        // Not used - Ignore commands that have two parameters
    }
}

void AVRCommandHandler::executeExlc(const char *param1, const char *param2, const uint8_t paramCount) {
    // Implementation for EXLC command
    ESP_DBG(F("ExclusiveCommand executed"));

    if (paramCount == 0) {
        // No parameters provided, handle accordingly
        ExclusiveModePrinter printer;
        Kinematics *kinematics = m_CollectionCarrier->getKinematics();
        kinematics->getConfig()->accept(printer); // Accept the printer visitor to print the YZ switch configuration
        return;
    }

    if (paramCount == 1) {
        // One parameter provided, handle accordingly
        long requestedLevel = 0; // Default value for the second word
        if (!convertWordNumber(param1, (long *)&requestedLevel)) {
            return; // First parameter is not a number
        }

        KinematicsConfig *config = m_CollectionCarrier->getKinematics()->getConfig(); // Get the kinematics configuration instance
        // TODO - Check for Null pointer (on ESP)
        config->setExclusiveMode(requestedLevel);
        config->persist();

        return;
    }

    if (paramCount == 2) {
        // Not used - Ignore commands that have two parameters
    }
}

/**
 * @brief Executes axis-related commands.
 * @details This function handles the execution of axis-related commands based on the provided parameters.
 *          It can be used to set the sensitivity, gate, or mod function type for a specific axis.
 * @param param1 The first parameter (axis name or [+|-]axisname ).
 * @param param2 The second parameter (value).
 * @param paramCount The number of parameters provided.
 * @return The result of the command execution.
 * @retval -1 If no, incorrect or unsufficient parameters are provided.
 * @retval The requested value (float) if two parameters are provided and the command is executed successfully. The m_knobAxisDirectionConfig array is updated to point to the correct AxisDirectionConfig object(s).
 */
float AVRCommandHandler::executeAxis(const char *param1, const char *param2, uint8_t paramCount) {
    if (paramCount == 0) {
        // No params provided, show current configuration values of the axes.
        AxisConfigPrinter printer;
        m_CollectionCarrier->getKnobAxisCollection()->accept(printer);
        return -1;
    }

    if (paramCount == 1) {
        return -1;
    }

    if (paramCount == 2) {
        // Command received, fe. SENS [+|-]<axisname> <value>

        // Erase the m_knobAxisDirectionConfig pointers
        for (uint8_t i = 0; i < NUM_AX_DIRCFG; i++) {
            m_knobAxisDirectionConfig[i] = nullptr;
        }

        float requestedValue = 0;
        if (!convertWordFloat(param2, &requestedValue)) {
            return -1; // Second parameter is not a float
        }

        char directionChar = param1[0]; // The direction entered in the user command (first character of the first parameter, + or - or axisname's first character)
        if (directionChar == '+' || directionChar == '-') {
            // The first character is a direction
            char *reqAxisName = (char *)param1 + 1; // Pointer to the axis name (skip the first character)
            m_knobAxis = m_CollectionCarrier->getKnobAxisCollection()->getAxis(reqAxisName);
            if (m_knobAxis == nullptr) {
                return -1; // Error: KnobAxis not found, exit the function
            }

            if (directionChar == '+') {
                // The positive direction config should be used
                m_knobAxisDirectionConfig[0] = &m_knobAxis->getConfig()->posConfig;

            } else if (directionChar == '-') {
                // The negative direction config should be used
                m_knobAxisDirectionConfig[0] = &m_knobAxis->getConfig()->negConfig;
            }

        } else {
            // The first character is not a direction, test if the KnobAxis name is specified.
            m_knobAxis = m_CollectionCarrier->getKnobAxisCollection()->getAxis(param1);
            if (m_knobAxis == nullptr) {
                return -1; // KnobAxis is not found, exit the function
            }

            // There is an axis name, but no direction provided. Both directions have to be updated.
            m_knobAxisDirectionConfig[0] = &m_knobAxis->getConfig()->posConfig;
            m_knobAxisDirectionConfig[1] = &m_knobAxis->getConfig()->negConfig;
        }

        return requestedValue; // Return the requested value
    }

    return -1; // Return 0 if no valid number of parameters is provided.
}

/**
 * @brief Converts a string to a long integer.
 * @param str The string to convert.
 * @param n Pointer to the long integer to store the result.
 * @return The result of the conversion.
 * @retval true Conversion successful
 * @retval false Conversion failed
 * @note The function uses strtod to convert the string to a long integer for code size purposes.
 *       The strtod function is already used somewhere else in the code and to preserve space in the compiled code
 *       strtol or atof is not used.
 */
const bool AVRCommandHandler::convertWordNumber(const char *str, long *n) const {
    // Check if the string is a number (integer or float)
    char *endptr = nullptr;
    *n = (long)strtod(str, &endptr); // Convert to long integer

    return (*endptr == '\0'); // Check if the conversion was successful
}

/**
 * @brief Converts a string to a boolean.
 * @details The function checks if the first character of the string is '0' or '1'.
 *          If the first character is '0', it sets the boolean to false, otherwise it sets it to true.
 * @param str The string to convert.
 * @param n Pointer to the bool to store the result.
 * @return The result of the conversion.
 * @retval true Conversion successful
 * @retval false Conversion failed
 */
const bool AVRCommandHandler::convertWordBool(const char *str, bool *n) const {
    if (str == nullptr || *str == '\0') {
        return false; // If the string is null or empty, conversion fails
    }

    if (*str == '0') {
        *n = false; // If the first character is '0', set boolean to false
    } else if (*str == '1') {
        *n = true; // If the first character is '1', set boolean to true
    } else {
        return false; // If the first character is not '0' or '1', conversion fails
    }

    return true; // Conversion successful
}

/**
 * @brief Converts a string to a float.
 * @param str The string to convert.
 * @param value Pointer to the float to store the result.
 * @return The result of the conversion.
 * @retval true Conversion successful
 * @retval false Conversion failed
 * @note The function uses strtod to convert the string to a long integer for code size purposes.
 *       The strtod function is already used somewhere else in the code and to preserve space in the compiled code
 *       strtol or atof is not used.
 */
const bool AVRCommandHandler::convertWordFloat(const char *str, float *value) const {

    // Check if the string is a number (integer or float)
    char *endptr = nullptr;
    *value = float(strtod(str, &endptr)); // Convert to float

    return (*endptr == '\0'); // Check if the conversion was successful
}

void AVRCommandHandler::DebugParamOff() {
    // REMOVE DetachCurrentObservers(); // Detach the previous observer if it exists
    //  Implementation for DebugParamOff command
    ESP_PRINT(F("DebugParamOff executed"));
    // DEVNOTE - Empty state, no functionality necessary
}

void AVRCommandHandler::DebugParamSensorInformationRaw() {
    // REMOVE DetachCurrentObservers(); // Detach the previous observer if it exists

    // Instantiate the Observer for the RawSensor values and attach it to the hardware
    m_SensorObserver = new DebugOutputSensorsRaw();
    getCollectionCarrier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the observer to the sensor collection
}

void AVRCommandHandler::DebugParamSensorInformationCentered() {
    // REMOVE DetachCurrentObservers(); // Detach the previous observer if it exists

    // Instantiate the Observer for the CenteredSensor values and attach it to the hardware
    m_SensorObserver = new DebugOutputSensorsCentered();
    getCollectionCarrier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the observer to the sensor collection
}

void AVRCommandHandler::DebugParamSensorInformationFiltered() {
    // REMOVE DetachCurrentObservers(); // Detach the previous observer if it exists

    // Instantiate the Observer for the FilteredSensor values and attach it to the hardware
    m_SensorObserver = new DebugOutputSensorsFiltered();
    getCollectionCarrier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the observer to the sensor collection
}

void AVRCommandHandler::DebugParamAxisInformation() {
    // REMOVE DetachCurrentObservers(); // Detach the previous observer if it exists

    // Instantiate the Observer for the KnobAxis values and attach it to the hardware
    m_AxisObserver = new DebugOutputAxesSensitivity();
    getCollectionCarrier()->getKnobAxisCollection()->attachObserver(m_AxisObserver); // Attach the observer to the axis collection
}

void AVRCommandHandler::DebugParamSensorAxisInformation() {
    // REMOVE DetachCurrentObservers(); // Detach the previous observer if it exists

    // Instantiate the Observers and attach them to the hardware
    m_SensorObserver = new DebugOutputSensorsCentered(false); // false means no newline at the end
    m_AxisObserver = new DebugOutputAxesModified();

    getCollectionCarrier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the sensor observer to the sensor collection
    getCollectionCarrier()->getKnobAxisCollection()->attachObserver(m_AxisObserver); // Attach the axis observer to the axis collection
}

void AVRCommandHandler::DebugParamSensorAxisKeysInformation() {
    // REMOVE DetachCurrentObservers(); // Detach the previous observer if it exists

    // Instantiate the Observers and attach them to the hardware
    m_SensorObserver = new DebugOutputSensorsCentered(false); // false means no newline at the end
    m_AxisObserver = new DebugOutputAxesModified();

    getCollectionCarrier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the sensor observer to the sensor collection
    getCollectionCarrier()->getKnobAxisCollection()->attachObserver(m_AxisObserver); // Attach the axis observer to the axis collection
}

void AVRCommandHandler::DebugParamLoopFrequency() {
    // REMOVE DetachCurrentObservers(); // Detach the previous observer if it exists

    // Instantiate the Observer for the Loop Frequency values and attach it to the hardware
    m_LoopFrequencyObserver = new DebugOutputLoopFrequency();
    getCollectionCarrier()->getKnobAxisCollection()->attachObserver(m_LoopFrequencyObserver); // Attach the observer to the axis collection
}

void AVRCommandHandler::DetachCurrentObservers() {
    if (m_AxisObserver != nullptr) {
        getCollectionCarrier()->getKnobAxisCollection()->detachObserver(m_AxisObserver); // Detach the observer from the axis collection
        delete m_AxisObserver;                                                           // Delete the previous observer if it exists
        m_AxisObserver = nullptr;                                                        // Set the observer pointer to null
    }

    if (m_SensorObserver != nullptr) {
        getCollectionCarrier()->getSensorCollection()->detachObserver(m_SensorObserver); // Detach the observer from the sensor collection
        delete m_SensorObserver;                                                         // Delete the previous observer if it exists
        m_SensorObserver = nullptr;                                                      // Set the observer pointer to null
    }

    if (m_LoopFrequencyObserver != nullptr) {
        getCollectionCarrier()->getKnobAxisCollection()->detachObserver(m_LoopFrequencyObserver); // Detach the observer from the axis collection
        delete m_LoopFrequencyObserver;                                                           // Delete the previous observer if it exists
        m_LoopFrequencyObserver = nullptr;                                                        // Set the observer pointer to null
    }
}
