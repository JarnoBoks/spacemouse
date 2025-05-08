#pragma once
#include "CommandBase.h"

/// @brief command text for the Invert configuration command
static const char CMD_BOOTLOADER[] PROGMEM = "BL";

class BootloaderCommand : public CommandBase {
private:
public:
    BootloaderCommand() : CommandBase(CMD_BOOTLOADER) {}
    ~BootloaderCommand() {};

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
