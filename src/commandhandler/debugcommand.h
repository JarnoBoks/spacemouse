#pragma once

#include "CommandBase.h"
// REMOVE #include "DebugParam\IDebugParam.hpp"

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
    IDebugParam *currentState = nullptr;            // Pointer to the current debug parameter state
    SensorCollection *m_SensorCollection = nullptr; // Pointer to the sensor collection

public:
    DebugCommand() = delete;

    DebugCommand()
        : CommandBase(CMD_DEBUG),
          currentState(new DebugParamOff()) {}

    DebugCommand(SensorCollection *sensorCollection)
        : CommandBase(CMD_DEBUG),
          m_SensorCollection(sensorCollection),
          currentState(new DebugParamOff()) {} // Constructor with sensor collection

    ~DebugCommand() { delete currentState; };

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
    void stop() override; // Stop the command execution

    void setState(IDebugParam *state);
    IDebugParam *getState() const;

    inline SensorCollection *getSensorCollection() const { return m_SensorCollection; }
};
