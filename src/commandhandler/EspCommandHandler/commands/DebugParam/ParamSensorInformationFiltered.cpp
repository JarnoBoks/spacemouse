#include "ParamSensorInformationFiltered.hpp"

#include <commandhandler/CollectionCarrier/CollectionCarrier.hpp>
#include <sensor/SensorCollection.hpp>
#include <observers/DebugOutput/DebugOutputSensorsFiltered.hpp>

#include <common/esp_print.h> // For ESP_PRINT

void DebugParamSensorInformationFiltered::apply() {

    // Instantiate the Observer for the FilteredSensor values and attach it to the hardware
    m_SensorObserver = new DebugOutputSensorsFiltered();
    m_Context->getCollectionCarrier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the observer to the sensor collection
}
