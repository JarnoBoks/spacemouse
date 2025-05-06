#pragma once

#include "IDebugParam.h"
#include "..\..\observers\DebugOutput\DebugOutputAxes.hpp"    // Base class for axes observers       //REVIEW - Can we use the Interface instead of the base class?
#include "..\..\observers\DebugOutput\DebugOutputSensors.hpp" // Base class for sensor observers     //REVIEW - Can we use the Interface instead of the base class?

class DebugParamSensorAxisKeysInformation : public IDebugParam {
protected:
    DebugOutputAxes *AxisObserver = nullptr;      // Pointer to the raw sensor observer instance       // REVIEW - Why is this necesar
    DebugOutputSensors *SensorObserver = nullptr; // Pointer to the raw sensor observer instance
public:
    DebugParamSensorAxisKeysInformation() = default; // Constructor
    virtual ~DebugParamSensorAxisKeysInformation();  // Destructor to clean up the observer instance

    void apply() override;
    void report() override;
};