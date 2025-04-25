#pragma once
#ifndef DEBUGCOMMAND_H
#define DEBUGCOMMAND_H

#include "ICommand.h"
#include "debugstate/idebugstate.h"

// TODO - Add the STARTUPDEBUG definition to the default constructor

// Static (global) command name for the DEBUG command
static const char CMD_DEBUG[] PROGMEM = "DEBUG";

class DebugCommand : public ICommand {
private:
    IDebugState *currentState;

public:
    DebugCommand();
    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
    void setState(IDebugState *state);
    IDebugState *getState() const;
    ~DebugCommand();
};

#endif // DEBUGCOMMAND_H