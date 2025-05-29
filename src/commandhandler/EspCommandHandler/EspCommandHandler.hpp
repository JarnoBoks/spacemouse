#pragma once
#include <stdint.h>

class CommandBase;

#define MAX_COMMANDS 11

class ESPCommandHandler {
private:
    CommandBase *commands[MAX_COMMANDS] = {nullptr}; // Array of command pointers
    uint8_t commandCount = 0;                        // Number of commands registered
    int8_t lastCommandIndex = -1;                    // Index of the last command executed

public:
    ESPCommandHandler();
    ~ESPCommandHandler();
    bool registerCommand(CommandBase *cmd);

    void parseSerialMonitorInput();
    void handleInput(char input[], const uint8_t inputsize, const int8_t bytesRead);
};
