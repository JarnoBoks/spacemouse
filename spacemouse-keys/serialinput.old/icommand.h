#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <Arduino.h>

enum CommandType : uint8_t {
    CMD_NONE = 0,
    CMD_SHOW,
    CMD_IDLE,
    CMD_DEADZONE,
    CMD_MINMAX,
    CMD_SWITCHYZ,
    CMD_EXCLUSIVEMODE,
    CMD_DEBUG,
    CMD_MODFUNC,
    CMD_SENS
};

enum CommandAction : uint8_t {
    ACTION_NONE = 0,
    ACTION_PRINT,
    ACTION_UPDATE,
    ACTION_CALIBRATE,
    ACTION_SET
};

class ICommand {
protected:
    CommandAction _action = ACTION_NONE; // Action should be: print, update, etc.
public:
    ICommand() : _action(ACTION_NONE) {}                // Default constructor to initialize action
    ICommand(CommandAction action) : _action(action) {} // Constructor to initialize action
    ~ICommand() {}

    virtual void ExecuteCommand() = 0;
};

#endif // ICOMMAND_H