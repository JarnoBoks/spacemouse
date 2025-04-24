
#include "hardware.h"
#include "sensor_hall.h"
#include "sensor_joystick.h"
#include "config.h"

void Hardware::setAnalogReference(const uint8_t voltage) {
    // analogReference(voltage);
    referenceVoltage = voltage;
    analogReference(referenceVoltage);
}

HALL *HALL::instance = nullptr;

HALL *HALL::getInstance() {
    if (!instance) {
        instance = new HALL();
    }
    return instance;
}

HALL::HALL() {
    // Initialize the sensors
    const uint8_t sensorPins[HallSensorsId_t::LENGTH] = PINLIST; // Pins for the sensors, as defined in config.h

    for (uint8_t i = 0; i < HallSensorsId_t::LENGTH; i++) {
        sensors[i] = new HallSensor(sensorPins[i], static_cast<HallSensorsId_t>(i)); // Create new HallSensor objects
    }
}

Joystick *Joystick::instance = nullptr;

Joystick *Joystick::getInstance() {
    if (!instance) {
        instance = new Joystick();
    }
    return instance;
}

Joystick::Joystick() {
    // Initialize the sensors
    const uint8_t sensorPins[JoystickSensorsId_t::LENGTH] = PINLIST; // Pins for the sensors, as defined in config.h

    for (uint8_t i = 0; i < JoystickSensorsId_t::LENGTH; i++) {
        sensors[i] = new JoystickSensor(sensorPins[i], static_cast<JoystickSensorsId_t>(i)); // Create new JoystickSensor objects
    }
}

#define VAL(X) sensors[X]->getFilteredValue()
int16_t HALL::calculateRawValue(AxisType_t axistype) {
    switch (axistype) {
    case TX:
        // calculate sensors transX
        return (VAL(HES1) - VAL(HES0) + VAL(HES6) - VAL(HES7)) / 2;
        break;
    case TY:
        // calculate sensors transY
        return (VAL(HES2) - VAL(HES3) + VAL(HES9) - VAL(HES8)) / 2;
        break;
    case TZ:
        return (VAL(HES0) + VAL(HES1) + VAL(HES2) + VAL(HES3) + VAL(HES6) + VAL(HES7) + VAL(HES8) + VAL(HES9)) / 4;
        break;
    case RX:
        // rotX
        return (VAL(HES0) + VAL(HES1) - VAL(HES6) - VAL(HES7)) / 2;
        break;
    case RY:
        // rotY
        return (VAL(HES8) + VAL(HES9) - VAL(HES2) - VAL(HES3)) / 2;
        break;
    case RZ:
        // rotZ
        return (VAL(HES0) + VAL(HES2) + VAL(HES6) + VAL(HES8) - VAL(HES1) - VAL(HES3) - VAL(HES7) - VAL(HES9)) / 4;
        break;
    default:
        // Handle invalid axis type if necessary
        break;
    }
}
#undef VAL(X)

#define VAL(X) sensors[X]->getFilteredValue()
int16_t Joystick::calculateRawValue(AxisType_t axistype) {

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
