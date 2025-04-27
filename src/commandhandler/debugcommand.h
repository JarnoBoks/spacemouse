#pragma once
#ifndef DEBUGCOMMAND_H
#define DEBUGCOMMAND_H

#include "ICommand.h"
#include "DebugParam/IDebugParam.h"

// TODO - Add the STARTUPDEBUG definition to the default constructor

// Static (global) command name for the DEBUG command
static const char CMD_DEBUG[] PROGMEM = "DEBUG";

/**
 * @brief Class DebugCommand
 * @details This class implements the ICommand interface and is responsible for handling debug commands received from the serial monitor.
 * The class allows the user to set and change the current debug state of the Spacemouse.
 * @note The constructor initializes the current state to nullptr.
 */
class DebugCommand : public ICommand {
private:
    IDebugParam *currentParam = nullptr; // Pointer to the current debug parameter state

public:
    DebugCommand();
    ~DebugCommand();

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;

    void setState(IDebugParam *state);
    IDebugParam *getState() const;
};

#endif // DEBUGCOMMAND_H