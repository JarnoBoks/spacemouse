#pragma once

#include "DebugParam.hpp"

class DebugOutputAxes;
class DebugOutputSensors;
class DebugOutputKeysState;

class DebugParamSensorAxisKeysInformationExclusive : public DebugParam {
protected:
    DebugOutputAxes *m_AxisObserver = nullptr;      // Pointer to the axis observer instance (stored for deletion)
    DebugOutputSensors *m_SensorObserver = nullptr; // Pointer to the sensor observer instance (stored for deletion)
    DebugOutputKeysState *m_KeysObserver = nullptr; // Pointer to the keys observer instance (stored for deletion)

public:
    DebugParamSensorAxisKeysInformationExclusive() = delete; // Constructor
    DebugParamSensorAxisKeysInformationExclusive(DebugCommand *context) : DebugParam(context) {}
    virtual ~DebugParamSensorAxisKeysInformationExclusive(); // Destructor to clean up the observer instance

    void apply() override;
};