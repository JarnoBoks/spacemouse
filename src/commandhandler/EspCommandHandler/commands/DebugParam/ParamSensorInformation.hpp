#pragma once
#include "DebugParam.hpp"

class DebugOutputSensors; // Forward declaration of the DebugOutputSensors class

class DebugParamSensorInformation : public DebugParam {
protected:
    DebugOutputSensors *m_SensorObserver = nullptr; // Pointer to the raw sensor observer instance
public:
    DebugParamSensorInformation() = delete; // Delete the default constructor
    DebugParamSensorInformation(DebugCommand *context) : DebugParam(context) {};
    virtual ~DebugParamSensorInformation();

    virtual void apply() override = 0; // Pure virtual function to be implemented by derived classes
};
