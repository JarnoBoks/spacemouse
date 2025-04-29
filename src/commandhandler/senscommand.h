#pragma once
#ifndef SENSCOMMAND_H
#define SENSCOMMAND_H

#include "ICommand.h"

// Static (global) command name for the SENS command
static const char CMD_SENS[] PROGMEM = "SENS";

class SensCommand : public ICommand {
private:
public:
    SensCommand();
    ~SensCommand();

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};

#endif // SENSCOMMAND_H