
#include "hardware_joystick.h"
#include "sensor_joystick.h"

Hardware_Joystick *Hardware_Joystick::instance = nullptr;

Hardware_Joystick *Hardware_Joystick::getInstance() {
    if (!instance) {
        instance = new Hardware_Joystick();
    }
    return instance;
}

Hardware_Joystick::Hardware_Joystick() {
    // Initialize the sensors
    const uint8_t sensorPins[JoystickSensorsId_t::LENGTH] = PINLIST; // Pins for the sensors, as defined in config.h

    for (uint8_t i = 0; i < JoystickSensorsId_t::LENGTH; i++) {
        sensors[i] = new JoystickSensor(sensorPins[i], static_cast<JoystickSensorsId_t>(i)); // Create new JoystickSensor objects
    }
}

#define VAL(X) sensors[X]->getFilteredValue()
int16_t Hardware_Joystick::calculateRawValue(AxisType_t axistype) {

    switch (axistype) {
    case TX:
        // calculate sensors transX
        return (-VAL(CY) + VAL(AY));
        break;
    case TY:
        // calculate sensors transY
        return (-VAL(BY) + VAL(DY));
        break;
    case TZ:
        return -VAL(AX) - VAL(BX) - VAL(CX) - VAL(DX);
        break;
    case RX:
        // rotX
        return (-VAL(CX) + VAL(AX));
        break;
    case RY:
        // rotY
        return (-VAL(BX) + VAL(DX));
        break;
    case RZ:
        // rotZ
        return (VAL(AY) + VAL(BY) + VAL(CY) + VAL(DY));
        break;
    default:
        // Handle invalid axis type if necessary
        break;
    }
}
#undef VAL(X)
