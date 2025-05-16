#include "ParamSensorAxisKeysInformation.hpp"

#include "ParamSensorAxisInformation.hpp"
#include "..\CollectionCarrier\CollectionCarrier.hpp"

// Observable classes that are used in this file
#include "sensor/SensorCollection.hpp" // For SensorCollection class
#include "axis/AxisCollection.hpp"     // For AxisCollection class

// Observers that are used in this file.
#include "observers/DebugOutput/DebugOutputAxesModified.hpp"             // Implementation of the ODebugOutputAxes class
#include "observers/DebugOutput/DebugOutputSensorsCenteredNoNewline.hpp" // Implementation of the ODebugOutputSensors class

/**
 * @brief Destructor to clean up the observer instance
 * @details This destructor checks if the rawSensorObserver pointer is not null.
 *          If it is not null, it detaches the observer from the hardware and deletes the observer instance.
 *          This ensures that the observer is properly cleaned up and does not cause memory leaks.
 */
DebugParamSensorAxisKeysInformation::~DebugParamSensorAxisKeysInformation() {

    m_Context->getCollectionIdentifier()->getSensorCollection()->detachObserver(m_SensorObserver); // Detach the observer from the sensor collection
    delete m_SensorObserver;

    m_Context->getCollectionIdentifier()->getAxisCollection()->detachObserver(m_AxisObserver); // Detach the observer from the axis collection
    delete m_AxisObserver;
}

void DebugParamSensorAxisKeysInformation::apply() {

    // Instantiate the Observers and attach them to the hardware
    m_SensorObserver = new DebugOutputSensorsCenteredNoNewline();
    m_AxisObserver = new DebugOutputAxesModified();

    m_Context->getCollectionIdentifier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the sensor observer to the sensor collection
    m_Context->getCollectionIdentifier()->getAxisCollection()->attachObserver(m_AxisObserver);     // Attach the axis observer to the axis collection
}

void DebugParamSensorAxisKeysInformation::report() {
}