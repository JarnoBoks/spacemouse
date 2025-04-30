#pragma once

#include "IDebugParam.h"
#include "observers/output_axisvalues.h"
#include "observers/output_sensorvalues.h"

class DebugParamSensorAxisKeysInformation : public IDebugParam {
protected:
    Output_AxisValues *AxisObserver = nullptr;     // Pointer to the raw sensor observer instance       // REVIEW - Why is this necesar
    Output_SensorValues *SensorObserver = nullptr; // Pointer to the raw sensor observer instance
public:
    DebugParamSensorAxisKeysInformation() = default; // Constructor
    virtual ~DebugParamSensorAxisKeysInformation();  // Destructor to clean up the observer instance

    void apply() override;
    void report() override;
};