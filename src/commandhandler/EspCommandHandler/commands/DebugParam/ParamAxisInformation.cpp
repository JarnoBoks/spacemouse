#include "ParamAxisInformation.hpp"
#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>
#include <axis/KnobMotionVectorCollection.hpp>
#include <observers/DebugOutput/DebugOutputAxesSensitivity.hpp>

/**
 * @brief Destructor to clean up the observer instance
 * @details This destructor checks if the rawSensorObserver pointer is not null.
 *          If it is not null, it detaches the observer from the hardware and deletes the observer instance.
 *          This ensures that the observer is properly cleaned up and does not cause memory leaks.
 */
DebugParamAxisInformation::~DebugParamAxisInformation() {
    m_Context->getCollectionIdentifier()->getKnobMotionVectors()->detachObserver(m_AxisObserver); // Detach the observer from the axis collection
    delete m_AxisObserver;                                                                        // Clean up the observer instance
}

void DebugParamAxisInformation::apply() {
    // Instantiate the Observer for the RawSensor values and attach it to the hardware
    m_AxisObserver = new DebugOutputAxesSensitivity();
    m_Context->getCollectionIdentifier()->getKnobMotionVectors()->attachObserver(m_AxisObserver); // Attach the observer to the axis collection
}

void DebugParamAxisInformation::report() {
}