#pragma once

#include "DebugParam.hpp"
#include "..\..\observers\DebugOutput\DebugOutputLoopFrequency.hpp"

class DebugParamLoopFrequency : public DebugParam {
protected:
    DebugOutputLoopFrequency *AxisObserver = nullptr; // Pointer to the loop frequency observer instance       // REVIEW - Why is this necessary //REVIEW - Can we use the Interface instead of the base class?
public:
    DebugParamLoopFrequency() = delete; // Constructor
    DebugParamLoopFrequency(DebugCommand *context) : DebugParam(context) {}
    virtual ~DebugParamLoopFrequency(); // Destructor to clean up the observer instance

    void apply() override;
    void report() override;
};