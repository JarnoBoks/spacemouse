#pragma once

#include "DebugParam.hpp"

class DebugOutputAxes;
class DebugOutputSensors;

class DebugParamSensorAxisKeysInformation : public DebugParam {
protected:
    DebugOutputAxes *m_AxisObserver = nullptr; // REVIEW - Why is this necessary
    DebugOutputSensors *m_SensorObserver = nullptr;

public:
    DebugParamSensorAxisKeysInformation() = delete; // Constructor
    DebugParamSensorAxisKeysInformation(DebugCommand *context) : DebugParam(context) {}
    virtual ~DebugParamSensorAxisKeysInformation(); // Destructor to clean up the observer instance

    void apply() override;
};