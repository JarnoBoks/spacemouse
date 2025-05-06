#pragma once

#include "IDebugParam.h"
#include "..\..\observers\DebugOutput\DebugOutputLoopFrequency.hpp"

class DebugParamLoopFrequency : public IDebugParam {
protected:
    DebugOutputLoopFrequency *AxisObserver = nullptr; // Pointer to the loop frequency observer instance       // REVIEW - Why is this necessary //REVIEW - Can we use the Interface instead of the base class?
public:
    DebugParamLoopFrequency() = default; // Constructor
    virtual ~DebugParamLoopFrequency();  // Destructor to clean up the observer instance

    void apply() override;
    void report() override;
};