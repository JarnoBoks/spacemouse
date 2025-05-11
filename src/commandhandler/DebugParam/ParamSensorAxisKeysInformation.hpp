#pragma once

#include "DebugParam.hpp"

class DebugOutputAxes;
class DebugOutputSensors;

class DebugParamSensorAxisKeysInformation : public DebugParam {
protected:
    DebugOutputAxes *AxisObserver = nullptr;      // Pointer to the raw sensor observer instance       // REVIEW - Why is this necessary
    DebugOutputSensors *SensorObserver = nullptr; // Pointer to the raw sensor observer instance
public:
    DebugParamSensorAxisKeysInformation() = delete; // Constructor
    DebugParamSensorAxisKeysInformation(DebugCommand *context) : DebugParam(context) {}
    virtual ~DebugParamSensorAxisKeysInformation(); // Destructor to clean up the observer instance

    void apply() override;
    void report() override;
};