#pragma once

#include "CommandBase.hpp"

// DebugParam classes for default state
#include "DebugParam\ParamOff.hpp"

/// @brief command text for the debug command
static const char CMD_DEBUG_P[] PROGMEM = "DEBUG";

class IDebugParam;
class CollectionCarrier;

/**
 * @brief This class handles the debug commands and manages the current debug state.
 * @details The class allows switching between different debug states and executing commands related to debugging.
 * It uses the IDebugState interface to define the behavior of different debug states.
 * The class also provides methods to set the current debug state and execute commands based on the parameters provided.
 */
class DebugCommand : public CommandBase {
private:
    IDebugParam *currentState = nullptr; // Pointer to the current debug parameter state

public:
    DebugCommand() = delete; // Default constructor is deleted

    /**
     * @brief Constructor for the DebugCommand class.
     * @param collectionCarrier Pointer to the CollectionCarrier instance.
     * @details Initializes the command with the debug command name and the collection carrier.
     *          The initial state is set to DebugParamOff, which means no debug output is active.
     */
    DebugCommand(CollectionCarrier *collectionCarrier)
        : CommandBase(CMD_DEBUG_P, collectionCarrier),
          currentState(new DebugParamOff()) {}

    ~DebugCommand() { delete currentState; };

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
    void stop() override; // Stop the command execution

    void setState(IDebugParam *state);
    IDebugParam *getState() const;
};
