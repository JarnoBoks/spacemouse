#include "SensorFactoryHall.hpp" // Include the header file for the sensor factory

// Include header file for the key to create
#include "sensor/sensors/HallSensor.hpp"

// Include the configuration header file for the key configuration in this factory
#include "config.h"

#include <pins_arduino.h> // For the pin definitions
#include <binary.h>       // For the binary operations
#include <Arduino.h>

#define NUM_HALL_SENSORS 8          // TODO - We have to find a singular place to define this number
#define HALL_SENSOR_PINLIST PINLIST // Pins for the sensors, as defined in config.h

Sensor *SensorFactoryHall::create(uint8_t id) {
    if (id >= NUM_HALL_SENSORS) {
        return nullptr; // Return null if the id is out of range
    }

    // Get the pinNumber from the config.h object for this sensor
    constexpr uint8_t sensorPins[NUM_HALL_SENSORS] = HALL_SENSOR_PINLIST;                 // Pins for the sensors, as defined in config.h
    Sensor *newSensor = new HallSensor(sensorPins[id], static_cast<HallSensorsId_t>(id)); // Create a new hall sensor instance
    // FIXME setupFunctionality(newKey);                        // Set up the functionality for the new key

    // REVIEW - Does the ESP32 have something alike this
#ifdef ARDUINO_ARCH_AVR
                                                                                          // Set the ADC Prescaler to 16 in order to read the ADC much faster.
    // NOTE: Maybe this is relevant for the Joystick sensors as well, but cannot test it.
    ADCSRA = (ADCSRA & B11111000) | 4; // Set prescaler to 16 for ADC
#endif

    return newSensor; // Return the new Hall sensor instance
}
