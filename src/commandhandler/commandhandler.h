// commandhandler.h
#pragma once
#include "CommandBase.h"

#define MAX_COMMANDS 11

class CommandHandler {
private:
    CommandBase *commands[MAX_COMMANDS] = {nullptr}; // Array of command pointers
    uint8_t commandCount = 0;                        // Number of commands registered
    int8_t lastCommandIndex = -1;                    // Index of the last command executed
public:
    CommandHandler() : commands{nullptr}, commandCount(0), lastCommandIndex(-1) {}
    ~CommandHandler() {
        for (int i = 0; i < commandCount; ++i) {
            delete commands[i]; // Clean up the command pointers
        }
    }
    bool registerCommand(CommandBase *cmd);

    void parseSerialMonitorInput();
    void handleInput(char input[], const uint8_t inputsize, const int8_t bytesRead);
};
