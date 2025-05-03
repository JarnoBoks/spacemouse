#include "DebugCommand.h"

// Include necessary headers for the Debug states
#include "DebugParam/ParamOff.h"
#include "DebugParam/ParamSensorInformation.h"
#include "DebugParam/ParamAxisInformation.h"
#include "DebugParam/ParamSensorAxisInformation.h"
#include "DebugParam/ParamSensorAxisKeysInformation.h"
#include "DebugParam/ParamLoopFrequency.h"
// ...include other debug states...

/**
 * @brief This class handles the debug commands and manages the current debug state.
 * @details The class allows switching between different debug states and executing commands related to debugging.
 * It uses the IDebugState interface to define the behavior of different debug states.
 * The class also provides methods to set the current debug state and execute commands based on the parameters provided.
 * This class is essential for managing the debugging process and ensuring that the correct state is applied during operation. *
 */

DebugCommand::DebugCommand() : CommandBase(CMD_DEBUG), currentParam(new DebugParamOff()) {}

/**
 * @brief Destructor for the DebugCommand class.
 * @details Cleans up the current debug state by deleting it.
 */
DebugCommand::~DebugCommand() {
    delete currentParam;
}

/**
 * @brief Sets the current debug state.
 * @param state Pointer to the new debug state to be set.
 * @details Deletes the previous state and applies the new state.
 */
void DebugCommand::setState(IDebugParam *state) {
    delete currentParam;

    currentParam = state;
    currentParam->apply();
}

/**
 * @brief Gets the current debug state.
 * @return Pointer to the current debug state.
 */
IDebugParam *DebugCommand::getState() const { return currentParam; }

/**
 * @brief Stops the current debug command execution.
 * @details Removes the current debug state to clean up resources and stop any ongoing processes.
 */
void DebugCommand::stop() {
#if 0
    // Two options to stop the command execution:
    // 1. Delete the current debug state and set it to nullptr.
    // 2. Set the state to a new instance of DebugParamOff.
    // The first option is more efficient as it avoids creating a new instance of DebugParamOff.
    // The second option is more explicit and may be easier to understand for future developers.
    delete currentParam; // Clean up the current debug state
    currentParam = nullptr; // Set the current parameter to nullptr to indicate no active state
#endif
    setState(new DebugParamOff()); // Set the state to off
}

/**
 * @brief Executes the debug command based on the provided parameters.
 * @param param1 First parameter, typically the debug level.
 * @param param2 Second parameter (unused).
 * @param paramCount Number of parameters provided.
 * @details Handles switching between debug states or reporting the current state.
 */
void DebugCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    if (paramCount == 0) {
        currentParam->report();
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
        setState(new DebugParamOff());
        break;
    case 0:
        setState(new DebugParamOff());
        break;
    case 1:
        // Raw sensors
        setState(new DebugParamSensorInformationRaw());
        break;
    case 2:
        // Centered sensors
        setState(new DebugParamSensorInformationCentered());
        break;
    case 3:
        // Filtered sensors
        setState(new DebugParamSensorInformationFiltered());
        break;
    case 4:
        // Translation and rotation values - withoud modifier function, inversion, YZ switching nor Exclusivemode applied
        setState(new DebugParamAxisInformation());
        break;
    case 5:
        // Centered values (2nd debug) and translation & rotation values side by side for direct reference. Modifier function and inversion are applied. Any configured YZ switching or Exclusivemode is not applied.
        setState(new DebugParamSensorAxisInformation());
        break;
    case 6:
        // Debug 5 and the key state after applying the kill-key functionality
        setState(new DebugParamSensorAxisKeysInformation());
        break;
    case 7:
        // Centered values, translation & rotation, keystate (with kill switch) and exclusive mode applied
        // TODO setState(new DebugParamSensorInformationCentered());
        break;
    case 8:
        // Report the frequency of the loop()
        setState(new DebugParamLoopFrequency());
        break;
    case 9:
        // Report the bits and bytes send as button codes
        // TODO setState(new DS_ButtonCodesInformation());
        break;
    case 10:
        // Report details about the encoder wheel, if ROTARY_AXIS > 0 or ROTARY_KEYS>0
        // TODO setState(new DS_EncoderWheelInformation());
        break;
    default:
        // Handle unknown debug level
#ifndef ARDUINO_ARCH_AVR // Only for device with more flash memory
        Serial.println(F("Unknown debug mode"));
#endif
        break;
    }
}