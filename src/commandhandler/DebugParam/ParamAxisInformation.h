#pragma once

#include "IDebugParam.h"
#include "observers/output_axisvalues.h"

class DebugParamAxisInformation : public IDebugParam {
protected:
    Output_AxisValues *AxisObserver = nullptr; // Pointer to the raw sensor observer instance       // REVIEW - Why is this necessary
public:
    DebugParamAxisInformation() = default; // Constructor
    virtual ~DebugParamAxisInformation();  // Destructor to clean up the observer instance

    void apply() override;
    void report() override;
};