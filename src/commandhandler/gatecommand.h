#pragma once

#include "IAxisConfigCommand.h"
/// @brief command text for the Sensitivity configuration command
static const char CMD_GATE[] PROGMEM = "GATE";

class GateCommand : public IAxisConfigCommand {
private:
public:
    GateCommand() : IAxisConfigCommand(CMD_GATE) {}
    ~GateCommand() {};

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
