#include "hardware.h"

// -- Static part of the class (= application specific)
// In this part the hardware class is defined and the static instance is created

Hardware *Hardware::_instance = nullptr;

Hardware *Hardware::getInstance() {
    /*if (_instance == nullptr) {
        _instance = HW_TYPE::getInstance(); // Create the hardware instance based on the defined hardware type
    }*/
    return _instance;
}

const int Hardware::value(uint8_t const id) const {
    // Get the value of the sensor with the given id
    return static_cast<Sensor *>(m_sensorCollection->getItem(id))->getFilteredValue();
}

void Hardware::setAnalogReference(const bool isDebug) {
    Serial.println(F("Set analog reference voltage")); // Debug message to indicate the reference voltage settings
    referenceVoltage = DEFAULT;                        // Set the default reference voltage to DEFAULT
#ifdef ARDUINO_ARCH_AVR
    analogReference(referenceVoltage); // Set the analog reference voltage to DEFAULT
#endif
}
