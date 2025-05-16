
#include "SensorsCalculatorJoystick.hpp"
#include "sensor/sensors/JoystickSensor.hpp" // Include the JoystickSensor header file
#include "axis/axes/Axis.hpp"
#include "axis/axes/axistype.h"

// Macro to simplify the access to the sensor values
#define VAL(x) value(x)
void SensorsCalculatorJoystick::evaluate(Axis *axis) {
    if (!m_sensorCollection || !axis)
        return;

    int16_t retval = 0;

    switch (axis->getAxisType()) {
    case AxisType_t::TRANSX:
        retval = (-VAL(CY) + VAL(AY));
        break;
    case AxisType_t::TRANSY:
        retval = (-VAL(BY) + VAL(DY));
        break;
    case AxisType_t::TRANSZ:
        retval = -VAL(AX) - VAL(BX) - VAL(CX) - VAL(DX);
        break;
    case AxisType_t::ROTX:
        retval = (-VAL(CX) + VAL(AX));
        break;
    case AxisType_t::ROTY:
        retval = (-VAL(BX) + VAL(DX));
        break;
    case AxisType_t::ROTZ:
        retval = (VAL(AY) + VAL(BY) + VAL(CY) + VAL(DY));
        break;
    default:
        // Handle invalid axis type if necessary
        break;
    }

    axis->setSensorValue(retval);
}
#undef VAL