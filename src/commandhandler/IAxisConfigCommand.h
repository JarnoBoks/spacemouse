#pragma once

#include "ICommand.h"

class DirectionConfig;
class Axis;

class IAxisConfigCommand : public ICommand {
private:
protected:
    DirectionConfig *_directionConfig;
    Axis *_axis;
    float _requestedValue = 0;

public:
    IAxisConfigCommand(const char *cmdName) : ICommand(cmdName), _directionConfig(nullptr), _axis(nullptr), _requestedValue(0) {} // Constructor to initialize command name;
    virtual ~IAxisConfigCommand() {};

    virtual void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
