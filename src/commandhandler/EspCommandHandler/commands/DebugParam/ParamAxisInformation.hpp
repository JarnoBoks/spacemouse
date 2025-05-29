#pragma once

#include "DebugParam.hpp"

class DebugOutputAxes; // Forward declaration of DebugOutputAxes class

class DebugParamAxisInformation : public DebugParam {
protected:
    DebugOutputAxes *m_AxisObserver = nullptr; // Pointer to the raw sensor observer instance
public:
    DebugParamAxisInformation() = delete; // Constructor
    DebugParamAxisInformation(DebugCommand *context) : DebugParam(context) {}
    virtual ~DebugParamAxisInformation(); // Destructor to clean up the observer instance

    void apply() override;
    void report() override;
};