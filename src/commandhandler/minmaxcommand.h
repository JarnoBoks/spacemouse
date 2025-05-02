// minmaxcommand.h
#pragma once

#include "CommandBase.h"

/// @brief command text for the MinMax calibration command
static const char CMD_MINMAX[] PROGMEM = "MINMAX";

class MinMaxCommand : public CommandBase {
private:
public:
    MinMaxCommand();
    ~MinMaxCommand();

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
