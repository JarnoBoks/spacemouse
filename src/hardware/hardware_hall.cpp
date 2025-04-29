#include "hardware_hall.h"
#include "sensor/sensor_hall.h"
#include "config.h" // For PINLIST

// REMOVE Hardware_HALL *Hardware_HALL::instance = nullptr;

#if 0
Hardware *Hardware_HALL::getInstance() {
    if (!instance) {
        instance = new Hardware_HALL();
    }
    return instance;
}
#endif

Hardware_HALL::Hardware_HALL() {
    registerInstance(this); // Register the instance of the derived class in the base class

    // Initialize the sensors
    const uint8_t sensorPins[HallSensorsId_t::HALL_LENGTH] = PINLIST; // Pins for the sensors, as defined in config.h

    for (uint8_t i = 0; i < HallSensorsId_t::HALL_LENGTH; i++) {
        sensors[i] = new HallSensor(sensorPins[i], static_cast<HallSensorsId_t>(i)); // Create new HallSensor objects
    }
}

#define VAL(x) sensors[x]->getFilteredValue()
int16_t Hardware_HALL::calculateRawValue(AxisType_t axistype) {

    updateSensorValues();

    int16_t retval = 0; // Initialize the value to 0
    switch (axistype) {
    case TRANSX:
        // calculate sensors transX
        retval = (VAL(HES1) - VAL(HES0) + VAL(HES6) - VAL(HES7)) / 2;
        break;
    case TRANSY:
        // calculate sensors transY
        retval = (VAL(HES2) - VAL(HES3) + VAL(HES9) - VAL(HES8)) / 2;
        break;
    case TRANSZ:
        retval = (VAL(HES0) + VAL(HES1) + VAL(HES2) + VAL(HES3) + VAL(HES6) + VAL(HES7) + VAL(HES8) + VAL(HES9)) / 4;
        break;
    case ROTX:
        // rotX
        retval = (VAL(HES0) + VAL(HES1) - VAL(HES6) - VAL(HES7)) / 2;
        break;
    case ROTY:
        // rotY
        retval = (VAL(HES8) + VAL(HES9) - VAL(HES2) - VAL(HES3)) / 2;
        break;
    case ROTZ:
        // rotZ
        retval = (VAL(HES0) + VAL(HES2) + VAL(HES6) + VAL(HES8) - VAL(HES1) - VAL(HES3) - VAL(HES7) - VAL(HES9)) / 4;
        break;
    default:
        // Handle invalid axis type if necessary - nothing to do - retval is already 0
        break;
    }

    notifyObservers();
    return retval; // Default return value if no valid axis type is found
}
#undef VAL
