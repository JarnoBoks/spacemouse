#pragma once

#include "CommandBase.hpp"

/// @brief command text for the Switch YZ command
static const char CMD_SWITCHYZ[] PROGMEM = "SWITCHYZ";

class SwitchYZCommand : public CommandBase {
private:
public:
    SwitchYZCommand() : CommandBase(CMD_SWITCHYZ) {} // Constructor to initialize command name

    void execute(const char *param1, const char *param2, const uint8_t paramCount) override;
};
