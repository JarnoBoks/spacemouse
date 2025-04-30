#pragma once

#include "IDebugParam.h"
#include "observers/output_loopfrequency.h"

class DebugParamLoopFrequency : public IDebugParam {
protected:
    Output_LoopFrequency *AxisObserver = nullptr; // Pointer to the loop frequency observer instance       // REVIEW - Why is this necesar
public:
    DebugParamLoopFrequency() = default; // Constructor
    virtual ~DebugParamLoopFrequency();  // Destructor to clean up the observer instance

    void apply() override;
    void report() override;
};