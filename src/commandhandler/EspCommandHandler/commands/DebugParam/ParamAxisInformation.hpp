#pragma once

#include "DebugParam.hpp"

class DebugOutputAxes; // Forward declaration of DebugOutputAxes class

/**
 * @brief Applies the debug parameter axis information.
 * @details This class inherits from DebugParam and implements the apply method to create an observer for axis information.
 *         The observer will print the axis values to the serial monitor with sensitivity applied.
 */
class DebugParamAxisInformation : public DebugParam {
protected:
    DebugOutputAxes *m_AxisObserver = nullptr; // Pointer to the raw sensor observer instance
public:
    DebugParamAxisInformation() = delete; // Constructor
    DebugParamAxisInformation(DebugCommand *context) : DebugParam(context) {}
    virtual ~DebugParamAxisInformation(); // Destructor to clean up the observer instance

    void apply() override;
};