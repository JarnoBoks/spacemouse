#include "DebugCommand.h"

// Include necessary headers for the Debug states
#include "DebugParam\ParamOff.hpp"
#include "DebugParam\ParamSensorInformationRaw.hpp"
#include "DebugParam\ParamSensorInformationCentered.hpp"
#include "DebugParam\ParamSensorInformationFiltered.hpp"
#include "DebugParam\ParamAxisInformation.hpp"
#include "DebugParam\ParamSensorAxisInformation.hpp"
#include "DebugParam\ParamSensorAxisKeysInformation.hpp"
#include "DebugParam\ParamSensorAxisKeysInformationExclusive.hpp"
#include "DebugParam\ParamLoopFrequency.hpp"
// ...include other debug states...

#include <common/esp_print.h> // For ESP_PRINT

/**
 * @brief Sets the current debug state and executes the new state.
 * @param state Pointer to the new debug state to be set.
 * @details Deletes the previous state (freeing the used memory).
 */
void DebugCommand::setState(IDebugParam *state) {
    delete currentState;

    currentState = state;
    currentState->apply();
}

/**
 * @brief Gets the current debug state.
 * @return Pointer to the current debug state.
 */
IDebugParam *DebugCommand::getState() const { return currentState; }

/**
 * @brief Stops the current debug command execution.
 * @details Removes the current debug state to clean up resources and stop any ongoing processes.
 */
void DebugCommand::stop() {
    setState(new DebugParamOff()); // Set the state to off
}

/**
 * @brief Executes a received debug command, by parsing the parameters and switching to the requested debug state.
 * @param param1 First parameter, typically the debug level.
 * @param param2 Second parameter (unused).
 * @param paramCount Number of parameters provided.
 * @details Handles switching between debug states or reporting the current state.
 */
void DebugCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    if (paramCount == 0 || paramCount >= 2) {
        ESP_INFO("Mallformed command");
        return;
    }

    // Command is called with one or two parameters. We assume the requested level is set in the first parameter.
    // The second parameter is not used in this implementation, but it can be used for future extensions.

    long requestedLevel = 0;
    if (!convertWordNumber(param1, (long *)&requestedLevel)) {
        ESP_WARN("Param not number");
        return;
    }

    switch (requestedLevel) {
    case 0:
        setState(new DebugParamOff());
        break;
    case 1:
        // Raw sensors
        setState(new DebugParamSensorInformationRaw(this));
        break;
    case 2:
        // Centered sensors
        setState(new DebugParamSensorInformationCentered(this));
        break;
    case 3:
        // Filtered sensors
        setState(new DebugParamSensorInformationFiltered(this));
        break;
    case 4:
        // Translation and rotation values - without modifier function, inversion, YZ switching nor Exclusivemode applied
        setState(new DebugParamAxisInformation(this));
        break;
    case 5:
        // Centered Sensor values (2nd debug) and Translation & Rotation values side by side for direct reference. Modifier function and inversion are applied.
        // Any configured YZ switching or Exclusivemode is not applied.
        setState(new DebugParamSensorAxisInformation(this));
        break;
    case 6:
        // Debug 5 and the key state after applying the kill-key functionality
        setState(new DebugParamSensorAxisKeysInformation(this));
        break;
    case 7:
        // Centered values, translation & rotation, keystate (with kill switch) and inversion & exclusive mode applied (the final data that is sent to the host).
        setState(new DebugParamSensorAxisKeysInformationExclusive(this));
        break;
    case 8:
        // Report the frequency of the loop, including free RAM memory.
        setState(new DebugParamLoopFrequency(this));
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
        ESP_WARN("Unknown debug level");
        break;
    }
}