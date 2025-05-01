// CommandButtonFunctionality.h
#pragma once
#include "IButtonFunctionality.h"
#include "CommandType.h"

class CommandButtonFunctionality : public IButtonFunctionality {
    CommandType command;

public:
    CommandButtonFunctionality(CommandType cmd) : command(cmd) {}
    inline void onPress() {};   // Called when the button is pressed
    inline void onRelease() {}; // Called when the button is released

    inline CommandType getCommand() const { return command; }
    inline void setCommand(CommandType cmd) { command = cmd; }
};
