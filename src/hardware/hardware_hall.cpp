#include "hardware_hall.h"
#include "sensor/sensors/HallSensor.hpp" // Include the header file for the Hall sensor
#include "config.h"                      // For PINLIST

Hardware_HALL::Hardware_HALL() {
#if 0 // REMOVE
    // Initialize the sensors
    const uint8_t sensorPins[HallSensorsId_t::HALL_LENGTH] = PINLIST; // Pins for the sensors, as defined in config.h

    for (uint8_t i = 0; i < HallSensorsId_t::HALL_LENGTH; i++) {
        sensors[i] = new HallSensor(sensorPins[i], static_cast<HallSensorsId_t>(i)); // Create new HallSensor objects
    }

    // Set the ADC Prescaler to 16 in order to read the ADC much faster.
    // NOTE: Maybe this is relevant for the Joystick hardware as well, but cannot test it.
    ADCSRA = (ADCSRA & B11111000) | 4; // Set prescaler to 16 for ADC
#endif

    // Set the analog reference voltage for the sensors
    setAnalogReference(INTERNAL);
}

void Hardware_HALL::setAnalogReference(const bool isDebug) {
    referenceVoltage = (isDebug) ? DEFAULT : INTERNAL; // Set the default reference voltage to DEFAULT or INTERNAL based on isDebug
#ifdef ARDUINO_ARCH_AVR
    // REVIEW analogReference(referenceVoltage); // Set the analog reference voltage to DEFAULT
    if (referenceVoltage == DEFAULT) {
        Serial.println(F("Set analog reference voltage to DEFAULT")); // Debug message to indicate the reference voltage settings
    } else {
        Serial.println(F("Set analog reference voltage to INTERNAL"));
    }
#endif
}

// Define a macro to simplify the access to the sensor values
#define VAL(x) value(x)
int16_t Hardware_HALL::calculateRawValue(AxisType_t axistype) {
    if (!m_sensorCollection) {
        return 0; // Return 0 if the sensor collection is not initialized
    }

    int16_t retval = 0; // Initialize the value to 0

    switch (axistype) {
    case AxisType_t::TRANSX:
        // calculate sensors transX
        retval = (VAL(HES1) - VAL(HES0) + VAL(HES6) - VAL(HES7)) / 2;
        break;
    case AxisType_t::TRANSY:
        // calculate sensors transY
        retval = (VAL(HES2) - VAL(HES3) + VAL(HES9) - VAL(HES8)) / 2;
        break;
    case AxisType_t::TRANSZ:
        retval = (VAL(HES0) + VAL(HES1) + VAL(HES2) + VAL(HES3) + VAL(HES6) + VAL(HES7) + VAL(HES8) + VAL(HES9)) / 4;
        break;
    case AxisType_t::ROTX:
        // rotX
        retval = (VAL(HES0) + VAL(HES1) - VAL(HES6) - VAL(HES7)) / 2;
        break;
    case AxisType_t::ROTY:
        // rotY
        retval = (VAL(HES8) + VAL(HES9) - VAL(HES2) - VAL(HES3)) / 2;
        break;
    case AxisType_t::ROTZ:
        // rotZ
        retval = (VAL(HES0) + VAL(HES2) + VAL(HES6) + VAL(HES8) - VAL(HES1) - VAL(HES3) - VAL(HES7) - VAL(HES9)) / 4;
        break;
    default:
        // Handle invalid axis type if necessary - nothing to do - retval is already 0
        break;
    }
    return retval; // Default return value if no valid axis type is found
}
#undef VAL
