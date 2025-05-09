
#include "hardware_joystick.h"
#include "sensor/sensors/JoystickSensor.hpp" // Include the JoystickSensor header file
#include "config.h"                          // For PINLIST

// Define a macro to simplify the access to the sensor values
#define VAL(x) value(x)
int16_t Hardware_Joystick::calculateRawValue(AxisType_t axistype) {

    evaluateSensorCollection();

    switch (axistype) {
    case AxisType_t::TRANSX:
        // calculate sensors transX
        return (-VAL(CY) + VAL(AY));
        break;
    case AxisType_t::TRANSY:
        // calculate sensors transY
        return (-VAL(BY) + VAL(DY));
        break;
    case AxisType_t::TRANSZ:
        return -VAL(AX) - VAL(BX) - VAL(CX) - VAL(DX);
        break;
    case AxisType_t::ROTX:
        // rotX
        return (-VAL(CX) + VAL(AX));
        break;
    case AxisType_t::ROTY:
        // rotY
        return (-VAL(BX) + VAL(DX));
        break;
    case AxisType_t::ROTZ:
        // rotZ
        return (VAL(AY) + VAL(BY) + VAL(CY) + VAL(DY));
        break;
    default:
        // Handle invalid axis type if necessary
        break;
    }

    return 0; // Default return value if no valid axis type is found
}
#undef VAL