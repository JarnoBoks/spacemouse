// TODO - Remove all ESP print and ESP_DBG from the AVR version
#include "AVRCommandHandler.hpp"

#include "commandhandler/CollectionCarrier/CollectionCarrier.hpp"

#include "sensor/calibration/SensorCalibrationManagerIdle.hpp"
#include "sensor/calibration/SensorCalibrationManagerMinMax.hpp"

#include "axis/axes/Axis.hpp"
#include "axis/config/AxisConfig.hpp"
#include "axis/config/AxisDirectionConfig.hpp"

#include "kinematics/kinematics.h"
#include "kinematics/kinematicsconfig.h"

#include "visitors/AxisConfigPrinter.h"
#include "visitors/SwitchYZPrinter.h"
#include "visitors/ExclusiveModePrinter.h"
#include "visitors/MinMaxPrinter.h"

#include "sensor/SensorCollection.hpp"
#include "sensor/config/SensorConfig.h"

#include "observers/DebugOutput/DebugOutputSensorsRaw.hpp"
#include "observers/DebugOutput/DebugOutputSensorsCentered.hpp"
#include "observers/DebugOutput/DebugOutputSensorsCenteredNoNewline.hpp" // Implementation of the ODebugOutputSensors class
#include "observers/DebugOutput/DebugOutputSensorsFiltered.hpp"
#include "observers/DebugOutput/DebugOutputAxesModified.hpp" // Implementation of the ODebugOutputAxes class
#include "observers/DebugOutput/DebugOutputAxesSensitivity.hpp"
#include "observers/DebugOutput/DebugOutputLoopFrequency.hpp"

#include <common/esp_print.h>
#include <common/freeRAM.h>
#include <Arduino.h> // For Serial

#define MAX_INPUT_SIZE 48 // Maximum size of the input buffer

// Commands to Handle
static const char CMD_IDLE[] PROGMEM = "IDLE";
static const char CMD_MINMAX[] PROGMEM = "MINMAX";
static const char CMD_DEBUG[] PROGMEM = "DEBUG";
static const char CMD_SENS[] PROGMEM = "SENS";
static const char CMD_GATE[] PROGMEM = "GATE";
static const char CMD_MODFUNC[] PROGMEM = "MODFUNC";
static const char CMD_INVERT[] PROGMEM = "INVERT";
static const char CMD_SWITCHXY[] PROGMEM = "SWITCHXY";
static const char CMD_EXLC[] PROGMEM = "EXLC";

/**
 * @brief Handles the input received from the serial monitor.
 * @details Parses the input string and executes the corresponding command.
 * @param input The input string received from the serial monitor.
 * @param inputsize The size of the input string.
 * @param bytesRead The number of bytes read from the input.
 */
