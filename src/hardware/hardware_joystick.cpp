
#include "hardware_joystick.h"
#include "sensor/sensors/JoystickSensor.hpp" // Include the JoystickSensor header file
#include "config.h"                          // For PINLIST

Hardware_Joystick::Hardware_Joystick() {
#if 0 // REMOVE
    // Initialize the sensors
    const uint8_t sensorPins[JoystickSensorsId_t::JS_LENGTH] = PINLIST; // Pins for the sensors, as defined in config.h

    for (uint8_t i = 0; i < JoystickSensorsId_t::JS_LENGTH; i++) {
        sensors[i] = new JoystickSensor(sensorPins[i], static_cast<JoystickSensorsId_t>(i)); // Create new JoystickSensor objects
    }
#endif
}

// Define a macro to simplify the access to the sensor values
#define VAL(x) static_cast<Sensor *>(m_sensorCollection->getItem(x))->getFilteredValue()
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