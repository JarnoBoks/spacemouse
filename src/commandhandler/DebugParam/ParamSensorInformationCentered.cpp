#include "ParamSensorInformationCentered.hpp"

#include "..\CollectionCarrier\CollectionCarrier.hpp"
#include "sensor/SensorCollection.hpp"
#include "observers/DebugOutput/DebugOutputSensorsCentered.hpp"

#include <common/esp_print.h> // For ESP_PRINT

void DebugParamSensorInformationCentered::apply() {

    // Instantiate the Observer for the CenteredSensor values and attach it to the hardware
    m_SensorObserver = new DebugOutputSensorsCentered();
    m_Context->getCollectionIdentifier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the observer to the sensor collection
}

void DebugParamSensorInformationCentered::report() {
    ESP_PRINT(F("DebugParamSensorInformationCentered active")); // Report the active state of the class
}
