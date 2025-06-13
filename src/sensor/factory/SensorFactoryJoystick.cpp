#include "SensorFactoryJoystick.hpp" // Include the header file for the sensor factory

// Include header file for the key to create
#include <sensor/sensors/JoystickSensor.hpp>

// Include the configuration header file for the key configuration in this factory
#include "config.h"

#include <common/esp_print.h> // For ESP_ERROR() and ESP_DEBUG() macros

#include <pins_arduino.h> // For the pin definitions

#define NUM_JOYSTICK_SENSORS 8          // TODO - We have to find a singular place to define this number
#define JOYSTICK_SENSOR_PINLIST PINLIST // Pins for the sensors, as defined in config.h

Sensor *SensorFactoryJoystick::create(uint8_t id) {
    if (id >= JS_LENGTH) {
        ESP_ERROR(F("Invalid Joystick sensor id")); // Log an error if the id is invalid
        return nullptr;                             // Return null if the id is out of range
    }

    // Get the pinNumber from the config.h object for this sensor
    constexpr uint8_t sensorPins[JS_LENGTH] = JOYSTICK_SENSOR_PINLIST;                            // Pins for the joystick sensors, as defined in config.h
    Sensor *newSensor = new JoystickSensor(sensorPins[id], static_cast<JoystickSensorsId_t>(id)); // Create a new joystick sensor instance

    return newSensor; // Return the new joystick sensor instance
}
