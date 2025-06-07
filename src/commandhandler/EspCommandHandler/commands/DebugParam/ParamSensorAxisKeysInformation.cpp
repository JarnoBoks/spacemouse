#include "ParamSensorAxisKeysInformation.hpp"

#include "ParamSensorAxisInformation.hpp"
#include "commandhandler/CollectionCarrier/CollectionCarrier.hpp"

// Observable classes that are used in this file
#include <sensor/SensorCollection.hpp> // For SensorCollection class
#include <knob/KnobAxisCollection.hpp>

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

    m_Context->getCollectionCarrier()->getSensorCollection()->detachObserver(m_SensorObserver); // Detach the observer from the sensor collection
    delete m_SensorObserver;

    m_Context->getCollectionCarrier()->getKnobAxisCollection()->detachObserver(m_AxisObserver); // Detach the observer from the axis collection
    delete m_AxisObserver;
}

/**
 * @brief Applies the debug parameter sensor axis keys information.
 * @details This method creates instances of the DebugOutputSensorsCenteredNoNewline and DebugOutputAxesModified observers,
 *          and attaches them to the SensorCollection and KnobAxisCollection respectively.
 */
void DebugParamSensorAxisKeysInformation::apply() {

    // Instantiate the Observers and attach them to the hardware
    m_SensorObserver = new DebugOutputSensorsCenteredNoNewline();
    m_AxisObserver = new DebugOutputAxesModified();

    m_Context->getCollectionCarrier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the sensor observer to the sensor collection
    m_Context->getCollectionCarrier()->getKnobAxisCollection()->attachObserver(m_AxisObserver); // Attach the axis observer to the axis collection
}
