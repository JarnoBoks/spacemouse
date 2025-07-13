#include "SensorsCalculatorHall.hpp"
#include "sensor/sensors/HallSensor.hpp" // Include the header file for the Hall sensor
#include <base/axis/MotionVectorType.h>  // For MotionVector_t enum
#include <knob/axis/KnobAxis.hpp>

#ifndef INTERNAL
#define INTERNAL 0x03 // Define the INTERNAL constant if not already defined (necessary for ESP32)
#endif

// FIXME - Hardware design should take care of the reference voltage.
SensorsCalculatorHall::SensorsCalculatorHall() {
    // Set the analog reference voltage for the sensors
    setAnalogReference(INTERNAL);
}

/**
 * @brief Sets the analog reference voltage for the sensors.
 * @param isDebug If true, set the reference voltage to DEFAULT, otherwise set it to INTERNAL.
 * @deprecated
 */
void SensorsCalculatorHall::setAnalogReference(const bool isDebug) {
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

// Macro to simplify the access to the sensor values
#define VAL(x) value(x)
void SensorsCalculatorHall::evaluate(KnobAxis *knobVector) {
    if (!m_sensorCollection || !knobVector)
        return;

    int16_t retval = 0;

#ifdef ARDUINO_ARCH_AVR
    // AVR architecture specific code to read the sensors
    switch (knobVector->getType()) {
    case MotionVector_t::TRANSX:
        // calculate sensors transX
        retval = (VAL(HES1) - VAL(HES0) + VAL(HES6) - VAL(HES7)) / 2;
        break;
    case MotionVector_t::TRANSY:
        // calculate sensors transY
        retval = (VAL(HES2) - VAL(HES3) + VAL(HES9) - VAL(HES8)) / 2;
        break;
    case MotionVector_t::TRANSZ:
        retval = (VAL(HES0) + VAL(HES1) + VAL(HES2) + VAL(HES3) + VAL(HES6) + VAL(HES7) + VAL(HES8) + VAL(HES9)) / 4;
        break;
    case MotionVector_t::ROTX:
        // rotX
        retval = (VAL(HES0) + VAL(HES1) - VAL(HES6) - VAL(HES7)) / 2;
        break;
    case MotionVector_t::ROTY:
        // rotY
        retval = (VAL(HES8) + VAL(HES9) - VAL(HES2) - VAL(HES3)) / 2;
        break;
    case MotionVector_t::ROTZ:
        // rotZ
        retval = (VAL(HES0) + VAL(HES2) + VAL(HES6) + VAL(HES8) - VAL(HES1) - VAL(HES3) - VAL(HES7) - VAL(HES9)) / 4;
        break;
    default:
        // Handle invalid knobVector type if necessary - nothing to do - retval is already 0
        break;
    }
#endif

    // John Crombee translated the TeachingTech code (using Joysticks) to the HALL effect sensors code.
    // This code is based on the TeachingTech code and adapted to the HALL effect sensors.
    // The Joysticks have a distinct output for each joystick axis, while the HALL effect sensors derive their values from pairs of sensors.
    // This will work for each movement vector but the ROTZ and TRANSZ vectors. Due to the nature of the calculation and the
    // physical properties of magnetic fields, a ROTZ will result in an unwanted TRANSZ value.
    // We correct this by subtracting the difference in a sensor pair from the TRANSZ value, assuming a TRANSZ movement will
    // have the same absolute effect on both sensors in the pair, while a ROTZ movement will have a different effect on the sensors in the pair.

    // 4-3, 5-6, 2-1, 8-7
    int corrRZ_TZ = abs(abs(VAL(HES3)) - abs(VAL(HES4))) +
                    abs(abs(VAL(HES5)) - abs(VAL(HES6))) +
                    abs(abs(VAL(HES1)) - abs(VAL(HES2))) +
                    abs(abs(VAL(HES7)) - abs(VAL(HES8)));

#ifdef ARDUINO_ARCH_ESP32
    // AVR architecture specific code to read the sensors
    switch (knobVector->getType()) {
    case MotionVector_t::TRANSX:
        // calculate sensors transX
        retval = (VAL(HES3) - VAL(HES4) + VAL(HES2) - VAL(HES1)) / 2;
        break;
    case MotionVector_t::TRANSY:
        // calculate sensors transY
        retval = (VAL(HES6) - VAL(HES5) + VAL(HES7) - VAL(HES8)) / 2;
        break;
    case MotionVector_t::TRANSZ:
        retval = (corrRZ_TZ + (HES1) + VAL(HES2) + VAL(HES3) + VAL(HES4) + VAL(HES5) + VAL(HES6) + VAL(HES7) + VAL(HES8)) / 4;
        break;
    case MotionVector_t::ROTX:
        // rotX
        retval = (VAL(HES4) + VAL(HES3) - VAL(HES2) - VAL(HES1)) / 2;
        break;
    case MotionVector_t::ROTY:
        // rotY
        retval = (VAL(HES8) + VAL(HES7) - VAL(HES6) - VAL(HES5)) / 2;
        break;
    case MotionVector_t::ROTZ:
        // rotZ
        retval = (VAL(HES2) + VAL(HES4) + VAL(HES6) + VAL(HES8) - VAL(HES1) - VAL(HES3) - VAL(HES5) - VAL(HES7)) / 4;
        break;
    default:
        // Handle invalid knobVector type if necessary - nothing to do - retval is already 0
        break;
    }
#endif
    knobVector->setRawValue(retval);
}
#undef VAL
