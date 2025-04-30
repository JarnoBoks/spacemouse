#pragma once

#include "ICommand.h"

/// @brief command text for the Exclusive mode command
static const char CMD_EXCLUSIVE[] PROGMEM = "EXCL"; // Command name for the SHOW command

class ExclusiveCommand : public ICommand {
private:
public:
    ExclusiveCommand() : ICommand(CMD_EXCLUSIVE) {} // Constructor to initialize command name

    void execute(const char *param1, const char *param2, const uint8_t paramCount) override;
};