void AVRCommandHandler::handleInput(char input[], const uint8_t inputsize, const int8_t bytesRead) {
    Serial.println(F("AVRCommandHandler::handleInput()")); // Print a message to indicate that we are handling the input
    FreeRAM::display_freeram();                            // Print the free RAM to the serial monitor
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

    if (strcmp_P(words[0], CMD_IDLE) == 0) {
        // Handle IDLE command
        executeIdle(words[1], words[2], --wordCount); // Execute the command with the retrieved parameters (wordCount decremented, while the first word is the command name)
    } else if (strcmp_P(words[0], CMD_MINMAX) == 0) {
        // Handle MINMAX command
        executeMinMax(words[1], words[2], --wordCount);
    } else if (strcmp_P(words[0], CMD_DEBUG) == 0) {
        // Handle DEBUG command
        executeDebug(words[1], words[2], --wordCount);
    } else if (strcmp_P(words[0], CMD_SENS) == 0) {
        // Handle SENS command
        executeSens(words[1], words[2], --wordCount);
    } else if (strcmp_P(words[0], CMD_GATE) == 0) {
        // Handle GATE command
        executeGate(words[1], words[2], --wordCount);
    } else if (strcmp_P(words[0], CMD_MODFUNC) == 0) {
        // Handle MODFUNC command
        executeModFunc(words[1], words[2], --wordCount);
    } else if (strcmp_P(words[0], CMD_INVERT) == 0) {
        // Handle INVERT command
        executeInvert(words[1], words[2], --wordCount);
    } else if (strcmp_P(words[0], CMD_SWITCHXY) == 0) {
        // Handle SWITCHXY command
        executeSwitchXY(words[1], words[2], --wordCount);
    } else if (strcmp_P(words[0], CMD_EXLC) == 0) {
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
    FreeRAM::display_freeram();                  // Print the free RAM to the serial monitor
    char inputBuffer[MAX_INPUT_SIZE];            // Buffer to store the input command
    memset(inputBuffer, 0, sizeof(inputBuffer)); // Clear the buffer
    uint8_t bytesRead = 0;

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

    Serial.print(F("---- Received: ")); // Print the input string
    Serial.println(inputBuffer);        // Print the input string

    handleInput(inputBuffer, sizeof(inputBuffer), bytesRead); // Call the handleInput function to process the input
}

void AVRCommandHandler::executeIdle(const char *param1, const char *param2, const uint8_t paramCount) {
    // Implementation for IDLE command
    if (getCollectionIdentifier() == nullptr) {
        ESP_PRINT(F("IdleCommand::execute: No collection identifier available"));
        return; // No collection identifier available, exit the function
    }
    if (getCollectionIdentifier()->getSensorCollection() == nullptr) {
        ESP_PRINT(F("IdleCommand::execute: No sensor collection available"));
        return; // No sensor collection available, exit the function
    }
    SensorCollection *sensorCollection = getCollectionIdentifier()->getSensorCollection();

    SensorCalibrationManagerIdle *m_SensorCalibrationManager = new SensorCalibrationManagerIdle(sensorCollection); // Create a new instance of the sensor calibration manager
    m_SensorCalibrationManager->activate(2000);                                                                    // Start the idle calibration with 2000 iterations
}

void AVRCommandHandler::executeMinMax(const char *param1, const char *param2, const uint8_t paramCount) {
    // Implementation for MINMAX command

    if (getCollectionIdentifier() == nullptr) {
        ESP_PRINT(F("MinMaxCommand::execute: No collection identifier available"));
        return; // No collection identifier available, exit the function
    }
    if (getCollectionIdentifier()->getSensorCollection() == nullptr) {
        ESP_PRINT(F("MinMaxCommand::execute: No sensor collection available"));
        return; // No sensor collection available, exit the function
    }
    SensorCollection *sensorCollection = getCollectionIdentifier()->getSensorCollection();

    if (paramCount == 0) {
        // No params provided, show config
        ESP_PRINT(F("MinMaxCommand::execute: Show config"));

        MinMaxPrinter Printer;

        for (uint8_t id = 0; id < cHW_MAX_SENSORS; id++) {
            Sensor *sensor = sensorCollection->getSensor(id); // Pointer to the sensor
            if (sensor == nullptr) {
                continue; // Skip if the sensor is not available
            }
            sensor->accept(Printer);              // Let the sensor accept the Printer visitor to print the sensor name
            sensor->getConfig()->accept(Printer); // Let the sensorconfig accept the Printer visitor to print the sensor configuration values
        }
        return;
    }

    if (paramCount == 1) {
        long requestedCalibration = 0; // Default value for the second word
        if (!convertWordNumber(param1, (long *)&requestedCalibration)) {
            return; // First parameter is not a number
        }

        ESP_PRINT(F("MinMaxCommand::execute: MinMax calibration requested: "));
        ESP_PRINT(requestedCalibration);
        m_SensorCalibrationManager = new SensorCalibrationManagerMinMax(sensorCollection); // Create a new instance of the sensor calibration manager
        if (requestedCalibration == 0) {
            ESP_PRINT(F("MinMaxCommand::execute: Start minmax calibration"));
            m_SensorCalibrationManager->activate();
        } else if (requestedCalibration == 1) {
            ESP_PRINT(F("MinMaxCommand::execute: Start minmax calibration and store in EEPROM"));
            m_SensorCalibrationManager->activate();
            // TODO - Store the values in EEPROM
        } else {
            ESP_PRINT(F("MinMaxCommand::execute: Unknown command"));
        }
    }
    if (paramCount == 2) {
        // REVIEW - Not implemented on the AVR version
        // Command received: MINMAX <+|-><sensorname> <value>
        // TODO - Add functionality for the second parameter

        // Get the value that has to be set
        long requestedValue = 0; // Default value for the second word
        if (!convertWordNumber(param2, (long *)&requestedValue)) {
            return; // Second parameter is not a number
        }

        // Get the direction (+ is maximum, - is minimum)
        char direction = param1[0]; // Get the first character of the first parameter

        // Get the sensor from the sensorname (fe. HES0 = 1, HES1 = 2, etc.)
        char *reqSensorName = (char *)param1 + 1;                    // Get the sensor name (skip the first character)
        Sensor *sensor = sensorCollection->getSensor(reqSensorName); // Get the sensor by its name

        // REVIEW - Failsafe: Sensor not found can be removed from Arduino.
        if (sensor == nullptr) {
            ESP_PRINT(F("MinMaxCommand::execute: Sensor not found"));
            return; // Sensor not found, exit the function
        }

        if (sensor != nullptr && direction == '+') {
            // Set the maximum value for the sensor
            ESP_PRINT(F("MinMaxCommand::execute: Set max for sensor "));
            sensor->getConfig()->setMax(requestedValue);

        } else if (sensor != nullptr && direction == '-') {
            // Set the minimum value for the sensor
            ESP_PRINT(F("MinMaxCommand::execute: Set min for sensor "));
            sensor->getConfig()->setMin(requestedValue);

        } else {
            ESP_PRINT(F("MinMaxCommand::execute: Unknown command"));
            return; // Invalid direction, exit the function
        }

        ESP_PRINT(F("MinMaxCommand::execute: Set minmax for sensor "));
        ESP_PRINT(param1);
        ESP_PRINT(F(" to "));
        ESP_PRINT(requestedValue);

        // Store the value in the EEPROM
        sensor->getConfig()->persist(sensor->getId());
        ESP_PRINT(F("MinMaxCommand::execute: Store minmax for sensor "));
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
    // Implementation for SENS command
    ESP_PRINT(F("SensCommand executed"));

    // Call the base class execute function to handle common functionality
    float requestedValue = executeAxis(param1, param2, paramCount);

    if (!m_DirectionConfig) {
        ESP_PRINT(F("SensCommand::execute: No direction config available"));
        return; // No direction config available, exit the function
    }

    m_DirectionConfig->sensitivity = requestedValue;     // Set the sensitivity value to the requested value
    m_Axis->getConfig()->persist(m_Axis->getAxisType()); // Store the value in the EEPROM
}

void AVRCommandHandler::executeGate(const char *param1, const char *param2, const uint8_t paramCount) {
    // Implementation for GATE command
    ESP_PRINT(F("GateCommand executed"));

    // Call the base class execute function to handle common functionality
    float requestedValue = executeAxis(param1, param2, paramCount);

    if (m_DirectionConfig == nullptr) {
        ESP_PRINT(F("GateCommand::execute: No direction config available"));
        return; // No direction config available, exit the function
    }

    m_DirectionConfig->gate = requestedValue;            // Set the gate value to the requested value
    m_Axis->getConfig()->persist(m_Axis->getAxisType()); // Store the value in the EEPROM
}

void AVRCommandHandler::executeModFunc(const char *param1, const char *param2, const uint8_t paramCount) {
    // Implementation for MODFUNC command
    ESP_PRINT(F("ModFuncCommand executed"));

    // Call the base class execute function to handle common functionality
    int requestedValue = static_cast<int>(executeAxis(param1, param2, paramCount));

    // Check if the directionConfig is valid
    if (!m_DirectionConfig) {
        ESP_PRINT(F("ModFuncCommand::execute: No direction config available"));
        return; // No direction config available, exit the function
    }

    m_DirectionConfig->modFuncType = static_cast<ModFunc_t>(requestedValue); // Set the mod function type to the requested value
    m_Axis->getConfig()->persist(m_Axis->getAxisType());                     // Store the value in the EEPROM
}

void AVRCommandHandler::executeInvert(const char *param1, const char *param2, const uint8_t paramCount) {
    // Implementation for INVERT command
    ESP_PRINT(F("InvertCommand executed"));

    // Call the base class execute function to handle common functionality
    float requestedValue = executeAxis(param1, param2, paramCount);

    // Check if the axis has a valid AxisDirectionConfig
    if (!m_DirectionConfig) {
        ESP_PRINT(F("InvertCommand::execute: No direction config available"));
        return; // No direction config available, exit the function
    }

    AxisConfig *axisConfig = m_Axis->getConfig(); // Get the axis configuration instance
    axisConfig->inversion = requestedValue;       // Set the inversion value to the requested value
    axisConfig->persist(m_Axis->getAxisType());   // Store the value in the EEPROM                          // REVIEW - Config should have context to the axis so the parameter is not needed
}

void AVRCommandHandler::executeSwitchXY(const char *param1, const char *param2, const uint8_t paramCount) {
    // Implementation for SWITCHXY command
    ESP_DBG(F("SwitchYZCommand executed"));

    if (paramCount == 0) {
        // No parameters provided, handle accordingly
        SwitchYZPrinter printer;
        Kinematics *kinematics = Kinematics::getInstance();
        kinematics->getConfig()->accept(printer); // Accept the printer visitor to print the YZ switch configuration
        return;
    }

    if (paramCount == 1) {
        // One parameter provided, handle accordingly

        long requestedLevel = 0; // Default value for the second word
        if (!convertWordNumber(param1, (long *)&requestedLevel)) {
            return; // First parameter is not a number
        }

        KinematicsConfig *config = Kinematics::getInstance()->getConfig(); // Get the kinematics configuration instance
        // TODO - Check for Null pointer (on ESP)
        config->switchYZ = requestedLevel;
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
        Kinematics *kinematics = Kinematics::getInstance();
        kinematics->getConfig()->accept(printer); // Accept the printer visitor to print the YZ switch configuration
        return;
    }

    if (paramCount == 1) {
        // One parameter provided, handle accordingly

        long requestedLevel = 0; // Default value for the second word
        if (!convertWordNumber(param1, (long *)&requestedLevel)) {
            return; // First parameter is not a number
        }
#ifdef ARCH_ESP32
        if (requestedLevel < 0 || requestedLevel > 1) {
            ESP_DBG(F("Invalid parameter value. Expected 0 or 1."));
            return; // Invalid parameter value
        }
#endif
        KinematicsConfig *config = Kinematics::getInstance()->getConfig(); // Get the kinematics configuration instance
        // TODO - Check for Null pointer (on ESP)
        config->exclusiveMode = requestedLevel;
        config->persist();

        return;
    }

    if (paramCount == 2) {
        // Not used - Ignore commands that have two parameters
    }
}

float AVRCommandHandler::executeAxis(const char *param1, const char *param2, uint8_t paramCount) {
    if (paramCount == 0) {
        // No params provided, show config
        ESP_PRINT(F("IAxisConfigCommand::execute: Show config"));

        AxisConfigPrinter printer;
        // REVIEW - Move this to the kinematics class?
        Kinematics *kinematics = Kinematics::getInstance();
        for (uint8_t id = 0; id < AxisType_t::LENGTH; id++) {
            Axis *axis = kinematics->getAxis((AxisType_t)id); // Pointer to the axis
            if (axis == nullptr) {
                continue; // Skip if the axis is not available
            }
            axis->accept(printer); // Accept the printer visitor to print the axis configuration
        }

        return 0;
    }

    if (paramCount == 1) {
        ESP_PRINT(F("IAxisConfigCommand::execute: First parameter: "));
        ESP_PRINT(param1);

        return 0;
    }

    if (paramCount == 2) {
        // Command received: SENS <+|-><axisname> <value>
        // TODO - Add functionality for the second parameter
        ESP_PRINT(F("IAxisConfigCommand::execute: Second parameter: "));
        ESP_PRINT(param2);

        float requestedValue = 0;
        // Get the value that has to be set
        if (!convertWordFloat(param2, &requestedValue)) {
            ESP_PRINT(F("IAxisConfigCommand::execute: Second parameter is not a float"));
            return 0; // Second parameter is not a float
        }

        // Get the direction (+ is maximum, - is minimum)
        char direction = param1[0]; // Get the first character of the first parameter

        // Get the axis from the axis name
        char *reqAxisName = (char *)param1 + 1;                   // Get the axis name (skip the first character)
        m_Axis = Kinematics::getInstance()->getAxis(reqAxisName); // Get the axis by its name

        // REVIEW - Failsafe: Axis not found can be removed from Arduino.
        if (m_Axis == nullptr) {
            ESP_PRINT(F("IAxisConfigCommand::execute: Axis not found"));
            return 0; // Axis not found, exit the function
        }

        if (direction == '+') {
            // Set the maximum value for the sensor
            ESP_PRINT(F("IAxisConfigCommand::execute: Set positive dir for axis "));
            m_DirectionConfig = &m_Axis->getConfig()->posConfig; // Set the direction config to the positive direction

        } else if (direction == '-') {
            // Set the minimum value for the sensor
            ESP_PRINT(F("IAxisConfigCommand::execute: Set negative dir for axis "));
            m_DirectionConfig = &m_Axis->getConfig()->negConfig; // Set the direction config to the negative direction

        } else {
            ESP_PRINT(F("IAxisConfigCommand::execute: Unknown command"));
            return 0; // Invalid direction, exit the function
        }

        return requestedValue; // Return the requested value
    }

    return 0; // Default return value
}

// DEVNOTE - The number conversion functions are both written with the usage of the strtod function, for code size purposes.
//           The strtod function is already used somewhere else in the code and to preserve space in the compiled code we do
//           use strtol or atof.
const bool AVRCommandHandler::convertWordNumber(const char *str, long *n) const {
    // Check if the string is a number (integer or float)
    char *endptr = nullptr;
    *n = (long)strtod(str, &endptr); // Convert to long integer

    if (*endptr != '\0') {
        // TODO: Check if compile size is smaller with the usage of F() macro
        return false; // Not a valid number
    }

    return true; // Valid number
}

const bool AVRCommandHandler::convertWordFloat(const char *str, float *value) const {

    // Check if the string is a number (integer or float)
    char *endptr = nullptr;
    *value = float(strtod(str, &endptr)); // Convert to float

    if (*endptr != '\0') {
        // TODO: Check if compile size is smaller with the usage of F() macro
        return false; // Not a valid float
    }

    return true; // Valid float
}

void AVRCommandHandler::DebugParamOff() {
    DetachCurrentObservers(); // Detach the previous observer if it exists
    // Implementation for DebugParamOff command
    ESP_PRINT(F("DebugParamOff executed"));
    // TODO - Add functionality for the DebugParamOff command
}

void AVRCommandHandler::DebugParamSensorInformationRaw() {
    DetachCurrentObservers(); // Detach the previous observer if it exists
    delay(1000);              // Delay to allow the observer to detach properly

    delay(1000); // Delay to allow the observer to detach properly
    // Instantiate the Observer for the RawSensor values and attach it to the hardware
    m_SensorObserver = new DebugOutputSensorsRaw();
    delay(1000);                                                                        // Delay to allow the observer to detach properly
    getCollectionIdentifier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the observer to the sensor collection
    delay(1000);                                                                        // Delay to allow the observer to detach properly
}

void AVRCommandHandler::DebugParamSensorInformationCentered() {
    DetachCurrentObservers(); // Detach the previous observer if it exists

    // Instantiate the Observer for the CenteredSensor values and attach it to the hardware
    m_SensorObserver = new DebugOutputSensorsCentered();
    getCollectionIdentifier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the observer to the sensor collection
}

void AVRCommandHandler::DebugParamSensorInformationFiltered() {
    DetachCurrentObservers(); // Detach the previous observer if it exists

    // Instantiate the Observer for the FilteredSensor values and attach it to the hardware
    m_SensorObserver = new DebugOutputSensorsFiltered();
    getCollectionIdentifier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the observer to the sensor collection
}

void AVRCommandHandler::DebugParamAxisInformation() {
    DetachCurrentObservers(); // Detach the previous observer if it exists

    // Instantiate the Observer for the Axis values and attach it to the hardware
    m_AxisObserver = new DebugOutputAxesSensitivity();
    getCollectionIdentifier()->getAxisCollection()->attachObserver(m_AxisObserver); // Attach the observer to the axis collection
}

void AVRCommandHandler::DebugParamSensorAxisInformation() {
    DetachCurrentObservers(); // Detach the previous observer if it exists

    // Instantiate the Observers and attach them to the hardware
    m_SensorObserver = new DebugOutputSensorsCenteredNoNewline();
    m_AxisObserver = new DebugOutputAxesModified();

    getCollectionIdentifier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the sensor observer to the sensor collection
    getCollectionIdentifier()->getAxisCollection()->attachObserver(m_AxisObserver);     // Attach the axis observer to the axis collection
}

void AVRCommandHandler::DebugParamSensorAxisKeysInformation() {
    DetachCurrentObservers(); // Detach the previous observer if it exists

    // Instantiate the Observers and attach them to the hardware
    m_SensorObserver = new DebugOutputSensorsCenteredNoNewline();
    m_AxisObserver = new DebugOutputAxesModified();

    getCollectionIdentifier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the sensor observer to the sensor collection
    getCollectionIdentifier()->getAxisCollection()->attachObserver(m_AxisObserver);     // Attach the axis observer to the axis collection
}

void AVRCommandHandler::DebugParamLoopFrequency() {
    DetachCurrentObservers(); // Detach the previous observer if it exists

    // Instantiate the Observer for the Loop Frequency values and attach it to the hardware
    m_LoopFrequencyObserver = new DebugOutputLoopFrequency();
    getCollectionIdentifier()->getAxisCollection()->attachObserver(m_LoopFrequencyObserver); // Attach the observer to the axis collection
}

void AVRCommandHandler::DetachCurrentObservers() {
    Serial.println(F("AVRCommandHandler::DetachCurrentObservers()")); // Print a message to indicate that we are detaching the observers
    if (m_AxisObserver != nullptr) {
        getCollectionIdentifier()->getAxisCollection()->detachObserver(m_AxisObserver); // Detach the observer from the axis collection
        delete m_AxisObserver;                                                          // Delete the previous observer if it exists
        m_AxisObserver = nullptr;                                                       // Set the observer pointer to null
    }

    if (m_SensorObserver != nullptr) {
        getCollectionIdentifier()->getSensorCollection()->detachObserver(m_SensorObserver); // Detach the observer from the sensor collection
        delete m_SensorObserver;                                                            // Delete the previous observer if it exists
        m_SensorObserver = nullptr;                                                         // Set the observer pointer to null
    }

    if (m_LoopFrequencyObserver != nullptr) {
        getCollectionIdentifier()->getAxisCollection()->detachObserver(m_LoopFrequencyObserver); // Detach the observer from the axis collection
        delete m_LoopFrequencyObserver;                                                          // Delete the previous observer if it exists
        m_LoopFrequencyObserver = nullptr;                                                       // Set the observer pointer to null
    }
}
