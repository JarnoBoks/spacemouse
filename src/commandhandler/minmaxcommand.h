#pragma once
#ifndef MINMAXCOMMAND_H
#define MINMAXCOMMAND_H

#include "ICommand.h"

// Static (global) command name for the MINMAX command
static const char CMD_MINMAX[] PROGMEM = "MINMAX";

class MinMaxCommand : public ICommand {
private:
public:
    MinMaxCommand();
    ~MinMaxCommand();

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};

#endif // MINMAXCOMMAND_H