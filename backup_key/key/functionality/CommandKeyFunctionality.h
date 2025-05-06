// CommandKeyFunctionality.h
#pragma once
#include "IKeyFunctionality.h"
#include "CommandType.h"

class CommandKeyFunctionality : public IKeyFunctionality {
    CommandType command;

public:
    CommandKeyFunctionality(CommandType cmd) : command(cmd) {}
    inline void onPress() override {};   // Called when the key is pressed
    inline void onRelease() override {}; // Called when the key is released

    inline CommandType getCommand() const { return command; }
    inline void setCommand(CommandType cmd) { command = cmd; }
};
