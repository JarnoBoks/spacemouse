#include "SensorsCalculator.hpp"
#include "sensor/SensorCollection.hpp"
#include "sensor/sensors/Sensor.hpp" // Include the header file for the Sensor class

const int SensorsCalculator::value(uint8_t const id) const {
    return m_sensorCollection->getSensor(id)->getFinValue();
}

void SensorsCalculator::setAnalogReference(const bool isDebug) {
    Serial.println(F("Set analog reference voltage")); // Debug message to indicate the reference voltage settings
    referenceVoltage = DEFAULT;                        // Set the default reference voltage to DEFAULT
#ifdef ARDUINO_ARCH_AVR
    analogReference(referenceVoltage); // Set the analog reference voltage to DEFAULT
#endif
}
