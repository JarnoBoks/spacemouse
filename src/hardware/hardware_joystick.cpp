
#include "hardware_joystick.h"
#include "sensor/sensor_joystick.h"

Hardware_Joystick *Hardware_Joystick::instance = nullptr;

Hardware_Joystick *Hardware_Joystick::getInstance() {
    if (!instance) {
        instance = new Hardware_Joystick();
    }
    return instance;
}

Hardware_Joystick::Hardware_Joystick() {
    // Initialize the sensors
    const uint8_t sensorPins[JoystickSensorsId_t::JS_LENGTH] = PINLIST; // Pins for the sensors, as defined in config.h

    for (uint8_t i = 0; i < JoystickSensorsId_t::JS_LENGTH; i++) {
        sensors[i] = new JoystickSensor(sensorPins[i], static_cast<JoystickSensorsId_t>(i)); // Create new JoystickSensor objects
    }
}

#define VAL(X) sensors[X]->getFilteredValue()
int16_t Hardware_Joystick::calculateRawValue(AxisType_t axistype) {

    updateSensorValues();

    switch (axistype) {
    case TRANSX:
        // calculate sensors transX
        return (-VAL(CY) + VAL(AY));
        break;
    case TRANSY:
        // calculate sensors transY
        return (-VAL(BY) + VAL(DY));
        break;
    case TRANSZ:
        return -VAL(AX) - VAL(BX) - VAL(CX) - VAL(DX);
        break;
    case ROTX:
        // rotX
        return (-VAL(CX) + VAL(AX));
        break;
    case ROTY:
        // rotY
        return (-VAL(BX) + VAL(DX));
        break;
    case ROTZ:
        // rotZ
        return (VAL(AY) + VAL(BY) + VAL(CY) + VAL(DY));
        break;
    default:
        // Handle invalid axis type if necessary
        break;
    }

    notifyObservers();
    return 0; // Default return value if no valid axis type is found
}
#undef VAL
