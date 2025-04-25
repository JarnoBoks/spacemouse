#pragma once
#ifndef SHOWCOMMAND_H
#define SHOWCOMMAND_H

#include "ICommand.h"

// Static (global) command name for the SHOW command
static const char CMD_SHOW[] PROGMEM = "SHOW";

class ShowCommand : public ICommand {
private:
public:
    ShowCommand() : ICommand(CMD_SHOW) {} // Constructor to initialize command name

    void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};

#endif // SHOWCOMMAND_H-