#pragma once

#include "CommandBase.hpp"

#define NUM_AX_DIRCFG 2 // Number of axis direction configurations per AxisConfig in the software (Current: Positive and Negative)

class KnobAxisDirectionConfig;
class KnobAxis;

/**
 * @brief   Interface for knob MotionVector configuration commands.
 * @details This interface defines the structure for commands that configure knob MotionVector settings.
 *          It includes methods for executing commands with parameters.
 * @note    Derived classes should override the execute method, and call the Baseclass functionality.
 */
class IAxisConfigCommand : public CommandBase {
private:
protected:
    KnobAxisDirectionConfig *knobAxisDirectionConfigs[NUM_AX_DIRCFG]; // Array of pointers to AxisDirectionConfig objects
    KnobAxis *m_knobAxis = nullptr;                                   // Pointer to the knob MotionVector object that will be configured
    float m_requestedValue = 0;                                       // Requested value for the knob MotionVector configuration (sensitivity, gate, etc.) derived from the command. -1 if no update is needed.

public:
    /**
     * @brief Constructor for the SensCommand class.
     * @param collectionCarrier Pointer to the CollectionCarrier for accessing collections.
     * @details Initializes the command with the CMD_SENS command name.
     */
    // TODO - Make the collectionCarrier a const reference
    IAxisConfigCommand(const char *cmdName, CollectionCarrier *collectionCarrier) : CommandBase(cmdName, collectionCarrier) {
        for (uint8_t i = 0; i < NUM_AX_DIRCFG; i++) {
            knobAxisDirectionConfigs[i] = nullptr;
        }
    }
    virtual ~IAxisConfigCommand() {};

    virtual void execute(const char *param1, const char *param2, uint8_t paramCount) override;
};
