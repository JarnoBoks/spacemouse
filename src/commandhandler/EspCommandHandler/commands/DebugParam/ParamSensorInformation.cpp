#include "ParamSensorInformation.hpp"
#include "commandhandler/CollectionCarrier/CollectionCarrier.hpp"
#include "sensor/SensorCollection.hpp"
#include "..\..\..\..\observers\DebugOutput\DebugOutputSensors.hpp"

DebugParamSensorInformation::~DebugParamSensorInformation() {
    if (m_SensorObserver != nullptr) {
        m_Context->getCollectionCarrier()->getSensorCollection()->detachObserver(m_SensorObserver); // Detach the sensor observer from the sensor collection
        delete m_SensorObserver;                                                                    // Clean up the observer instance
        m_SensorObserver = nullptr;
    }
}
