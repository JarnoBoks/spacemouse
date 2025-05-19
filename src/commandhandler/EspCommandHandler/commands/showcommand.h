#pragma once

#include "CommandBase.hpp"

/// @brief command text for the Show command
static const char CMD_SHOW_P[] PROGMEM = "SHOW";

class ShowCommand : public CommandBase {
private:
public:
    ShowCommand() : CommandBase(CMD_SHOW_P) {} // Constructor to initialize command name

    void execute(const char *param1, const char *param2, const uint8_t paramCount) override;
};
