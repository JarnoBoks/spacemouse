#include "ParamSensorInformationRaw.hpp"

#include "sensor/SensorCollection.hpp"
#include "observers/DebugOutput/DebugOutputSensorsRaw.hpp"

#include <common/esp_print.h> // For ESP_PRINT

// ----------------- RAW SENSORS ----------------

DebugParamSensorInformationRaw::DebugParamSensorInformationRaw() {
    // FIXME Hardware::getInstance()->setAnalogReference(true);
}
DebugParamSensorInformationRaw::~DebugParamSensorInformationRaw() {
    // FIXME Hardware::getInstance()->setAnalogReference(false);
}

void DebugParamSensorInformationRaw::apply() {

    // Instantiate the Observer for the RawSensor values and attach it to the hardware
    m_SensorObserver = new DebugOutputSensorsRaw();
    m_Context->getSensorCollection()->attachObserver(m_SensorObserver); // Attach the observer to the sensor collection
}

void DebugParamSensorInformationRaw::report() {
    ESP_PRINT(F("DebugParamSensorInformationRaw active")); // Report the active state of the class
}
