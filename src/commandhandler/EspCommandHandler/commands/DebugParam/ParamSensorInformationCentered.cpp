#include "ParamSensorInformationCentered.hpp"

#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>
#include <sensor/SensorCollection.hpp>
#include <observers/DebugOutput/DebugOutputSensorsCentered.hpp>

#include <common/esp_print.h> // For ESP_PRINT

void DebugParamSensorInformationCentered::apply() {

    // Instantiate the Observer for the CenteredSensor values and attach it to the hardware
    m_SensorObserver = new DebugOutputSensorsCentered();
    m_Context->getCollectionCarrier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the observer to the sensor collection
}
