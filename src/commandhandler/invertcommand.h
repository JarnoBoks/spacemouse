#pragma once

#include "IAxisConfigCommand.h"
/// @brief command text for the Sensitivity configuration command
static const char CMD_INVERT[] PROGMEM = "INVERT";

class InvertCommand : public IAxisConfigCommand {
private:
public:
    InvertCommand() : IAxisConfigCommand(CMD_INVERT) {}
    ~InvertCommand() {};

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
