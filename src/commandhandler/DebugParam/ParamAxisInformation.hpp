#pragma once

#include "DebugParam.hpp"
#include "observers\DebugOutput\DebugOutputAxes.hpp"

class DebugParamAxisInformation : public DebugParam {
protected:
    DebugOutputAxes *AxisObserver = nullptr; // Pointer to the raw sensor observer instance       // REVIEW - Why is this necessary //REVIEW - Can we use the Interface instead of the base class?
public:
    DebugParamAxisInformation() = default; // Constructor
    virtual ~DebugParamAxisInformation();  // Destructor to clean up the observer instance

    void apply() override;
    void report() override;
};