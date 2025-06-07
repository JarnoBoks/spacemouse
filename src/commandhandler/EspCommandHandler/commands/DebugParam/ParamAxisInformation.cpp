#include "ParamAxisInformation.hpp"
#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>
#include <knob/KnobAxisCollection.hpp>
#include <observers/DebugOutput/DebugOutputAxesSensitivity.hpp>

/**
 * @brief   Destructor to clean up the observer instance
 * @details This destructor checks if the rawSensorObserver pointer is not null.
 *          If it is not null, it detaches the observer from the hardware and deletes the observer instance.
 *          This ensures that the observer is properly cleaned up and does not cause memory leaks.
 */
DebugParamAxisInformation::~DebugParamAxisInformation() {
    m_Context->getCollectionCarrier()->getKnobAxisCollection()->detachObserver(m_AxisObserver); // Detach the observer from the axis collection
    delete m_AxisObserver;                                                                      // Clean up the observer instance
}

/**
 * @brief Applies the debug parameter axis information.
 * @details This method creates an instance of the DebugOutputAxesSensitivity observer and attaches it to the KnobAxes collection.
 *          The observer will then be notified of changes in the axis values and will print the sensitivity-adjusted values to the serial monitor.
 */
void DebugParamAxisInformation::apply() {
    // Instantiate the Observer for the RawSensor values and attach it to the hardware
    m_AxisObserver = new DebugOutputAxesSensitivity();
    m_Context->getCollectionCarrier()->getKnobAxisCollection()->attachObserver(m_AxisObserver); // Attach the observer to the axis collection
}
