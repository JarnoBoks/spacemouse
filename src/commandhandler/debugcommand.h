#pragma once

#include "CommandBase.h"

// DebugParam classes for default state
#include "DebugParam\ParamOff.hpp"

// TODO - Add the STARTUPDEBUG definition to the default constructor

/// @brief command text for the command
static const char CMD_DEBUG[] PROGMEM = "DEBUG";

class IDebugParam;      // Forward declaration of IDebugParam class
class SensorCollection; // Forward declaration of SensorCollection class

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
#if 0 // REMOVE
    DebugCommand()
        : CommandBase(CMD_DEBUG),
          currentState(new DebugParamOff()) {}
#endif
    DebugCommand() = delete; // Default constructor is deleted

    /**
     * @brief Constructor for DebugCommand class.
     * @param sensorCollection Pointer to the SensorCollection instance.
     * @details Initializes the command with the provided sensor collection and sets the default debug state.
     */
    DebugCommand(SensorCollection *sensorCollection)
        : CommandBase(CMD_DEBUG, sensorCollection),
          currentState(new DebugParamOff()) {}

    ~DebugCommand() { delete currentState; };

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
    void stop() override; // Stop the command execution

    void setState(IDebugParam *state);
    IDebugParam *getState() const;
};
