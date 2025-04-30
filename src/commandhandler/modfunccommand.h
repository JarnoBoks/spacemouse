#pragma once

#include "IAxisConfigCommand.h"

/// @brief command text for the Sensitivity configuration command
static const char CMD_MODFUNC[] PROGMEM = "MODFUNC";

class ModFuncCommand : public IAxisConfigCommand {
private:
public:
    ModFuncCommand() : IAxisConfigCommand(CMD_MODFUNC) {};
    ~ModFuncCommand() {};

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
