#include "ParamSensorInformation.hpp"

#include "sensor/SensorCollection.hpp"
#include "observers/DebugOutput/DebugOutputSensors.hpp"

DebugParamSensorInformation::DebugParamSensorInformation() : DebugParam(), m_SensorObserver(nullptr) {} // Constructor to initialize currentLevel

DebugParamSensorInformation::~DebugParamSensorInformation() {
    if (m_SensorObserver != nullptr) {
        m_Context->getSensorCollection()->detachObserver(m_SensorObserver); // Detach the sensor observer from the sensor collection
        delete m_SensorObserver;                                            // Clean up the observer instance
        m_SensorObserver = nullptr;
    }
}
