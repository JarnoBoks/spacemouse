
#include "hardware_hall.h"
#include "sensor_hall.h"

Hardware_HALL *Hardware_HALL::instance = nullptr;

Hardware_HALL *Hardware_HALL::getInstance() {
    if (!instance) {
        instance = new Hardware_HALL();
    }
    return instance;
}

Hardware_HALL::Hardware_HALL() {
    // Initialize the sensors
    const uint8_t sensorPins[HallSensorsId_t::LENGTH] = PINLIST; // Pins for the sensors, as defined in config.h

    for (uint8_t i = 0; i < HallSensorsId_t::LENGTH; i++) {
        sensors[i] = new HallSensor(sensorPins[i], static_cast<HallSensorsId_t>(i)); // Create new HallSensor objects
    }
}

#define VAL(X) sensors[X]->getFilteredValue()
int16_t Hardware_HALL::calculateRawValue(AxisType_t axistype) {
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
