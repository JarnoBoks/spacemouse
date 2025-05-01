// CommandButtonFunctionality.h
#pragma once
#include "IButtonFunctionality.h"
#include "CommandType.h"

class CommandButtonFunctionality : public IButtonFunctionality {
    CommandType command;

public:
    CommandButtonFunctionality(CommandType cmd) : command(cmd) {}
    void execute() override {
        // Implement command execution
    }
    CommandType getCommand() const { return command; }
    void setCommand(CommandType cmd) { command = cmd; }
};
