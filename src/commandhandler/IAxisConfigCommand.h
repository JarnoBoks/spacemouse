#pragma once

#include "CommandBase.h"

class AxisDirectionConfig;
class Axis;

/**
 * @brief   Interface for axis configuration commands.
 * @details This interface defines the structure for commands that configure axis settings.
 *          It includes methods for executing commands with parameters.
 * @note    Derived classes should override the execute method, and call the Baseclass functionality.
 */
class IAxisConfigCommand : public CommandBase {
private:
protected:
    AxisDirectionConfig *m_AxisDirectionConfig = nullptr;
    Axis *m_Axis = nullptr;
    float m_requestedValue = 0;

public:
    /**
     * @brief Constructor for the SensCommand class.
     * @param collectionCarrier Pointer to the CollectionCarrier for accessing collections.
     * @details Initializes the command with the CMD_SENS command name.
     */
    // TODO - Make the collectionCarrier a const reference
    IAxisConfigCommand(const char *cmdName, CollectionCarrier *collectionCarrier) : CommandBase(cmdName, collectionCarrier) {}

    // REMOVE IAxisConfigCommand(const char *cmdName) : CommandBase(cmdName) {}
    virtual ~IAxisConfigCommand() {};

    virtual void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
