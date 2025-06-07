#include "ParamLoopFrequency.hpp"
#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>
#include <knob/KnobAxisCollection.hpp>
#include <observers/DebugOutput/DebugOutputLoopFrequency.hpp>

/**
 * @brief Destructor to clean up the observer instance
 * @details This destructor checks if the rawSensorObserver pointer is not null.
 *          If it is not null, it detaches the observer from the hardware and deletes the observer instance.
 *          This ensures that the observer is properly cleaned up and does not cause memory leaks.
 */
DebugParamLoopFrequency::~DebugParamLoopFrequency() {
    m_Context->getCollectionCarrier()->getKnobAxisCollection()->detachObserver(m_AxisObserver); // Detach the observer from the axis collection
    delete m_AxisObserver;                                                                      // Clean up the observer instance
    m_AxisObserver = nullptr;
}

void DebugParamLoopFrequency::apply() {
    // Instantiate the Observer for the Loop Frequency values and attach it to the hardware
    m_AxisObserver = new DebugOutputLoopFrequency();
    m_Context->getCollectionCarrier()->getKnobAxisCollection()->attachObserver(m_AxisObserver); // Attach the observer to the axis collection
}
