#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "icommand.h"

#define MAX_COMMANDS 10

class CommandHandler {
private:
    ICommand *commands[MAX_COMMANDS] = {nullptr}; // Array of command pointers

public:
    CommandHandler();
    void registerCommand(int index, ICommand *cmd);

    void parseSerialMonitorInput();
    void handleInput(char input[], const uint8_t inputsize, const int8_t bytesRead);
};

#endif // COMMANDHANDLER_H