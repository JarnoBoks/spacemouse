#include "CalibratorStateBase.hpp"     // Include the ICalibratorState interface header file
#include <observers/IObserver.hpp>     // Include the IObserver interface header file
#include <sensor/SensorCollection.hpp> // Include the SensorCollection class header file

CalibratorStateBase::~CalibratorStateBase() {
    if (!m_sensorObserver)
        return;

    SensorCollection *sensorCollection = context->getSensorCollection(); // Get the sensor collection from the context
    if (sensorCollection)
        sensorCollection->detachObserver(m_sensorObserver); // Detach the observer from the sensor collection

    delete m_sensorObserver;    // Delete the observer to free memory
    m_sensorObserver = nullptr; // Set the observer pointer to null
}
