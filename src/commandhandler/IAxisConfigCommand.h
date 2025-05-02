// IAxisConfigCommand.h
#pragma once

#include "CommandBase.h"

class DirectionConfig;
class Axis;

/**
 * @brief Interface for axis configuration commands.
 * @details This interface defines the structure for commands that configure axis settings.
 * It includes methods for executing commands with parameters.
 * @note Derived classes must implement the execute method.
 */
class IAxisConfigCommand : public CommandBase {
private:
protected:
    DirectionConfig *_directionConfig;
    Axis *_axis;
    float _requestedValue = 0;

public:
    IAxisConfigCommand(const char *cmdName) : CommandBase(cmdName), _directionConfig(nullptr), _axis(nullptr), _requestedValue(0) {} // Constructor to initialize command name;
    virtual ~IAxisConfigCommand() {};

    virtual void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
