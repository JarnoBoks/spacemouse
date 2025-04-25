#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "icommand.h"

#define MAX_COMMANDS 10

class CommandHandler {
private:
    ICommand *commands[MAX_COMMANDS] = {nullptr}; // Array of command pointers

    void handleInput(char input[], const uint8_t inputsize, int8_t bytesRead);

public:
    CommandHandler();
    void registerCommand(int index, ICommand *cmd);

    void parseSerialMonitorInput();
};

#endif // COMMANDHANDLER_H