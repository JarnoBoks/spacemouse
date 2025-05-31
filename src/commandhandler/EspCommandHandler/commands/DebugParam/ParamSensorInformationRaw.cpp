#include "ParamSensorInformationRaw.hpp"

#include "commandhandler/CollectionCarrier/CollectionCarrier.hpp"
#include "sensor/SensorCollection.hpp"
#include "..\..\..\..\observers\DebugOutput\DebugOutputSensorsRaw.hpp"

#include <common/esp_print.h> // For ESP_PRINT

// ----------------- RAW SENSORS ----------------

DebugParamSensorInformationRaw::~DebugParamSensorInformationRaw() {
    // REVIEW - Check if it is necessary to change the analog reference Voltage to 5V on the Pro Micro.
}

void DebugParamSensorInformationRaw::apply() {

    // Instantiate the Observer for the RawSensor values and attach it to the hardware
    m_SensorObserver = new DebugOutputSensorsRaw();
    m_Context->getCollectionCarrier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the observer to the sensor collection
}
