// debugcommand.h
#pragma once

#include "CommandBase.h"
#include "DebugParam/IDebugParam.h"

// TODO - Add the STARTUPDEBUG definition to the default constructor

/// @brief command text for the command
static const char CMD_DEBUG[] PROGMEM = "DEBUG";

/**
 * @brief  Class to handle the DEBUG command.
 * @details This class implements the DEBUG command functionality.
 * It includes methods for executing the command and managing debug parameters.
 */
class DebugCommand : public CommandBase {
private:
    IDebugParam *currentParam = nullptr; // Pointer to the current debug parameter state

public:
    DebugCommand();
    ~DebugCommand();

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
    void stop() override; // Stop the command execution

    void setState(IDebugParam *state);
    IDebugParam *getState() const;
};
