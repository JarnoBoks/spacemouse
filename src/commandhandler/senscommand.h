// senscommand.h
#pragma once

#include "IAxisConfigCommand.h"

/// @brief command text for the Sensitivity configuration command
static const char CMD_SENS[] PROGMEM = "SENS";

class SensCommand : public IAxisConfigCommand {
private:
public:
    SensCommand() : IAxisConfigCommand(CMD_SENS) {};
    ~SensCommand() {};

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
