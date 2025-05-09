#pragma once

#include "DebugParam.hpp"
// REMOVE #include "observers\DebugOutput\DebugOutputAxes.hpp"    // Base class for axes observers       //REVIEW - Can we use the Interface instead of the base class?
// REMOVE #include "observers\DebugOutput\DebugOutputSensors.hpp" // Base class for sensor observers     //REVIEW - Can we use the Interface instead of the base class?

class DebugOutputAxes;
class DebugOutputSensors;

class DebugParamSensorAxisInformation : public DebugParam {
protected:
    DebugOutputAxes *AxisObserver = nullptr;      // Pointer to the raw axis observer instance
    DebugOutputSensors *SensorObserver = nullptr; // Pointer to the raw sensor observer instance
public:
    DebugParamSensorAxisInformation() = default; // Constructor
    virtual ~DebugParamSensorAxisInformation();  // Destructor to clean up the observer instance

    void apply() override;
    void report() override;
};