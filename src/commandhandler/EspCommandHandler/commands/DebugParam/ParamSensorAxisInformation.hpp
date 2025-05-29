#pragma once

#include "DebugParam.hpp"

class DebugOutputAxes;
class DebugOutputSensors;

class DebugParamSensorAxisInformation : public DebugParam {
protected:
    DebugOutputAxes *m_AxisObserver = nullptr;      // Pointer to the raw axis observer instance
    DebugOutputSensors *m_SensorObserver = nullptr; // Pointer to the raw sensor observer instance
public:
    DebugParamSensorAxisInformation() = delete; // Constructor
    DebugParamSensorAxisInformation(DebugCommand *context) : DebugParam(context) {}
    virtual ~DebugParamSensorAxisInformation(); // Destructor to clean up the observer instance

    void apply() override;
};