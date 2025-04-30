#pragma once

#include "ICommand.h"

/// @brief command text for the Switch YZ command
static const char CMD_SWITCHYZ[] PROGMEM = "SWITCHYZ";

class SwitchYZCommand : public ICommand {
private:
public:
    SwitchYZCommand() : ICommand(CMD_SWITCHYZ) {} // Constructor to initialize command name

    void execute(const char *param1, const char *param2, const uint8_t paramCount) override;
};
