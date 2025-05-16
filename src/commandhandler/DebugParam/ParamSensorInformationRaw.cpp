#include "ParamSensorInformationRaw.hpp"

#include "..\CollectionCarrier\CollectionCarrier.hpp"
#include "sensor/SensorCollection.hpp"
#include "observers/DebugOutput/DebugOutputSensorsRaw.hpp"

#include <common/esp_print.h> // For ESP_PRINT

// ----------------- RAW SENSORS ----------------

#if 0
DebugParamSensorInformationRaw::DebugParamSensorInformationRaw(DebugCommand *context) : DebugParamSensorInformation(context) {
    Serial.println(F("DebugParamSensorInformationRaw::DebugParamSensorInformationRaw()")); // Debug message to indicate the constructor call
    // FIXME Hardware::getInstance()->setAnalogReference(true);
}
#endif
DebugParamSensorInformationRaw::~DebugParamSensorInformationRaw() {
    // FIXME Hardware::getInstance()->setAnalogReference(false);
}

void DebugParamSensorInformationRaw::apply() {

    // Instantiate the Observer for the RawSensor values and attach it to the hardware
    m_SensorObserver = new DebugOutputSensorsRaw();
    m_Context->getCollectionIdentifier()->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the observer to the sensor collection
}

void DebugParamSensorInformationRaw::report() {
    ESP_PRINT(F("DebugParamSensorInformationRaw active")); // Report the active state of the class
}
