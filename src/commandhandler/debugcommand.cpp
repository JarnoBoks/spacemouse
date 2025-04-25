#include "DebugCommand.h"

#include "debugstate/DebugOff.h"
#include "debugstate/DebugRawSensors.h"
// ...include other debug states...

/**
 * @brief This class handles the debug commands and manages the current debug state.
 * @details The class allows switching between different debug states and executing commands related to debugging.
 * It uses the IDebugState interface to define the behavior of different debug states.
 * The class also provides methods to set the current debug state and execute commands based on the parameters provided.
 * This class is essential for managing the debugging process and ensuring that the correct state is applied during operation. *
 */

/**
 * @brief Constructor for DebugCommand.
 * @details Initializes the debug command handler and sets the initial debug state to DebugOff.
 */
DebugCommand::DebugCommand() : currentState(new DebugOff()) {}

/**
 * @brief Destructor for the DebugCommand class.
 * @details Cleans up the current debug state by deleting it.
 */
DebugCommand::~DebugCommand() { delete currentState; }

/**
 * @brief Sets the current debug state.
 * @param state Pointer to the new debug state to be set.
 * @details Deletes the previous state and applies the new state.
 */
void DebugCommand::setState(IDebugState *state) {
    if (currentState)
        delete currentState;
    currentState = state;
    currentState->apply();
}

/**
 * @brief Gets the current debug state.
 * @return Pointer to the current debug state.
 */
IDebugState *DebugCommand::getState() const { return currentState; }

/**
 * @brief Executes the debug command based on the provided parameters.
 * @param param1 First parameter, typically the debug level.
 * @param param2 Second parameter (unused).
 * @param paramCount Number of parameters provided.
 * @details Handles switching between debug states or reporting the current state.
 */
void DebugCommand::execute(const char *param1, const char *param2, uint8_t paramCount) {
    if (paramCount == 0) {
        currentState->report();
        return;
    }
    if (paramCount > 1) {
#ifdef ARDUINO_ARCH_ESP32 // Only for device with more flash memory
        Serial.println("Too many parameters for DEBUG command");
#endif
        return;
    }

    int8_t requestedLevel = 0; // Default value for the second word
    if (!convertWordNumber(param1, (long *)&requestedLevel)) {
        return; // First parameter is not a number
    }

    switch (requestedLevel) {
    case -1:
        setState(new DebugOff());
        break;
    /* case 0:
        setState(new DebugSilent());
        break; */
    case 1:
        setState(new DebugRawSensors());
        break;
    default:
        // Handle unknown debug level
#ifdef ARDUINO_ARCH_ESP32 // Only for device with more flash memory
        Serial.println(F("Unknown debug mode"));
#endif
        break;
    }
}