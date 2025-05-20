
#include "SensorsCalculatorJoystick.hpp"
#include "sensor/sensors/JoystickSensor.hpp"
#include <motionvector/MotionVectorType.h> // For MotionVector_t enum
#include <Knob/MotionVector/KnobMotionVector.hpp>

// Macro to simplify the access to the sensor values
#define VAL(x) value(x)
void SensorsCalculatorJoystick::evaluate(KnobMotionVector *knobVector) {
    if (!m_sensorCollection || !knobVector)
        return;

    int16_t retval = 0;

    switch (knobVector->getType()) {
    case MotionVector_t::TRANSX:
        retval = (-VAL(CY) + VAL(AY));
        break;
    case MotionVector_t::TRANSY:
        retval = (-VAL(BY) + VAL(DY));
        break;
    case MotionVector_t::TRANSZ:
        retval = -VAL(AX) - VAL(BX) - VAL(CX) - VAL(DX);
        break;
    case MotionVector_t::ROTX:
        retval = (-VAL(CX) + VAL(AX));
        break;
    case MotionVector_t::ROTY:
        retval = (-VAL(BX) + VAL(DX));
        break;
    case MotionVector_t::ROTZ:
        retval = (VAL(AY) + VAL(BY) + VAL(CY) + VAL(DY));
        break;
    default:
        // Handle invalid knobVector type if necessary
        break;
    }

    knobVector->setRawValue(retval);
}
#undef VAL