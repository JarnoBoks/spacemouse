#include "CalibratorStateBase.hpp"     // Include the ICalibratorState interface header file
#include <sensor/SensorCollection.hpp> // Include the SensorCollection class header file

CalibratorStateBase::~CalibratorStateBase() {
    if (!sensorObserver)
        return;

    SensorCollection *sensorCollection = context->getSensorCollection(); // Get the sensor collection from the context
    if (sensorCollection)
        sensorCollection->detachObserver(sensorObserver); // Detach the observer from the sensor collection

    delete sensorObserver;    // Delete the observer to free memory
    sensorObserver = nullptr; // Set the observer pointer to null
}
