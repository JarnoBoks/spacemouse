#include "ParamSensorAxisKeysInformation.hpp"

#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>

// Observable classes that are used in this file
#include <sensor/SensorCollection.hpp>
#include <knob/KnobAxisCollection.hpp>
#include <key/KeyCollection.hpp>

// Observers that are used in this file.
#include <observers/DebugOutput/DebugOutputAxesModified.hpp>
#include <observers/DebugOutput/DebugOutputSensorsCentered.hpp>
#include <observers/DebugOutput/DebugOutputKeysState.hpp>

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

    m_Context->getCollectionCarrier()->getKeyCollection()->detachObserver(m_KeysObserver); // Detach the keys observer from the key collection
    delete m_KeysObserver;
}

/**
 * @brief Applies the debug parameter sensor axis keys information.
 * @details This method creates instances of the DebugOutputSensorsCenteredNoNewline and DebugOutputAxesModified observers,
 *          and attaches them to the SensorCollection and KnobAxisCollection respectively.
 */
void DebugParamSensorAxisKeysInformation::apply() {

    m_SensorObserver = new DebugOutputSensorsCentered(false); // Print separator after output, no newline
    m_AxisObserver = new DebugOutputAxesModified(false);      // Print separator after output, no newline
    m_KeysObserver = new DebugOutputKeysState();

    m_Context->getCollectionCarrier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the sensor observer to the sensor collection
    m_Context->getCollectionCarrier()->getKnobAxisCollection()->attachObserver(m_AxisObserver); // Attach the axis observer to the axis collection
    m_Context->getCollectionCarrier()->getKeyCollection()->attachObserver(m_KeysObserver);      // Attach the keys observer to the key collection
}
