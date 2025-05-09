#pragma once

#include "CommandBase.h"

class AxisDirectionConfig;
class Axis;

/**
 * @brief Interface for axis configuration commands.
 * @details This interface defines the structure for commands that configure axis settings.
 *          It includes methods for executing commands with parameters.
 * @note Derived classes should override the execute method.
 */
class IAxisConfigCommand : public CommandBase {
private:
protected:
    AxisDirectionConfig *m_DirectionConfig = nullptr;
    Axis *m_Axis = nullptr;
    float m_requestedValue = 0;

public:
    IAxisConfigCommand(const char *cmdName) : CommandBase(cmdName) {}
    virtual ~IAxisConfigCommand() {};

    virtual void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
