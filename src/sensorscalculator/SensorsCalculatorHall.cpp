#include "SensorsCalculatorHall.hpp"
#include "sensor/sensors/HallSensor.hpp" // Include the header file for the Hall sensor
#include "..\axis\axes\Axis.hpp"         // Include the header file for the Axis class
#include "axis/axes/axistype.h"          // Include the header file for the AxisType_t enum

SensorsCalculatorHall::SensorsCalculatorHall() {
    // Set the analog reference voltage for the sensors
    setAnalogReference(INTERNAL);
}

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

// Define a macro to simplify the access to the sensor values
#define VAL(x) value(x)
void SensorsCalculatorHall::calculate(Axis *axis) {
    if (!m_sensorCollection)
        return;

    int16_t retval = 0;

    switch (axis->getAxisType()) {
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
    axis->setSensorValue(retval);
}
#undef VAL
